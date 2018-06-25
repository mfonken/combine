//
//  rho_utility.h
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_utility_h
#define rho_utility_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "stm32l4xx_hal.h"
#include "rho_types.h"
#include "state_machine_utility.h"

/* Camera Config */
#define CAMERA_WIDTH 	(1280)
#define CAMERA_HEIGHT	(800)

#define DYNAMIC_BUFFER

/* Capture Config */
#define CAPTURE_DIV			2
#define CAPTURE_WIDTH 	(CAMERA_WIDTH>>CAPTURE_DIV)
#define CAPTURE_HEIGHT	(CAMERA_HEIGHT>>CAPTURE_DIV)
#define FRAME_SIZE 			(CAPTURE_WIDTH*CAPTURE_HEIGHT)
#define	CAPTURE_BUFFER_WIDTH	(uint32_t)CAPTURE_WIDTH
#define CAPTURE_BUFFER_HEIGHT (uint32_t)CAPTURE_HEIGHT
#ifdef DYNAMIC_BUFFER
#define CAPTURE_BUFFER_SIZE CAPTURE_BUFFER_WIDTH	//*CAPTURE_BUFFER_HEIGHT
#else
#define CAPTURE_BUFFER_SIZE (CAPTURE_BUFFER_WIDTH*CAPTURE_BUFFER_HEIGHT)
#endif
#define COVERAGE_NORMAL_MAX	0.45
#define THRESH_BUFFER_SIZE 	((uint32_t)((CAPTURE_BUFFER_WIDTH*(CAPTURE_BUFFER_HEIGHT+1.))*COVERAGE_NORMAL_MAX)+2)
#define DEFAULT_THRESH			0xfa//fc

#define RHO_SQRT_HEIGHT     sqrt((float)CAPTURE_HEIGHT)
#define RHO_DIM_INFLUENCE   0.1
#define RHO_K_TARGET_IND    0.3
#define RHO_K_TARGET        RHO_K_TARGET_IND+(10/RHO_SQRT_HEIGHT*RHO_DIM_INFLUENCE)          //0.3
#define RHO_VARIANCE_NORMAL (float)(RHO_SQRT_HEIGHT/5.0)             //5
#define RHO_VARIANCE_SCALE  (float)(RHO_SQRT_HEIGHT/3.0)//1.32        //20
#define RHO_VARIANCE(X)     ( RHO_VARIANCE_NORMAL * ( 1 + RHO_VARIANCE_SCALE * ( RHO_K_TARGET - X ) ) )

#define RHO_PUNISH(X)       (X<<1)

#define RHO_GAP_MAX 2//10

#define FILTERED_CONVERAGE_TARGET  0.00002//0.01

#define MAX_ABSENCE_PROBABILITY		0.5

#define RHO_PREDICTION_LS   1.0
#define RHO_PREDICTION_VU   0.5
#define RHO_PREDICTION_BU   0.01
#define RHO_PREDICTION_SU   0.001

#define RHO_DEFAULT_LS      5
#define RHO_DEFAULT_VU      0.001
#define RHO_DEFAULT_BU      0.5//0.5
#define RHO_DEFAULT_SU      0.01//0.7

#define RHO_THRESH_LS				10
#define RHO_THRESH_VU				0.5
#define RHO_THRESH_BU				0.01
#define RHO_THRESH_SU				0.05

#define MAX_COVERAGE						1
#define BACKGROUND_PERCENT_MIN	0.02
#define BACKGROUND_COVERAGE_MIN	( (uint32_t)( BACKGROUND_PERCENT_MIN * FRAME_SIZE ) )
#define BACKGORUND_COVERAGE_TOL_PR 0.001
#define	BACKGROUND_COVERAGE_TOL_PX ( (uint32_t)( BACKGORUND_COVERAGE_TOL_PR * FRAME_SIZE ) )

#define BACKGROUND_CENTROID_CALC_THRESH 10 // pixels

#define ALTERNATE_TUNING_FACTOR 	0.5
#define FILTERED_COVERAGE_TARGET	0.004
#define FILTERED_COVERAGE_PIXELS	( (uint32_t)( FILTERED_COVERAGE_TARGET*FRAME_SIZE ) )

#define THRESH_STEP	1
#define THRESH_MIN	100
#define THRESH_MAX 	255

static void cma( FLOAT new_val, FLOAT *avg, index_t num ) { *avg+=(new_val-*avg)/(FLOAT)(num+1); }
static void cma_M0_M1( FLOAT v, FLOAT i, FLOAT *m0, FLOAT *m1, index_t * n )
{FLOAT n_=1/(++(*n));*m0+=(v-*m0)*n_;*m1+=((v*i)-*m1)*n_;}
static inline void iswap(uw*a,uw*b){uw t=*a;*a=*b;*b=t;}

typedef struct
{
    density_map_pair_t     density_map_pair;
    prediction_pair_t     prediction_pair;
    bayesian_system_t   sys;
    rho_kalman_t        thresh_filter;
    index_t
            width,
            height,
            Cx,
            Cy,
            Bx,
            By;
    byte_t thresh;

    density_t
            Q[4],
            Qb[4],
            Qf[4],
            QT,
            QbT;
    // double  QF, FT;
} rho_utility;

struct rho_functions
{
	void (*Init)( rho_utility *, UART_HandleTypeDef *, index_t, index_t );
  void (*Perform)( rho_utility *, bool );
	void (*Redistribute_Densities)( rho_utility * );
	void (*Generate_Background)( rho_utility * );
	void (*Filter_and_Select_Pairs)( rho_utility * );
	void (*Filter_and_Select)( rho_utility *, density_map_t *, prediction_t * );
	void (*Update_Prediction)( rho_utility * );
  void (*Update_Threshold)( rho_utility * );
};

//extern rho_c_utility utility;
extern const struct rho_functions RhoFunctions;
extern const density_redistribution_lookup_t rlookup;

#endif
