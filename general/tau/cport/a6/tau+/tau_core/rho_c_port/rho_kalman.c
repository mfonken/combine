#include "rho_kalman.h"

/** SOURCE: http://preview.tinyurl.com/9djhrem */
static double timestamp(void)
{
    struct timeval stamp;
    gettimeofday(&stamp, NULL);
    return stamp.tv_sec + stamp.tv_usec/1000000.0;
}

static void init( rho_kalman_t * k, double v, double ls, double vu, double bu, double su )
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

    k->lifespan    = ls;
    k->uncertainty.value   = vu;
    k->uncertainty.bias    = bu;
    k->uncertainty.sensor  = su;
}

static void update( rho_kalman_t * k, double value_new, double rate_new )
{
    double delta_time = timestamp() - k->timestamp;

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

    double dt_P_1_1 = delta_time * k->P[1][1];
    k->P[0][0] +=   delta_time * ( dt_P_1_1 -
                                   k->P[0][1] -
                                   k->P[1][0] +
                                   k->uncertainty.value );
    k->P[0][1] -=   dt_P_1_1;
    k->P[1][0] -=   dt_P_1_1;
    k->P[1][1] +=   k->uncertainty.bias * delta_time;

    double S_     = 1 / ( k->P[0][0] + k->uncertainty.sensor );
    k->K[0]       = k->P[0][0] * S_;
    k->K[1]       = k->P[1][0] * S_;
    double delta_value = value_new - k->value;
    k->value     += k->K[0] * delta_value;
    k->bias      += k->K[1] * delta_value;

    k->P[0][0] -= k->K[0] * k->P[0][0];
    k->P[0][1] -= k->K[0] * k->P[0][1];
    k->P[1][0] -= k->K[1] * k->P[0][0];
    k->P[1][1] -= k->K[1] * k->P[0][1];
    
    k->timestamp = timestamp();
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


