//
//  global_types.h
//  tau+
//
//  Created by Matthew Fonken on 8/22/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif
    
#ifndef global_types_h
#define global_types_h
    
#include "test_setup.h"
    
#include <stdint.h>
#include <stdbool.h>
#include "kalman.h"
    
#define ABSINV(X)   ((X<1)?1-X:X-1)
#define ABS(X)      ((X>=0)?X:-X)
    
#define GTTHR(X,Y,T)    X>=(Y+T)
#define EQTHR(X,Y,T)  (X>(Y-T)&&X<(Y+T))
    
/* Kalman Defaults */
#define IMAGE_MAX_KALMAN_LIFE     5.0 // (seconds)
#define IMAGE_VALUE_UNCERTAINTY   0.1//0.01
#define IMAGE_BIAS_UNCERTAINTY    0.005//0.003
#define IMAGE_SENSOR_UNCERTAINTY  0.005//0.02
    
    typedef int16_t         image_dimension_base_t;
    typedef uint8_t         pixel_base_t;
    typedef pixel_base_t*   cimage_t;
    
    typedef double          FLOAT;
    
    static FLOAT timeDiff( struct timeval a, struct timeval b ) { return ((b.tv_sec  - a.tv_sec) + (b.tv_usec - a.tv_usec)/1000000.0) + 0.0005; }
    
    typedef enum
    {
        UNWEIGHTED = 0,
        WEIGHTED = 1
    } sorting_settingss;
    
#define MAX_PERSISTENCE ( 1 << 7 ) - 1 // MAX OF SIGNED BYTE
    
    typedef struct
    {
        image_dimension_base_t x;
        image_dimension_base_t y;
    } coordinate_t;
    
    typedef enum
    {
        SIMILAR = 0,
        OPPOSITE
    } selection_pair_t;
    
    typedef struct
    {
        image_dimension_base_t  primary;
        FLOAT                   primary_probability;
        image_dimension_base_t  secondary;
        FLOAT                   secondary_probability;
        FLOAT                   alternate_probability;
    } prediction_t;
    
    typedef struct
    {
        prediction_t x;
        prediction_t y;
        selection_pair_t        selection_pair;
    } prediction_pair_t;
#endif /* global_types_h */
    
#ifdef __cplusplus
}
#endif
