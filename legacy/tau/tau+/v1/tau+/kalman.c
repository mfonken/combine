#include "kalman.h"

void Kalman_Init( kalman_t *k,
                  double    v )
{
    k->K[0]        = 0;
    k->K[1]        = 0;
    k->P_k[0][0]   = 0;
    k->P_k[0][1]   = 0;
    k->P_k[1][0]   = 0;
    k->P_k[1][1]   = 0;
    k->rate        = 0;
    k->bias        = 0;
    k->value       = v;
}

void Kalman_Update( kalman_t *k,
                    double    value_new,
                    double    rate_new )
{
    double delta_time = timestamp() - k->timestamp;
    k->rate       = rate_new - k->bias;
    k->value     += delta_time * k->rate;
    
    double P_k_diag = delta_time * k->P_k[1][1];
    k->P_k[0][0] +=   delta_time * ( ( delta_time * k->P_k[1][1] ) -
                                     k->P_k[0][1] -
                                     k->P_k[1][0] )
                      + VALUE_UNCERTAINTY;
    k->P_k[0][1] -=   P_k_diag;
    k->P_k[1][0] -=   P_k_diag;
    k->P_k[1][1] +=   BIAS_UNCERTAINTY * delta_time;
    
    double S      = k->P_k[0][0] + SENSOR_UNCERTAINTY;
    k->K[0]       = k->P_k[0][0] / S;
    k->K[1]       = k->P_k[1][0] / S;
    double delta_value = value_new - k->value;
    k->value     += k->K[0] * delta_value;
    k->bias      += k->K[1] * delta_value;
    
    k->P_k[0][0] -= k->K[0] * k->P_k[0][0];
    k->P_k[0][1] -= k->K[0] * k->P_k[0][1];
    k->P_k[1][0] -= k->K[1] * k->P_k[0][0];
    k->P_k[1][1] -= k->K[1] * k->P_k[0][1];
    
    k->timestamp = timestamp();
};

double timestamp(void)
{
    struct timeval stamp;
    gettimeofday(&stamp, NULL);
    return stamp.tv_sec + stamp.tv_usec/1000000.0;
}
