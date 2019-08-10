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

void Tau::init( void )
{
    thresh = THRESHOLD_MIN;
    count = 0;
    avg = 0;
}
void Tau::trigger( void )
{
    pthread_mutex_lock(&utility->outimage_mutex);
    image = utility->outimage;
    double p = perform( &image );
    pthread_mutex_unlock(&utility->outimage_mutex);
    
    if(count < MAX_COUNT)
        cma(p, &avg, ++count);
}

std::string Tau::serialize( void )
{
    return "";
}

Tau::Tau( std::string name, ImageUtility * util, int width, int height ) : rho(width, height)
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

double Tau::perform( cimage_t * img )
{
    struct timeval a,b;
    gettimeofday( &a, NULL);
    rho.perform( img, &predictions );
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
    if(thresh < 0) thresh = 0;
    else if(thresh > 255) thresh = 255;
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
    A = { a.x, a.y };
    B = { b.x, b.y };
}

void Tau::updatePredictionFromPeaks()
{
    Point2f a((double)rho.utility.prediction_pair.x.primary.value,   (double)rho.utility.prediction_pair.y.primary.value  ),
            b((double)rho.utility.prediction_pair.x.secondary.value, (double)rho.utility.prediction_pair.y.secondary.value);
    
    putText(utility->outframe, "A", Point(a.y, a.x), FONT_HERSHEY_PLAIN, 2, Vec3b(0,55,255), 3);
    putText(utility->outframe, "B", Point(b.y, b.x), FONT_HERSHEY_PLAIN, 2, Vec3b(0,255,55), 3);
}
