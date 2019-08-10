//
//  sigma.h
//  tau+
//
//  Created by Matthew Fonken on 8/22/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#ifndef sigma_h
#define sigma_h
    
#define LINEAR_VELOCITY_BIAS    0.5
#define ANGULAR_VELOCITY_BIAS   0.0
#define TRUST                   0.5
    
#define MINIMUM_RADIUS          15

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "global_types.h"
//#include "kalman.h"
    
extern FLOAT radius;

void initSigma( uint16_t w, uint16_t h, prediction_pair_t * l );
void performSigma( prediction_pair_t * prediction, prediction_pair_t * last, prediction_pair_t * result  );
void updateKalmans( prediction_pair_t * prediction, prediction_pair_t * last, prediction_pair_t * result );
void framePrediction( prediction_pair_t *prediction);
void updateRadiusAndOffset( prediction_pair_t * pair);
void getCenter( prediction_pair_t * pair, coordinate_t * c);
FLOAT getRotationCoupling( prediction_t * prediction, prediction_t * last );
void getExtrapolation( prediction_pair_t * prediction, prediction_pair_t * last, prediction_pair_t * result );
void getLinearExtrapolation( prediction_pair_t * prediction, prediction_pair_t * last, prediction_pair_t * result );
void getAngularExtrapolation( prediction_pair_t * prediction, prediction_pair_t * last, prediction_pair_t * result );

#endif /* sigma_h */

#ifdef __cplusplus
}
#endif
