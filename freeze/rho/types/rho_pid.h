//
//  rho_pid.h
//  rho_client
//
//  Created by Matthew Fonken on 10/21/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_pid_h
#define rho_pid_h

#include <string.h>
#include "rho_global.h"

typedef double pidfl_t;

#define DEFAULT_PROPORTIONAL_FACTOR 0.6
#define DEFAULT_INTEGRAL_FACTOR     2
#define DEFAULT_DERIVATIVE_FACTOR   0.125
#define DEFAULT_PID_GAIN { DEFAULT_PROPORTIONAL_FACTOR, DEFAULT_INTEGRAL_FACTOR, DEFAULT_DERIVATIVE_FACTOR }

typedef struct
{
    pidfl_t Kp, Ki, Kd, Ku, Pu;
} rho_pid_gain_t;

typedef struct
{
    rho_pid_gain_t Gain;
pidfl_t
    Pv,
    Iv,
    Dv,
    Value,
    Bias,
    Dt,
    Timestamp,
    PrevError,
    Error,
    DeltaError,
    TotalError,
    MinValue,
    MaxValue;
} rho_pid_t;

typedef struct
{
    void (*Init)( rho_pid_t *, rho_pid_gain_t * );
    void (*Update)( rho_pid_t *, pidfl_t, pidfl_t );
} rho_pid_functions;

void RhoPIDInit( rho_pid_t *, rho_pid_gain_t * );
void RhoPIDUpdate( rho_pid_t *, pidfl_t, pidfl_t );

extern const rho_pid_functions RhoPID;

#endif /* rho_pid_h */
