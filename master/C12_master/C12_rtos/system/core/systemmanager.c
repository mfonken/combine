//
//  systemmanager.c
//  C12_rtos
//
//  Created by Matthew Fonken on 8/6/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "systemmanager.h"

sysioctl_t SysIOCtl = { 0 };
system_master_t System = { DEFAULT_SYSTEM_STATE, 0 };

void SystemManager_Init( system_profile_t * profile )
{
    LOG_SYSTEM(SYSTEM_DEBUG, "Initializing System Manager\n");
    SystemFunctions.Register.Profile( profile );
    
    SystemFunctions.Register.State(SYSTEM_STATE_STARTUP);
    SystemFunctions.Register.Error(DEFAULT_SYSTEM_ERROR);
    SystemFunctions.Register.Consumption(SYSTEM_CONSUMPTION_NONE);
}

void SystemManager_PerformRoutine( system_activity_routine_t * routine )
{
    SystemFunctions.Register.Activity(routine->activity);
    SystemFunctions.Perform.Subactivities( routine->subactivities, routine->length );
    SystemFunctions.Register.ExitState( routine->exit_state );
}

void SystemManager_PerformRoutineSubactivities( system_subactivity_t * subactivities, uint8_t len )
{
    for( uint8_t i = 0; i < len; i++ )
    {
        system_subactivity_t subactivity = subactivities[i];
        SystemFunctions.Perform.Subactivity( subactivity );
    }
}

void SystemManager_PerformSubactivity( system_subactivity_t subactivity )
{
    uint8_t id = subactivity;
    LOG_SYSTEM(SYSTEM_DEBUG, "Performing subactivity: %s(%d)\n", subactivity_strings[id], id);
    system_subactivity_map_entry_t * entry = SystemFunctions.Get.SubactivityMapEntry(id);
    if( entry == NULL ) return;

    if( entry->function == NULL ) return;
    else entry->function( entry->data );
}

void SystemManager_PerformEnableProfileEntryState( system_profile_entry_t * entry )
{
    uint8_t id = entry->ID;
    LOG_SYSTEM(SYSTEM_DEBUG, "Enabling profile entry state: %s(%d)\n", task_id_strings[id], id);
    entry->header.state = SYSTEM_PROFILE_ENTRY_STATE_ENABLED;
    
    os_task_data_t * task_data = SystemFunctions.Get.TaskById( entry->ID );
    if( task_data == NULL ) return;
    
//    if( TaskHasValidTimer( task_data ) )
//        OSFunctions.Task.Resume(task_data);
//    else
        OSFunctions.Task.Create(task_data);
}

void SystemManager_PerformExitState( void )
{
    if( System.state == System.exit_state )
    {
        LOG_SYSTEM(SYSTEM_DEBUG, "Rejecting state exit.\n");
        return;
    }
    System.prev_state = System.state;
    System.state = System.exit_state;
    LOG_SYSTEM(SYSTEM_DEBUG, "Exiting to state: %s\n", state_strings[System.state]);
    SystemFunctions.Instate.StateProfile( &System.profile->state_profiles[System.state] );
}

void SystemManager_PerformDisableProfileEntryState( system_profile_entry_t * entry )
{
    uint8_t id = entry->ID;
    LOG_SYSTEM(SYSTEM_DEBUG, "Disabling profile entry state: %s(%d)\n", task_id_strings[id], id);
    entry->header.state = SYSTEM_PROFILE_ENTRY_STATE_DISABLED;
    
    os_task_data_t * task_data = SystemFunctions.Get.TaskById( entry->ID );
//    if( TaskHasValidTimer( task_data ) )
//        OSFunctions.Task.Suspend(task_data);
//    else
        OSFunctions.Task.Delete(task_data);
}
void SystemManager_RegisterTaskList( os_task_list_t * task_list )
{
    System.os_tasks = task_list;
//    for( uint8_t i = 0; i < NUM_SYSTEM_TASKS; i++ )
//        OSFunctions.Task.Create( &((*System.os_tasks)[i]) );
}
void SystemManager_RegisterTaskShelf( system_task_shelf_t * shelf )
{
    for(uint8_t i = 0; i < MAX_TASK_SHELF_ENTRIES; i++)
    {
        system_task_shelf_entry_t * entry = &(*shelf)[i];
        for( uint8_t j = 0; j < entry->num_interrupts; j++ )
            SystemFunctions.Register.ProfileEntry( &System.profile->shelf[i].interrupts[j], false );
        for( uint8_t j = 0; j < entry->num_scheduled; j++ )
            SystemFunctions.Register.ProfileEntry( &System.profile->shelf[i].scheduled[j], true );
    }
}
void SystemManager_RegisterSubactivityMap( system_subactivity_map_t * map)
{
    System.subactivity_map = map;
}
void SystemManager_RegisterProfile( system_profile_t * profile )
{
    System.profile = profile;
    SystemFunctions.Register.TaskShelf( &profile->shelf );
    SystemFunctions.Register.StateProfileList( &profile->state_profiles );
}
void SystemManager_RegisterProfileEntry( system_profile_entry_t * entry, bool scheduled )
{
    uint8_t id = entry->ID;
    if(System.registration.profile_entries[id])
    {
        LOG_SYSTEM( SYSTEM_DEBUG, "Profile entry %s already registered.\n", task_id_strings[id]);
        return;
    }
    LOG_SYSTEM( SYSTEM_DEBUG, "Registering profile entry %s(%d).\n", task_id_strings[id], id);
    System.registration.profile_entries[id] = true;
//    system_subactivity_map_entry_t * handler = SystemFunctions.Get.SubactivityMapEntry( entry->handler_id );
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
    os_task_data_t * task_data = SystemFunctions.Get.TaskById( entry->ID );
    if( task_data == NULL ) return;
    
    // Create OS utilities with task_data
    if( scheduled && entry->data.schedule != 0)
    {
        task_data->event_data.schedule.dly = 0;
        task_data->event_data.schedule.period = HZ_TO_TICK(entry->data.schedule);
        task_data->event_data.schedule.opt = OS_OPT_TMR_PERIODIC;
        
        os_timer_data_t timer_data = TIMER_FROM_SCHEDULED_TASK( task_data );
        OSFunctions.Timer.Create( &timer_data );
    }
    else if( entry->data.action != INTERRUPT_ACTION_IGNORE )
    {
        task_data->event_data.interrupt.action = entry->data.action;
        task_data->event_data.interrupt.component_id = entry->component_id;
        
        int8_t component_number = SystemFunctions.Get.ComponentNumber(entry->component_id);
        if(component_number < 0 )
        {
            LOG_SYSTEM( SYSTEM_DEBUG, "Provided component is invalid!\n");
        }
        else
            System.registration.component_tasks[component_number] = task_data->ID;
    }
    ///TODO: Implement and hardware interrupt functionality!!!
}

int8_t SystemManager_GetSystemComponentNumber( component_id_t component_id )
{
    for( uint8_t i = 0; i < System.profile->component_list.num_entries; i++ )
    {
        component_t * check = &System.profile->component_list.entries[i];
        if( check->ID.macro == component_id.macro
           && check->ID.micro == component_id.micro ) return i;
    }
    return -1;
}

void SystemManager_RegisterStateProfileList( system_state_profile_list_t * state_profiles )
{
//    System.profile->state_profiles = state_profiles;
}
void SystemManager_RegisterState( system_state_t state )
{
    if( System.state == state ) return;
    LOG_SYSTEM(SYSTEM_DEBUG, "Registering state: %s\n", state_strings[state]);
    
    SystemFunctions.Terminate.StateProfile( &System.profile->state_profiles[System.state] );
    System.prev_state = System.state;
    System.state = state;
    SystemFunctions.Instate.StateProfile( &System.profile->state_profiles[System.state] );
}

void SystemManager_RegisterExitState( system_state_t exit_state )
{
    System.exit_state = exit_state;
}

void SystemManager_RegisterActivity( system_activity_t activity )
{
    uint8_t id = activity;
    LOG_SYSTEM(SYSTEM_DEBUG, "Registering activity: %s(%d)\n", activity_strings[id], id);
    System.activity = activity;
}
void SystemManager_RegisterSubactivity( system_subactivity_t subactivity )
{
    uint8_t id = subactivity;
    LOG_SYSTEM(SYSTEM_DEBUG, "Registering subactivity: %s(%d)\n", subactivity_strings[id], id);
    System.subactivity = subactivity;
}
void SystemManager_RegisterError( system_error_t error )
{
    uint8_t id = error;
    LOG_SYSTEM(SYSTEM_DEBUG, "Registering error: %s(%d)\n", error_strings[id], id);
    System.error.type = error;
}
void SystemManager_RegisterConsumption( system_consumption_t consumption )
{
    System.consumption_level = consumption;
}
system_subactivity_map_entry_t * SystemManager_GetSubactivityMapEntryById( system_subactivity_t entry_id )
{
    for( uint8_t i = 0; i < NUM_SYSTEM_SUBACTIVITIES; i++ )
    {
        system_subactivity_map_entry_t * entry = &((*System.subactivity_map)[i]);
        if( entry == NULL ) continue;
        if( entry->ID == entry_id ) return entry;
    }
    return NULL;
}
system_task_shelf_entry_t * SystemManager_GetTaskShelfEntryById( system_task_shelf_entry_id_t entry_id )
{
    for( uint8_t i = 0; i < MAX_TASK_SHELF_ENTRIES; i++ )
    {
        system_task_shelf_entry_t * entry = &System.profile->shelf[i];
        if( entry == NULL ) continue;
        if( entry->task_id == entry_id ) return entry;
    }
    return NULL;
}
os_task_data_t * SystemManager_GetTaskDataById( system_task_id_t task_id )
{
    for( uint8_t i = 0; i < NUM_SYSTEM_TASKS; i++ )
    {
        os_task_data_t * task_data = &((*System.os_tasks)[i]);
        if( task_data == NULL ) continue;
        if( task_data->ID == task_id ) return task_data;
    }
    return NULL;
}

os_task_data_t * SystemManager_GetTaskDataByComponentId( component_id_t component_id )
{
    for( uint8_t i = 0; i < NUM_SYSTEM_TASKS; i++ )
    {
        os_task_data_t * task_data = &((*System.os_tasks)[i]);
        if( task_data == NULL ) continue;
        SystemFunctions.Get.ComponentNumber( component_id );
    }
    return NULL;
}

component_id_t SystemManager_GetComponentIdFromPortPin( port_t port, pin_t pin )
{
    for( uint8_t i = 0; i < System.profile->component_list.num_entries; i++ )
    {
        component_t * check = &System.profile->component_list.entries[i];
        if( check->port == port && check->pin == pin ) return check->ID;
    }
    return SYSTEM_COMPONENT_NONE;
}

void SystemManager_InstateTaskShelfEntry( system_task_shelf_entry_id_t entry_id )
{
    if( entry_id == APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS ) return;
    
    uint8_t id  = entry_id;
    LOG_SYSTEM(SYSTEM_DEBUG, "Instating task shelf entry: %s(%d)\n", task_shelf_entry_id_strings[id], id);
    system_task_shelf_entry_t * entry = SystemFunctions.Get.TaskShelfEntry( entry_id );
    if( entry == NULL ) return;
    for( uint8_t i = 0; i < entry->num_interrupts; i++ )
        SystemFunctions.Perform.EnableProfileEntry( &entry->interrupts[i] );
    for( uint8_t i = 0; i < entry->num_scheduled; i++ )
        SystemFunctions.Perform.EnableProfileEntry( &entry->scheduled[i] );
}

void SystemManager_InstateStateProfile( system_state_profile_t * state_profile )
{
    uint8_t id = state_profile->state_id;
    LOG_SYSTEM(SYSTEM_DEBUG, "Instating state profile: %s(%d)\n", state_strings[id], id);
    
    if( SysIOCtl.system != NULL )
    {
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
    }
    else
    {
        LOG_SYSTEM(SYSTEM_DEBUG, "SYSIOCTL not initialized yet.\n");
    }

    /* Enable tasks */
    for( uint8_t i = 0; i < state_profile->num_entries; i++ )
        SystemFunctions.Instate.TaskShelfEntry( state_profile->entries[i] );

    SystemFunctions.Perform.Routine( &state_profile->routine );
}

void SystemManager_TerminateStateProfile( system_state_profile_t * state_profile )
{
    uint8_t id = state_profile->state_id;
    LOG_SYSTEM(SYSTEM_DEBUG, "Terminating state profile: %s(%d)\n", state_strings[id], id);
    
    /* Enable tasks */
    for( uint8_t i = 0; i < state_profile->num_entries; i++ )
        SystemFunctions.Terminate.TaskShelfEntry( state_profile->entries[i] );
}

void SystemManager_TerminateTaskShelfEntry( system_task_shelf_entry_id_t entry_id )
{
    if( entry_id == APPLICATION_TASK_SHELF_ENTRY_ID_NULL_TASKS ) return;
    
    uint8_t id = entry_id;
    LOG_SYSTEM(SYSTEM_DEBUG, "Terminating task shelf entry: %s(%d)\n", task_shelf_entry_id_strings[id], id);
    system_task_shelf_entry_t * entry = SystemFunctions.Get.TaskShelfEntry( entry_id );
    if( entry == NULL ) return;
    for( uint8_t i = 0; i < entry->num_interrupts; i++ )
        SystemFunctions.Perform.DisableProfileEntry( &entry->interrupts[i] );
    for( uint8_t i = 0; i < entry->num_scheduled; i++ )
        SystemFunctions.Perform.DisableProfileEntry( &entry->scheduled[i] );
}
