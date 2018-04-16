//
//  sigma.c
//  tau+
//
//  Created by Matthew Fonken on 8/22/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "sigma.h"

void initSigma( sigma_t * s, uint16_t width, uint16_t height )
{
    s->width   = width;
    s->height  = height;
    initKMatPair(&s->loc_km);
}

void performSigma( sigma_t * s, peak_list_pair_t * p, prediction_pair_t * d  )
{
    updateKMatPair(    &s->loc_km, p  );
    getKMatPrediction( &s->loc_km, d );
    framePrediction( s, d );
}

void framePrediction( sigma_t * s, prediction_pair_t * d )
{
    image_dimension_base_t xp = d->x.primary, xs = d->x.secondary, yp = d->y.primary, ys = d->y.secondary;
    if( yp >= s->width  ) d->y.primary   = s->width  - 1;
    if( ys >= s->width  ) d->y.secondary = s->width  - 1;
    if( xp >= s->height ) d->x.primary   = s->height - 1;
    if( xs >= s->height ) d->x.secondary = s->height - 1;
    if( yp <  0      ) d->y.primary   = 0;
    if( ys <  0      ) d->y.secondary = 0;
    if( xp <  0      ) d->x.primary   = 0;
    if( xs <  0      ) d->x.secondary = 0;
}

