//
//  tau.c
//  tau+
//
//  Created by Matthew Fonken on 8/25/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "tau.h"

peak_list_pair_t  peaks;
prediction_pair_t locations;
prediction_pair_t densities;

void initTauA( uint16_t width, uint16_t height )
{
    locations.x.primary     = DEFAULT_X_PRIMARY_LOCATION;
    locations.x.secondary   = DEFAULT_X_SECONDARY_LOCATION;
    locations.y.primary     = DEFAULT_Y_PRIMARY_LOCATION;
    locations.y.secondary   = DEFAULT_Y_SECONDARY_LOCATION;
    
    densities.x.primary     = DEFAULT_X_PRIMARY_DENSITY;
    densities.x.secondary   = DEFAULT_X_SECONDARY_DENSITY;
    densities.y.primary     = DEFAULT_Y_PRIMARY_DENSITY;
    densities.y.secondary   = DEFAULT_Y_SECONDARY_DENSITY;
    
    initPeaksListPair(&peaks, width, height);
    initRho(                  width, height );
    initSigma(                width, height );
}

void performTauA(double * times, pixel_base_t ** img, prediction_pair_t * result )
{
    struct timeval a,b,c;
    gettimeofday(&a, NULL);
    performRho( img, &peaks );
    gettimeofday(&b, NULL);
    performSigma(    &peaks, result);
    gettimeofday(&c, NULL);
    times[0] = timeDiff(a,b);
    times[1] = timeDiff(b,c);
}

void updateLocations( prediction_pair_t * n )
{
    locations = *n;
}
void getDensityMap( pixel_base_t ** img, density_map_pair_t * result )
{
    generateDensityMap(img, result);
}
void getPeakList( density_map_pair_t * map, peak_list_pair_t * peaks )
{
    generatePeakListPair(map, peaks);
}
