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
    printf("Initializing System Manager\n");
    SystemFunctions.Register.Profile( profile );
    
    SystemFunctions.Register.State(SYSTEM_STATE_STARTUP);
    SystemFunctions.Enstate.TaskShelfEntry(SYSTEM_TASK_SHELF_ENTRY_ID_GLOBAL_TASKS);
    
    SystemFunctions.Register.Error(DEFAULT_SYSTEM_ERROR);
    SystemFunctions.Register.Consumption(SYSTEM_CONSUMPTION_NONE);
}
void PerformSystemManagerRoutine( system_activity_routine_t * routine )
{
    SystemFunctions.Register.Activity(routine->activity);
    SystemFunctions.Perform.Subactivities( routine->subactivities, routine->length );
    SystemFunctions.Register.State( routine->exit_state );
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
    printd("Performing subactivity: %s\n", subactivity_strings[subactivity]);
    system_subactivity_map_entry_t * entry = SystemFunctions.Get.SubactivityMapEntry(subactivity);
    if( entry == NULL ) return;
    
    if( entry->data.pointer != NULL && IS_VALID_PTR(entry->data.pointer)) entry->function.pointer(entry->data.pointer);
    else if( entry->data.byte != NO_DATA ) entry->function.blank();
    else entry->function.byte(entry->data.byte);
}
void PerformSystemManagerEnableProfileEntryState( system_profile_entry_t * entry )
{
    printd("Enabling profile entry state: %s(%d)\n", task_id_strings[entry->ID], entry->ID);
    entry->header.state = SYSTEM_PROFILE_ENTRY_STATE_ENABLED;
    
    os_task_data_t * task_data = SystemFunctions.Get.TaskData( entry->ID );
    if( task_data == NULL ) return;
    InterfaceFunctions.OS.Resume(task_data);
}
void PerformSystemManagerDisableProfileEntryState( system_profile_entry_t * entry )
{
    printd("Disabling profile entry state: %s(%d)\n", task_id_strings[entry->ID], entry->ID);
    entry->header.state = SYSTEM_PROFILE_ENTRY_STATE_DISABLED;
    
    os_task_data_t * task_data = SystemFunctions.Get.TaskData( entry->ID );
    InterfaceFunctions.OS.Suspend(task_data);
}
void RegisterSystemManagerOSTaskList( os_task_list_t * task_list )
{
    System.os_tasks = task_list;
    for( uint8_t i = 0; i < NUM_SYSTEM_TASKS; i++ )
        InterfaceFunctions.OS.Create( &((*System.os_tasks)[i]) );
}
void RegisterSystemManagerTaskShelf( system_task_shelf_t * shelf )
{
    for(uint8_t i = 0; i < MAX_TASK_SHELF_ENTRIES; i++)
    {
        system_task_shelf_entry_t * entry = &(*shelf)[i];
        for( uint8_t j = 0; j < entry->num_interrupts; j++ )
            SystemFunctions.Register.ProfileEntry( &System.profile->shelf[i].interrupts[j]);
        for( uint8_t j = 0; j < entry->num_scheduled; j++ )
            SystemFunctions.Register.ProfileEntry( &System.profile->shelf[i].scheduled[j]);
    }
}
void RegisterSystemManagerSubactivityMap( system_subactivity_map_t * map)
{
    System.subactivity_map = map;
}
void RegisterSystemManagerProfile( system_profile_t * profile )
{
    System.profile = profile;
    SystemFunctions.Register.TaskShelf( &profile->shelf );
    SystemFunctions.Register.StateProfileList( &profile->state_profiles );
}
void RegisterSystemManagerProfileEntry( system_profile_entry_t * entry )
{
    system_subactivity_map_entry_t * handler = SystemFunctions.Get.SubactivityMapEntry( entry->handler_id );
    if( entry->header.state == SYSTEM_PROFILE_ENTRY_STATE_ENABLED)
    {
        switch( entry->header.type )
        {
            case SYSTEM_PROFILE_ENTRY_TYPE_INTERRUPT:
                switch( entry->header.direction )
                {
                    case SYSTEM_PROFILE_ENTRY_DIRECTION_INPUT:
//                        handler->blank();
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
    os_task_data_t * task_data = SystemFunctions.Get.TaskData( entry->ID );
    if( task_data == NULL ) return;
    // Create OS Task with task_data

    ///TODO: Implement interval delay and hardware interrupt functionality!!!
}
void RegisterSystemManagerStateProfileList( system_state_profile_list_t * state_profiles )
{
//    System.profile->state_profiles = state_profiles;
}
void RegisterSystemManagerState( system_state_t state )
{
    if( System.state == state ) return;
    
    printf("\n+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+~+\n\n");
    printd("Registering state: %s\n", state_strings[state]);
    
    System.state = state;
    SystemFunctions.Enstate.StateProfile( &System.profile->state_profiles[state] );
}
void RegisterSystemManagerActivity( system_activity_t activity )
{
    printd("Registering activity: %s\n", activity_strings[activity]);
    System.activity = activity;
}
void RegisterSystemManagerSubactivity( system_subactivity_t subactivity )
{
    printd("Registering subactivity: %s\n", subactivity_strings[subactivity]);
    System.subactivity = subactivity;
}
void RegisterSystemManagerError( system_error_t error )
{
    printd("Registering error: %s\n", error_strings[error]);
    System.error.type = error;
}
void RegisterSystemManagerConsumption( system_consumption_t consumption )
{
    System.consumption_level = consumption;
}
system_subactivity_map_entry_t * GetSystemManagerSubactivityMapEntryById( system_subactivity_t entry_id )
{
    for( uint8_t i = 0; i < NUM_SYSTEM_SUBACTIVITIES; i++ )
    {
        system_subactivity_map_entry_t * entry = &((*System.subactivity_map)[i]);
        if( entry == NULL ) continue;
        if( entry->ID == entry_id ) return entry;
    }
    return NULL;
}
system_task_shelf_entry_t * GetSystemManagerTaskShelfEntryById( system_task_shelf_entry_id_t entry_id )
{
    for( uint8_t i = 0; i < MAX_TASK_SHELF_ENTRIES; i++ )
    {
        system_task_shelf_entry_t * entry = &System.profile->shelf[i];
        if( entry == NULL ) continue;
        if( entry->task_id == entry_id ) return entry;
    }
    return NULL;
}
os_task_data_t * GetSystemManagerTaskDataById( system_task_id_t task_id )
{
    for( uint8_t i = 0; i < NUM_SYSTEM_TASKS; i++ )
    {
        os_task_data_t * task_data = &((*System.os_tasks)[i]);
        if( task_data == NULL ) continue;
        if( task_data->ID == task_id ) return task_data;
    }
    return NULL;
}
void EnstateSystemManagerTaskShelfEntry( system_task_shelf_entry_id_t entry_id )
{
    if( entry_id == SYSTEM_TASK_SHELF_ENTRY_ID_NULL_TASKS ) return;
    printd("Enstating task shelf entry: %s\n", task_shelf_entry_id_strings[entry_id]);
    system_task_shelf_entry_t * entry = SystemFunctions.Get.TaskShelfEntry( entry_id );
    if( entry == NULL ) return;
    for( uint8_t i = 0; i < entry->num_interrupts; i++ )
        SystemFunctions.Perform.EnableProfileEntry( &entry->interrupts[i] );
    for( uint8_t i = 0; i < entry->num_scheduled; i++ )
        SystemFunctions.Perform.EnableProfileEntry( &entry->scheduled[i] );
}
void EnstateSystemManagerStateProfile( system_state_profile_t * state_profile )
{
    printd("Enstating state profile: %s\n", state_strings[state_profile->state_id]);
    
    /* Enable active families */
    uint16_t active = 0;
    for( uint8_t i = 0; i < NUM_SYSTEM_FAMILIES; i++ )
    {
        system_family_t family = state_profile->families[i];
        SysIOCtlFunctions.EnableFamily( family );
        active |= ( 1 << family );
    }
    /* Disable inactive families */
    for( uint8_t i = 0; i < NUM_SYSTEM_FAMILIES; i++ )
    {
        if( active & ( 1 << i ) ) continue;
        SysIOCtlFunctions.DisableFamily( (system_family_t)i );
    }
    
    /* Enable tasks */
    for( uint8_t i = 0; i < state_profile->num_entries; i++ )
        SystemFunctions.Enstate.TaskShelfEntry( state_profile->entries[i] );

    SystemFunctions.Perform.Routine( &state_profile->routine );
}
