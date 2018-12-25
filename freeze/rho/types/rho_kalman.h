#ifndef rho_kalman_h
#define rho_kalman_h

#include <stdlib.h>
#include <stdbool.h>
#include "rho_global.h"
#include "timestamp.h"
//#include <sys/time.h>

#define KALMAN_PUNISH_FACTOR  0.2
#define MIN_KALMAN_GAIN       0.001
#define KALMAN_MATURATION     3 // Seconds

#ifdef __cplusplus
extern "C" {
#endif
    
    /* Standard headers */
//    typedef double floating_t;
    
    /** Kalman Uncertainties */
    typedef struct
    {
        floating_t value;
        floating_t bias;
        floating_t sensor;
    } rho_kalman_uncertainty_c;
    
    /** Kalman structure */
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
        timestamp,
        origin,
        score;
        rho_kalman_uncertainty_c
        uncertainty;
        bool
        flag;
        index_t
        min_value,
        max_value;
    } rho_kalman_t;
    
//    floating_t timestamp(void);
    void        InitRhoKalman( rho_kalman_t * k, floating_t v, floating_t ls, floating_t vu, floating_t bu, floating_t su, index_t minv, index_t maxv );
    void       ResetRhoKalman( rho_kalman_t * k, floating_t v );
    void     PredictRhoKalman( rho_kalman_t * k, floating_t rate_new );
    void      UpdateRhoKalman( rho_kalman_t * k, floating_t value_new );
    floating_t  StepRhoKalman( rho_kalman_t * k, floating_t value_new, floating_t rate_new );
    bool   IsRhoKalmanExpired( rho_kalman_t * k );
    floating_t ScoreRhoKalman( rho_kalman_t * k );
    void      PunishRhoKalman( rho_kalman_t * k );
    
    struct rho_kalman {
        void (*       Init)( rho_kalman_t *, floating_t, floating_t, floating_t, floating_t, floating_t, index_t, index_t );
        void (*      Reset)( rho_kalman_t *, floating_t );
        void (*    Predict)( rho_kalman_t *, floating_t );
        void (*     Update)( rho_kalman_t *, floating_t );
        floating_t (* Step)( rho_kalman_t *, floating_t, floating_t );
        bool (*  IsExpired)( rho_kalman_t * );
        floating_t (*Score)( rho_kalman_t * );
        void (*     Punish)( rho_kalman_t * );
    };
    
    static const struct rho_kalman RhoKalman =
    {
        .Init = InitRhoKalman,
        .Reset = ResetRhoKalman,
        .Predict = PredictRhoKalman,
        .Update = UpdateRhoKalman,
        .Step = StepRhoKalman,
        .IsExpired = IsRhoKalmanExpired,
        .Score = ScoreRhoKalman,
        .Punish = PunishRhoKalman
    };
    
#ifdef __cplusplus
}
#endif

#endif /* kalman_h */

