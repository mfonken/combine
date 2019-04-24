//
//  hmm.c
//  hmmtest
//
//  Created by Matthew Fonken on 2/10/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#include "hmm.h"

void InitializeHMM( hidden_markov_model_t * model )
{
    FSMFunctions.Sys.Initialize( &model->A, UNDER_POPULATED );
    memset( &model->B, 0, sizeof(observation_matrix_t) );
    model->T = 0;
    model->N = NUM_STATE_GROUPS;
    model->M = NUM_OBSERVATION_SYMBOLS;

    double beta = NUM_STATE_GROUPS + 1;
    double half_beta_step = 1 / ( beta );
    
    double row_sum1[NUM_STATE_GROUPS] = {0.};
    for( uint8_t i = 0; i < NUM_STATE_GROUPS; i++ )
    {
        double alpha = (double)i+1;
        double x = half_beta_step;
        for( uint8_t j = 0; j < NUM_STATE_GROUPS; j++, x += 1 / beta )
        {
            double Kij = alpha * beta * pow( x, (alpha - 1) ) * pow( 1 - pow( x, alpha ), beta - 1 );
            model->A.probabilities.map[i][j] = Kij;
            row_sum1[i] += Kij;
        }
    }
    for( uint8_t i = 0; i < NUM_STATE_GROUPS; i++ )
        for( uint8_t j = 0; j < NUM_STATE_GROUPS; j++ )
            model->A.probabilities.map[i][j] /= row_sum1[i];
    
    beta = NUM_OBSERVATION_SYMBOLS + 1;
    half_beta_step = 1 / ( 2 * beta );
    
    double row_sum2[NUM_STATE_GROUPS] = {0.};
    for( uint8_t i = 0; i < NUM_STATE_GROUPS; i++ )
    {
        double alpha = (double)i+1;
        double x = half_beta_step;
        
        for( uint8_t j = 0; j < NUM_OBSERVATION_SYMBOLS; j++, x += 1 / beta )
        {
            double Kij = alpha * beta * pow( x, (alpha - 1) ) * pow( 1 - pow( x, alpha ), beta - 1 );
            model->B.expected[i][j] = Kij;
            row_sum2[i] += Kij;
        }
    }
    for( uint8_t i = 0; i < NUM_STATE_GROUPS; i++ )
        for( uint8_t j = 0; j < NUM_OBSERVATION_SYMBOLS; j++ )
            model->B.expected[i][j] /= row_sum2[i];
    
    double v = 1./(double)NUM_STATE_GROUPS;
    for(uint8_t i = 0; i < NUM_STATE_GROUPS; i++)
        model->p[i] = v;
    
    HMMFunctions.PrintObservationMatrix( model );
}

double ForwardRecursionHMM( hidden_markov_model_t * model, uint8_t i, uint8_t t )
{
    return 0;
//    if( model->alpha[i] == 0 )
//    {
//        double sum = 0.;
//        if( t == model->O.first )
//        {
//            uint8_t o = prev;
//            LOG_HMM(DEBUG_2, "c:%.3f d:%.3f ", model->p[i], model->B.expected[i][o]);
//            return model->p[i] * model->B.expected[i][o];
//
//        }
//        else
//        {
//            uint8_t t_ = t - 1;
//            if( !t ) t_ = MAX_OBSERVATION_MASK;
//            double a, b;
//            for( uint8_t j = 0; j < model->N; j++ )
//            {
//                a = HMMFunctions.ForwardRecursion( model, j, t_ );
//                b = model->A.probabilities.map[j][i];
//                sum += a * b;
//            }
//            uint8_t o = model->O.data[t];
//            model->alpha[i] = sum * model->B.expected[i][o];
//        }
//    }
//    return model->alpha[i];
}

double ForwardSolveHMM( hidden_markov_model_t * model, uint8_t t )
{
    double pr_now_given_model = 0.;
    for( uint8_t i = 0; i < model->N; i++ )
    {
        double check_probability = HMMFunctions.ForwardRecursion( model, i, t - 1);
        if( check_probability > model->best_confidence )
        {
            model->best_confidence = check_probability;
            model->best_state = i;
        }
        pr_now_given_model += check_probability;
    }
    return pr_now_given_model;
}

double BackwardRecursionHMM( hidden_markov_model_t * model, uint8_t i, uint8_t t )
{
    return 0;
//    if( model->beta[i] == 0 )
//    {
//        double sum = 0.;
//        if( t >= model->O.last )
//            return 1.;
//        else
//        {
//            uint8_t t_ = t + 1;
//            if( !t ) t_ = MAX_OBSERVATION_MASK;
//            double a, b;
//            for( uint8_t j = 0; j < model->N; j++ )
//            {
//                a = HMMFunctions.BackwardRecursion( model, i, t_ );
//                uint8_t o = model->O.data[t_];
//                b = model->A.probabilities.map[i][j] * model->B.expected[j][o];
//                sum += a * b;
//            }
//        }
//        model->beta[i] = sum;
//    }
//    return model->beta[i];
}

double BackwardSolveHMM( hidden_markov_model_t * model, uint8_t t )
{
    double pr_now_given_model = 0.;
    for( uint8_t i = 0; i < model->N; i++ )
    {
        double check_probability = HMMFunctions.BackwardRecursion( model, i, t ) * model->p[i] * model->B.expected[0][i];
        if( check_probability > model->best_confidence )
        {
            model->best_confidence = check_probability;
            model->best_state = i;
        }
        pr_now_given_model += check_probability;
    }
    return pr_now_given_model;
}

void UpdateObservationMatrixHMM( hidden_markov_model_t * model )
{
    /* Gamma matrix update */
    HMMFunctions.BaumWelchGammaSolve( model );
    HMMFunctions.PrintObservationMatrix( model );
    
    /* Observation matrix update */
    for( uint8_t j = 0; j < NUM_STATES; j++ )
    {
        double row_sum = 0.;
        for( uint8_t i = 0; i < NUM_OBSERVATION_SYMBOLS; i++ )
            row_sum += model->G.cumulative_value[i][j];
        
        LOG_HMM(DEBUG_2, "Gamma:\n");
        for( uint8_t i = 0; i < NUM_OBSERVATION_SYMBOLS; i++ )
        {
            LOG_HMM_BARE(DEBUG_2, "|%.4f|", model->G.cumulative_value[i][j]);
            model->B.expected[j][i] = ZDIV( model->G.cumulative_value[i][j], row_sum );
        }
        LOG_HMM_BARE(DEBUG_2, "\n");
    }
}

static double ForwardBackward( hidden_markov_model_t * model, uint8_t k, uint8_t l, uint8_t i, uint8_t j )
{
    double a = model->A.probabilities.map[i][j],
    b = model->B.expected[j][l],
    c = model->p[i],
    d = model->B.expected[i][k];
    return a * b * c * d;
}

void BaumWelchGammaSolveHMM( hidden_markov_model_t * model )
{
    /* Update state expectation matrix */
    uint8_t k = model->O.prev, l = model->O.curr;
//    LOG_HMM(DEBUG_2, "%s,%s  ", k?"E":"N", l?"E":"N");
    for( uint8_t i = 0; i < NUM_STATES; i++ )
    {
        for( uint8_t j = 0; j < NUM_STATES; j++ )
        {            
//            double a = model->A.probabilities.map[i][j],
//            b = model->B.expected[j][l],
//            c = model->p[i],
//            d = model->B.expected[i][k];
            model->Es[k][l][i][j] = ForwardBackward(model, k, l, i, j);//)a * b * c * d;
//            LOG_HMM(DEBUG_2, "%.4f%s", model->Es[k][l][i][j], i&&j?" ":",");
        }
    }
    /* Update maximum expectation matarix */
    bool diff = ( k != l );
    double observation_max = 0.;
    for( uint8_t i = 0; i < NUM_STATES; i++ )
    {
        double curr_max = 0., prev_max = 0., curr = 0., diag = ForwardBackward( model, k, l, i, i );
        if( !diff )
            curr_max = diag;
        else
        {
            // Assume l comes from i
            for( uint8_t j = 0; j < NUM_STATES; j++ )
            {
                curr = ForwardBackward( model, k, l, j, i );// model->Es[k][l][j][i];
                if( curr > curr_max )
                    curr_max = curr;
            }
            
            // Assume k comes from i
            for( uint8_t j = 0; j < NUM_STATES; j++ )
            {
                curr = ForwardBackward( model, k, l, i, j );// model->Es[k][l][i][j];
                if( curr > prev_max )
                    prev_max = curr;
            }
        }
        if( diff )
        {
            model->G.value[k][i] = prev_max;
            model->G.cumulative_value[k][i] += prev_max;
        }
        model->G.value[l][i] = curr_max;
        model->G.cumulative_value[l][i] += curr_max;
        
        SET_MAX(observation_max, curr_max);
    }
    
    /* Update gamma expectation maximums */
    if( diff )
        model->G.maximum[k] += observation_max;
    model->G.maximum[l] += observation_max;

    LOG_HMM_BARE(DEBUG_2, "\n");
}


void BaumWelchTransitionSolveHMM( hidden_markov_model_t * model, uint8_t t )
{
//    /* Expectation matrix update */
//    memset( model->beta, 0., sizeof(model->beta) );
//    double state_expectation = 0., cumulated_expectation = 0., best_expectation = 0.;
//    uint8_t o = model->O.data[t+1];
//    LOG_HMM(DEBUG_2, "%d:\n",o);
//    for( uint8_t i = 0; i < NUM_STATES; i++ )
//    {
//        for( uint8_t j = 0; j < NUM_STATES; j++ )
//        {
//            double a = model->A.probabilities.map[i][j],
//            b = model->B.expected[j][o];
//            LOG_HMM(DEBUG_2, "[%d][%d] a:%.3f b:%.3f ", i, j, a, b);
//            double c = HMMFunctions.ForwardRecursion( model, i, t ),
//            d = HMMFunctions.BackwardRecursion( model, j, t + 1 );
//            state_expectation = a * b * c * d;
//            LOG_HMM(DEBUG_2, " e:%.3f\n", state_expectation);
//            
//            model->E[j][i] = state_expectation;
//            if( state_expectation > best_expectation )
//                best_expectation = state_expectation;
//            cumulated_expectation += state_expectation;
//        }
//    }
//    LOG_HMM(DEBUG_2, "E:");
//    for( uint8_t i = 0; i < NUM_STATES; i++ )
//    {
//        for( uint8_t j = 0; j < NUM_STATES; j++ )
//        {
//            LOG_HMM(DEBUG_2, " %.4f", model->E[j][i] / cumulated_expectation);
//        }
//    }
//    LOG_HMM(DEBUG_2, "\n");
//    
//    
//    prev = model->O.data[model->O.first];
}


void BaumWelchTransitionStepHMM( hidden_markov_model_t * model, uint8_t t )
{
    
}

void BaumWelchSolveHMM( hidden_markov_model_t * model, uint8_t t )
{
//    HMMFunctions.BaumWelchTransitionSolve( model, t );
//    HMMFunctions.BaumWelchObservationSolve( model );
}

void NormalizeObservationMatrixHMM( hidden_markov_model_t * model )
{
//    for( uint8_t j = 0; j < NUM_STATES; j++ )
//    {
//        double row_sum = 0.;
//        for( uint8_t i = 0; i < NUM_OBSERVATION_SYMBOLS; i++ )
//            row_sum += model->B.expected[i][j];
//        for( uint8_t i = 0; i < NUM_OBSERVATION_SYMBOLS; i++ )
//            model->B.expected[i][j] /= row_sum;
//    }
}

void PrintObservationMatrixHMM( hidden_markov_model_t * model )
{
    LOG_HMM(DEBUG_2, "|B|:\n");
    for( uint8_t j = 0; j < NUM_STATES; j++ )
    {
        double row_s = 0;
        for( uint8_t i = 0; i < NUM_OBSERVATION_SYMBOLS; i++ )
        {
            LOG_HMM_BARE(DEBUG_2, "|%.4f|", model->B.expected[j][i]);
            row_s += model->B.expected[j][i];
        }
        LOG_HMM_BARE(DEBUG_2, " = %.3f\n", row_s);
    }
}
