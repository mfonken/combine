//
//  rho_types.h
//  tau+
//
//  Created by Matthew Fonken on 4/22/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_types_h
#define rho_types_h

#include "rho_kalman.h"

typedef double FLOAT;
typedef uint8_t		byte_t;
typedef uint16_t	index_t;
typedef uint8_t 	capture_t;
typedef uint32_t	density_t;
typedef uint32_t	address_t;

typedef struct
{
    density_t * map;
    uint16_t    length;
    density_t   max;
    FLOAT       variance;
    rho_kalman_t kalman;
} density_map_t;

typedef struct
{
    density_map_t x,y;
} density_map_pair_t;

typedef struct
{
    FLOAT   primary,
            secondary,
            alternate;
} prediction_probabilities;

typedef struct
{
    rho_kalman_t    primary,
                    secondary;
    FLOAT           primary_new,
                    secondary_new;
    prediction_probabilities probabilities;
} prediction_t;

typedef struct
{
    prediction_t         x,y;
} prediction_pair_t;


#endif /* rho_c_types_h */
