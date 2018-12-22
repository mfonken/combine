#include "rho_kalman.h"

/** SOURCE: http://preview.tinyurl.com/9djhrem */

void InitRhoKalman( rho_kalman_t * k, floating_t v, floating_t ls, floating_t vu, floating_t bu, floating_t su, index_t minv, index_t maxv )
{
    k->K[0]        = 0;
    k->K[1]        = 0;
    k->P[0][0]     = 0;
    k->P[0][1]     = 0;
    k->P[1][0]     = 0;
    k->P[1][1]     = 0;
    k->rate        = 0;
    k->bias        = 0;
    k->value       = v;
    k->prev        = 0;
    k->velocity    = 0;
    k->variance    = 0;
    k->flag        = 0;
    
    k->lifespan    = ls;
    k->uncertainty.value   = vu;
    k->uncertainty.bias    = bu;
    k->uncertainty.sensor  = su;
    
    k->min_value = minv;
    k->max_value = maxv;
}

void PredictRhoKalman( rho_kalman_t * k, floating_t rate_new )
{
    floating_t delta_time = timestamp() - k->timestamp;
    
    /* Quick expiration check */
    if(delta_time > k->lifespan)
    {
        k->timestamp = timestamp();
        return;
    }
    k->velocity   = k->value - k->prev;
    k->prev       = k->value;
    k->rate       = rate_new - k->bias;
    k->value     += delta_time * k->rate;
    k->value      = BOUND(k->value, k->min_value, k->max_value);
    
    floating_t dt_P_1_1 = delta_time * k->P[1][1];
    k->P[0][0]   += delta_time * ( dt_P_1_1 -
                                  k->P[0][1] -
                                  k->P[1][0] +
                                  k->uncertainty.value );
    k->P[0][1]   -= dt_P_1_1;
    k->P[1][0]   -= dt_P_1_1;
    k->P[1][1]   += k->uncertainty.bias * delta_time;
}

void UpdateRhoKalman( rho_kalman_t * k, floating_t value_new )
{
    floating_t S_ = 1. / ( k->P[0][0] + k->uncertainty.sensor );
    k->K[0]       = k->P[0][0] * S_;
    k->K[1]       = k->P[1][0] * S_;
    floating_t delta_value = value_new - k->value;
    k->value     += k->K[0] * delta_value;
    k->bias      += k->K[1] * delta_value;
    
    k->P[0][0]   -= k->K[0] * k->P[0][0];
    k->P[0][1]   -= k->K[0] * k->P[0][1];
    k->P[1][0]   -= k->K[1] * k->P[0][0];
    k->P[1][1]   -= k->K[1] * k->P[0][1];
    
    k->timestamp  = timestamp();
    
    k->flag = 0;
};

floating_t StepRhoKalman( rho_kalman_t * k, floating_t value_new, floating_t rate_new )
{
    PredictRhoKalman(k, rate_new);
    UpdateRhoKalman(k, value_new);
    return k->value;
}

bool IsRhoKalmanExpired( rho_kalman_t * k )
{
    return false;//((timestamp() - k->timestamp) > k->lifespan);
}

inline floating_t ScoreRhoKalman( rho_kalman_t * k )
{
    if(k->flag) return 0;
    return k->K[0];
}

void PunishRhoKalman( rho_kalman_t * k )
{
    k->K[0] *= KALMAN_PUNISH_FACTOR;
}
