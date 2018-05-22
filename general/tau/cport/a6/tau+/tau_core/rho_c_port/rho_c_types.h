//
//  rho_c_types.h
//  tau+
//
//  Created by Matthew Fonken on 4/22/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_c_types_h
#define rho_c_types_h

#include "rho_kalman.h"

typedef struct
{
    int         *map,
                 length,
                 max[2],
                 centroid;
    double       variance[2];
    rho_kalman_t kalmans[2];
} DensityMapC;

typedef struct
{
    DensityMapC x,y;
} DensityMapPairC;

typedef struct
{
    double  primary,
    secondary,
    alternate;
} prediction_probabilities_c;

typedef enum
{
    SIM = 0,
    OPP
} selection_pair_c;

typedef struct
{
    rho_kalman_t    primary,
    secondary;
    double          primary_new,
    secondary_new;
    prediction_probabilities_c probabilities;
} PredictionC;

typedef struct
{
    PredictionC         x,y;
    selection_pair_c    selection_pair;
} PredictionPairC;


#endif /* rho_c_types_h */
