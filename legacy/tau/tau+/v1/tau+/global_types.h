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

#include <stdint.h>
#include <stdbool.h>
#include "kalman.h"

#define ABSINV(X)   ((X<1)?1-X:X-1)
#define ABS(X)      ((X>=0)?X:-X)

/***PROBABILITY BIASES***/
#define LOCATION_BIAS   0.5
#define DENSITY_BIAS    1 - LOCATION_BIAS
/************************/

/*******PEAK TUNING******/
/************************/

typedef int16_t         image_dimension_base_t;
typedef uint8_t         pixel_base_t;
typedef pixel_base_t**  cimage_t;

typedef double          FLOAT;

typedef struct
{
    image_dimension_base_t x;
    image_dimension_base_t y;
} coordinate_t;

typedef struct
{
    image_dimension_base_t i;
    image_dimension_base_t j;
} vector_t;

typedef struct
{
    image_dimension_base_t primary;
    image_dimension_base_t secondary;
} prediction_t;

typedef struct
{
    prediction_t x;
    prediction_t y;
} prediction_pair_t;
    
typedef struct
{
    kalman_t primary;
    kalman_t secondary;
} prediction_kalman_t;

typedef struct
{
    prediction_kalman_t x;
    prediction_kalman_t y;
} prediction_kalman_pair_t;

typedef struct
{
    float*          map;
    float*          vel;
    float*          acc;
    float*          jrk;
    float*          jnc;
    uint16_t        length;
}density_map_t;

typedef struct
{
    density_map_t   x;
    density_map_t   y;
} density_map_pair_t;

typedef struct
{
    uint16_t  length;
    uint16_t* locations;
    uint16_t* peaks;
    int*     dir;
}peak_list_t;

typedef struct
{
    peak_list_t x;
    peak_list_t y;
} peak_list_pair_t;

typedef struct
{
    uint16_t  length;
    FLOAT* primary;
    FLOAT* secondary;
}probability_list_t;
typedef struct
{
    probability_list_t x;
    probability_list_t y;
}probability_list_pair_t;

#endif /* global_types_h */
    
#ifdef __cplusplus
}
#endif
