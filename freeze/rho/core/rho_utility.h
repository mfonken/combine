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
    rho_kalman_t        ThreshFilter,
                        TargetFilter;
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
    

struct rho_functions
{
    void (*Init)(                       rho_utility * );
    void (*Perform)(                    rho_utility *, bool );
    void (*Generate_Background)(        rho_utility * );
    void (*Redistribute_Densities)(     rho_utility * );
    void (*Filter_and_Select_Pairs)(    rho_utility * );
    void (*Filter_and_Select)(          rho_utility *, density_map_t *, prediction_t * );
    void (*Update_Prediction)(          rho_utility * );
    void (*Update_Threshold)(           rho_utility * );
    void (*Generate_Packet)(            rho_utility * );
};
    
typedef struct
{
    density_t
        max,
        den;
    index_t
        loc;
} blob_t;
    
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
        gapc;
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
        sel;
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
        fvf_;   /* filtered variance inverse (float) */
    blob_t
        blobs[2];
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
(address_t)&r.y.primary.value,         /* px */ \
(address_t)&r.x.primary.value,         /* py */ \
(address_t)&r.y.secondary.value,       /* sx */ \
(address_t)&r.x.secondary.value,       /* sy */ \
(address_t)&r.probabilities.primary,   /* pp */ \
(address_t)&r.probabilities.secondary, /* ap */ \
(address_t)&r.probabilities.alternate  /* ap */ \
}
    
extern const density_redistribution_lookup_t rlookup;
extern const struct rho_functions RhoFunctions;    
    
#ifdef __cplusplus
}
#endif

#endif
