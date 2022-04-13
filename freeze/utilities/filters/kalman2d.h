//
//  kalman2d.h
//  ots-proto
//
//  Created by Matthew Fonken on 4/10/22.
//

#ifndef kalman2d_h
#define kalman2d_h

#include <stdio.h>
#include <string.h>

#include "matrix.h"
#include "timestamp.h"

typedef double floatp;

typedef struct
{
    floatp px;
    floatp py;
    floatp vx;
    floatp vy;
    floatp ax;
    floatp ay;
} physical_2d_t;

typedef struct
{
    floatp value[2];
    physical_2d_t state;
    floatp A[4][4];
    floatp B[4][2];
    floatp H[2][4];
    floatp Q[4][4];
    floatp R[2][2];
    floatp P[4][4];
    floatp meas_var[2];
    floatp process_noise_sq;
    floatp t;
} kalman2d_t;

struct kalman2d_functions
{
    void (*init)( kalman2d_t *, floatp process_noise );
    void (*predict)( kalman2d_t * );
    void (*update)( kalman2d_t *, floatp z[2] );
};

extern const struct kalman2d_functions Kalman2D;

#endif /* kalman2d_h */
