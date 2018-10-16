#ifndef rho_kalman_h
#define rho_kalman_h


    /* Standard headers */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "stm32l4xx_hal.h"

static double timestamp( void )
{
	return (double)HAL_GetTick()/1000;
}

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
                    velocity;
				double 		timestamp;
    } rho_kalman_t;

    struct rho_kalman {
        void (*init)( rho_kalman_t *, double, double, double, double, double );
        void (*update)( rho_kalman_t *, double, double );
        int  (*isExpired)( rho_kalman_t * );
    };

    extern const struct rho_kalman RhoKalman;


#endif /* kalman_h */
