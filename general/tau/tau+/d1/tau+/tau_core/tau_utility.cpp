//
//  tau_utility.cpp
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "tau_utility.hpp"
#include "utility_functions.hpp"

using namespace cv;

static void cma( double new_val, double *avg, int num ) { *avg += ( new_val - *avg ) / (double)( num + 1 ); }

void Tau::init( void )
{
    thresh = THRESHOLD_MIN;
    count = 0;
    avg = 0;
}
void Tau::trigger( void )
{
    pthread_mutex_lock(&utility->outimage_mutex);
    double p = perform( utility->outframe );
    pthread_mutex_unlock(&utility->outimage_mutex);
    
    if(count < MAX_COUNT)
        cma(p, &avg, ++count);
}

std::string Tau::serialize( void )
{
    return "";
}

Tau::Tau( std::string name, ImageUtility * util, int width, int height ) : rhodetector(width, height)
{
    this->name = name;
    this->utility = util;
    this->width = width;
    this->height = height;
    cimageInit(&image, width, height);
    pthread_mutex_init(&predictions_mutex, NULL);
    
    predictions.x.primary.value     = FNL_RESIZE_W_HALF;
    predictions.x.secondary.value   = FNL_RESIZE_W_HALF;
    predictions.y.primary.value     = FNL_RESIZE_H_HALF;
    predictions.y.secondary.value   = FNL_RESIZE_H_HALF;
}

double Tau::perform( Mat img )
{
    struct timeval a,b;
    gettimeofday( &a, NULL);
    rhodetector.perform( img, &predictions );
    sys.update( &predictions );
    updateThresh();
    updatePrediction();
    gettimeofday( &b, NULL);
    return timeDiff(a,b);
}

void Tau::updateThresh()
{
    int thresh = utility->thresh;
    switch(sys.state)
    {
        case STABLE_MANY:
            thresh += THRESH_STEP;
            break;
        case STABLE_NONE:
            thresh -= THRESH_STEP*2;
            break;
        case STABLE_SINGLE:
            thresh -= THRESH_STEP;
            break;
        default:
            break;
    }
    if(thresh < THRESHOLD_MIN) thresh = THRESHOLD_MIN;
    else if(thresh >THRESHOLD_MAX) thresh = THRESHOLD_MAX;
    if(thresh != utility->thresh)
    {
        utility->thresh = thresh;
#ifdef TAU_DEBUG
        printf("*** THRESH IS %d ***\n", thresh);
#endif
    }
}

void Tau::updatePrediction()
{
    Point2f a((double)predictions.y.primary.value,   (double)predictions.x.primary.value),
            b((double)predictions.y.secondary.value, (double)predictions.x.secondary.value);
    if( a.x > b.x )
    {
        Point2f t = b;
        b = a;
        a = t;
    }
#ifdef FISHEYE
    invfisheye(&a, FNL_RESIZE_W, FNL_RESIZE_H, STRENGTH, ZOOM);
    invfisheye(&b, FNL_RESIZE_W, FNL_RESIZE_H, STRENGTH, ZOOM);
#endif
    
    AxKalman.update(a.x, a.x - AxKalman.value);
    AyKalman.update(a.y, a.y - AyKalman.value);
    BxKalman.update(b.x, b.x - BxKalman.value);
    ByKalman.update(b.y, b.y - ByKalman.value);
    
    A = { AxKalman.value, AyKalman.value };
    B = { BxKalman.value, ByKalman.value };
}

