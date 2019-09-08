//
//  hmm.c
//  hmmtest
//
//  Created by Matthew Fonken on 2/10/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#ifdef __PSM__

#include "hmm.h"

#define HMM_ERROR_PADDING 0.01
#define SOFTEN(X) (X==1?(1-HMM_ERROR_PADDING):(X==0?HMM_ERROR_PADDING:X))

void InitializeHMM( hidden_markov_model_t * model, transition_matrix_t * A, observation_matrix_t * B, state_vector_t * pi )
{
    memset( model, 0, sizeof(hidden_markov_model_t) );
    memcpy( &model->A, A, sizeof(transition_matrix_t) );
    memcpy( &model->B, B, sizeof(observation_matrix_t) );
    memcpy( &model->pi, pi, sizeof(state_vector_t) );
    
}

uint8_t ReportObservationToHMM( hidden_markov_model_t * model, hmm_observation_t o )
{
    return PushToObservationBuffer( &model->O, o );
}

double UpdateAllHMM( hidden_markov_model_t * model )
{
    HMMFunctions.Update.Alpha(  model );
    HMMFunctions.Update.Beta(   model );
    HMMFunctions.Update.Gamma(  model );
    HMMFunctions.Update.Xi(     model );
    double new_P = HMMFunctions.Update.Probability( model ),
    delta_P = model->P - new_P;
    model->P = new_P;
    return delta_P;
}

void UpdateAlphaHMM( hidden_markov_model_t * model )
{
    memset( model->alpha, 0., sizeof(model->alpha) );
    uint8_t T = model->O.length;
    hmm_observation_t o = GetIndexObservationBuffer( &model->O, 0 );
    double p;
    
    /* Source - https://en.wikipedia.org/wiki/Baum–Welch_algorithm#cite_note-6 */
    /* 1. α_i(0) = π_i x B_i(y_0) */
    for( uint8_t i = 0; i < NUM_STATES; i++ )
        model->alpha[0][i] = model->pi[i] * GetProbabilityFromEmission( &model->B[i], o );
    
    /*  2. α_i(t) = B_i(y_(t)) x ∑_{j=0}^N ( α_j(t-1) x A_{ji} ) */
    for( uint8_t t = 1; t < T; t++ )
    {
        for( uint8_t i = 0; i < NUM_STATES; i++ )
        {
            double sum = 0;
            for( uint8_t j = 0; j < NUM_STATES; j++ )
                sum += model->alpha[t - 1][j] * model->A[j][i];
            o = GetIndexObservationBuffer( &model->O, t );
            p = GetProbabilityFromEmission( &model->B[i], o );
            model->alpha[t][i] = sum * p;
        }
    }
}

void UpdateBetaHMM(hidden_markov_model_t * model )
{
    memset( model->beta, 0., sizeof(model->beta) );
    uint8_t T = model->O.length;
    hmm_observation_t o;
    double p;
    
    /* Source - https://en.wikipedia.org/wiki/Baum–Welch_algorithm#cite_note-6 */
    /* 1. β_i(T-1) = 1 */
    for( uint8_t i = 0; i < NUM_STATES; i++ )
        model->beta[T - 1][i] = 1.;
    
    /* 2. β_i(t) = ∑_{j=1}^N ( β_j(t+1) x A_{ij} x B_j(y_{t+1}) ) */
    for( int8_t t = T - 2; t >= 0; --t)
    {
        for( uint8_t i = 0; i < NUM_STATES; i++ )
        {
            double sum = 0;
            for( uint8_t j = 0; j < NUM_STATES; j++ )
            {
                o = GetIndexObservationBuffer( &model->O, t + 1 );
                p = GetProbabilityFromEmission( &model->B[j], o );
                sum += model->beta[t + 1][j] * model->A[i][j] * p;
            }
            model->beta[t][i] = sum;
        }
    }
}

void UpdateGammaHMM(hidden_markov_model_t * model )
{
    memset( model->gamma, 0., sizeof(model->gamma) );
    uint8_t T = model->O.length;
    
    /* Source - https://en.wikipedia.org/wiki/Baum–Welch_algorithm#cite_note-6 */
    /* γ_i(t) = ( α_i(t) x β_i(t) ) / ( ∑_{j=1)^N ( α_j(t) x β_j(t) ) ) */
    for( uint8_t t = 0; t < T; t++ )
    {
        double sum = 0;
        for( uint8_t i = 0; i < NUM_STATES; i++ )
        {
            model->gamma[t][i] = model->alpha[t][i] * model->beta[t][i];
            sum += model->gamma[t][i];
        }
        for( uint8_t i = 0; i < NUM_STATES; i++ )
            model->gamma[t][i] /= sum;
    }
}

void UpdateXiHMM( hidden_markov_model_t * model )
{
    memset( model->xi, 0., sizeof(model->xi) );
    uint8_t T = model->O.length;
    hmm_observation_t o;
    double p;
    
    /* Source - https://en.wikipedia.org/wiki/Baum–Welch_algorithm#cite_note-6 */
    /* ξ_{ij}(t) = ( α_i(t) x A_{ij} x β_j(t+1) x B_j(y_{t+1}) ) / ( ∑_{i=1}^N ∑_{j=1)^N ( α_i(t) x A_{ij} x β_j(t+1) x B_j(y_{t+1}) ) ) */
    for( uint8_t t = 0; t < T; t++ )
    {
        double sum = 0;
        for( uint8_t i = 0; i < NUM_STATES; i++ )
        {
            for( uint8_t j = 0; j < NUM_STATES; j++ )
            {
                o = GetIndexObservationBuffer( &model->O, t + 1 );
                p = GetProbabilityFromEmission( &model->B[j], o );
                model->xi[i][t][j] = model->alpha[t][i] * model->A[i][j] * model->beta[t + 1][j] * p;
                sum += model->xi[i][t][j];
            }
        }
        if( sum == 0 ) continue;
        for( uint8_t i = 0; i < NUM_STATES; i++ )
            for( uint8_t j = 0; j < NUM_STATES; j++ )
                model->xi[i][t][j] /= sum;
    }
}

double UpdateProbabilityHMM( hidden_markov_model_t * model )
{
    /* Source - http://www.utstat.toronto.edu/~rsalakhu/sta4273/notes/Lecture11.pdf - Computing Likelihood */
    double sum = 0;
    uint8_t T = model->O.length;
    
    /* P(T) = ∑_{i=1}^N α_i(T) */
    for( uint8_t i = 0; i < NUM_STATES; i++ )
        sum += model->alpha[T - 1][i];
    return sum;
}

void UpdateInitialProbabilitiesHMM( hidden_markov_model_t * model )
{
    /* Source - https://en.wikipedia.org/wiki/Baum–Welch_algorithm#cite_note-6 */
    /* π_i^* = γ_i(0) */
    for( uint8_t i = 0; i < NUM_STATES; i++ )
        model->pi[i] = SOFTEN( model->gamma[0][i] );
}

void UpdateTransitionProbabilitiesHMM( hidden_markov_model_t * model )
{
    uint8_t T = model->O.length;
    double n = 0, d = 0;
    
    /* Source - https://en.wikipedia.org/wiki/Baum–Welch_algorithm#cite_note-6 */
    /* A_{ij}^* = ( ∑_{t=1}^{T-1} ξ_{ij}(t) ) / ( ∑_{t=1}^{T-1} γ_i(t) ) */
    for( uint8_t i = 0; i < NUM_STATES; i++ )
    {
        for( uint8_t j = 0; j < NUM_STATES; j++ )
        {
            n = 0; d = 0;
            for( uint8_t t = 0; t < T - 1; t++ )
            {
                n += model->xi[i][t][j];
                d += model->gamma[t][i];
            }
            model->A[i][j] = SOFTEN( ZDIV( n, d ) );
        }
    }
}

void UpdateEmissionProbabilitiesHMM( hidden_markov_model_t * model )
{
#ifdef HMM_GAUSSIAN_EMISSIONS
    /* Source - https://sambaiga.github.io/ml/hmm/2017/06/12/hmm-gausian.html */
    /* P( y_t | s_t=i ) = Ν(nu)( y_t | µ_i, ∑_i ) */
    
    uint8_t T = model->O.length;
    double gamma_sum = 0., inv_gamma_sum;
    hmm_observation_t o;
#ifdef HMM_2D_EMISSIONS
    vec2 mean_diff = { 0. }, mean_sum = { 0. }, working_vec;
    mat2x2 cov_est = { 0. }, cov_sum = { 0. }, working_mat;
#else
    double mean_diff = 0., sum = 0.;
#endif
    
    for( uint8_t i = 0; i < NUM_STATES; i++ )
    {
        /* µ_i = ( ∑_{t=1}^T γ_i(t) x y(t) ) / ( ∑_{t=1}^T γ_i(t) ) */
#ifdef HMM_2D_EMISSIONS
        mean_sum = (vec2){ 0. };
#else
        sum = 0.;
#endif
        gamma_sum = 0.;
        for( uint8_t t = 0; t < T; t++ )
        {
            o = GetIndexObservationBuffer( &model->O, t );
#ifdef HMM_2D_EMISSIONS
            MatVec.Vec2.ScalarMultiply(model->gamma[t][i], &o, &working_vec);
            MatVec.Vec2.Add( &mean_sum, &working_vec, &mean_sum );
#else
            sum += model->gamma[t][i] * o;
#endif
            gamma_sum += model->gamma[t][i];
        }
#ifdef HMM_2D_EMISSIONS
        inv_gamma_sum = ZDIV( 1, gamma_sum );
        MatVec.Vec2.ScalarMultiply( inv_gamma_sum, &mean_sum, &model->B[i].mean );
#else
        model->B[i].mean = SOFTEN( ZDIV( sum, gamma_sum ) );
#endif
        
        /* ∑_i = ( ∑_{t=1}^T γ_i(t) x [y(t) - µ_i][y(t) - µ_i]^T ) / ( ∑_{t=1}^T γ_i(t) ) */
#ifdef HMM_2D_EMISSIONS
        cov_sum = (mat2x2){ 0. };
#else
        sum = 0.;
#endif
        for( uint8_t t = 0; t < T; t++ )
        {
            o = GetIndexObservationBuffer( &model->O, t );
#ifdef HMM_2D_EMISSIONS
            MatVec.Vec2.Subtract( &o, &model->B[i].mean, &mean_diff );
            MatVec.Vec2.AAT( &mean_diff, &cov_est );
            MatVec.Mat2x2.ScalarMultiply( model->gamma[t][i], &cov_est, &working_mat );
            MatVec.Mat2x2.Add( &cov_sum, &working_mat, &cov_sum );
#else
            mean_diff = (double)o - model->B[i].mean;
            sum += model->gamma[t][i] * mean_diff * mean_diff;
#endif
        }
#ifdef HMM_2D_EMISSIONS
        MatVec.Mat2x2.ScalarMultiply( gamma_sum, &cov_sum, &model->B[i].covariance );
#else
        model->B[i].std_dev = SOFTEN( ZDIV( sum, gamma_sum ) );
#endif
    }
#else
    /* Source - https://en.wikipedia.org/wiki/Baum–Welch_algorithm#cite_note-6 */
    /* B_i^*(v_k) = ( ∑_{t=1}^{T-1} ( 1_{y_t=v_k} x γ_i(t) ) ) / ( ∑_{t=1}^{T-1} γ_i(t) ) */
    for( uint8_t j = 0; j < NUM_STATES; j++ )
    {
        for( uint8_t k = 0; k < NUM_OBSERVATION_SYMBOLS; ++k)
        {
            n = 0; d = 0;
            for( uint8_t t = 0; t < T; ++t )
            {
                o = GetIndexObservationBuffer( &model->O, t );
                if( o == k )
                    n += model->gamma[t][j];
                d += model->gamma[t][j];
            }
            model->B[j][k] = SOFTEN( n / d );
        }
    }
#endif
}

void BaumWelchSolveHMM( hidden_markov_model_t * model, double DELTA )
{
    HMMFunctions.Update.All( model );
    do
    {
        HMMFunctions.Update.Pi( model );
        HMMFunctions.Update.A( model );
        HMMFunctions.Update.B( model );
    } while( HMMFunctions.Update.All( model ) > DELTA );
}

void PrintHMM( hidden_markov_model_t * model )
{
    LOG_HMM(HMM_DEBUG, "\n A:");
    for( uint8_t i = 0; i < NUM_STATES; i++ )
    {
        LOG_HMM_BARE(HMM_DEBUG, "\t[");
        for( uint8_t j = 0; j < NUM_STATES; j++ )
            LOG_HMM_BARE(HMM_DEBUG, "%.3f%s", model->A[i][j], (j==NUM_STATES-1?"":","));
        LOG_HMM_BARE(HMM_DEBUG, "]\n");
    }
    LOG_HMM_BARE(HMM_DEBUG, "\n B:");
    for( uint8_t i = 0; i < NUM_STATES; i++ )
    {
        LOG_HMM_BARE(HMM_DEBUG, "\t[");
#ifdef HMM_2D_EMISSIONS
        LOG_HMM_BARE(HMM_DEBUG, "{%.4f,%.4f}{%.4f,%.4f,%.4f,%.4f}", model->B[i].mean.a, model->B[i].mean.b, model->B[i].covariance.a, model->B[i].covariance.b, model->B[i].covariance.c, model->B[i].covariance.d);
#else
        LOG_HMM_BARE(HMM_DEBUG, "%.4f,%.4f", model->B[i].mean, model->B[i].std_dev);
#endif
        LOG_HMM_BARE(HMM_DEBUG, "]");
        if( !i ) { LOG_HMM_BARE(HMM_DEBUG, " \tP: "); }
        else { LOG_HMM_BARE(HMM_DEBUG, " \t   "); }
        LOG_HMM_BARE(HMM_DEBUG, "[%.4f]\n", model->pi[i]);
    }
    LOG_HMM_BARE(HMM_DEBUG, "\n");
}

#endif
