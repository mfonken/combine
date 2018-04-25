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
#define CAMERA_WIDTH 	1280
#define CAMERA_HEIGHT	800

#define DYNAMIC_BUFFER

/* Capture Config */
#define CAPTURE_DIV			4
#define CAPTURE_WIDTH 	(CAMERA_WIDTH/CAPTURE_DIV)
#define CAPTURE_HEIGHT	(CAMERA_HEIGHT/CAPTURE_DIV)
#define	CAPTURE_BUFFER_WIDTH	CAPTURE_WIDTH
#define CAPTURE_BUFFER_HEIGHT CAPTURE_HEIGHT
#ifdef DYNAMIC_BUFFER
#define CAPTURE_BUFFER_SIZE CAPTURE_BUFFER_WIDTH
#else
#define CAPTURE_BUFFER_SIZE (CAPTURE_BUFFER_WIDTH*CAPTURE_BUFFER_HEIGHT)
#endif
#define COVERAGE_NORMAL_MAX	0.02
#define THRESH_BUFFER_SIZE 	((uint32_t)((CAPTURE_BUFFER_WIDTH*(CAPTURE_BUFFER_HEIGHT+1.))*COVERAGE_NORMAL_MAX)+2)
#define DEFAULT_THRESH			0x15//fc

#define MAX_PEAKS           3

#define RHO_SQRT_HEIGHT     sqrt(CAPTURE_HEIGHT)
#define RHO_DIM_INFLUENCE   0.1
#define RHO_K_TARGET_IND    0.3
#define RHO_K_TARGET        RHO_K_TARGET_IND+(10/RHO_SQRT_HEIGHT*RHO_DIM_INFLUENCE)          //0.3
#define RHO_VARIANCE_NORMAL RHO_SQRT_HEIGHT/5.0             //5
#define RHO_VARIANCE_SCALE  RHO_SQRT_HEIGHT/3.0//1.32        //20

#define RHO_PUNISH_FACTOR   2

#define RHO_GAP_MAX 2//10

#define FILTERED_CONVERAGE_TARGET  0.00002//0.01

#define RHO_PREDICTION_LS   1.0
#define RHO_PREDICTION_VU   0.5
#define RHO_PREDICTION_BU   0.01
#define RHO_PREDICTION_SU   0.001

#define RHO_DEFAULT_LS      5
#define RHO_DEFAULT_VU      0.001//0.001
#define RHO_DEFAULT_BU      0.5//0.5
#define RHO_DEFAULT_SU      0.01//0.7

static void cma( FLOAT new_val, FLOAT *avg, int num ) { *avg+=(new_val-*avg)/(FLOAT)(num+1); }
static void cma_M0_M1( FLOAT v, FLOAT i, FLOAT *m0, FLOAT *m1, int * n )
{FLOAT n_=1/(++(*n));*m0+=(v-*m0)*n_;*m1+=((v*i)-*m1)*n_;}
static void iswap( int *a, int *b ) { int t=(*a);*a=*b;*b=t; }

typedef struct
{
    density_map_pair_t  density_map_pair;
    prediction_pair_t  	prediction_pair;
		bayesian_system_t		sys;

    index_t width,
            height,
						Cx,
            Cy;
		byte_t
            thresh;
    density_t
            Q[4],
            QT;
    FLOAT  QF, FT;
} rho_utility;

struct rho_functions
{
	void (*Init)(rho_utility *, UART_HandleTypeDef *, int, int);
	void (*Find_Map_Max)( density_map_t * d );
	void (*Filter_and_Select_Pairs)( rho_utility * );
	void (*Filter_and_Select)( rho_utility *, density_map_t *, prediction_t * );
	void (*Update_Prediction)( rho_utility * );
};

//extern rho_c_utility utility;
extern const struct rho_functions RhoFunctions;

#endif
