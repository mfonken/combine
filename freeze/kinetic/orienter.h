//
//  orienter.h
//  ots-proto
//
//  Created by Matthew Fonken on 5/2/22.
//

#ifndef orienter_h
#define orienter_h

#include <stdio.h>

#include "kalman.h"
#include "qmath.h"

/* Kalman Defaults */
#define MOTION_MAX_KALMAN_LIFE     10.0
#define MOTION_VALUE_UNCERTAINTY   0.01
#define MOTION_BIAS_UNCERTAINTY    0.003
#define MOTION_SENSOR_UNCERTAINTY  0.02

typedef struct
{
//    kalman_t rotation_filters[3];
    ang3_t rotation_raw;
    ang3_t rotation;
    vec3_t gravity;
    vec3_t offset;
} orienter_t;

void Orienter_Init( orienter_t * o );
void Orienter_Update( orienter_t * o, vec3_t * ang );
void Orienter_Tare( orienter_t * o );
void Orienter_Print( orienter_t * o );

typedef struct
{
    void (*Init)( orienter_t * );
    void (*Update)( orienter_t *, vec3_t * ang );
    void (*Tare)( orienter_t * );
    void (*Print)( orienter_t * );
} orienter_functions_t;

extern orienter_functions_t OrienterFunctions;

#endif /* orienter_h */
