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
    
    SystemFunctions.Register.State(STATE_NAME_STARTUP);
    SystemFunctions.Register.Error(DEFAULT_SYSTEM_ERROR);
    SystemFunctions.Register.Consumption(SYSTEM_CONSUMPTION_NONE);
}

void SystemManager_PerformRoutine( system_activity_routine_t * routine )
{
    SystemFunctions.Register.Activity(routine->activity);
    SystemFunctions.Perform.Subactivities( routine->subactivities, routine->num_subactivities );
    SystemFunctions.Register.ExitState( routine->exit_state );
}

void SystemManager_PerformRoutineSubactivities( system_subactivity_id_t * subactivities, uint8_t len )
{
    for( uint8_t i = 0; i < len; i++ )
    {
        system_subactivity_id_t subactivity = subactivities[i];
        SystemFunctions.Perform.Subactivity( subactivity );
    }
}

void SystemManager_PerformSubactivity( system_subactivity_id_t subactivity )
{
    uint8_t id = subactivity;
    LOG_SYSTEM(SYSTEM_DEBUG, "Performing subactivity: %s(%d)\n", SYSTEM_SUBACTIVITY_ID_STRINGS[id], id);
    system_subactivity_t * entry = SystemFunctions.Get.SubactivityMapEntry(id);
    if( entry == NULL ) return;
    
    if( entry->function == NULL ) return;
    else
    {
#warning fix for multiple components
        if( entry->num_component_id > 0 && entry->data != &System)
        {
            uint8_t temp_byte;
            if( (uint32_t)entry->data < ( 1 << 7 ) )
                temp_byte = (uint8_t)entry->data;
            entry->data = &temp_byte;
            SystemFunctions.Perform.InjectCommHostIntoTaskData( &entry->data, entry->component_id[0] );
        }
        entry->function( entry->data );
    }
}

void SystemManager_PerformEnableTaskState( system_task_id_t task_id )
{
    LOG_SYSTEM(SYSTEM_DEBUG, "Enabling profile entry state: %s(%d)\n", SYSTEM_TASK_ID_STRINGS[task_id], task_id);
//    entry->header.state = SYSTEM_PROFILE_ENTRY_STATE_ENABLED;
    
    system_task_t * task = SystemFunctions.Get.TaskById( task_id );
    if( task == NULL ) return;
    
//    if( TaskHasValidTimer( task_data ) )
//        OS.Task.Resume(task_data);
//    else
        OS.Task.Create(&task->os_task_data);
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
    LOG_SYSTEM(SYSTEM_DEBUG, "Exiting to state: %s(%d)\n", SYSTEM_STATE_STRINGS[System.state], (uint8_t)System.state);
    SystemFunctions.Instate.StateProfile( &System.profile->state_profiles[System.state-1] );
}

void SystemManager_PerformDisableTaskState( system_task_id_t task_id )
{
    LOG_SYSTEM(SYSTEM_DEBUG, "Disabling profile entry state: %s(%d)\n", SYSTEM_TASK_ID_STRINGS[task_id], task_id);
//    entry->header.state = SYSTEM_PROFILE_ENTRY_STATE_DISABLED;
    
    system_task_t * task = SystemFunctions.Get.TaskById( task_id );
//    if( TaskHasValidTimer( task_data ) )
//        OS.Task.Suspend(task_data);
//    else
        OS.Task.Delete( &task->os_task_data );
}

void SystemManager_PopulateTaskDataOfTask( system_task_t * task )
{
    task->os_task_data.ID = task->ID;
    task->os_task_data.p_name = SYSTEM_TASK_ID_STRINGS[ task->ID ];
    task->os_task_data.p_task = task->function;
    task->os_task_data.p_arg = task->object;
    task->os_task_data.prio = task->PRIORITY;
    task->os_task_data.p_err = task->error;
    
    task->os_task_data.stk_limit = (CPU_STK_SIZE)DEFAULT_STACK_SIZE / DEFAULT_TASK_STACK_LIMIT_FACTOR;
    task->os_task_data.stk_size = (CPU_STK_SIZE)DEFAULT_STACK_SIZE; /* Stack size */
    task->os_task_data.q_size = DEFAULT_QUEUE_SIZE; /* Max messages that can be received through queue */
    task->os_task_data.time_quanta = 0;
    task->os_task_data.p_ext = NULL;
    task->os_task_data.opt = (OS_OPT)DEFAULT_TASK_OS_OPTIONS;
}

void SystemManager_RegisterTaskShelf( system_task_shelf_t * shelf )
{
    for(uint8_t i = 0; i < shelf->num_tasks; i++)
    {
        system_task_shelf_entry_t * entry = &(shelf->tasks[i]);
        for( uint8_t j = 0; j < entry->num_interrupts; j++ )
            SystemFunctions.Register.Task( System.profile->shelf.tasks[i].interrupts[j], false );
        for( uint8_t j = 0; j < entry->num_scheduled; j++ )
            SystemFunctions.Register.Task( System.profile->shelf.tasks[i].scheduled[j], true );
    }
}

void SystemManager_RegisterProfile( system_profile_t * profile )
{
    System.profile = profile;
    SystemFunctions.Register.TaskShelf( &profile->shelf );
    SystemFunctions.Register.StateProfileList( &profile->state_profiles);
}

void SystemManager_RegisterTask( system_task_id_t task_id, bool scheduled )
{    
    if(System.registration.tasks[task_id])
    {
        LOG_SYSTEM( SYSTEM_DEBUG, "Profile entry %s already registered.\n", SYSTEM_TASK_ID_STRINGS[task_id]);
        return;
    }
    LOG_SYSTEM( SYSTEM_DEBUG, "Registering profile entry %s(%d).\n", SYSTEM_TASK_ID_STRINGS[task_id], task_id);
    System.registration.tasks[task_id] = true;
    
    system_task_t * task = SystemFunctions.Get.TaskById( task_id );
    if( task == NULL ) return;

    if(task->function != NULL)
        SystemFunctions.Perform.InjectCommHostIntoTaskData( &task->function, task->component_id[0]);
    
    SystemFunctions.Perform.PopulateTaskData( task );
    
    // Create OS utilities with task_data
    if( scheduled && task->ACTION == TASK_ACTION_SCHEDULE)
    {
        task->os_task_data.tmr_opt = OS_OPT_TMR_PERIODIC;
        task->os_task_data.period = HZ_TO_TICK(task->data.schedule);
        
        os_task_data_t * task_data = &task->os_task_data;
        os_timer_data_t timer_data = TIMER_FROM_SCHEDULED_TASK( task_data );
        OS.Timer.Create( &timer_data );
    }
    else if( task->ACTION == TASK_ACTION_INTERRUPT )
    {
        for( uint8_t i = 0; i < task->num_component_id; i++ )
        {
            int8_t component_number = SystemFunctions.Get.ComponentNumber(task->component_id[i]);
            if(component_number < 0 )
            {
                LOG_SYSTEM( SYSTEM_DEBUG, "Provided component is invalid!\n");
            }
            else
                System.registration.component_tasks[component_number][task->ID] = true;
        }
    }
}

int8_t SystemManager_GetSystemComponentNumber( component_id_t component_id )
{
    for( uint8_t i = 0; i < System.profile->component_list.num_entries; i++ )
    {
        component_t * check = &System.profile->component_list.entries[i];
        if( check->ID == component_id ) return i;
    }
    return -1;
}

void SystemManager_RegisterStateProfileList( system_state_profile_list_t * state_profiles )
{
//    for( uint8_t i = 0; i < NUM_SYSTEM_STATES; i++ )
//    {
//        for( uint8_t j = 0; j < state_profiles[i]->routine.num_subactivities; j++ )
//        {
//            system_subactivity_id_t subactivity_id = state_profiles[i]->routine.subactivities[j];
//
//            component_id_t * component_id_to_assign = NULL;
//            for( uint8_t k = 0; k < state_profiles[i]->num_entries; k++ )
//            {
//                system_task_shelf_entry_id_t task_shelf_entry_id = state_profiles[i]->entries[k];
//                system_task_shelf_entry_t * task_shelf_entry = SystemFunctions.Get.TaskShelfEntry( task_shelf_entry_id );
//                if( task_shelf_entry == NULL ) continue;
//                for( uint8_t l = 0; l < task_shelf_entry->num_interrupts; l++ )
//                {
//                    system_task_t * task = task_shelf_entry->interrupts[l];
//                    if( task->handler_id == subactivity_id )
//                    {
#warning fix for multiple component
////                        component_id_to_assign = &profile_entry->components.ids[0];
//                        break;
//                    }
//                }
//                if( component_id_to_assign == NULL)
//                {
//                    for( uint8_t l = 0; l < task_shelf_entry->num_scheduled; l++ )
//                    {
//                        system_task_t * task = task_shelf_entry->scheduled[l];
//                        if( task->handler_id == subactivity_id )
//                        {
//#warning fix for multiple component
////                            component_id_to_assign = &profile_entry->components.ids[0];
//                            break;
//                        }
//                    }
//                }
//
//                if( component_id_to_assign != NULL)
//                {
//                    system_subactivity_t * subactivity = SystemFunctions.Get.SubactivityMapEntry( subactivity_id );
//                    if( subactivity != NULL )
//#warning fix for multiple component
//                        subactivity->components.ids[0] = *component_id_to_assign;
//                    break;
//                }
//            }
//        }
//    }
}
void SystemManager_RegisterState( system_state_t state )
{
    if( System.state == state ) return;
    LOG_SYSTEM(SYSTEM_DEBUG, "Registering state: %s\n", SYSTEM_STATE_STRINGS[state]);
    
    if( System.state != STATE_NAME_UNKNOWN )
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
    LOG_SYSTEM(SYSTEM_DEBUG, "Registering activity: %s(%d)\n", SYSTEM_ACTIVITY_STRINGS[id], id);
    System.activity = activity;
}
void SystemManager_RegisterSubactivity( system_subactivity_id_t subactivity )
{
    uint8_t id = subactivity;
    LOG_SYSTEM(SYSTEM_DEBUG, "Registering subactivity: %s(%d)\n", SYSTEM_SUBACTIVITY_ID_STRINGS[id], id);
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

system_subactivity_t * SystemManager_GetSubactivityMapEntryById( system_subactivity_id_t entry_id )
{
    for( uint8_t i = 0; i < NUM_SYSTEM_SUBACTIVITY_ID; i++ )
    {
        system_subactivity_t * entry = &System.profile->subactivity_map.entry[i];
        if( entry == NULL ) continue;
        if( entry->ID == entry_id )
            return entry;
    }
    return NULL;
}
system_task_shelf_entry_t * SystemManager_GetTaskShelfEntryById( system_task_shelf_entry_id_t entry_id )
{
    for( uint8_t i = 0; i < MAX_TASK_SHELF_ENTRIES; i++ )
    {
        system_task_shelf_entry_t * entry = &System.profile->shelf.tasks[i];
        if( entry == NULL ) continue;
        if( entry->ID == entry_id ) return entry;
    }
    return NULL;
}

//system_task_t * SystemManager_GetTaskById( system_task_id_t task_id )
//{
//    return &System.profile->task_list.entries[task_id];
//}

system_task_t * SystemManager_GetTaskById( system_task_id_t task_id )
{
    for( uint8_t i = 0; i < NUM_SYSTEM_TASK_ID; i++ )
    {
        system_task_t * task = &System.profile->task_list.entries[i];
        if( task == NULL ) continue;
        if( task->ID == task_id ) return task;
    }
    return NULL;
}

os_task_data_t * SystemManager_GetTaskDataByComponentId( component_id_t component_id )
{
    for( uint8_t i = 0; i < NUM_SYSTEM_TASK_ID; i++ )
    {
        system_task_t * task = SystemFunctions.Get.TaskById(i);
        if( task == NULL ) continue;
        SystemFunctions.Get.ComponentNumber( component_id );
        ///TODO: Finish
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
    if( entry_id == SYSTEM_TASK_SHELF_ENTRY_ID_NONE ) return;
    
    uint8_t id  = entry_id;
    LOG_SYSTEM(SYSTEM_DEBUG, "Instating task shelf entry: %s(%d)\n", SYSTEM_TASK_SHELF_ENTRY_ID_STRINGS[id], id);
    system_task_shelf_entry_t * entry = SystemFunctions.Get.TaskShelfEntry( entry_id );
    if( entry == NULL ) return;
    for( uint8_t i = 0; i < entry->num_interrupts; i++ )
        SystemFunctions.Perform.EnableTask( entry->interrupts[i] );
    for( uint8_t i = 0; i < entry->num_scheduled; i++ )
        SystemFunctions.Perform.EnableTask( entry->scheduled[i] );
}

void SystemManager_InstateStateProfile( system_state_profile_t * state_profile )
{
    uint8_t id = state_profile->state_id;
    LOG_SYSTEM(SYSTEM_DEBUG, "Instating state profile: %s(%d)\n", SYSTEM_STATE_STRINGS[id], id);
    
    if( SysIOCtl.system != NULL )
    {
        /* Enable active families */
        uint16_t active = 0;
        for( uint8_t i = 0; i < NUM_SYSTEM_COMPONENT_FAMILY ; i++ )
        {
            system_family_t family = state_profile->families[i];
            SysIOCtlFunctions.EnableFamily( family );
            active |= ( 1 << family );
        }
        /* Disable inactive families */
        for( uint8_t i = 0; i < NUM_SYSTEM_COMPONENT_FAMILY; i++ )
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
    LOG_SYSTEM(SYSTEM_DEBUG, "Terminating state profile: %s(%d)\n", SYSTEM_STATE_STRINGS[id], id);
    
    /* Enable tasks */
    for( uint8_t i = 0; i < state_profile->num_entries; i++ )
        SystemFunctions.Terminate.TaskShelfEntry( state_profile->entries[i] );
}

void SystemManager_TerminateTaskShelfEntry( system_task_shelf_entry_id_t entry_id )
{
    if( entry_id == SYSTEM_TASK_SHELF_ENTRY_ID_NONE ) return;
    
    uint8_t id = entry_id;
    LOG_SYSTEM(SYSTEM_DEBUG, "Terminating task shelf entry: %s(%d)\n", SYSTEM_TASK_SHELF_ENTRY_ID_STRINGS[id], id);
    system_task_shelf_entry_t * entry = SystemFunctions.Get.TaskShelfEntry( entry_id );
    if( entry == NULL ) return;
    for( uint8_t i = 0; i < entry->num_interrupts; i++ )
        SystemFunctions.Perform.DisableTask( entry->interrupts[i] );
    for( uint8_t i = 0; i < entry->num_scheduled; i++ )
        SystemFunctions.Perform.DisableTask( entry->scheduled[i] );
}

comm_host_t SystemManager_GetCommHostForComponentById( component_id_t component_id )
{
    int8_t component_number = SystemFunctions.Get.ComponentNumber( component_id );
    if( component_number < 0 )
        return NULL_HOST;
    
    component_t * component = &System.profile->component_list.entries[component_number];
    
    comm_host_t comm_host = { component->protocol };
    I2C_Channel * I2C_Channels[] = CHANNEL_I2C;
    SPI_Channel * SPI_Channels[] = CHANNEL_SPI;
    
    switch(component->protocol)
    {
        case COMPONENT_PROTOCOL_I2C:
            comm_host.i2c_host.address = component->addr;
            comm_host.i2c_host.device = I2C_Channels[component->route - 1];
            break;
        case COMPONENT_PROTOCOL_SPI:
            comm_host.spi_host.cs.gpio.pin = component->pin;
            comm_host.spi_host.cs.gpio.port = component->port;
            comm_host.spi_host.device = SPI_Channels[component->route - 1];
            break;
        default:
            break;
    }
    
    return comm_host;
}

void SystemManager_InjectCommHostIntoTaskData( void ** data, component_id_t component_id )
{
    if(*data == NULL) return;
    int8_t component_number = SystemFunctions.Get.ComponentNumber(component_id);
    if( component_number >= 0 )
    { /* If component is included, populate communication host */
        System.registration.comm_hosts[component_number] = SystemFunctions.Get.CommHostForComponentById( component_id );
        System.registration.comm_hosts[component_number].generic_comm_host.buffer = *data; // Slide argument to buffer part of comm_host
        *data = &System.registration.comm_hosts[component_number];
    }
}
