//
//  rho_utility.hpp
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_utility_hpp
#define rho_utility_hpp

#include <stdio.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "test_setup.h"

/* Global Included */
#include "global_types.h"
#include "test_setup.h"
#include "tau_structures.hpp"
#include "gaussian.hpp"
#include "kalman.hpp"

#include "utility_functions.hpp"

#define RHO_DIM_INFLUENCE   0.1
#define RHO_K_TARGET_IND    0.3
#define RHO_K_TARGET        RHO_K_TARGET_IND+(10/sqrt(FNL_RESIZE_H)*RHO_DIM_INFLUENCE)          //0.3
#define RHO_VARIANCE_NORMAL sqrt(FNL_RESIZE_H)/5.0             //5
#define RHO_VARIANCE_SCALE  sqrt(FNL_RESIZE_H)/3.0//1.32        //20

#define RHO_PUNISH_FACTOR   2

#define RHO_GAP_MAX 10

#define FILTERED_CONVERAGE_TARGET  0.01

static void cma( double new_val, double *avg, int num ) { *avg += ( new_val - *avg ) / (double)( num + 1 ); }
static void cma_M0_M1( double v, double i, double *m0, double *m1, int * n )
{double n_=1/(++(*n));*m0+=(v-*m0)*n_;*m1+=((v*i)-*m1)*n_;}
static void fswap( double *a, double *b ) { double t = (*a); *a = *b; *b = t; }
static void iswap( int *a, int *b ) { int t = (*a); *a = *b; *b = t; }

int matchKalmans( KalmanFilter *, KalmanFilter *, double, double );

class Rho
{
public:
    int                width;
    int                height;
    int                Cx, Cy;
    pthread_mutex_t    c_mutex;
    int                Q[4], QT;
    double             QF, FT;
    cimage_t                image;
    pthread_mutex_t         frame_mutex;
    cv::Mat                 frame;
    DensityMapPair          density_map_pair;
    pthread_mutex_t         density_map_pair_mutex;
    PeakListPair            peak_list_pair;
    Gaussian                gaussian;
    PredictionPair      rho_predictions;
    
    Rho( int, int );
    void perform( cimage_t *, PredictionPair * );
    void perform( cv::Mat, PredictionPair * );
    void generateCenterOfMass( PredictionPair * );
    
    void generateDensityMap();
    void generateDensityMapFromCImage();
    void generateDensityMapFromCImageWithQuadrantMasses();
    void getDensityMaxAndUpdateVarianceThenFilterAnalyzeAndSelectPeakPair( PredictionPair * );
    void getDensityMaxAndUpdateVarianceThenFilterAnalyzeAndSelectPeak( DensityMap *, PeakList *, Prediction * );
    void updatePredictions( PredictionPair * );
};

#endif /* rho_utility_hpp */
