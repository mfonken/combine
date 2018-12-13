#ifndef rho_kalman_h
#define rho_kalman_h

#include <stdlib.h>
#include <stdbool.h>
#include "rho_global.h"
//#include <sys/time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Standard headers */    
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

kfl_t timestamp(void);
void InitRhoKalman( rho_kalman_t * k, kfl_t v, kfl_t ls, kfl_t vu, kfl_t bu, kfl_t su );
void PredictRhoKalman( rho_kalman_t * k, kfl_t rate_new );
void UpdateRhoKalman( rho_kalman_t * k, kfl_t value_new );
void StepRhoKalman( rho_kalman_t * k, kfl_t value_new, kfl_t rate_new );
bool IsRhoKalmanExpired( rho_kalman_t * k );
    
struct rho_kalman {
    void (*Init)( rho_kalman_t *, kfl_t, kfl_t, kfl_t, kfl_t, kfl_t );
    void (*Step)( rho_kalman_t * k, kfl_t, kfl_t );
    void (*Predict)( rho_kalman_t * k, kfl_t );
    void (*Update)( rho_kalman_t *, kfl_t );
    bool (*IsExpired)( rho_kalman_t * );
};
    
static const struct rho_kalman RhoKalman =
{
    .Init = InitRhoKalman,
    .Predict = PredictRhoKalman,
    .Update = UpdateRhoKalman,
    .Step = StepRhoKalman,
    .IsExpired = IsRhoKalmanExpired
};
    
#ifdef __cplusplus
}
#endif

#endif /* kalman_h */
