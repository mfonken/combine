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

typedef FLOAT double;

typedef struct
{
    int *       map;
    int         length;
    int         max;
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
} Prediction;

typedef struct
{
    Prediction         x,y;
} PredictionPair;

typedef struct
{
    density_map_pair_t  density_map_pair;
    PredictionPair  prediction_pair;

    int     width,
            height,
            thresh,
            Cx,
            Cy,
            Q[4],
            QT;
    FLOAT  QF, FT;
} rho_utility;


#endif /* rho_c_types_h */
