//
//  sigma_utility.cpp
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "sigma_utility.hpp"

Sigma::Sigma( int width, int height )
{
    printf("Initializing Sigma Utility: %dx%d\n", width, height);
    this->width   = width;
    this->height  = height;
}

void Sigma::perform( PeakListPair * p, PredictionPair * d  )
{
    kalman_matrix.update( p );
    kalman_matrix.predict( d );
    framePrediction( d );
}

void Sigma::framePrediction( PredictionPair * d )
{
    image_dimension_base_t xp = d->x.primary, xs = d->x.secondary, yp = d->y.primary, ys = d->y.secondary;
    if( yp >= width  ) d->y.primary   = width  - 1;
    if( ys >= width  ) d->y.secondary = width  - 1;
    if( xp >= height ) d->x.primary   = height - 1;
    if( xs >= height ) d->x.secondary = height - 1;
    if( yp <  0 ) d->y.primary   = 0;
    if( ys <  0 ) d->y.secondary = 0;
    if( xp <  0 ) d->x.primary   = 0;
    if( xs <  0 ) d->x.secondary = 0;
}

