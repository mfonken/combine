//
//  systemmanager.c
//  C11_rtos
//
//  Created by Matthew Fonken on 8/6/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "systemmanager.h"

void InitSystemManager( )
{
    InitProfile( &Profile );
}

void PerformRoutine( system_activity_routine_t * list )
{
    RegisterSystemActivity(list->activity);
    
    uint8_t len = list->length;
    PerformRoutineSubactivities( list->subactivities, len );
}

void PerformRoutineSubactivities( system_subactivity_t * subactivities, uint8_t len )
{
    for( uint8_t i = 0; i < len; i++ )
    {
        system_subactivity_t subactivity = subactivities[i];
        PerformSubactivity( subactivity );
    }
}

void PerformSubactivity( system_subactivity_t subactivity )
{
    RegisterSystemSubactivity(subactivity);
    
    switch( subactivity )
    {
        case SYSTEM_SUBACTIVITY_INIT_COMMUNICATION:
            break;
        case SYSTEM_SUBACTIVITY_INIT_COMPONENTS:
            Bridge.init.components();
            break;
        case SYSTEM_SUBACTIVITY_INIT_TAU_CLIENT:
            break;
        case SYSTEM_SUBACTIVITY_INIT_KINETIC:
            Bridge.init.kinetic();
            break;
        case SYSTEM_SUBACTIVITY_INIT_CONFIRM:
            break;
        default:
            break;
    }
}

void PerformSystemRoutine( const system_activity_routine_t * list )
{
    PerformRoutine( (system_activity_routine_t *)list );
}
void RegisterSystemStateProfile( system_state_t state, system_state_profile_t state_profile )
{
	System.state_profile[state] = state_profile;
}
void RegisterSystemState( system_state_t state )
{
    System.state = state;
}
void RegisterSystemAction( system_action_t action )
{
    System.action = action;
}
void RegisterSystemActivity( system_activity_t activity )
{
    System.activity = activity;
}
void RegisterSystemSubactivity( system_subactivity_t subactivity )
{
    System.subactivity = subactivity;
}
void RegisterSystemError( system_error_t error )
{
    System.error = error;
}
void RegisterSystemConsumption( system_consumption_t consumption )
{
    System.consumption_level = consumption;
}

