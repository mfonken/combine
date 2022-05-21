#include "kalman.h"

/** SOURCE: http://preview.tinyurl.com/9djhrem */

//#include "test_setup.h"
#ifdef KALMAN_IS_TIMELESS
#define TIMELESS
#endif

void init( kalman_t * k, double value, double lifespan, kalman_uncertainty_c uncertainty )
{
    k->K[0]        = 0;
    k->K[1]        = 0;
    k->P[0][0]     = 0;
    k->P[0][1]     = 0;
    k->P[1][0]     = 0;
    k->P[1][1]     = 0;
    k->rate        = 0;
    k->bias        = 0;
    k->value       = value;
    k->prev        = 0;
    k->velocity    = 0;
    k->density     = 0;

    k->lifespan    = lifespan;
    k->uncertainty.value   = uncertainty.value;
    k->uncertainty.bias    = uncertainty.bias;
    k->uncertainty.sensor  = uncertainty.sensor;
    
    k->timestamp = TIMESTAMP();
}

double update( kalman_t * k, double value_new, double rate_new, update_type_c type )
{
    double now = TIMESTAMP();
    double delta_time = now - k->timestamp;

    /* Quick expiration check */
    if(delta_time > k->lifespan)
    {
        k->timestamp = now;
        return k->value;
    }
    k->prev       = k->value;
    switch(type)
    {
        default:
        case VELOCITY:
            k->rate   = rate_new - k->bias;
            break;
        case ACCELERATION:
            k->rate   = delta_time * rate_new - k->bias + k->velocity;
            break;
    }
        
    k->value     += delta_time * k->rate;

    double dt_P_1_1 = delta_time * k->P[1][1];
    k->P[0][0] +=   delta_time * ( dt_P_1_1 -
                                   k->P[0][1] -
                                   k->P[1][0] +
                                   k->uncertainty.value );
    k->P[0][1] -=   dt_P_1_1;
    k->P[1][0] -=   dt_P_1_1;
    k->P[1][1] +=   k->uncertainty.bias * delta_time;

    double S      = k->P[0][0] + k->uncertainty.sensor;
    k->K[0]       = k->P[0][0] / S;
    k->K[1]       = k->P[1][0] / S;
    double delta_value = value_new - k->value;
    k->value     += k->K[0] * delta_value;
    k->bias      += k->K[1] * delta_value;

    k->P[0][0] -= k->K[0] * k->P[0][0];
    k->P[0][1] -= k->K[0] * k->P[0][1];
    k->P[1][0] -= k->K[1] * k->P[0][0];
    k->P[1][1] -= k->K[1] * k->P[0][1];

    k->velocity = k->rate;
    k->timestamp = now;
    
    return k->value;
};

int isExpired( kalman_t * k)
{
    return ((TIMESTAMP() - k->timestamp) > k->lifespan);
}

const struct kalman_functions Kalman =
{
    .init = init,
    .update = update,
    .isExpired = isExpired
};

