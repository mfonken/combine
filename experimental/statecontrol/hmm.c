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

double ForwardRecursionHMM( hidden_markov_model_t * model, uint8_t j, uint8_t t )
{
    if( t == model->O.first ) return  model->p[1] * model->B.map[0][j];
    uint8_t t_ = t - 1;
    if( !t ) t_ = MAX_OBSERVATION_MASK;
    double forward_sum = 0., a, b;
    for( uint8_t i = 0; i < model->N; i++ )
    {
        a = HMMFunctions.ForwardRecursion( model, i, t_ );
        b = model->A.probabilities.map[j][i];
        forward_sum += a * b;
    }
    return forward_sum * model->B.map[model->O.data[j]][t];
}

void ForwardSolveHMM( hidden_markov_model_t * model )
{
    double pr_now_given_model = 0.;
    for( uint8_t i = 0, T = model->T; i < model->N; i++ )
    {
        double check_probability = HMMFunctions.ForwardRecursion( model, i, T - 1);
        if( check_probability > model->best_confidence )
        {
            model->best_confidence = check_probability;
            model->best_state = i;
        }
        pr_now_given_model += check_probability;
    }
}
