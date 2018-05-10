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
    
#include "test_setup.h"

/* Global Included */
#include "global_types.h"
#include "sigma.h"
#include "gaussian.h"
    
void initRho( rho_t * r, uint16_t width, uint16_t height );
void performRho( rho_t * r, cimage_t img );
void generateDensityMap( rho_t * r, cimage_t img );
void generatePeakListPair( density_map_pair_t * density_pair, peak_list_pair_t * peaks );
void generatePeakList( density_map_t * density_map, peak_list_t * peaks );
void fillDensityMapDataInt( int * m, int * v, int l );
void initDensityMapPair( density_map_pair_t * m, int w, int h );
void initPeaksListPair( peak_list_pair_t * p, int w, int h );
    
#endif /* rho_h */
    
#ifdef __cplusplus
}
#endif
