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
#include "unilog.h"

//#define SET_DT_SEC  (100 * 1e-3)

//#define DEBUG_KALMAN2D

#ifdef DEBUG_KALMAN2D
#define LOG_K2(L, ...) LOG(L, "<Kalman2d> " __VA_ARGS__)
#else
#define LOG_K2(L, ...)
#endif

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
    floatp A[4][4]; // State Transition
    floatp B[4][2]; // Control Input Matrix
    floatp H[2][4]; // Measurement Mapping Matrix
    floatp Q[4][4]; // Process Noise Covariance
    floatp R[2][2]; // Measurement Noise Covariance
    floatp P[4][4]; // Covariance Matrix
    floatp meas_var[2];
    floatp process_noise_sq;
    floatp t;
} kalman2d_t;

struct kalman2d_functions
{
    void (*init)( kalman2d_t *, floatp process_noise, floatp x_std_meas, floatp y_std_meas );
    void (*test)( kalman2d_t *, floatp x_new[4], bool update_A );
    void (*predict)( kalman2d_t * );
    void (*update)( kalman2d_t *, floatp z[2] );
};

extern const struct kalman2d_functions Kalman2D;

#endif /* kalman2d_h */
