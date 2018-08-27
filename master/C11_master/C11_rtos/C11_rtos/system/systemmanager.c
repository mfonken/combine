//
//  systemmanager.c
//  C11_rtos
//
//  Created by Matthew Fonken on 8/6/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "systemmanager.h"

void InitSystemManager( system_profile_t * profile )
{
    SystemFunctions.Registers.Profile( profile );
    SystemFunctions.Registers.State(SYSTEM_STATE_STARTUP);
//    SystemFunctions.Registers.Action(DEFAULT_SYSTEM_ACTION);
    SystemFunctions.Registers.Activity(DEFAULT_SYSTEM_ACTIVITY);
    SystemFunctions.Registers.Subactivity(DEFAULT_SYSTEM_SUBACTIVITY);
    SystemFunctions.Registers.Error(DEFAULT_SYSTEM_ERROR);
    SystemFunctions.Registers.Consumption(SYSTEM_CONSUMPTION_NONE);
}

void PerformSystemManagerRoutine( system_activity_routine_t * routine )
{
    SystemFunctions.Registers.Activity(routine->activity);
    SystemFunctions.Perform.Subactivities( routine->subactivities, routine->length );
    SystemFunctions.Registers.State( routine->exit_state );
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
    system_subactivity_map_entry_t entry = System.subactivity_map.entries[subactivity];
    if( entry.data.byte != NO_DATA ) entry.function.blank();
    else if( entry.data.pointer != NULL ) entry.function.pointer(entry.data.pointer);
    else entry.function.byte(entry.data.byte);
}
void RegisterSystemManangerTaskShelf( system_task_shelf_t * shelf)
{
    for(uint8_t i = 0; i < MAX_TASK_SHELF_ENTRIES; i++)
    {
        system_task_shelf_entry_t * entry = &(*shelf)[i];
        for( uint8_t j = 0; j < entry->num_interrupts; j++ )
            BehaviorFunctions.InitEntry( &System.profile->shelf[i].interrupts[j]);
        for( uint8_t j = 0; j < entry->num_scheduled; j++ )
            BehaviorFunctions.InitEntry( &System.profile->shelf[i].scheduled[j]);
    }
}
void RegisterSystemManangerSubactivityMap( system_subactivity_map_t map)
{
    System.subactivity_map = map;
}
void RegisterSystemManagerProfile( system_profile_t * profile )
{
    System.profile = profile;
    ProfileFunctions.Init( profile );
}
void RegisterSystemManagerStateProfileList( system_state_profile_list_t * state_profiles )
{
//    System.profile->state_profiles = state_profiles;
}
void RegisterSystemManagerState( system_state_t state )
{
    if( System.state == state ) return;
    SystemFunctions.Enstate.StateProfile( &System.profile->state_profiles[state] );
    System.state = state;
}
//void RegisterSystemManagerAction( system_action_t action )
//{
//    System.action = action;
//}
void RegisterSystemManagerActivity( system_activity_t activity )
{
    System.activity = activity;
}
void RegisterSystemManagerSubactivity( system_subactivity_t subactivity )
{
    System.subactivity = subactivity;
}
void RegisterSystemManagerError( system_error_t error )
{
    System.error = error;
}
void RegisterSystemManagerConsumption( system_consumption_t consumption )
{
    System.consumption_level = consumption;
}
system_subactivity_map_entry_t * GetSubactivityMapEntryById( system_subactivity_t entry_id )
{
    for( uint8_t i = 0; i < NUM_SYSTEM_SUBACTIVITIES; i++ )
    {
        system_subactivity_map_entry_t * entry = &System.subactivity_map.entries[i];
        if( entry->ID == entry_id ) return entry;
    }
    return NULL;
}
system_task_shelf_entry_t * GetTaskShelfEntryById( system_task_shelf_entry_id_t entry_id )
{
    for( uint8_t i = 0; i < MAX_TASK_SHELF_ENTRIES; i++ )
    {
        system_task_shelf_entry_t * entry = &System.profile->shelf[i];
        if( entry->task_id == entry_id ) return entry;
    }
    return NULL;
}
void EnstateSystemManagerTaskShelfEntry( system_task_shelf_entry_id_t entry_id )
{
    system_task_shelf_entry_t * entry = GetTaskShelfEntryById( entry_id );
    for( uint8_t i = 0; i < entry->num_interrupts; i++ )
        entry->interrupts[i].header.state = SYSTEM_PROFILE_ENTRY_STATE_ENABLED;
    for( uint8_t i = 0; i < entry->num_scheduled; i++ )
        entry->scheduled[i].header.state = SYSTEM_PROFILE_ENTRY_STATE_ENABLED;
}
void EnstateSystemManagerStateProfile( system_state_profile_t * state_profile )
{
    if( state_profile->routine.activity != SYSTEM_ACTIVITY_NONE )
        SystemFunctions.Perform.Routine( &state_profile->routine );
    
    /* Enable families */
    for( uint8_t i = 0; i < NUM_SYSTEM_FAMILIES; i++ )
        SysIOCtlFunctions.EnableFamily( state_profile->families[i] );
    
    /* Enable tasks */
    for( uint8_t i = 0; i < MAX_STATE_PROFILE_ENTRIES; i++ )
        SystemFunctions.Enstate.TaskShelfEntry( state_profile->entries[i] );
}

