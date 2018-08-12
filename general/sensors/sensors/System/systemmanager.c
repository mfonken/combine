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
}

void PerformSystemManagerRoutine( system_activity_routine_t * list )
{
    RegisterSystemActivity(list->activity);
    
    uint8_t len = list->length;
    SystemFunctions.Perform.Subactivities( list->subactivities, len );
}

void PerformSystemManagerRoutineSubactivities( system_subactivity_t * subactivities, uint8_t len )
{
    for( uint8_t i = 0; i < len; i++ )
    {
        system_subactivity_t subactivity = subactivities[i];
        SystemFunctions.Perform.Subactivity( subactivity );
    }
}

void PerformSystemManagerSubactivity( system_subactivity_t subactivity )
{
    system_subactivity_map_entry_t entry = System.subactivity_map[subactivity];
    if( entry.data == NULL ) entry.function();
    else entry.function(entry.data);
}

void PerformSystemManagerSystemRoutine( const system_activity_routine_t * list )
{
    SystemFunctions.Perform.Routine( (system_activity_routine_t *)list );
}

void RegisterSystemManangerSubactivityMap( system_subactivity_map_t * map)
{
    System.subactivity_map = map;
}
void RegisterSystemManagerProfile( system_profile_t * profile )
{
    System.profile = profile;
}
void RegisterSystemManagerSystemStateProfile( system_state_t state, system_state_profile_t state_profile )
{
    System.state_profile[state] = state_profile;
}
void RegisterSystemManagerSystemState( system_state_t state )
{
    SystemFunctions.Enstate.StateProfile( &System.state_profile[state] );
    System.state = state;
}
void RegisterSystemManagerAction( system_action_t action )
{
    System.action = action;
}
void RegisterSystemManagerActivity( system_activity_t activity )
{
    System.activity = activity;
}
void RegisterSystemManagerSubactivity( system_subactivity_t subactivity )
{
    System.subactivity = subactivity;
}
void RegisterSystemError( system_error_t error )
{
    System.error = error;
}
void RegisterSystemManagerConsumption( system_consumption_t consumption )
{
    System.consumption_level = consumption;
}

system_task_shelf_entry_t GetTaskShelfEntryById( system_task_shelf_entry_id_t entry_id )
{
    uint8_t num_entries = sizeof(System.shelf) / sizeof(System.shelf[0]);
    for( uint8_t i = 0; i < num_entries; i++ )
        if( System.shelf[i].task_id == entry_id ) return System.shelf[i];
    return System.profile->shelf[0];
}
void EnstateSystemManagerTaskShelfEntry( system_task_shelf_entry_id_t entry_id )
{
    system_task_shelf_entry_t entry = GetTaskShelfEntryById( entry_id );
    for( uint8_t i = 0; i < entry.num_interrupts; i++ )
        entry.interrupt[i].header.state = SYSTEM_PROFILE_ENTRY_STATE_ENABLED;
    for( uint8_t i = 0; i < entry.num_sheduled; i++ )
        entry.scheduled[i].header.state = SYSTEM_PROFILE_ENTRY_STATE_ENABLED;
}
void EnstateSystemManagerSystemStateProfile( system_state_profile_t * state_profile )
{
    /* Enable families */
    uint8_t num_families = sizeof(state_profile->families) / sizeof( state_profile->families[0] );
    for( uint8_t i = 0; i < num_families; i++ );
//        SYSIOCTLFunctions.EnableFamily( &state_profile->families[i] );
    
    /* Enable Tasks */
    uint8_t num_entries = sizeof(state_profile->entries) / sizeof( state_profile->entries[0] );
    for( uint8_t i = 0; i < num_entries; i++ )
        SystemFunctions.Enstate.TaskShelfEntry( state_profile->entries[i] );
}

void PerformSystemManagerCommEvent( comm_event_t event, uint8_t * data )
{
    switch( event.channel )
    {
        case SYSTEM_COMM_I2C:
            performI2CEvent(*(i2c_event_t *)&event, data );
            break;
        case SYSTEM_COMM_SPI:
            performSPIEvent(*(spi_event_t *)&event, data );
            break;
        case SYSTEM_COMM_UART:
            //performUARTEvent(*(uart_event_t *)&event, data );
            break;
        default:
            break;
    }
}

