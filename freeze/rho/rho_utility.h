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
    index_t             width,
                        height,
                        Cx,
                        Cy,
                        Bx,
                        By;
    density_map_pair_t  density_map_pair;
    prediction_pair_t   prediction_pair;
    bayesian_system_t   sys;
    rho_kalman_t        thresh_filter,
                        target_filter;
    byte_t *            thresh;
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
        fbandl,
        c1,
        c2,
        b,
        p;
    variance_t
        fvar;
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

static void printPacket( packet_t * p, int l )
{
    printf("Packet Size - %lubytes\n", sizeof(packet_t));
    for(int i = 0; i < sizeof(packet_t); )
    {
        printf("(%02d)", i);
        for( int j = 0; j < l && i < sizeof(packet_t); j++, i++ )
            printf(" 0x%02x", *(byte_t*)(&((byte_t*)&p->header)[i]));
        printf("\n");
    }
    uint32_t px = *(uint32_t*)p->data;
    floating_t pxf = *(floating_t*)p->data;
    uint8_t * ptr = (uint8_t*)&px;
    
    uint8_t pxr[4] = {ptr[3],ptr[2],ptr[1],ptr[0]};
    floating_t pxfr = *(floating_t*)&pxr;
    printf("{%02x}{%02x}{%02x}{%02x} %f:%f\n",ptr[0],ptr[1],ptr[2],ptr[3],pxf,pxfr);
}
    
    
#ifdef __cplusplus
}
#endif

#endif
