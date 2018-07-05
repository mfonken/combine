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
    
typedef double kalman_floating_t;
    
    /** Kalman Uncertainties */
    typedef struct
    {
        kalman_floating_t value;
        kalman_floating_t bias;
        kalman_floating_t sensor;
    } rho_kalman_uncertainty_c;
    
    /** Kalman structure */
    typedef struct
    {
        /* Calibration */
        kalman_floating_t lifespan;
        rho_kalman_uncertainty_c uncertainty;
        /* Data */
        kalman_floating_t
            K[2],
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
        void (*init)( rho_kalman_t *, kalman_floating_t, kalman_floating_t, kalman_floating_t, kalman_floating_t, kalman_floating_t );
        void (*update)( rho_kalman_t *, kalman_floating_t, kalman_floating_t );
        bool  (*isExpired)( rho_kalman_t * );
    };
    
//    kalman_floating_t timestamp(void);
    
    extern const struct rho_kalman RhoKalman;
    
#ifdef __cplusplus
}
#endif

#endif /* kalman_h */
