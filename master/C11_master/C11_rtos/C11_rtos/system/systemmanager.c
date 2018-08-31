//
//  systemmanager.c
//  C11_rtos
//
//  Created by Matthew Fonken on 8/6/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "systemmanager.h"

system_master_t System = { DEFAULT_SYSTEM_STATE };

void InitSystemManager( system_profile_t * profile )
{
    printf("Manager System pointer is %p\n", &System);
    SystemFunctions.Registers.Profile( profile );
    SystemFunctions.Registers.State(SYSTEM_STATE_STARTUP);
    SystemFunctions.Registers.Error(DEFAULT_SYSTEM_ERROR);
    SystemFunctions.Registers.Consumption(SYSTEM_CONSUMPTION_NONE);
    
    EnstateSystemManagerTaskShelfEntry(SYSTEM_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS);
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
    printd("Performing %s\n", subactivity_strings[subactivity]);
    
    system_subactivity_map_entry_t * entry = SystemFunctions.Get.SubactivityMapEntry(subactivity);
    
    if( entry->data.pointer != NULL ) entry->function.pointer(entry->data.pointer);
    else if( entry->data.byte != NO_DATA ) entry->function.blank();
    else entry->function.byte(entry->data.byte);
}
void PerformSystemManagerEnableProfileEntryState( system_profile_entry_t * entry )
{
    printd("Enabling %s(%d)\n", action_id_string[entry->ID.interrupter], entry->ID.interrupter);
    entry->header.state = SYSTEM_PROFILE_ENTRY_STATE_ENABLED;
}
void PerformSystemManagerDisableProfileEntryState( system_profile_entry_t * entry )
{
    printd("Disabling %s(%d)\n", action_id_string[entry->ID.interrupter], entry->ID.interrupter);
    entry->header.state = SYSTEM_PROFILE_ENTRY_STATE_DISABLED;
}

void RegisterSystemManangerTaskShelf( system_task_shelf_t * shelf)
{
    for(uint8_t i = 0; i < MAX_TASK_SHELF_ENTRIES; i++)
    {
        system_task_shelf_entry_t * entry = &(*shelf)[i];
        for( uint8_t j = 0; j < entry->num_interrupts; j++ )
            SystemFunctions.Registers.ProfileEntry( &System.profile->shelf[i].interrupts[j]);
        for( uint8_t j = 0; j < entry->num_scheduled; j++ )
            SystemFunctions.Registers.ProfileEntry( &System.profile->shelf[i].scheduled[j]);
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

void RegisterSystemManagerProfileEntry( system_profile_entry_t * entry )
{
    if( entry->header.state == SYSTEM_PROFILE_ENTRY_STATE_ENABLED)
    {
        switch( entry->header.type )
        {
            case SYSTEM_PROFILE_ENTRY_TYPE_INTERRUPT:
                switch( entry->header.direction )
            {
                case SYSTEM_PROFILE_ENTRY_DIRECTION_INPUT:
                    break;
                case SYSTEM_PROFILE_ENTRY_DIRECTION_OUTPUT:
                    break;
                default:
                    break;
            }
                break;
            case SYSTEM_PROFILE_ENTRY_TYPE_SCHEDULED:
                switch( entry->header.direction )
            {
                case SYSTEM_PROFILE_ENTRY_DIRECTION_INPUT:
                    break;
                case SYSTEM_PROFILE_ENTRY_DIRECTION_OUTPUT:
                    break;
                default:
                    break;
            }
                break;
            default:
                break;
        }
    }
}
void RegisterSystemManagerStateProfileList( system_state_profile_list_t * state_profiles )
{
//    System.profile->state_profiles = state_profiles;
}
void RegisterSystemManagerState( system_state_t state )
{
    printd("Registering %s\n", state_strings[state]);
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
    printd("Registering %s\n", activity_strings[activity]);
    System.activity = activity;
}
void RegisterSystemManagerSubactivity( system_subactivity_t subactivity )
{
    printd("Registering %s\n", subactivity_strings[subactivity]);
    System.subactivity = subactivity;
}
void RegisterSystemManagerError( system_error_t error )
{
    printd("Registering %s\n", error_strings[error]);
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
    printd("Enstating %s\n", task_shelf_entry_id_strings[entry_id]);
    system_task_shelf_entry_t * entry = GetTaskShelfEntryById( entry_id );
    for( uint8_t i = 0; i < entry->num_interrupts; i++ )
        SystemFunctions.Perform.EnableProfileEntry( &entry->interrupts[i] );
    for( uint8_t i = 0; i < entry->num_scheduled; i++ )
        SystemFunctions.Perform.EnableProfileEntry( &entry->scheduled[i] );
}
void EnstateSystemManagerStateProfile( system_state_profile_t * state_profile )
{
    if( state_profile->routine.activity != SYSTEM_ACTIVITY_NONE )
        SystemFunctions.Perform.Routine( &state_profile->routine );
    
    /* Enable families */
    for( uint8_t i = 0; i < NUM_SYSTEM_FAMILIES; i++ )
        SysIOCtlFunctions.EnableFamily( state_profile->families[i] );
    
    /* Enable tasks */
    for( uint8_t i = 0; i < state_profile->num_entries; i++ )
        SystemFunctions.Enstate.TaskShelfEntry( state_profile->entries[i] );
}

