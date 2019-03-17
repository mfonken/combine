//
//  psm.h
//  hmmtest
//
//  Created by Matthew Fonken on 2/12/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//
// Description: Predictive State Model

#ifndef psm_h
#define psm_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>

#include "gmm.h"

void InitializePSM(                     psm_t * );
void ReportObservationsPSM(             psm_t *, observation_t *, uint8_t );
void UpdatePSM(                         psm_t *, double  );
void UpdateStateBandsPSM(               psm_t *, double , double *, uint8_t );
void UpdateStateBandPSM(                band_list_t *, uint8_t, int8_t, gaussian2d_t * );
void DiscoverStateBandsPSM(             psm_t *, band_list_t * );
uint8_t FindMostLikelyHiddenStatePSM(   psm_t *, uint8_t, double * );
void UpdateBestClusterPSM(              psm_t *, band_list_t * );
uint8_t GetCurrentBandPSM(              psm_t *, band_list_t * );
void GenerateProposalsPSM(              psm_t * );

typedef struct
{
    void (*Initialize)(                     psm_t * );
    void (*ReportObservations)(             psm_t *, observation_t *, uint8_t );
    void (*Update)(                         psm_t *, double  );
    void (*UpdateStateBands)(               psm_t *, double , double *, uint8_t );
    void (*UpdateStateBand)(                band_list_t *, uint8_t, int8_t, gaussian2d_t * );
    void (*DiscoverStateBands)(             psm_t *, band_list_t * );
    uint8_t (*FindMostLikelyHiddenState)(   psm_t *, uint8_t, double * );
    void (*UpdateBestCluster)(              psm_t *, band_list_t * );
    uint8_t (*GetCurrentBand)(              psm_t *, band_list_t * );
    void (*GenerateProposals)(              psm_t * );
} psm_functions_t;

static const psm_functions_t PSMFunctions =
{
    .Initialize = InitializePSM,
    .ReportObservations = ReportObservationsPSM,
    .Update = UpdatePSM,
    .UpdateStateBands = UpdateStateBandsPSM,
    .UpdateStateBand = UpdateStateBandPSM,
    .DiscoverStateBands = DiscoverStateBandsPSM,
    .FindMostLikelyHiddenState = FindMostLikelyHiddenStatePSM,
    .UpdateBestCluster = UpdateBestClusterPSM,
    .GetCurrentBand = GetCurrentBandPSM,
    .GenerateProposals = GenerateProposalsPSM,
};
    
#ifdef __cplusplus
}
#endif

#endif /* psm_h */
