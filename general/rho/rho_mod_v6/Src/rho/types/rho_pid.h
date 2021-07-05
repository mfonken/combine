//
//  pid.h
//  rho_deco
//
//  Created by Matthew Fonken on 10/21/18.
//  Copyright © 2019 Marbl. All rights reserved.
//

#ifndef pid_h
#define pid_h

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
    
#include "rho_global.h"
    
    /* PID Filter Configs */
#define PID_SCALE   1//10
#define PID_DRIFT   1.1
    
#define DEFAULT_PROPORTIONAL_FACTOR 0.5
#define DEFAULT_INTEGRAL_FACTOR     0.002
#define DEFAULT_DERIVATIVE_FACTOR   0.001
#define DEFAULT_PID_GAIN (pid_gain_t){ DEFAULT_PROPORTIONAL_FACTOR, DEFAULT_INTEGRAL_FACTOR, DEFAULT_DERIVATIVE_FACTOR, 0., 0. }

    
    typedef struct
    {
        floating_t Kp, Ki, Kd, Ku, Pu;
    } pid_gain_t;
    
    typedef struct
    {
      pid_gain_t
        Gain;
      floating_t
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
    } pid_filter_t;
    
    typedef struct
    {
        void (*Initialize)( pid_filter_t *, pid_gain_t );
        void (*Update)( pid_filter_t *, floating_t, floating_t );
        void (*Print)( pid_filter_t * );
    } pid_functions;
    
    void RhoPIDInitialize( pid_filter_t *, pid_gain_t );
    void RhoPIDUpdate( pid_filter_t *, floating_t, floating_t );
    void RhoPIDPrint( pid_filter_t * );
    
    static const pid_functions RhoPID =
    {
        .Initialize = RhoPIDInitialize,
        .Update = RhoPIDUpdate,
        .Print = RhoPIDPrint
    };
    
#ifdef __cplusplus
}
#endif

#endif /* pid_h */
