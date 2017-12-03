//
//  tau.c
//  tau+
//
//  Created by Matthew Fonken on 8/25/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "tau.h"

void initTau( tau_t * t, uint16_t width, uint16_t height )
{
    initPeaksListPair( &t->rho.peak_list_pair, width, height);
    initRho( &t->rho, width, height );
    initSigma( &t->sigma, width, height );
    initSystem( &t->sys );
    t->width = width;
    t->height = height;
}

void deinitTau( tau_t * t )
{
  deinitRho( &t->rho );
}

void performTauA(tau_t * t, double * times, cimage_t img )
{
    struct timeval a,b,c,d;
    gettimeofday( &a, NULL);

    performRho(   &t->rho, img );
    gettimeofday( &b, NULL);

    performSigma( &t->sigma, &t->rho.peak_list_pair, &t->predictions );
    gettimeofday( &c, NULL);

    updateSystem( &t->sys, &t->predictions );
    gettimeofday( &d, NULL);

    times[0] = timeDiff(a,b);
    times[1] = timeDiff(b,c);
    times[2] = timeDiff(c,d);

    if(t->predictions.x.primary > t->predictions.x.secondary)
    {
        double py, px;
        py = t->predictions.y.primary;
        px = t->predictions.x.primary;
        t->predictions.y.primary = t->predictions.y.secondary;
        t->predictions.x.primary = t->predictions.x.secondary;
        t->predictions.y.secondary = py;
        t->predictions.x.secondary = px;
    }
}
