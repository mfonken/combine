#include "rho_kalman.h"

/** SOURCE: http://preview.tinyurl.com/9djhrem */
static kalman_floating_t timestamp(void)
{
    struct timeval stamp;
    gettimeofday(&stamp, NULL);
    return stamp.tv_sec + stamp.tv_usec/1000000.0;
}

static void init( rho_kalman_t * k, kalman_floating_t v, kalman_floating_t ls, kalman_floating_t vu, kalman_floating_t bu, kalman_floating_t su )
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

    k->lifespan    = ls;
    k->uncertainty.value   = vu;
    k->uncertainty.bias    = bu;
    k->uncertainty.sensor  = su;
}

static void update( rho_kalman_t * k, kalman_floating_t value_new, kalman_floating_t rate_new )
{
    kalman_floating_t delta_time = timestamp() - k->timestamp;

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

    kalman_floating_t dt_P_1_1 = delta_time * k->P[1][1];
    k->P[0][0]   +=   delta_time * ( dt_P_1_1 -
                                   k->P[0][1] -
                                   k->P[1][0] +
                                   k->uncertainty.value );
    k->P[0][1]   -=   dt_P_1_1;
    k->P[1][0]   -=   dt_P_1_1;
    k->P[1][1]   +=   k->uncertainty.bias * delta_time;

    kalman_floating_t S_     = 1 / ( k->P[0][0] + k->uncertainty.sensor );
    k->K[0]       = k->P[0][0] * S_;
    k->K[1]       = k->P[1][0] * S_;
    kalman_floating_t delta_value = value_new - k->value;
    k->value     += k->K[0] * delta_value;
    k->bias      += k->K[1] * delta_value;

    k->P[0][0]   -= k->K[0] * k->P[0][0];
    k->P[0][1]   -= k->K[0] * k->P[0][1];
    k->P[1][0]   -= k->K[1] * k->P[0][0];
    k->P[1][1]   -= k->K[1] * k->P[0][1];
    
    k->timestamp  = timestamp();
};

static int isExpired( rho_kalman_t * k )
{
    return ((timestamp() - k->timestamp) > k->lifespan);
}

const struct rho_kalman RhoKalman =
{
    .init = init,
    .update = update,
    .isExpired = isExpired
};


