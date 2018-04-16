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
#include "tau_structures.hpp"
#include "gaussian.hpp"
#include "kalman.hpp"

#include "utility_functions.hpp"

#define RHO_K_TARGET        0.3
#define RHO_VARIANCE_NORMAL 5
#define RHO_VARIANCE_SCALE  20

#define RHO_PUNISH_FACTOR   5

#define FILTERED_CONVERAGE_TARGET  0.01

static void cma( double new_val, double *avg, int num ) { *avg += ( new_val - *avg ) / (double)( num + 1 ); }
static void fswap( double *a, double *b ) { double t = (*a); *a = *b; *b = t; }

int matchKalmans( KalmanFilter *, KalmanFilter *, double, double );

class Rho
{
public:
    int                width;
    int                height;
    int                comX, comY;
    int                Q[2], QT;
    double             QF, FT;
    cimage_t                image;
    pthread_mutex_t         frame_mutex;
    cv::Mat                 frame;
    DensityMapPair          density_map_pair;
    pthread_mutex_t         density_map_pair_mutex;
    PeakListPair            peak_list_pair;
    Gaussian                gaussian;
    KalmanFilter        KAx, KAy, KBx, KBy;
    PredictionPair      rho_predictions;
    
    Rho( int, int );
    void perform( cimage_t *, PredictionPair * );
    void perform( cv::Mat, PredictionPair * );
    void generateCenterOfMass( PredictionPair * );
    void generateDensityMap();
    void generateDensityMapFromCImage();
    void generateDensityMapFromCImageWithQuadrantMasses();
    void getDensityMaxPair();
    void getDensityMax( DensityMap * );
    void updateDensityKalmanPair();
    void updateDensityKalman( DensityMap * );
    void filterDensityPair();
    void filterDensity( DensityMap * );
    void analyzeDensityPair();
    void analyzeDensity( DensityMap *, PeakList * );
    void filterAndAnalyzeDensityPair();
    void filterAndAnalyzeDensity( DensityMap *, PeakList * );
    void selectPeakListPair( PredictionPair * );
    void selectPeakList( double, PeakList *, Prediction * );
    void updatePredictions( PredictionPair * );
};

#endif /* rho_utility_hpp */
