//
//  rho_core.h
//  pcr
//
//  Created by Matthew Fonken on 1/3/19.
//  Copyright © 2019 Matthew Fonken. All rights reserved.
//

#ifndef rho_core_h
#define rho_core_h

#include "rho_utility.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    void InitializeRhoCore(             rho_core_t *, index_t, index_t );
    void PerformRhoCore(                rho_core_t *, bool );
    void DetectRhoCorePairs(            rho_core_t * );
    void DetectRhoCore(                 rho_core_t *, density_map_t *, prediction_t * );
    void UpdateRhoCorePrediction(       prediction_t * );
    void UpdateRhoCorePredictions(      rho_core_t * );
    void UpdateRhoCoreThreshold(        rho_core_t * );
    void GenerateRhoCorePacket(         rho_core_t * );
    
    typedef struct
    {
        void (*Initialize)(             rho_core_t *, index_t, index_t );
        void (*Perform)(                rho_core_t *, bool );
        void (*DetectPairs)(            rho_core_t * );
        void (*Detect)(                 rho_core_t *, density_map_t *, prediction_t * );
        void (*UpdatePrediction)(       prediction_t * );
        void (*UpdatePredictions)(      rho_core_t * );
        void (*UpdateThreshold)(        rho_core_t * );
        void (*GeneratePacket)(         rho_core_t * );
    } rho_core_functions;

    /* Rho global functions */
    static const rho_core_functions RhoCore =
    {
        .Initialize = InitializeRhoCore,
        .Perform = PerformRhoCore,
        .DetectPairs = DetectRhoCorePairs,
        .Detect = DetectRhoCore,
        .UpdatePrediction = UpdateRhoCorePrediction,
        .UpdatePredictions = UpdateRhoCorePredictions,
        .UpdateThreshold = UpdateRhoCoreThreshold,
        .GeneratePacket = GenerateRhoCorePacket
    };
    
#ifdef __cplusplus
}
#endif

#endif /* rho_core_h */
