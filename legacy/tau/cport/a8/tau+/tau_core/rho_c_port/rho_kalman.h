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
    
    /** Kalman Uncertainties */
    typedef struct
    {
        double value;
        double bias;
        double sensor;
    } rho_kalman_uncertainty_c;
    
    /** Kalman structure */
    typedef struct
    {
        /* Calibration */
        double lifespan;
        rho_kalman_uncertainty_c uncertainty;
        /* Data */
        double      K[2],
                    P[2][2],
                    rate,
                    bias,
                    value,
                    prev,
                    velocity,
                    variance,
                    timestamp;
    } rho_kalman_t;
    
    struct rho_kalman {
        void (*init)( rho_kalman_t *, double, double, double, double, double );
        void (*update)( rho_kalman_t *, double, double );
        int  (*isExpired)( rho_kalman_t * );
    };
    
//    double timestamp(void);
    
    extern const struct rho_kalman RhoKalman;
    
#ifdef __cplusplus
}
#endif

#endif /* kalman_h */
