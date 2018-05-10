#include "kalman.h"

/** SOURCE: http://preview.tinyurl.com/9djhrem */

#include "test_setup.h"
#ifdef KALMAN_IS_TIMELESS
#define TIMELESS
#endif

void Kalman_Init( kalman_t *k, double v, double ls, double v_u, double b_u, double s_u )
{
    k->K[0]        = 0;
    k->K[1]        = 0;
    k->P[0][0]   = 0;
    k->P[0][1]   = 0;
    k->P[1][0]   = 0;
    k->P[1][1]   = 0;
    k->rate        = 0;
    k->bias        = 0;
    k->value       = v;
    k->prev        = 0;
    k->density     = 0;

    k->lifespan    = ls;
    k->uncertainty.value   = v_u;
    k->uncertainty.bias    = b_u;
    k->uncertainty.sensor  = s_u;
}

void Kalman_Update( kalman_t *k,
                   double    value_new,
                   double    rate_new )
{
    double delta_time = timestamp() - k->timestamp;

    /* Quick expiration check */
    if(delta_time > k->lifespan)
    {
        k->timestamp = timestamp();
        return;
    }
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

    k->timestamp = timestamp();
};

void kalman_copy( kalman_t * a, kalman_t * b )
{
    b->bias         = a->bias;
    b->density      = a->density;
    b->K[0]         = a->K[0];
    b->K[1]         = a->K[1];
    b->P[0][0]      = a->P[0][0];
    b->P[1][0]      = a->P[1][0];
    b->P[0][1]      = a->P[0][1];
    b->P[1][1]      = a->P[1][1];
    b->prev         = a->prev;
    b->timestamp    = a->timestamp;
    b->uncertainty  = a->uncertainty;
    b->value        = a->value;
}

double timestamp(void)
{
#ifdef TIMELESS
    return 1.0;
#else
    struct timeval stamp;
    gettimeofday(&stamp, NULL);
    return stamp.tv_sec + stamp.tv_usec/1000000.0;
#endif
}
