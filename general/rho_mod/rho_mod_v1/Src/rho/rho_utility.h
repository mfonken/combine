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

#include "rho_kalman.h"

/* Camera Config */
#define CAMERA_WIDTH 	640
#define CAMERA_HEIGHT	400

/* Capture Config */
#define CAPTURE_DIV			1
#define CAPTURE_WIDTH 	(CAMERA_WIDTH/CAPTURE_DIV)
#define CAPTURE_HEIGHT	(CAMERA_HEIGHT/CAPTURE_DIV)
#define	CAPTURE_BUFFER_WIDTH	6
#define CAPTURE_BUFFER_HEIGHT 6
#define CAPTURE_BUFFER_SIZE (CAPTURE_BUFFER_WIDTH*CAPTURE_BUFFER_HEIGHT)
#define THRESH_BUFFER_SIZE 	(CAPTURE_BUFFER_WIDTH*(CAPTURE_BUFFER_HEIGHT+1))+2
#define DEFAULT_THRESH			0xba
	
#define MAX_PEAKS           3

#define RHO_SQRT_HEIGHT     sqrt(CAPTURE_HEIGHT)
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

static void cma( FLOAT new_val, FLOAT *avg, int num ) { *avg+=(new_val-*avg)/(FLOAT)(num+1); }
static void cma_M0_M1( FLOAT v, FLOAT i, FLOAT *m0, FLOAT *m1, int * n )
{FLOAT n_=1/(++(*n));*m0+=(v-*m0)*n_;*m1+=((v*i)-*m1)*n_;}
static void iswap( int *a, int *b ) { int t=(*a);*a=*b;*b=t; }
    
typedef FLOAT double;

typedef struct
{
    int *       map;
    int         length;
    int         max;
    FLOAT       variance;
    rho_kalman_t kalman;
} DensityMap;

typedef struct
{
    DensityMap x,y;
} DensityMapPair;

typedef struct
{
    FLOAT   primary,
            secondary,
            alternate;
} prediction_probabilities;

typedef struct
{
    rho_kalman_t    primary,
                    secondary;
    FLOAT           primary_new,
                    secondary_new;
    prediction_probabilities probabilities;
} Prediction;

typedef struct
{
    Prediction         x,y;
} PredictionPair;

typedef struct
{
    DensityMapPair  density_map_pair;
    PredictionPair  prediction_pair;
    
    int     width,
            height,
            thresh,
            Cx,
            Cy,
            Q[4],
            QT;
    FLOAT  QF, FT;
} rho_utility;

struct rho_functions
{
	void (*Init)(rho_utility *, int, int);
    void (*Find_Map_Max)( DensityMap * d );
	void (*Filter_and_Select_Pairs)( rho_c_utility * );
	void (*Filter_and_Select)( rho_utility *, DensityMap *, Prediction * );
	void (*Update_Prediction)( rho_utility * );
};

//extern rho_c_utility utility;
extern const struct rho_functions RhoFunctions;

#endif
