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
    
    typedef struct
    {
        index_t
                            Width,
                            Height,
                            Cx,
                            Cy,
                            Bx,
                            By;
        byte_t
                            ThreshByte,
                            BackgroundCounter,
                            BackgroundPeriod;
        density_2d_t        Q[4],
                            Qb[4],
                            Qf[4],
                            QbT,
                            TotalCoverage,
                            FilteredCoverage,
                            TargetCoverage;
        floating_t
                            FilteredPercentage,
                            TargetCoverageFactor,
                            CoverageFactor,
                            VarianceFactor,
                            Thresh;
        density_map_pair_t  DensityMapPair;
        prediction_pair_t   PredictionPair;
        bayesian_system_t   BayeSys;
        rho_pid_t           ThreshFilter;
        rho_kalman_t        TargetFilter;
        packet_t            Packet;
    
        index_t             cframe[C_FRAME_SIZE];
    } rho_core_t;
    
    void InitRhoCore(                   rho_core_t *, index_t, index_t );
    void PerformRhoCore(                rho_core_t *, bool );
    void GenerateRhoCoreBackground(     rho_core_t * );
    void RedistributeRhoCoreDensities(  rho_core_t * );
    void FilterAndSelectRhoCorePairs(   rho_core_t * );
    void FilterAndSelectRhoCore(        rho_core_t *, density_map_t *, prediction_t * );
    void UpdateRhoCorePrediction(       rho_core_t *, prediction_t * );
    void UpdateRhoCorePredictions(      rho_core_t * );
    void UpdateRhoCoreThreshold(        rho_core_t * );
    void GenerateRhoCorePacket(         rho_core_t * );
    
    typedef struct
    {
        void (*Init)(                   rho_core_t *, index_t, index_t );
        void (*Perform)(                rho_core_t *, bool );
        void (*GenerateBackground)(     rho_core_t * );
        void (*RedistributeDensities)(  rho_core_t * );
        void (*FilterAndSelectPairs)(   rho_core_t * );
        void (*FilterAndSelect)(        rho_core_t *, density_map_t *, prediction_t * );
        void (*UpdatePrediction)(       rho_core_t *, prediction_t * );
        void (*UpdatePredictions)(      rho_core_t * );
        void (*UpdateThreshold)(        rho_core_t * );
        void (*GeneratePacket)(         rho_core_t * );
    } rho_core_functions;

    /* Rho global functions */
    static const rho_core_functions RhoCore =
    {
        .Init = InitRhoCore,
        .Perform = PerformRhoCore,
        .GenerateBackground = GenerateRhoCoreBackground,
        .RedistributeDensities = RedistributeRhoCoreDensities,
        .FilterAndSelectPairs = FilterAndSelectRhoCorePairs,
        .FilterAndSelect = FilterAndSelectRhoCore,
        .UpdatePrediction = UpdateRhoCorePrediction,
        .UpdatePredictions = UpdateRhoCorePredictions,
        .UpdateThreshold = UpdateRhoCoreThreshold,
        .GeneratePacket = GenerateRhoCorePacket
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
    extern const density_redistribution_lookup_t rlookup;
    
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
        p,
        rcal_c; /* Recalculation counter */
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
        resc, /* Rescore */
        rcal; /* Recalculate */
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
    
#ifdef __cplusplus
}
#endif

#endif /* rho_core_h */
