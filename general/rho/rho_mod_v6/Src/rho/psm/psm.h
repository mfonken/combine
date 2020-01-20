//
//  psm.h
//  hmmtest
//
//  Created by Matthew Fonken on 2/12/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//
// Description: Predictive State Model

#ifdef __PSM__

#ifndef psm_h
#define psm_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "hmm.h"

    //// TEMP LOCATION
    //#define HEIGHT 700
    //#define WIDTH HEIGHT

#define MAX_CLUSTER_Y_VARIANCE 30

#ifdef __RHO__
#define PSM_OBSERVATION_MAX THRESH_MAX
#else
#define PSM_OBSERVATION_MAX (1<<7)
#endif


    void InitializePSM(                     psm_t *, const char * );
    void ReportObservationsPSM(             psm_t *, observation_list_t *, floating_t, uint8_t );
    void UpdateStateIntervalsPSM(           psm_t * );//, floating_t );
    void UpdatePSM(                         psm_t * );//, observation_list_t *, floating_t, uint8_t );
    void UpdateStateBandPSM(                band_list_t *, uint8_t, int8_t, gaussian2d_t * );
    void DiscoverStateBandsPSM(             psm_t *, band_list_t * );
    uint8_t FindMostLikelyHiddenStatePSM(   psm_t *, uint8_t, floating_t * );
    void UpdateBestClusterPSM(              psm_t *, band_list_t * );
    uint8_t GetCurrentBandPSM(              psm_t *, band_list_t * );
    void GenerateProposalsPSM(              psm_t * );

    typedef struct
    {
        void (*Initialize)(                     psm_t *, const char * );
        void (*ReportObservations)(             psm_t *, observation_list_t *, floating_t, uint8_t );
        void (*UpdateStateIntervals)(           psm_t * );//, floating_t );
        void (*Update)(                         psm_t * );//, observation_list_t *, floating_t, uint8_t );
        void (*UpdateStateBand)(                band_list_t *, uint8_t, int8_t, gaussian2d_t * );
        void (*DiscoverStateBands)(             psm_t *, band_list_t * );
        uint8_t (*FindMostLikelyHiddenState)(   psm_t *, uint8_t, floating_t * );
        void (*UpdateBestCluster)(              psm_t *, band_list_t * );
        uint8_t (*GetCurrentBand)(              psm_t *, band_list_t * );
        void (*GenerateProposals)(              psm_t * );
    } psm_functions_t;

    static const psm_functions_t PSMFunctions =
    {
        .Initialize                 = InitializePSM,
        .ReportObservations         = ReportObservationsPSM,
        .UpdateStateIntervals       = UpdateStateIntervalsPSM,
        .Update                     = UpdatePSM,
        .UpdateStateBand            = UpdateStateBandPSM,
        .DiscoverStateBands         = DiscoverStateBandsPSM,
        .FindMostLikelyHiddenState  = FindMostLikelyHiddenStatePSM,
        .UpdateBestCluster          = UpdateBestClusterPSM,
        .GetCurrentBand             = GetCurrentBandPSM,
        .GenerateProposals          = GenerateProposalsPSM,
    };

#ifdef __cplusplus
}
#endif

#endif /* psm_h */

#endif
