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
#define DEBUG_KALMAN2D_PRIO DEBUG_2

#ifdef DEBUG_KALMAN2D
#define LOG_KALMAN2D(L, ...) LOG(L, "<Kalman2d> " __VA_ARGS__)
#else
#define LOG_KALMAN2D(L, ...)
#endif

#ifdef __cplusplus
extern "C" {
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
    physical_2d_t x; // State
    floatp A[4][4];  // State Transition
    floatp B[4][2];  // Control Input Matrix
    floatp H[2][4];  // Measurement Mapping Matrix
    floatp Q[4][4];  // Process Noise Covariance
    floatp R[2][2];  // Measurement Noise Covariance
    floatp P[4][4];  // Covariance Matrix
    floatp K[4][2];  // Kalman Gain
//    floatp meas_var[2];
    floatp process_noise_sq;
    floatp t;
} kalman2d_t;

struct kalman2d_functions
{
    void (*Init)( kalman2d_t *, floatp u[2], floatp process_noise, floatp x_std_meas, floatp y_std_meas );
    void (*Reset)( kalman2d_t * );
    physical_2d_t * (*Test)( kalman2d_t *, floatp x_new[4], bool update_A );
    void (*Predict)( kalman2d_t * );
    void (*Update)( kalman2d_t *, floatp z[2] );
    physical_2d_t * (*Step)( kalman2d_t *, floatp z[2] );
    void (*Print)( kalman2d_t * );
};

extern const struct kalman2d_functions Kalman2D;

#ifdef __cplusplus
}
#endif

#endif /* kalman2d_h */
