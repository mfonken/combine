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
#include "timestamp.h"

//#define STRICT_PROPORTIONAL_FACTOR 0.98
//#define STRICT_INTEGRAL_FACTOR     0.02
//#define STRICT_DERIVATIVE_FACTOR   0.001

#define DEFAULT_PROPORTIONAL_FACTOR 0.98
#define DEFAULT_INTEGRAL_FACTOR     0.021
#define DEFAULT_DERIVATIVE_FACTOR   0.01
#define DEFAULT_PID_GAIN (rho_pid_gain_t){ DEFAULT_PROPORTIONAL_FACTOR, DEFAULT_INTEGRAL_FACTOR, DEFAULT_DERIVATIVE_FACTOR }

#define BOUNDU(X,MAX)    ((X>MAX)?(MAX-1):X)         // Bound in upper range
#define BOUND(X,MIN,MAX) ((X<MIN)?MIN:BOUNDU(X,MAX)) // Bound in upper and lower range
#define ZDIV_LNUM 1 << 10
#define ZDIV(X,Y) ((Y==0)?(X==0?0:ZDIV_LNUM):X/Y)

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef double floating_t;
    
    typedef struct
    {
        floating_t Kp, Ki, Kd, Ku, Pu;
    } rho_pid_gain_t;
    
    typedef struct
    {
        rho_pid_gain_t
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
    } rho_pid_t;
    
    typedef struct
    {
        void (*Init)( rho_pid_t *, rho_pid_gain_t );
        void (*Update)( rho_pid_t *, floating_t, floating_t );
        void (*Print)( rho_pid_t * );
    } rho_pid_functions;
    
    void RhoPIDInit( rho_pid_t *, rho_pid_gain_t );
    void RhoPIDUpdate( rho_pid_t *, floating_t, floating_t );
    void RhoPIDPrint( rho_pid_t * );
    
    extern const rho_pid_functions RhoPID;
    
#ifdef __cplusplus
}
#endif

#endif /* rho_pid_h */
