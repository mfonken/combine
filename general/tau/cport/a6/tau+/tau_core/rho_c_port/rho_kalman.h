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
    
#define RHO_SQRT_HEIGHT     sqrt(FNL_RESIZE_H)
#define RHO_DIM_INFLUENCE   0.1
#define RHO_K_TARGET_IND    0.3
#define RHO_K_TARGET        RHO_K_TARGET_IND+(10/RHO_SQRT_HEIGHT*RHO_DIM_INFLUENCE)          //0.3
#define RHO_VARIANCE_NORMAL RHO_SQRT_HEIGHT/5.0             //5
#define RHO_VARIANCE_SCALE  RHO_SQRT_HEIGHT/3.0//1.32        //20
    
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
                    timestamp,
                    variance;
    } rho_kalman_t;
    
    struct rho_kalman {
        void (*init)( rho_kalman_t *, double, double, double, double, double );
        void (*update)( rho_kalman_t *, double, double, bool );
        int  (*isExpired)( rho_kalman_t * );
    };
    
//    double timestamp(void);
    
    extern const struct rho_kalman RhoKalman;
    
#ifdef __cplusplus
}
#endif

#endif /* kalman_h */
