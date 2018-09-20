#ifndef rho_kalman_h
#define rho_kalman_h

#ifdef __cplusplus
extern "C" {
#endif

    /* Standard headers */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>
#include "global_types.h"
#include <math.h>
    
typedef double kfl_t;

/** Kalman Uncertainties */
typedef struct
{
    kfl_t value;
    kfl_t bias;
    kfl_t sensor;
} rho_kalman_uncertainty_c;

/** Kalman structure */
typedef struct
{
kfl_t
    K[2],
    P[2][2],
    rate,
    bias,
    value,
    prev,
    velocity,
    variance,
    lifespan,
    timestamp;
rho_kalman_uncertainty_c
    uncertainty;
} rho_kalman_t;

struct rho_kalman {
    void (*init)( rho_kalman_t *, kfl_t, kfl_t, kfl_t, kfl_t, kfl_t );
    void (*update)( rho_kalman_t *, kfl_t, kfl_t );
    bool  (*isExpired)( rho_kalman_t * );
};

extern const struct rho_kalman RhoKalman;
    
#ifdef __cplusplus
}
#endif

#endif /* kalman_h */
