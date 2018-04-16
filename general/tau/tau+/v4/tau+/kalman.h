#ifndef kalman_h
#define kalman_h

#ifdef __cplusplus
extern "C" {
#endif
    
    /* Standard headers */
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
    
    /** Uncertainty of value */
#define VALUE_UNCERTAINTY   0.1//0.01
    
    /** Uncertainty of bias */
#define BIAS_UNCERTAINTY    0.005//0.003
    
    /** Uncertainty of sensor */
#define SENSOR_UNCERTAINTY  0.005//0.02
    
    /** Uncertainty of density */
#define DENSITY_UNCERTAINTY 0.1
    
    /** Maximum lifespan of an unused kalman */
#define MAX_KALMAN_LIFE         5.0 // (seconds)
    
    /** Kalman structure */
    typedef struct _kalman_t
    {
        double      K[2];               /**< Gain matrix */
        double      P_k[2][2];          /**< Error covariance matrix */
        double      rate;               /**< Rate */
        double      bias;               /**< Bias */
        double      value;              /**< Value */
        double      prev;               /**< Previous value */
        double      density;            /**< Density */
//        bool        shunned;            /**< Shun value */
        uint32_t    timestamp;          /**< Timestamp */
    } kalman_t;
    
    void Kalman_Init( kalman_t *k, double v );
    void Kalman_Update( kalman_t *k, double value_new, double rate_new );
    double timestamp(void);
    static int isExpired( kalman_t * k ) { return ((timestamp() - k->timestamp) > MAX_KALMAN_LIFE); }
    
#ifdef __cplusplus
}
#endif

#endif /* kalman_h */


