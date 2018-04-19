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

prediction_kalman_pair_t loc_k;

void initSigma( uint16_t w, uint16_t h, prediction_pair_t * l )
{
    width = w;
    height = h;

    Kalman_Init(&loc_k.x.primary,   l->x.primary);
    Kalman_Init(&loc_k.x.secondary, l->x.secondary);
    Kalman_Init(&loc_k.y.primary,   l->y.primary);
    Kalman_Init(&loc_k.y.secondary, l->y.secondary);
}

void getSigmaData( prediction_kalman_pair_t * k )
{
    (*k) = loc_k;
}

void performSigma( prediction_pair_t * prediction, prediction_pair_t * last, prediction_pair_t * result  )
{
//    updateRadiusAndOffset(prediction);
//    getExtrapolation(prediction, last, result);
    
    updateKalmans(prediction, last, result);
    framePrediction(result);
}

void updateKalmans( prediction_pair_t * prediction, prediction_pair_t * last, prediction_pair_t * result )
{
    FLOAT pxp = prediction->x.primary, pxs = prediction->x.secondary;
    FLOAT pyp = prediction->y.primary, pys = prediction->y.secondary;
    FLOAT dxp = fabs(pxp - last->x.primary), dxs = fabs(pyp - last->x.secondary);
    FLOAT dyp = fabs(pxp - last->x.primary), dys = fabs(pyp - last->y.secondary);
    
    FLOAT factor = 1;
    dxp /= factor;
    dxs /= factor;
    dyp /= factor;
    dys /= factor;
    
    printf("Prediction:\txp%.2f & xs>%.2f | yp>%.2f & ys>%.2f\n", pxp, pxs, pyp, pys );
    printf("Difference:\txp%.2f & xs>%.2f | yp>%.2f & ys>%.2f\n", dxp, dxs, dyp, dys );
    
    Kalman_Update(&loc_k.x.primary,   pxp, dxp);
    Kalman_Update(&loc_k.x.secondary, pxs, dxs);
    Kalman_Update(&loc_k.y.primary,   pyp, dyp);
    Kalman_Update(&loc_k.y.secondary, pys, dys);
    
    result->x.primary   = loc_k.x.primary.value;
    result->x.secondary = loc_k.x.secondary.value;
    result->y.primary   = loc_k.y.primary.value;
    result->y.secondary = loc_k.y.secondary.value;
    
    printf("Kalmans:\txp%.2f & xs>%.2f | yp>%.2f & ys>%.2f\n", loc_k.x.primary.value, loc_k.x.secondary.value, loc_k.y.primary.value, loc_k.y.secondary.value );
    printf(" Pri: value>%.2f | rate>%.2f | bias>%.2f\n", loc_k.x.primary.value,loc_k.x.primary.rate, loc_k.x.primary.bias);
    printf("XP P:\t[%3.10f][%3.10f]\n\t\t[%3.10f][%3.10f]\n", loc_k.x.primary.P_k[0][0], loc_k.x.secondary.P_k[0][1], loc_k.x.primary.P_k[1][0], loc_k.x.secondary.P_k[1][1] );
    printf("XP K:\t[%3.10f][%3.10f]\n", loc_k.x.primary.K[0], loc_k.x.primary.K[1]);
//    printf("YP P:\t[%3.10f][%3.10f]\n\t\t[%3.10f][%3.10f]\n", loc_k.y.primary.P_k[0][0], loc_k.y.secondary.P_k[0][1], loc_k.y.primary.P_k[1][0], loc_k.y.secondary.P_k[1][1] );
//    printf("YP K:\t[%3.10f][%3.10f]\n", loc_k.y.primary.K[0], loc_k.y.primary.K[1]);
}

void framePrediction( prediction_pair_t * prediction )
{
    image_dimension_base_t xp = prediction->x.primary, xs = prediction->x.secondary, yp = prediction->y.primary, ys = prediction->y.secondary;
    if( yp >= width  ) prediction->y.primary   = width  - 1;
    if( ys >= width  ) prediction->y.secondary = width  - 1;
    if( xp >= height ) prediction->x.primary   = height - 1;
    if( xs >= height ) prediction->x.secondary = height - 1;
    if( yp <  0 )      prediction->y.primary   = 0;
    if( ys <  0 )      prediction->y.secondary = 0;
    if( xp <  0 )      prediction->x.primary   = 0;
    if( xs <  0 )      prediction->x.secondary = 0;
}

/******** OLD **********/
void updateRadiusAndOffset( prediction_pair_t * pair)
{
    image_dimension_base_t a, b;
    a = pair->x.secondary - pair->x.primary;
    b = pair->y.secondary - pair->y.primary;
    
    radius = sqrt( ( a * a ) + ( b * b ) ) / 2;
    offset = atan2(b, a);
}

FLOAT getRotationCoupling( prediction_t * prediction, prediction_t * last )
{
    FLOAT a, b, d;
    a = last->primary   - prediction->primary;
    b = last->secondary - prediction->secondary;
    d = fabs(b) + fabs(a);
    if(d == 0) return 1.00;
    return fabs( ( b - a ) / d );
}

void getExtrapolation( prediction_pair_t * prediction, prediction_pair_t * last, prediction_pair_t * result )
{
    FLOAT x_coupling, y_coupling;
    prediction_pair_t linear_extrapolation, angular_extrapolation;
    getLinearExtrapolation( prediction, last, &linear_extrapolation );
    
    x_coupling = getRotationCoupling( &prediction->x, &last->x );
    y_coupling = getRotationCoupling( &prediction->y, &last->y );
    
    if(radius >= MINIMUM_RADIUS) getAngularExtrapolation( prediction, last, &angular_extrapolation );
    else
    {
        x_coupling = 0;
        y_coupling = 0;
        angular_extrapolation.x.primary   = 0;
        angular_extrapolation.x.secondary = 0;
        angular_extrapolation.y.primary   = 0;
        angular_extrapolation.y.secondary = 0;
    }
    
//    result->x.primary   = linear_extrapolation.x.primary;
//    result->x.secondary = linear_extrapolation.x.secondary;
//    result->y.primary   = linear_extrapolation.y.primary;
//    result->y.secondary = linear_extrapolation.y.secondary;

//    result->x.primary   = (1 - TRUST) * (FLOAT)prediction->x.primary   + TRUST * (FLOAT)linear_extrapolation.x.primary;
//    result->x.secondary = (1 - TRUST) * (FLOAT)prediction->x.secondary + TRUST * (FLOAT)linear_extrapolation.x.secondary;
//    result->y.primary   = (1 - TRUST) * (FLOAT)prediction->y.primary   + TRUST * (FLOAT)linear_extrapolation.y.primary;
//    result->y.secondary = (1 - TRUST) * (FLOAT)prediction->y.secondary + TRUST * (FLOAT)linear_extrapolation.y.secondary;

    result->x.primary   = (1 - TRUST) * (FLOAT)prediction->x.primary   + TRUST * ( ( ( 1 - x_coupling ) * (FLOAT)linear_extrapolation.x.primary   ) + ( x_coupling * (FLOAT)angular_extrapolation.x.primary   ) );
    result->x.secondary = (1 - TRUST) * (FLOAT)prediction->x.secondary + TRUST * ( ( ( 1 - x_coupling ) * (FLOAT)linear_extrapolation.x.secondary ) + ( x_coupling * (FLOAT)angular_extrapolation.x.secondary ) );
    result->y.primary   = (1 - TRUST) * (FLOAT)prediction->y.primary   + TRUST * ( ( ( 1 - y_coupling ) * (FLOAT)linear_extrapolation.y.primary   ) + ( y_coupling * (FLOAT)angular_extrapolation.y.primary   ) );
    result->y.secondary = (1 - TRUST) * (FLOAT)prediction->y.secondary + TRUST * ( ( ( 1 - y_coupling ) * (FLOAT)linear_extrapolation.y.secondary ) + ( y_coupling * (FLOAT)angular_extrapolation.y.secondary ) );

    printf("X:\n");
    printf("\t  Coupling: %.2f\n", x_coupling);
    printf("\t Linear extrapolation is: pri>%d & sec>%d\n", linear_extrapolation.x.primary,linear_extrapolation.x.secondary);
    printf("\tAngular extrapolation is: pri>%d & sec>%d\n", angular_extrapolation.x.primary,angular_extrapolation.x.secondary);
    printf("Y:\n");
    printf("\t  Coupling: %.2f\n", y_coupling);
    printf("\t Linear extrapolation is: pri>%d & sec>%d\n", linear_extrapolation.y.primary,linear_extrapolation.y.secondary);
    printf("\tAngular extrapolation is: pri>%d & sec>%d\n", angular_extrapolation.y.primary,angular_extrapolation.y.secondary);
}

void getLinearExtrapolation( prediction_pair_t * prediction, prediction_pair_t * last, prediction_pair_t * result )
{
    result->x.primary   = LINEAR_VELOCITY_BIAS * ( prediction->x.primary   - last->x.primary )   + prediction->x.primary;
    result->x.secondary = LINEAR_VELOCITY_BIAS * ( prediction->x.secondary - last->x.secondary ) + prediction->x.secondary;
    result->y.primary   = LINEAR_VELOCITY_BIAS * ( prediction->y.primary   - last->y.primary )   + prediction->y.primary;
    result->y.secondary = LINEAR_VELOCITY_BIAS * ( prediction->y.secondary - last->y.secondary ) + prediction->y.secondary;
}

FLOAT dot( vector_t * a, vector_t * b )
{
    return a->i*b->i + a->j*b->j;
}

FLOAT len( vector_t * a )
{
    return sqrt(a->i*a->i + a->j*a->j);
}

void getAngularExtrapolation( prediction_pair_t * prediction, prediction_pair_t * last, prediction_pair_t * result )
{
    /* l - last, p - prediction, n - next */
    FLOAT pa, na, cna, sna;
    FLOAT axp = (FLOAT)prediction->x.primary,   axl = (FLOAT)last->x.primary;
    FLOAT bxp = (FLOAT)prediction->x.secondary, bxl = (FLOAT)last->x.secondary;
    FLOAT ayp = (FLOAT)prediction->y.primary,   ayl = (FLOAT)last->y.primary;
    FLOAT byp = (FLOAT)prediction->y.secondary, byl = (FLOAT)last->y.secondary;
    
    coordinate_t c;
    c.x = ( axl + bxl ) / 2;
    c.y = ( ayl + byl ) / 2;
    
    vector_t vl, vp;
    vl.i = bxl - axl;
    vl.j = byl - ayl;
    vp.i = bxp - axp;
    vp.j = byp - ayp;
    
    pa = acos( dot(&vl,&vp) / (len(&vl) * len(&vp)));
    
    na = ANGULAR_VELOCITY_BIAS * ( pa - offset ) - offset;
    cna = cos(na);
    sna = sin(na);
    
    result->x.primary   = c.x + radius * cna;
    result->x.secondary = c.x - radius * cna;
    result->y.primary   = c.y + radius * sna;
    result->y.secondary = c.y - radius * sna;
}
