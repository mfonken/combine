//
//  rho_pid.c
//  rho_client
//
//  Created by Matthew Fonken on 10/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_pid.h"

void RhoPIDInit( rho_pid_t * PID, rho_pid_gain_t * K )
{
    /* Zero Entire PID */
    memset( PID, 0, sizeof(rho_pid_t) );
    
    if( !K->Kp && !K->Ki && !K->Kd && !K->Pu )
    {
        PID->Gain.Kp = DEFAULT_PROPORTIONAL_FACTOR * K->Ku;
        PID->Gain.Ki = DEFAULT_INTEGRAL_FACTOR * ( PID->Gain.Kp / K->Pu );
        PID->Gain.Kd = DEFAULT_DERIVATIVE_FACTOR * ( PID->Gain.Kd * K->Pu );
    }
    else
    {
        PID->Gain.Kp = K->Kp;
        PID->Gain.Ki = K->Ki;
        PID->Gain.Kd = K->Kd;
    }
}

void RhoPIDUpdate( rho_pid_t * PID, floating_t actual, floating_t target )
{
    PID->Error = actual - target;
    
    PID->Pv = PID->Error * PID->Gain.Kp;
    
    PID->Dt = timestamp() - PID->Timestamp;
    PID->TotalError += PID->Error * PID->Dt;
    PID->Iv = PID->Gain.Ki * PID->TotalError;
    
    PID->DeltaError = PID->PrevError - PID->Error;
    PID->Dv = ( PID->Gain.Kd * PID->DeltaError ) / PID->Dt;
    
    PID->Value = PID->Pv + PID->Iv + PID->Dv + PID->Bias;
    
    PID->Value = BOUND(PID->Value, PID->MinValue, PID->MaxValue);
    
    PID->PrevError = PID->Error;
}

const rho_pid_functions RhoPID =
{
    .Init = RhoPIDInit,
    .Update = RhoPIDUpdate
};
