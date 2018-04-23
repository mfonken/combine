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

#define MAX_PEAKS           3

#define RHO_SQRT_HEIGHT     sqrt(FNL_RESIZE_H)
#define RHO_DIM_INFLUENCE   0.1
#define RHO_K_TARGET_IND    0.3
#define RHO_K_TARGET        RHO_K_TARGET_IND+(10/RHO_SQRT_HEIGHT*RHO_DIM_INFLUENCE)          //0.3
#define RHO_VARIANCE_NORMAL RHO_SQRT_HEIGHT/5.0             //5
#define RHO_VARIANCE_SCALE  RHO_SQRT_HEIGHT/3.0//1.32        //20

#define RHO_PUNISH_FACTOR   2

#define RHO_GAP_MAX 10

#define FILTERED_CONVERAGE_TARGET  0.01

#define RHO_PREDICTION_LS   1.0
#define RHO_PREDICTION_VU   0.5
#define RHO_PREDICTION_BU   0.01
#define RHO_PREDICTION_SU   0.001

#define RHO_DEFAULT_LS      5
#define RHO_DEFAULT_VU      0.001
#define RHO_DEFAULT_BU      0.5
#define RHO_DEFAULT_SU      0.7
    
#define MAX_COVERAGE        0.5
#define C_FRAME_SIZE        ((int)(MAX_COVERAGE * ( FNL_RESIZE_W * FNL_RESIZE_H )))
#define Y_DEL               0xaaaaaaaa

static void cma( double new_val, double *avg, int num ) { *avg+=(new_val-*avg)/(double)(num+1); }
static void cma_M0_M1( double v, double i, double *m0, double *m1, int * n ) { double n_=1/(++(*n));*m0+=(v-*m0)*n_;*m1+=((v*i)-*m1)*n_; }
static void fswap( double *a, double *b ) { double t=(*a);*a=*b;*b=t; }
static void iswap( int *a, int *b ) { int t=(*a);*a=*b;*b=t; }

#include "state_machine_utility.h"

typedef struct
{
    DensityMapPairC  density_map_pair;
    PredictionPairC  prediction_pair;
    bayesian_system_t sys;
    int     width,
            height,
            thresh,
            Cx,
            Cy,
            Q[4],
            QT;
    double  QF, FT;

    int cframe[C_FRAME_SIZE];
    pthread_mutex_t rho_int_mutex;
} rho_c_utility;

struct rho_functions
{
    void (*Init)(rho_c_utility *, int, int);
    void (*Generate_Density_Map_Using_Interrupt_Model)( rho_c_utility * utility, cimage_t image );
    void (*Filter_and_Select_Pairs)( rho_c_utility * );
    void (*Filter_and_Select)( rho_c_utility *, DensityMapC *, PredictionC * );
    void (*Update_Prediction)( rho_c_utility * );
};

extern const struct rho_functions RhoFunctions;

#ifdef __cplusplus
}
#endif

#endif
