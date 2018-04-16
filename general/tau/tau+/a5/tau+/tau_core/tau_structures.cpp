//
//  tau_structures.cpp
//  tau+
//
//  Created by Matthew Fonken on 2/8/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "tau_structures.hpp"

void fillDensityMapDataInt( int * m, int * v, int l )
{
    int i, c, p = m[0];
    for( i = 1; i < l; i++ )
    {
        c = m[i];
        v[i] = c - p;
        p = c;
    }
    m[0] = 0;
    v[0] = 0;
}

DensityMapPair::DensityMapPair( int w, int h )
{
    x.map = (int*)malloc(sizeof(int)*h);
    x.fil = (int*)malloc(sizeof(int)*h);
    x.length = 0;
    x.max = 0;
    x.variance = 0;
    x.kalman = KalmanFilter(0);
    
    y.map = (int*)malloc(sizeof(int)*w);
    y.fil = (int*)malloc(sizeof(int)*w);
    y.length = 0;
    y.max = 0;
    y.variance = 0;
    y.kalman = KalmanFilter(0);
}

PeakListPair::PeakListPair()
{
    x.map      = (int*)malloc(sizeof(int)*MAX_PEAKS);
    x.den      = (int*)malloc(sizeof(int)*MAX_PEAKS);
    x.max      = (int*)malloc(sizeof(int)*MAX_PEAKS);
    x.length   = 0;
    y.map      = (int*)malloc(sizeof(int)*MAX_PEAKS);
    y.den      = (int*)malloc(sizeof(int)*MAX_PEAKS);
    y.max      = (int*)malloc(sizeof(int)*MAX_PEAKS);
    y.length   = 0;
}

Prediction::Prediction()
{
    primary   = KalmanFilter(0., PREDICTION_LIFESPAN, PREDICTION_VALUE_UNCERTAINTY, PREDICTION_BIAS_UNCERTAINTY, PREDICTION_SENSOR_UNCERTAINTY);
    secondary = KalmanFilter(0., PREDICTION_LIFESPAN, PREDICTION_VALUE_UNCERTAINTY, PREDICTION_BIAS_UNCERTAINTY, PREDICTION_SENSOR_UNCERTAINTY);
    primary_new = 0.;
    secondary_new = 0.;
    probabilities = {0.,0.,0.};
}
