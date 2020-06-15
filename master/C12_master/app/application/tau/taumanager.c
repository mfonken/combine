//
//  taumanager.c
//  C12_rtos
//
//  Created by Matthew Fonken on 8/13/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include <app/application/tau/taumanager.h>
#include "timestamp.h"

#define TAU_ID 0xef

static tau_t Tau;

void RegisterTauManagerState( tau_state_t state )
{
    Tau.header.state = state;
    Tau.header.tasks = TauStateActionList[Tau.header.state];
}

void TauManagerTick(void)
{
    ///TODO: Register abort supervisor
    for( uint8_t i = 0; i < Tau.header.tasks.len; i++ )
    {
        tau_action_t action = Tau.header.tasks.actions[i];
        TauActionList[action]();
    }
}

void TauManagerInit(void)
{
    Tau.header.ID = TAU_ID;
    
    Tau.locks.packet = LOCK;
    Tau.locks.kinetic = LOCK;
    
    TauFunctions.RegisterState( TAU_STATE_START );
    KineticFunctions.DefaultInit( &Tau.kinetic );
    
    Tau.locks.kinetic = UNLOCK;
    Tau.locks.packet = UNLOCK;
}

void TauManagerDoNothing(void)
{
}

void TauManagerStart( void )
{
    RegisterTauManagerState( TAU_STATE_START );
}

void TauManagerPause(void)
{
    RegisterTauManagerState( TAU_STATE_IDLE );
}

void TauManagerStop(void)
{
    Tau.locks.kinetic = UNLOCK;
    Tau.locks.packet = UNLOCK;
}

void TauManagerPerformKinetic(void)
{
    Tau.locks.kinetic = LOCK;
    KineticFunctions.UpdatePosition( &Tau.kinetic, &Tau.orientation.data, &Tau.rho.data[0], &Tau.rho.data[1] );
    Tau.locks.kinetic = UNLOCK;
}

void TauManagerGeneratePacket(void)
{
    Tau.locks.packet = LOCK;
    Tau.packet.ID = Tau.header.ID;
    Tau.packet.state = Tau.header.state;
    for( uint8_t i = 0; i < 3; i++ )
        Tau.packet.data[i] = TauManagerGetElementFromKalman( &Tau.kinetic.filters.position[i] );
    Tau.packet.timestamp = TIMESTAMP();
    Tau.locks.packet = UNLOCK;
}
void TauManagerAnalyzeCycle(void)
{
    
}
