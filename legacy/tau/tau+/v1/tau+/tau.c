//
//  tau.c
//  tau+
//
//  Created by Matthew Fonken on 8/25/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "tau.h"

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
    
    initRho( width, height );
    initSigma( width, height, &locations );
}

void performTauA( pixel_base_t ** img, prediction_pair_t * result )
{
    prediction_pair_t prediction;
    prediction = locations;
    performRho(img, &prediction, &densities );
    
//    printf("Rho:\n\tX l:pri>%d & sec>%d, p:pri>%d & sec>%d\n\tY l:pri>%d & sec>%d, p:pri>%d & sec>%d\n", locations.x.primary, locations.x.secondary, prediction.x.primary, prediction.x.secondary, locations.y.primary, locations.y.secondary, prediction.y.primary, prediction.y.secondary );
    
    performSigma( &prediction, &locations, result);
    updateLocations(result);
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
void getProbabilityList( peak_list_pair_t * peaks, probability_list_pair_t * prob )
{
    generateProbabilityListPair(peaks, prob, &locations, &densities);
}
void getPredictions( peak_list_pair_t * peaks, probability_list_pair_t * prob, prediction_pair_t * pred )
{
    generatePredictions( prob, peaks, pred, &densities );
}
