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
#include "rho_c_types.h"
    
#include "rho_interrupt_model.h"

//#define MAX_PEAKS           3

#define RHO_PUNISH_FACTOR   1

#define RHO_GAP_MAX 10

#define ALTERNATE_TUNING_FACTOR    0.5
#define FILTERED_COVERAGE_TARGET   0.004
#define FILTERED_COVERAGE_PIXELS   ((int)(FILTERED_COVERAGE_TARGET*FRAME_SIZE))
    
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
    
#define MAX_COVERAGE        1
#define FRAME_SIZE          ( FNL_RESIZE_W * FNL_RESIZE_H )
#define C_FRAME_SIZE        ((int)(MAX_COVERAGE * FRAME_SIZE))
#define Y_DEL               0xaaaaaaaa
    
#define BACKGROUND_PERCENT_MIN  0.02
#define BACKGROUND_COVERAGE_MIN ((int)(BACKGROUND_PERCENT_MIN*FRAME_SIZE))
#define BACKGROUND_COVERAGE_TOL_PR   0.001
#define BACKGROUND_COVERAGE_TOL_PX   ((int)(BACKGROUND_COVERAGE_TOL_PR*FRAME_SIZE))

static void cma( double new_val, double *avg, int num )
    {
        *avg+=(new_val-*avg)/(double)(num+1);
    }
static void cma_M0_M1( double v, double i, double *m0, double *m1, int * n )
    {
        double n_=1/((double)(++(*n)));
        *m0+=((v-*m0)*n_);
        *m1+=(((v*i)-*m1)*n_);
    }
static void iswap( int *a, int *b ) { int t=(*a);*a=*b;*b=t; }

#include "state_machine_utility.h"

typedef struct
{
    DensityMapPairC     density_map_pair,
                        background_map_pair;
    PredictionPairC     prediction_pair;
    bayesian_system_t   sys;
    rho_kalman_t        thresh_filter;
    int     width,
            height,
            thresh,
            Cx,
            Cy,
            Bx,
            By,
            Q[4],
            Qb[4],
            Qf[4],
            QT,
            QbT;
    double  QF, FT;

    int cframe[C_FRAME_SIZE];
    pthread_mutex_t rho_int_mutex;
} rho_c_utility;
     
typedef struct
{
    int area[9];
} area_list_t;

typedef struct
{
    unsigned char
        background[4][4],
        current[4][4],
        factor[4][4],
        length[4];
} density_redistribution_lookup_config_t;

typedef struct
{
    density_redistribution_lookup_config_t config[4];
} density_redistribution_lookup_t;

struct rho_functions
{
    void (*Init)(                                       rho_c_utility *, int, int);
    void (*Generate_Density_Map_Using_Interrupt_Model)( rho_c_utility *, cimage_t, bool );
    void (*Redistribute_Densities)(                     rho_c_utility *, const density_redistribution_lookup_t * );
    void (*Filter_and_Select_Pairs)(                    rho_c_utility * );
    void (*Filter_and_Select)(                          rho_c_utility *, DensityMapC *, DensityMapC *, PredictionC * );
    void (*Update_Prediction)(                          rho_c_utility * );
    int  (*Update_Threshold)(                           rho_c_utility * );
};

extern const density_redistribution_lookup_t rlookup;
extern const struct rho_functions RhoFunctions;

#ifdef __cplusplus
}
#endif

#endif
