//
//  sigma.c
//  tau+
//
//  Created by Matthew Fonken on 8/22/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "sigma.h"

FLOAT radius, offset;
uint16_t width;
uint16_t height;

kmat_pair_t       loc_km;
prediction_pair_t loc_k;

void initSigma( uint16_t w, uint16_t h )
{
    width   = w;
    height  = h;
    initKMatPair(&loc_km);
}

void getSigmaData( kmat_pair_t * m )
{
    (*m) = loc_km;
}

void performSigma( peak_list_pair_t * p, prediction_pair_t * d  )
{
    updateKMatPair(    &loc_km, p  );
    getKMatPrediction( &loc_km, d );
    framePrediction(            d );
}

void framePrediction( prediction_pair_t * d )
{
    image_dimension_base_t xp = d->x.primary, xs = d->x.secondary, yp = d->y.primary, ys = d->y.secondary;
    if( yp >= width  ) d->y.primary   = width  - 1;
    if( ys >= width  ) d->y.secondary = width  - 1;
    if( xp >= height ) d->x.primary   = height - 1;
    if( xs >= height ) d->x.secondary = height - 1;
    if( yp <  0      ) d->y.primary   = 0;
    if( ys <  0      ) d->y.secondary = 0;
    if( xp <  0      ) d->x.primary   = 0;
    if( xs <  0      ) d->x.secondary = 0;
}

