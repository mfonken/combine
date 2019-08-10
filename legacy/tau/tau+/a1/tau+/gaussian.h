//
//  gaussian.h
//  tau+
//
//  Created by Matthew Fonken on 11/6/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef gaussian_h
#define gaussian_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef float GAUSS_FLOAT;
    
typedef struct
{
    GAUSS_FLOAT   *array;
    uint16_t       length;
    GAUSS_FLOAT    sigma;
} gaussian_t;
    
void gaussianBlur( gaussian_t * g, float * arr, size_t len );
GAUSS_FLOAT gaussianDistribution( int x, int mu, GAUSS_FLOAT sigma);
void generateGaussian( gaussian_t * g, int l, GAUSS_FLOAT s );
void initGaussian( gaussian_t * g, int l );
    
#ifdef __cplusplus
}
#endif

#endif /* gaussian_h */
