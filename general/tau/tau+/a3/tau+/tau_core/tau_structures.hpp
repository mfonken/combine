//
//  tau_structures.hpp
//  tau+
//
//  Created by Matthew Fonken on 2/8/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef tau_structures_hpp
#define tau_structures_hpp

#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "test_setup.h"
#include "global_types.h"
#include "kalman.hpp"

#define ABSINV(X)   ((X<1)?1-X:X-1)
#define ABS(X)      ((X>=0)?X:-X)

#define GTTHR(X,Y,T)    X>=(Y+T)
#define INRANGE(X,Y,T)  (X>(Y-T)&&X<(Y+T))

#define MAX_PERSISTENCE ( 1 << 7 ) - 1 // MAX OF SIGNED BYTE

static inline double timeDiff( struct timeval a, struct timeval b )
{ return ((b.tv_sec  - a.tv_sec) + (b.tv_usec - a.tv_usec)/1000000.0) + 0.0005; }


typedef int             image_dimension_base_t;
typedef int             pixel_base_t;
typedef struct
{
    int width, height;
    pixel_base_t * pixels;
}cimage_t;

static double pixelDist(cv::Vec3b p) { return sqrt(p[0]*p[0] + p[1]*p[1] + p[2]*p[2]); }
//    static double thresh(cv::Vec3b p) { return pixelDist(p); }
static void cimageInit( cimage_t * img, int width, int height )
{
    img->width = width;
    img->height = height;
    img->pixels = (pixel_base_t *)malloc(sizeof(pixel_base_t)*width*height);
}
static void cimageFromMat( cv::Mat mat, cimage_t * img )
{
    int w = mat.cols, h = mat.rows, d = 0;
    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            d = x + y*h;
            img->pixels[d] = pixelDist(mat.at<cv::Vec3b>(y,x));
        }
    }
}
static void cimageToMat( cimage_t * img, cv::Mat mat )
{
    int w = img->width, h = img->height;
    unsigned char d = 0;
    mat.cols = w;
    mat.rows = h;
    for(int y = 0; y < h; y++)
    {
        for(int x = 0; x < w; x++)
        {
            d = (char)img->pixels[x + y*h];
            cv::Vec3b p = {d,d,d};
            mat.at<cv::Vec3b>(y,x) = p;
        }
    }
}

typedef enum
{
    UNWEIGHTED = 0,
    WEIGHTED = 1
} sorting_settings;

/* Tau filter types specifically for kalman matrix structure */
typedef enum
{
    NO_FILTER = 0,
    SOFT_FILTER,
    HARSH_FILTER,
    CHAOS_FILTER
} filter_t;

/* Stability tracking for selec tions */
class Stability
{
public:
    double primary;
    double secondary;
    double alternate;
    double overall;
};

#define PREDICTION_LIFESPAN             1.0
#define PREDICTION_VALUE_UNCERTAINTY    0.5
#define PREDICTION_BIAS_UNCERTAINTY     0.01
#define PREDICTION_SENSOR_UNCERTAINTY   0.001

class Prediction
{
public:
    image_dimension_base_t  primary;
    image_dimension_base_t  secondary;
    double  primary_probability,
            secondary_probability,
            alternate_probability;
    
    KalmanFilter    a, b;
    
    Prediction();
};

typedef enum
{
    SIMILAR = 0,
    OPPOSITE
} selection_pair_t;

class PredictionPair
{
public:
    Prediction x;
    Prediction y;
    selection_pair_t        selection_pair;
};

/* RHO */
class DensityMap
{
public:
    int *      map;
    int *      fil;
    int        length;
    int        max;
    double          variance;
    KalmanFilter    kalman;
};

class DensityMapPair
{
public:
    DensityMap   x;
    DensityMap   y;
    
    DensityMapPair( int, int );
};

class PeakList
{
public:
    int     length;
    int*    map;
    int*    den;
    int*    dir;
    int*    max;
};

class PeakListPair
{
public:
    PeakList x;
    PeakList y;
    
    PeakListPair();
};

/* SIGMA */

#endif /* tau_structures_hpp */
