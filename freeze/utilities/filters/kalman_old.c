#include "kalman.h"
//#ifndef TIMESTAMP
#include "timestamp.h"

#define DEBUG_KALMAN_PRIO DEBUG_2

//#endif
/* Based on: http://tinyurl.com/9djhrem */

void Kalman_Initialize( kalman_t * k, floating_t v, floating_t ls, floating_t minv, floating_t maxv, kalman_uncertainty_c uncertainty )
{
    k->lifespan    = ls;
    k->uncertainty.value   = uncertainty.value;
    k->uncertainty.bias    = uncertainty.bias;
    k->uncertainty.sensor  = uncertainty.sensor;
    
    k->origin    = TIMESTAMP(KALMAN_TIME_UNITS);
    k->t = k->origin;
    
    k->min_value = minv;
    k->max_value = maxv;
    
    k->acceleration_mode = false;
    
    Kalman_Reset(k, v);
}

void Kalman_Reset( kalman_t * k, floating_t v )
{
    k->K[0]        = 0; /* Gain */
    k->K[1]        = 0;
    k->P[0][0]     = 0; /* Error covariance */
    k->P[0][1]     = 0;
    k->P[1][0]     = 0;
    k->P[1][1]     = 0;
    k->rate        = 0;
    k->bias        = 0;
    k->prev        = 0;
    k->velocity    = 0;
    k->variance    = 0;
    k->flag        = 0;
    k->score       = 0;
    k->value       = v;
    k->origin      = TIMESTAMP(KALMAN_TIME_UNITS);
}

floating_t Kalman_Test( kalman_t * k, floating_t rate_new )
{
    floating_t dt =
#ifdef SET_DT_SEC
        SET_DT_SEC;
#else
        ((floating_t)TIMESTAMP(TIME_SEC)) - k->t;
#endif
    
    /* \hat{x}_{k\mid k-1} = F \hat{x_{k-1\mid k-1}} + B \dot{\theta}_k */
    floating_t velocity = k->prev > 0 ? k->value - k->prev : 0.0;
    floating_t rate     = k->acceleration_mode ?
        dt * rate_new - k->bias + velocity
        : rate_new - k->bias;
    floating_t value   = k->value + dt * rate;
    return BOUND(value, k->min_value, k->max_value);
}

void Kalman_Predict( kalman_t * k, floating_t rate_new )
{
    floating_t dt =
#ifdef SET_DT_SEC
        SET_DT_SEC;
#else
        ((floating_t)TIMESTAMP(KALMAN_TIME_UNITS)) - k->t;
#endif
    
    /* Quick expiration check */
    if(dt > k->lifespan)
    {
        k->t = TIMESTAMP(KALMAN_TIME_UNITS);
        return;
    }
    
    /* \hat{x}_{k\mid k-1} = F \hat{x_{k-1\mid k-1}} + B \dot{\theta}_k */
    k->velocity = k->prev > 0 ? k->value - k->prev : 0.0;
    k->prev     = k->value;
    k->rate     = k->acceleration_mode ?
        dt * rate_new - k->bias + k->velocity
        : rate_new - k->bias;
    if( k->rate > 101)
        printf("!");
    k->value   += dt * k->rate;
    k->value    = BOUND(k->value, k->min_value, k->max_value);
    
    /* P_{k\mid k-1} = F P_{k-1\mid k-1} F^T + Q_k */
    floating_t dt_P_1_1 = dt * k->P[1][1];
    k->P[0][0]   += dt * ( dt_P_1_1 -
                                  k->P[0][1] -
                                  k->P[1][0] +
                                  k->uncertainty.value );
    k->P[0][1]   -= dt_P_1_1;
    k->P[1][0]   -= dt_P_1_1;
    k->P[1][1]   += k->uncertainty.bias * dt;
    
//    LOG_KALMAN(KALMAN_DEBUG_2, "Prediction - Value:%.2f Rate:%.2f Velocity:%.2f\n", k->value, k->rate, k->velocity);
}

void Kalman_Update( kalman_t * k, floating_t value_new )
{
    /* S_k = H P_{k\mid k-1} H^T + R */
    floating_t S_ = 1. / ( k->P[0][0] + k->uncertainty.sensor );
    
    /* K_k = P_{k\mid k-1} H^T S^{-1}_k */
    k->K[0]       = k->P[0][0] * S_;
    k->K[1]       = k->P[1][0] * S_;
    
    /* \tilde{y} = z_k - H \hat{x}_{k\mid k-1} */
    floating_t delta_value = value_new - k->value;
    k->value     += k->K[0] * delta_value;
    k->bias      += k->K[1] * delta_value;
    
    k->P[0][0]   -= k->K[0] * k->P[0][0];
    k->P[0][1]   -= k->K[0] * k->P[0][1];
    k->P[1][0]   -= k->K[1] * k->P[0][0];
    k->P[1][1]   -= k->K[1] * k->P[0][1];
    
    k->t  = TIMESTAMP(KALMAN_TIME_UNITS);
    
    k->value = BOUND(k->value, k->min_value, k->max_value);
    
    k->estimation_error = k->value - value_new;
//    LOG_KALMAN(KALMAN_DEBUG_2, "Update - Value:%.2f Bias:%.2f K:%.2f|%.2f\n", k->value, k->bias, k->K[0], k->K[1]);
};

floating_t Kalman_Tick( kalman_t * k, floating_t value_new )
{
    return Kalman.Step( k, value_new, k->velocity );
}

floating_t Kalman_Step( kalman_t * k, floating_t value_new, floating_t rate_new )
{
//    LOG_KALMAN(KALMAN_DEBUG_2, "Step - Id:%p NewVal:%.2f NewRate:%.2f\n", k, value_new, rate_new);
    Kalman_Predict(k, rate_new);
    Kalman_Update(k, value_new);
    return k->value;
}

bool Kalman_IsExpired( kalman_t * k )
{
#ifdef SET_DT_SEC
    return false;
#else
    return ((TIMESTAMP(KALMAN_TIME_UNITS) - k->t) > k->lifespan);
#endif
}

inline floating_t Kalman_Score( kalman_t * k )
{
    floating_t score = k->K[0];
    if(k->flag) score = 0.;
    k->score = score;
    return score;
}

void Kalman_Punish( kalman_t * k )
{
    k->K[0] *= KALMAN_PUNISH_FACTOR;
    if( Kalman_Score(k) < MIN_KALMAN_GAIN )
        Kalman_Reset(k, 0);
}

void Kalman_Print( kalman_t * k )
{
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "Val: %.4f | Rate: %.4f | Vel:%.4f\n", k->value, k->rate, k->velocity);
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "Bias: %.4f | Var: %.4f | Scr:%.4f\n", k->bias, k->variance, k->score);
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "K:\t[%.4f][%.4f]\n", k->K[0], k->K[1]);
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "P:\t[%.4f][%.4f]\n", k->P[0][0], k->P[0][1]);
    LOG_KALMAN(DEBUG_KALMAN_PRIO, "  \t[%.4f][%.4f]\n", k->P[1][0], k->P[1][1]);
}

#ifdef MATVEC_LIB
gaussian1d_t GetGaussian1DKalman( kalman_filter_t * k )
{
    return (gaussian1d_t){ k->value, k->P[0][0] };
}
#endif

const struct kalman_functions Kalman =
{
    .Initialize = Kalman_Initialize,
    .Reset = Kalman_Reset,
    .Test = Kalman_Test,
    .Predict = Kalman_Predict,
    .Update = Kalman_Update,
    .Tick = Kalman_Tick,
    .Step = Kalman_Step,
    .IsExpired = Kalman_IsExpired,
    .Score = Kalman_Score,
    .Punish = Kalman_Punish,
    .Print = Kalman_Print,
#ifdef MATVEC_LIB
    .Gaussian1D = GetGaussian1DKalman
#endif
};
