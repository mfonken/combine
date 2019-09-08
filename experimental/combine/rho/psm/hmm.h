//
//  hmm.h
//  hmmtest
//
//  Created by Matthew Fonken on 2/10/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#ifdef __PSM__

#ifndef hmm_h
#define hmm_h

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif
    
#include "gmm.h"
    
    void InitializeHMM(     hidden_markov_model_t *, transition_matrix_t *, observation_matrix_t *, state_vector_t * );
    uint8_t ReportObservationToHMM( hidden_markov_model_t *, hmm_observation_t );
    void BaumWelchSolveHMM( hidden_markov_model_t *, double );
    void PrintHMM(          hidden_markov_model_t * );
    double UpdateAllHMM(    hidden_markov_model_t * );
    void UpdateAlphaHMM(    hidden_markov_model_t * );
    void UpdateBetaHMM(     hidden_markov_model_t * );
    void UpdateGammaHMM(    hidden_markov_model_t * );
    void UpdateXiHMM(       hidden_markov_model_t * );
    double UpdateProbabilityHMM( hidden_markov_model_t * );
    void UpdateInitialProbabilitiesHMM( hidden_markov_model_t * );
    void UpdateTransitionProbabilitiesHMM( hidden_markov_model_t * );
    void UpdateEmissionProbabilitiesHMM( hidden_markov_model_t * );
    
    typedef struct
    {
        double (*All)(      hidden_markov_model_t * );
        void (*Alpha)(      hidden_markov_model_t * );
        void (*Beta)(       hidden_markov_model_t * );
        void (*Gamma)(      hidden_markov_model_t * );
        void (*Xi)(         hidden_markov_model_t * );
        double (*Probability)( hidden_markov_model_t * );
        void (*Pi)(         hidden_markov_model_t * );
        void (*A)(          hidden_markov_model_t * );
        void (*B)(          hidden_markov_model_t * );
    } hidden_markov_model_update_functions;
    typedef struct
    {
        void   (*Initialize)(         hidden_markov_model_t *, transition_matrix_t *, observation_matrix_t *, state_vector_t * );
        uint8_t (*ReportObservation)( hidden_markov_model_t *, hmm_observation_t );
        void   (*BaumWelchSolve)(     hidden_markov_model_t *, double );
        void   (*Print)(              hidden_markov_model_t * );
        hidden_markov_model_update_functions Update;
    } hidden_markov_model_functions_t;
    
    static const hidden_markov_model_functions_t HMMFunctions =
    {
        .Initialize = InitializeHMM,
        .ReportObservation = ReportObservationToHMM,
        .BaumWelchSolve = BaumWelchSolveHMM,
        .Print = PrintHMM,
        .Update.All = UpdateAllHMM,
        .Update.Alpha = UpdateAlphaHMM,
        .Update.Beta = UpdateBetaHMM,
        .Update.Gamma = UpdateGammaHMM,
        .Update.Xi = UpdateXiHMM,
        .Update.Probability = UpdateProbabilityHMM,
        .Update.Pi = UpdateInitialProbabilitiesHMM,
        .Update.A = UpdateTransitionProbabilitiesHMM,
        .Update.B = UpdateEmissionProbabilitiesHMM
    };
    
#ifdef __cplusplus
}
#endif

#endif /* hmm_h */

#endif
