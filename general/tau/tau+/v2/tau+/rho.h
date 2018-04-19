//
//  rho.h
//  tau+
//
//  Created by Matthew Fonken on 8/22/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#ifndef rho_h
#define rho_h

#include <stdio.h>
    
#define PEAK_HEIGHT 4

/* Global Included */
#include "global_types.h"
#include "ring_buffer.h"
#include "sigma.h"
    
#define MIN_PROX 10
#define DEFAULT_SIGMA 3.0

extern ring_buffer_t peak_buffer;

void initRho( uint16_t img_width, uint16_t img_height );
void getRhoData(density_map_pair_t * d, peak_list_pair_t * p, probability_list_pair_t * r );
void performRho( pixel_base_t ** img,  peak_list_pair_t * p );
void getRhoData( density_map_pair_t * d, peak_list_pair_t * p, probability_list_pair_t * r );
void generateDensityMap( pixel_base_t ** img, density_map_pair_t * density_maps );
void generatePeakListPair( density_map_pair_t * map, peak_list_pair_t * peaks );
void generatePeakList( density_map_t * map, peak_list_t * peaks );
    
void fillDensityMapPairData( density_map_pair_t * m );
void fillDensityMapData( density_map_t * m );
    
void smooth1D( float * arr, uint16_t len );
void gaussianBlur( float * arr, size_t len );
    
void generateGaussian( gaussian_t * g, int l, FLOAT s );
void initGaussian( gaussian_t * g, int l );
void initDensityMapPair( density_map_pair_t * m, int w, int h );
void initPeaksListPair( peak_list_pair_t * p, int w, int h );
void initProbabilityListPair( probability_list_pair_t * r, int w, int h );
    
extern prediction_pair_t last_locations;
extern prediction_pair_t last_densities;

#endif /* rho_h */
    
#ifdef __cplusplus
}
#endif
