#ifndef kalman_h
#define kalman_h

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

//#include "rho_global.h"
#include "unilog.h"

//#define MATVEC_LIB
#define SET_DT_SEC 0.25

typedef double floating_t;

#ifdef MATVEC_LIB
#include "matvec.h"
#endif

#define KALMAN_PUNISH_FACTOR  1.0
#define MIN_KALMAN_GAIN       0.001
#define KALMAN_MATURATION     1 // Seconds

#ifndef BOUND
#define BOUNDU(X,MAX)           ( ( X > MAX ) ? MAX : X )         // Bound in upper range
#define BOUND(X,MIN,MAX)        ( ( X < MIN ) ? MIN : BOUNDU( X, MAX ) ) // Bound in upper and lower range
#endif

//#define DEBUG_KALMAN
#define DEBUG_KALMAN_PRIO DEBUG_2

#ifdef DEBUG_KALMAN
#define LOG_KALMAN(L, ...) LOG(L, "<Kalman> " __VA_ARGS__)
#else
#define LOG_KALMAN(L, ...)
#endif

#define KALMAN_TIME_UNITS TIME_SEC
//typedef double floating_t;
//typedef int16_t uint16_t;

#ifdef __cplusplus
extern "C" {
#endif
    
    /*~ Kalman Uncertainties */
    typedef struct
    {
        floating_t value; /* Q */
        floating_t bias;
        floating_t sensor; /* R */
    } kalman_uncertainty_c;
    
    /*~ Kalman structure */
    typedef struct
    {
      floating_t
        K[2],
        P[2][2],
        rate,
        bias,
        value,
        prev,
        velocity,
        variance,
        lifespan,
        min_value,
        max_value,
        t,
        origin,
        score,
        estimation_error;
      kalman_uncertainty_c
        uncertainty;
      bool
        acceleration_mode,
//        sorted,
        valid,
        flag;
    } kalman_t;
    
    void  Kalman_Initialize( kalman_t *, floating_t, floating_t, floating_t, floating_t, kalman_uncertainty_c );
    void       Kalman_Reset( kalman_t *, floating_t );
    floating_t  Kalman_Test( kalman_t *, floating_t );
    void     Kalman_Predict( kalman_t *, floating_t );
    void      Kalman_Update( kalman_t *, floating_t );
    floating_t  Kalman_Tick( kalman_t *, floating_t );
    floating_t  Kalman_Step( kalman_t *, floating_t, floating_t );
    bool   Kalman_IsExpired( kalman_t * );
    floating_t Kalman_Score( kalman_t * );
    void      Kalman_Punish( kalman_t * );
    void       Kalman_Print( kalman_t * );
#ifdef MATVEC_LIB
    gaussian1d_t GetGaussian1DKalman( kalman_filter_t * );
#endif
    
    struct kalman_functions
    {
        void (      *Initialize)( kalman_t *, floating_t, floating_t, floating_t, floating_t, kalman_uncertainty_c );
        void (      *Reset)( kalman_t *, floating_t );
        floating_t (*Test)( kalman_t *, floating_t );
        void (      *Predict)( kalman_t *, floating_t );
        void (      *Update)( kalman_t *, floating_t );
        floating_t (*Tick)( kalman_t *, floating_t );
        floating_t (*Step)( kalman_t *, floating_t, floating_t );
        bool (      *IsExpired)( kalman_t * );
        floating_t (*Score)( kalman_t * );
        void (      *Punish)( kalman_t * );
        void (      *Print)( kalman_t * );
#ifdef MATVEC_LIB
        gaussian1d_t (*Gaussian1D)( kalman_filter_t * );
#endif
    };
    
extern const struct kalman_functions Kalman;
    
#ifdef __cplusplus
}
#endif

#endif /* kalman_h */

