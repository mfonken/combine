#ifndef kalman_h
#define kalman_h

#ifdef __cplusplus
extern "C" {
#endif

    /* Standard headers */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/time.h>

#include "timestamp.h"

    /* Kalman Uncertainties */
    typedef struct
    {
        double value;
        double bias;
        double sensor;
    } kalman_uncertainty_c;
    
    typedef enum
    {
        VELOCITY = 0,
        ACCELERATION
    } update_type_c;
    
    /** Kalman structure */
    typedef struct
    {
        /* Calibration */
        double      lifespan;
        kalman_uncertainty_c uncertainty;
        /* Data */
        double      K[2];               /** Gain matrix */
        double      P[2][2];            /** Error covariance matrix */
        double      rate;               /** Rate */
        double      bias;               /** Bias */
        double      value;              /** Value */
        double      prev;               /** Previous value */
        double      density;            /** Density */
        double      velocity;           /** Velocity */
        double      timestamp;          /** Timestamp */
    } kalman_t;
    
    struct kalman_functions
    {
        void (*init)( kalman_t *, double value, double lifespan, kalman_uncertainty_c uncertainty );
        double (*update)( kalman_t *, double, double, update_type_c );
        int  (*isExpired)( kalman_t * );
    };
    
    extern const struct kalman_functions Kalman;
    
#ifdef __cplusplus
}
#endif

#endif /* kalman_h */
