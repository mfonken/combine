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
    FSMFunctions.Sys.Initialize( &model->A );
    memset( &model->B, 0, sizeof(observation_matrix_t) );
}
static uint8_t prev = 0;
double ForwardRecursionHMM( hidden_markov_model_t * model, uint8_t i, uint8_t t )
{
    if( model->alpha[i] == 0 )
    {
        double sum = 0.;
        if( t == model->O.first )
        {
            uint8_t o = prev;
            printf("c:%.3f d:%.3f ", model->p[i], model->B.expected[i][o]);
            return model->p[i] * model->B.expected[i][o];
            
        }
        else
        {
            uint8_t t_ = t - 1;
            if( !t ) t_ = MAX_OBSERVATION_MASK;
            double a, b;
            for( uint8_t j = 0; j < model->N; j++ )
            {
                a = HMMFunctions.ForwardRecursion( model, j, t_ );
                b = model->A.probabilities.map[j][i];
                sum += a * b;
            }
            uint8_t o = model->O.data[t];
            model->alpha[i] = sum * model->B.expected[i][o];
        }
    }
    return model->alpha[i];
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
    if( model->beta[i] == 0 )
    {
        double sum = 0.;
        if( t >= model->O.last )
            return 1.;
        else
        {
            uint8_t t_ = t + 1;
            if( !t ) t_ = MAX_OBSERVATION_MASK;
            double a, b;
            for( uint8_t j = 0; j < model->N; j++ )
            {
                a = HMMFunctions.BackwardRecursion( model, i, t_ );
                uint8_t o = model->O.data[t_];
                b = model->A.probabilities.map[i][j] * model->B.expected[j][o];
                sum += a * b;
            }
        }
        model->beta[i] = sum;
    }
    return model->beta[i];
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

void UpdateObservationMatrixHMM( hidden_markov_model_t * model, uint8_t t )
{
//    /* Observation matrix update */
//    uint8_t o = model->O.data[t];
//    double current_gamma;
//    for( uint8_t i = 0; i < NUM_STATES; i++ )
//    {
//        current_gamma = model->G[i];
//        model->B.observed[i][o] += current_gamma;
//        model->B.gamma[i] += current_gamma;
//    }
//    for( uint8_t i = 0; i < NUM_STATES; i++ )
//    {
//        current_gamma = model->B.gamma[i];
//        double row_sum = 0.;
//        for( uint8_t j = 0; j < NUM_OBSERVATION_SYMBOLS; j++ )
//        {
//            model->B.expected[i][j] = ZDIV( model->B.observed[i][j], current_gamma );
//            row_sum += model->B.expected[i][j];
//        }
//        for( uint8_t j = 0; j < NUM_OBSERVATION_SYMBOLS; j++ )
//        {
//            model->B.expected[i][j] /= row_sum;
//        }
//    }
}

void BaumWelchTransitionSolveHMM( hidden_markov_model_t * model, uint8_t t )
{
    /* Expectation matrix update */
    memset( model->beta, 0., sizeof(model->beta) );
    double state_expectation = 0., cumulated_expectation = 0., best_expectation = 0.;
    uint8_t o = model->O.data[t+1];
    printf("%d:\n",o);
    for( uint8_t i = 0; i < NUM_STATES; i++ )
    {
        for( uint8_t j = 0; j < NUM_STATES; j++ )
        {
            double a = model->A.probabilities.map[i][j],
            b = model->B.expected[j][o];
            printf("[%d][%d] a:%.3f b:%.3f ", i, j, a, b);
            double c = HMMFunctions.ForwardRecursion( model, i, t ),
            d = HMMFunctions.BackwardRecursion( model, j, t + 1 );
            state_expectation = a * b * c * d;
            printf(" e:%.3f\n", state_expectation);
            
            model->E[j][i] = state_expectation;
            if( state_expectation > best_expectation )
                best_expectation = state_expectation;
            cumulated_expectation += state_expectation;
        }
    }
    printf("E:");
    for( uint8_t i = 0; i < NUM_STATES; i++ )
    {
        for( uint8_t j = 0; j < NUM_STATES; j++ )
        {
            printf(" %.4f", model->E[j][i] / cumulated_expectation);
        }
    }
    printf("\n");

    
    prev = model->O.data[model->O.first];
}

void BaumWelchObservationSolveHMM( hidden_markov_model_t * model )
{
    /* Expectation matrix update */
    double state_expectation = 0.;
    /* Update state expectation matrix */
    uint8_t k = model->O.prev, l = model->O.curr;
    printf("%s,%s  ", k?"E":"N", l?"E":"N");
    for( uint8_t i = 0; i < NUM_STATES; i++ )
    {
        for( uint8_t j = 0; j < NUM_STATES; j++ )
        {
            double a = model->A.probabilities.map[i][j],
            b = model->B.expected[j][l],
            c = model->p[i],
            d = model->B.expected[i][k];
            state_expectation = a * b * c * d;
            model->Es[k][l][i][j] = state_expectation;
            printf("%.4f%s", model->Es[k][l][i][j], i&&j?" ":",");
        }
    }
    /* Update maximum expectation matarix */
    bool diff = ( k != l );
    double observation_max = 0.;
    for( uint8_t i = 0; i < NUM_STATES; i++ )
    {
        double curr_max = 0., prev_max = 0., curr = 0.;
        if( !diff )
        {
            curr_max = model->Es[k][l][i][i];
        }
        else
        {
            // Assume l comes from i
            for( uint8_t j = 0; j < NUM_STATES; j++ )
            {
                curr = model->Es[k][l][j][i];
                if( curr > curr_max )
                    curr_max = curr;
            }
            
            // Assume k comes from i
            for( uint8_t j = 0; j < NUM_STATES; j++ )
            {
                curr = model->Es[k][l][i][j];
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

    printf("\n");
}

void BaumWelchTransitionStepHMM( hidden_markov_model_t * model, uint8_t t )
{
    
}

void BaumWelchSolveHMM( hidden_markov_model_t * model, uint8_t t )
{
//    HMMFunctions.BaumWelchTransitionSolve( model, t );
    HMMFunctions.BaumWelchObservationSolve( model );
}
