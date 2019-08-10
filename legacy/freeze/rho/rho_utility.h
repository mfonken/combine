//
//  rho_utility.h
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_utility_h
#define rho_utility_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#include "test_setup.h"

#include "global_types.h"
#include "rho_types.h"
#include "state_machine_utility.h"
#include "rho_interrupt_model.h"

#define PACKET_HEADER_ID 0xab
    
//#define MAX_PEAKS           3

#define RHO_PUNISH_FACTOR   1

#define RHO_GAP_MAX 10

#define ALTERNATE_TUNING_FACTOR    0.5
#define FILTERED_COVERAGE_TARGET   0.004
#define FILTERED_COVERAGE_PIXELS   ((index_t)(FILTERED_COVERAGE_TARGET*FRAME_SIZE))
    
#define MAX_ABSENCE_PROBABIILITY   0.5
    
#define TUNING_PRESIDENCE       0.75
#define HARD_TUNING_PRESIDENCE  TUNING_PRESIDENCE
#define SOFT_TUNING_PRESIDENCE  (1 - TUNING_PRESIDENCE)
    
#define RHO_PREDICTION_LS   1.0
#define RHO_PREDICTION_VU   0.05
#define RHO_PREDICTION_BU   0.001
#define RHO_PREDICTION_SU   0.01

#define RHO_DEFAULT_LS      5
#define RHO_DEFAULT_VU      0.001
#define RHO_DEFAULT_BU      0.5
#define RHO_DEFAULT_SU      0.7
    
#define RHO_THRESH_LS      10
#define RHO_THRESH_VU      0.5
#define RHO_THRESH_BU      0.01
#define RHO_THRESH_SU      0.05
    
#define RHO_TARGET_LS      5
#define RHO_TARGET_VU      0.5
#define RHO_TARGET_BU      0.01
#define RHO_TARGET_SU      0.05
    
#define MAX_COVERAGE        1
#define FRAME_SIZE          ( FNL_RESIZE_W * FNL_RESIZE_H )
#define C_FRAME_SIZE        ((int)(MAX_COVERAGE * FRAME_SIZE))
#define Y_DEL               0xaaaa
    
#define BACKGROUND_PERCENT_MIN  0.02
#define BACKGROUND_COVERAGE_MIN ((int)(BACKGROUND_PERCENT_MIN*FRAME_SIZE))
#define BACKGROUND_COVERAGE_TOL_PR   0.001
#define BACKGROUND_COVERAGE_TOL_PX   ((int)(BACKGROUND_COVERAGE_TOL_PR*FRAME_SIZE))
    
#define BACKGROUND_CENTROID_CALC_THRESH 10 // pixels
    
#define RHO_SQRT_HEIGHT     sqrt(FNL_RESIZE_H)
#define RHO_DIM_INFLUENCE   0.1
#define RHO_K_TARGET_IND    0.3
#define RHO_K_TARGET        RHO_K_TARGET_IND+(10/RHO_SQRT_HEIGHT*RHO_DIM_INFLUENCE)          //0.3
#define RHO_VARIANCE_NORMAL RHO_SQRT_HEIGHT/5.0             //5
#define RHO_VARIANCE_SCALE  RHO_SQRT_HEIGHT/3.0//1.32        //20
#define RHO_VARIANCE(X)     RHO_VARIANCE_NORMAL * ( 1 + RHO_VARIANCE_SCALE * ( RHO_K_TARGET - X ) );

typedef struct
{
    density_map_pair_t  density_map_pair;
    prediction_pair_t   prediction_pair;
    bayesian_system_t   sys;
    rho_kalman_t        thresh_filter,
                        target_filter;
    index_t             width,
                        height,
                        Cx,
                        Cy,
                        Bx,
                        By;
    density_t           thresh;
    density_2d_t        Q[4],
                        Qb[4],
                        Qf[4],
                        QbT,
                        total_coverage,
                        filtered_coverage,
                        target_coverage;
    floating_t          filtered_percentage,
                        target_coverage_factor,
                        coverage_factor,
                        variance_factor;

    index_t cframe[C_FRAME_SIZE];
    pthread_mutex_t rho_int_mutex;
    packet_t packet;
} rho_c_utility;
    

struct rho_functions
{
    void (*Init)(                       rho_c_utility *, index_t, index_t);
    void (*Perform)(                    rho_c_utility *, bool );
    void (*Generate_Background)(        rho_c_utility * );
    void (*Redistribute_Densities)(     rho_c_utility * );
    void (*Filter_and_Select_Pairs)(    rho_c_utility * );
    void (*Filter_and_Select)(          rho_c_utility *, density_map_t *, prediction_t * );
    void (*Update_Prediction)(          rho_c_utility * );
    void (*Update_Threshold)(           rho_c_utility * );
    void (*Generate_Packet)(            rho_c_utility * );
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
        fvar,
        fbandl,
        c1,
        c2,
        b,
        p;
    density_2d_t
        cmax,
        amax,
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
        x1,
        y1;
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
