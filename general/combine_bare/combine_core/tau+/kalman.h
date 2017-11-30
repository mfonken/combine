#ifndef kalman_h
#define kalman_h

#ifdef __cplusplus
extern "C" {
#endif
    
    /* Standard headers */
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
 
    /** Maximum lifespan of an unused kalman */
#define MAX_KALMAN_LIFE     3.0 // (seconds)
    
    /** Uncertainty of value */
#define VALUE_UNCERTAINTY   0.1//0.01
    
    /** Uncertainty of bias */
#define BIAS_UNCERTAINTY    0.005//0.003
    
    /** Uncertainty of sensor */
#define SENSOR_UNCERTAINTY  0.005//0.02
    
    /** Uncertainty of density */
//#define DENSITY_UNCERTAINTY 0.1
    
    /** Kalman Uncertainties */
    typedef struct _kalman_uncertainty_t
    {
        double value;
        double bias;
        double sensor;
//        double density;
    }kalman_uncertainty_t;
    
    /** Kalman structure */
    typedef struct _kalman_t
    {
        /* Calibration */
        double lifespan;
        kalman_uncertainty_t uncertainty;
        /* Data */
        double      K[2];               /**< Gain matrix */
        double      P_k[2][2];          /**< Error covariance matrix */
        double      rate;               /**< Rate */
        double      bias;               /**< Bias */
        double      value;              /**< Value */
        double      prev;               /**< Previous value */
        double      density;            /**< Density */
        uint32_t    timestamp;          /**< Timestamp */
    } kalman_t;
    
    void Kalman_Init( kalman_t *k, double v, double ls, double v_u, double b_u, double s_u );
    void Kalman_Update( kalman_t *k, double value_new, double rate_new );
    double timestamp(void);
    static inline int isExpired( kalman_t * k ) { return ((timestamp() - k->timestamp) > MAX_KALMAN_LIFE); }
    
#ifdef __cplusplus
}
#endif

#endif /* kalman_h */


