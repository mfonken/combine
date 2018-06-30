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
    uint16_t    *map,
                *background,
                *filtered,
                 length,
                 centroid,
                 max[2];
    bool         has_background;
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
            alternate,
            absence;
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

typedef struct
{
    int
        max,
        den;
    int
        loc;
} blob_t;

typedef struct
{
    int
        len,
        fpeak,
        fvar,
        fbandl,
        c1,
        c2,
        b,
        cloc,
        gapc,
        avgc;
    
    int
        cmax,
        amax,
    
        cden,
        tden,
        fden;
    
    bool
        has,
        sel;
    
    double
        cavg,
        mavg;
//        fcov;
    blob_t
        blobs[2];
} rho_selection_variables;

#endif /* rho_c_types_h */
