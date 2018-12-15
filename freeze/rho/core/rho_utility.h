//
//  rho_utility.h
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_utility_h
#define rho_utility_h

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "rho_structure.h"

#include "global_config.h"
#include "state_machine_utility.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct
    {
        index_t             Width,
        Height,
        Cx,
        Cy,
        Bx,
        By;
        byte_t              BackgroundCounter,
                            BackgroundPeriod;
        density_map_pair_t  DensityMapPair;
        prediction_pair_t   PredictionPair;
        bayesian_system_t   BayeSys;
        rho_pid_t           ThreshFilter;
        rho_kalman_t        TargetFilter;
        
        byte_t *            Thresh;
        density_2d_t        Q[4],
        Qb[4],
        Qf[4],
        QbT,
        TotalCoverage,
        FilteredCoverage,
        TargetCoverage;
        floating_t          FilteredPercentage,
        TargetCoverageFactor,
        CoverageFactor,
        VarianceFactor;
        packet_t            Packet;
    } rho_utility;
    
    void InitRhoUtility(                   rho_utility * );
    void PerformRhoUtility(                rho_utility *, bool );
    void GenerateRhoUtilityBackground(     rho_utility * );
    void RedistributeRhoUtilityDensities(  rho_utility * );
    void FilterAndSelectRhoUtilityPairs(   rho_utility * );
    void FilterAndSelectRhoUtility(        rho_utility *, density_map_t *, prediction_t * );
    void UpdateRhoUtilityPrediction(       rho_utility *, prediction_t * );
    void UpdateRhoUtilityPredictions(      rho_utility * );
    void UpdateRhoUtilityThreshold(        rho_utility * );
    void GenerateRhoUtilityPacket(         rho_utility * );
    
    struct rho_functions
    {
        void (*Init)(                   rho_utility * );
        void (*Perform)(                rho_utility *, bool );
        void (*GenerateBackground)(     rho_utility * );
        void (*RedistributeDensities)(  rho_utility * );
        void (*FilterAndSelectPairs)(   rho_utility * );
        void (*FilterAndSelect)(        rho_utility *, density_map_t *, prediction_t * );
        void (*UpdatePrediction)(       rho_utility *, prediction_t * );
        void (*UpdatePredictions)(      rho_utility * );
        void (*UpdateThreshold)(        rho_utility * );
        void (*GeneratePacket)(         rho_utility * );
    };
    
    typedef struct
    {
        index_t
        xl[3],
        yl[3];
        density_2d_t
        area[9];
        
        density_2d_t a, b, c, d, l, l_, p, q, x, y;
    } redistribution_variables;
    
    typedef struct
    {
        index_t
        len,
        cloc,
        gapc,
        width,
        blbi,            /* Blob index */
        blbf;            /* Blob fill */
        density_t
        fpeak,
        fpeak_2,
        fbandl,
        c1,
        c2,
        b,
        p;
        variance_t
        fvar;
        density_2d_t
        cmax,
        //        amax,
        cden,
        tden,
        fden;
        bool
        has,
        sel,
        resc; /* Rescore */
        floating_t
        avgc,
        fcov,   /* Filtered coverage ratio */
        cavg,   /* cumulative average */
        mavg,   /* moment average */
        afac,   /* alternate factor */
        bfac,   /* absence factor */
        pfac,   /* primary factor */
        sfac,   /* secondary factor */
        cfac,   /* coverage factor */
        vfac,   /* variance factor */
        fdn_,   /* filtered density (float) */
        tdnf,   /* target density (float) */
        fvf_,   /* filtered variance inverse (float) */
        chaos,
        denp,
        pkp;
    } rho_selection_variables;
    
    typedef struct
    {
        index_t
        Ax,
        Ay,
        Bx,
        By,
        Cx,
        Cy;
        floating_t
        x,
        y;
        byte_t non_diag;
        int8_t qcheck;
    } prediction_update_variables;
    
    typedef struct
    {
        packet_t * packet;
        address_t pdPtr,
        llPtr,
        * alPtr;
        byte_t
        includes,
        i,
        j,
        l,
        t;
    } packet_generation_variables;
    
#define PACKET_ADDRESS_INITIALIZER(r)           \
    {                                               \
        (address_t)&r.y.TrackingFilters[0].value,         /* px */ \
        (address_t)&r.x.TrackingFilters[0].value,         /* py */ \
        (address_t)&r.y.TrackingFilters[1].value,       /* sx */ \
        (address_t)&r.x.TrackingFilters[1].value,       /* sy */ \
        (address_t)&r.Probabilities.P[1],   /* pp */ \
        (address_t)&r.Probabilities.P[2], /* ap */ \
        (address_t)&r.Probabilities.P[3]  /* ap */ \
    }
    
    extern const density_redistribution_lookup_t rlookup;
    /* Rho global functions */
    static const struct rho_functions RhoFunctions =
    {
        .Init = InitRhoUtility,
        .Perform = PerformRhoUtility,
        .GenerateBackground = GenerateRhoUtilityBackground,
        .RedistributeDensities = RedistributeRhoUtilityDensities,
        .FilterAndSelectPairs = FilterAndSelectRhoUtilityPairs,
        .FilterAndSelect = FilterAndSelectRhoUtility,
        .UpdatePrediction = UpdateRhoUtilityPrediction,
        .UpdatePredictions = UpdateRhoUtilityPredictions,
        .UpdateThreshold = UpdateRhoUtilityThreshold,
        .GeneratePacket = GenerateRhoUtilityPacket
    };
    
#ifdef __cplusplus
}
#endif

#endif

