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

void SystemManager_Init( system_profile_t * p_profile )
{
    LOG_SYSTEM(SYSTEM_DEBUG, "Initializing System Manager\n");
    SystemFunctions.Register.Profile( p_profile );
    SystemFunctions.Register.State(STATE_NAME_STARTUP);
    SystemFunctions.Register.Error(DEFAULT_SYSTEM_ERROR);
    SystemFunctions.Register.Consumption(SYSTEM_CONSUMPTION_NONE);
    SystemFunctions.Register.MinImmediateHandlePriority(DEFAULT_SYSTEM_MIN_IMMEDIATE_HANDLE_PRIORITY);
}

void SystemManager_PerformRoutine( system_activity_routine_t * p_routine )
{
    SystemFunctions.Register.Activity( p_routine->activity );
    SystemFunctions.Perform.Subactivities( p_routine->subactivities, p_routine->num_subactivities );
    SystemFunctions.Register.ExitState( p_routine->exit_state );
}

void SystemManager_PerformRoutineSubactivities( system_subactivity_id_t * p_subactivities, uint8_t len )
{
    FOR( uint8_t, i, len )
    {
        system_subactivity_id_t subactivity = p_subactivities[i];
        SystemFunctions.Perform.Subactivity( subactivity );
    }
}

void SystemManager_PerformSubactivity( system_subactivity_id_t subactivity )
{
    uint8_t id = subactivity;
    LOG_SYSTEM(SYSTEM_DEBUG, "Performing subactivity: %s(%d)\n", SYSTEM_SUBACTIVITY_ID_STRINGS[id], id);
    system_subactivity_t * p_entry = SystemFunctions.Get.SubactivityMapEntry(subactivity);
    if( p_entry == NULL ) return;
    
    if( p_entry->function == NULL ) return;
    else
    {
#warning fix for multiple components
        if( p_entry->num_component_id > 0 && p_entry->data != &System)
        {
            uint8_t temp_byte;
            if( (uint32_t)p_entry->data < ( 1 << 7 ) )
                temp_byte = *(uint8_t*)&p_entry->data;
            p_entry->data = &temp_byte;
            SystemFunctions.Perform.InjectCommHostIntoTaskData( &p_entry->data, p_entry->component_id[0] );
        }
        p_entry->function( p_entry->data );
    }
}

void SystemManager_PerformEnableTaskState( system_task_id_t task_id )
{
    LOG_SYSTEM(SYSTEM_DEBUG, "Enabling profile entry state: %s(%d)\n", SYSTEM_TASK_ID_STRINGS[task_id], task_id);
//    entry->header.state = SYSTEM_PROFILE_ENTRY_STATE_ENABLED;
    
    system_task_t * p_task = SystemFunctions.Get.TaskById( task_id );
    if( p_task == NULL ) return;
    
//    if( TaskHasValidTimer( task_data ) )
//        OS.Task.Resume(task_data);
//    else
        OS.Task.Create(&p_task->os_task_data);
}

void SystemManager_PerformExitState( void )
{
    if( System.status.state == System.status.exit_state )
    {
        LOG_SYSTEM(SYSTEM_DEBUG, "Rejecting state exit.\n");
        return;
    }
    System.status.prev_state = System.status.state;
    System.status.state = System.status.exit_state;
    LOG_SYSTEM(SYSTEM_DEBUG, "Exiting to state: %s(%d)\n", SYSTEM_STATE_STRINGS[System.status.state], (uint8_t)System.status.state);
    SystemFunctions.Instate.StateProfile( &System.profile->state_profiles[System.status.state-1] );
}

void SystemManager_PerformDisableTaskState( system_task_id_t task_id )
{
    LOG_SYSTEM(SYSTEM_DEBUG, "Disabling profile entry state: %s(%d)\n", SYSTEM_TASK_ID_STRINGS[task_id], task_id);
//    entry->header.state = SYSTEM_PROFILE_ENTRY_STATE_DISABLED;
    
    system_task_t * p_task = SystemFunctions.Get.TaskById( task_id );
//    if( TaskHasValidTimer( task_data ) )
//        OS.Task.Suspend(task_data);
//    else
        OS.Task.Delete( &p_task->os_task_data );
}

void SystemManager_PopulateQueueDataOfQueue( queue_data_t * p_queue_data, os_queue_data_t * p_os_queue_data_t )
{
	p_os_queue_data_t->ID = p_queue_data->ID;
	p_os_queue_data_t->p_name = (CPU_CHAR*)QUEUE_ID_STRINGS[p_queue_data->ID];
	p_os_queue_data_t->max_qty = p_queue_data->max_qty;
	p_os_queue_data_t->p_err = p_queue_data->error;
    p_os_queue_data_t->opt = DEFAULT_OS_Q_OPT;
	p_os_queue_data_t->timeout = (OS_TICK)MS_TO_TICK(p_queue_data->timeout);
	p_os_queue_data_t->msg_size = 0;
}

void SystemManager_PerformCycleQueue( os_queue_data_t * p_queue_data )
{
    LOG_SYSTEM(SYSTEM_DEBUG_QUEUE, "Performing cycle on queue: %s\n", QUEUE_ID_STRINGS[p_queue_data->ID]);
//    ASSERT(queue_data->msg_size, sizeof(hw_event_message_t));
    hw_event_message_t * p_message = (hw_event_message_t *)p_queue_data->p_void;
    
    system_task_t * p_task = SystemFunctions.Get.TaskById( (system_task_id_t)p_message->handle_id );
    if( p_task != NULL )
        OS.Task.Resume( &p_task->os_task_data );
}

void SystemManager_PerformCycleQueues( void )
{
	os_queue_data_t * p_queue_data;
    FOR( uint8_t, i, System.profile->queue_list.num_entries )
    {
    	SystemFunctions.Perform.PopulateQueueData(&System.profile->queue_list.entries[i], p_queue_data);
        SystemFunctions.Perform.CycleQueue(p_queue_data);
    }
}

void SystemManager_PopulateTaskDataOfTask( system_task_t * p_task )
{
    p_task->os_task_data.ID = p_task->ID;
    p_task->os_task_data.p_name = (char *)SYSTEM_TASK_ID_STRINGS[ p_task->ID ];
    p_task->os_task_data.p_task = (OS_TASK_PTR)p_task->function;
    p_task->os_task_data.p_arg = p_task->object;
    p_task->os_task_data.prio = p_task->PRIORITY;
    p_task->os_task_data.p_err = p_task->error;
    
    p_task->os_task_data.stk_limit = (CPU_STK_SIZE)DEFAULT_STACK_SIZE / DEFAULT_TASK_STACK_LIMIT_FACTOR;
    p_task->os_task_data.stk_size = (CPU_STK_SIZE)DEFAULT_STACK_SIZE; /* Stack size */
    p_task->os_task_data.q_size = DEFAULT_QUEUE_SIZE; /* Max messages that can be received through queue */
    p_task->os_task_data.time_quanta = 0;
    p_task->os_task_data.p_ext = NULL;
    p_task->os_task_data.opt = (OS_OPT)DEFAULT_TASK_OS_OPTIONS;
}

bool SystemManager_IsTaskAssignedToComponent( system_task_id_t task_id, int8_t component_number )
{
    return System.registration.component_tasks[component_number][task_id];
}

void SystemManager_RegisterTaskShelf( system_task_shelf_t * p_shelf )
{
    FOR( uint8_t, i, p_shelf->num_tasks )
    {
        system_task_shelf_entry_t * p_entry = &(p_shelf->tasks[i]);

        FOR( uint8_t, j, p_entry->num_interrupts )
            SystemFunctions.Register.Task( System.profile->shelf.tasks[i].interrupts[j], false );
        FOR( uint8_t, j, p_entry->num_scheduled )
            SystemFunctions.Register.Task( System.profile->shelf.tasks[i].scheduled[j], true );
    }
}

void SystemManager_RegisterProfile( system_profile_t * p_profile )
{
    System.profile = p_profile;
    SystemFunctions.Register.TaskShelf( &p_profile->shelf );
    SystemFunctions.Register.StateProfileList( &p_profile->state_profiles);
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
    
    system_task_t * p_task = SystemFunctions.Get.TaskById( task_id );
    if( p_task == NULL ) return;

    if( p_task->function != NULL )
        SystemFunctions.Perform.InjectCommHostIntoTaskData( (void_p_function_data_t*)p_task->function, p_task->component_id[0]);
    
    SystemFunctions.Perform.PopulateTaskData( p_task );
    
    // Create OS utilities with task_data
    if( scheduled && p_task->ACTION == TASK_ACTION_SCHEDULE)
    {
        p_task->os_task_data.tmr_opt = OS_OPT_TMR_PERIODIC;
        p_task->os_task_data.period = HZ_TO_TICK(p_task->data.schedule);
        
        os_task_data_t * p_task_data = &p_task->os_task_data;
        os_timer_data_t timer_data = TIMER_FROM_SCHEDULED_TASK( p_task_data );
        OS.Timer.Create( &timer_data );
    }
    else if( p_task->ACTION == TASK_ACTION_INTERRUPT )
    {
        FOR( uint8_t, i, p_task->num_component_id )
        {
            int8_t component_number = SystemFunctions.Get.ComponentNumber(p_task->component_id[i]);
            if(component_number >= 0 && component_number < System.profile->component_list.num_entries)
                System.registration.component_tasks[component_number][p_task->ID] = true;
            else
                LOG_SYSTEM( SYSTEM_DEBUG, "Provided component is invalid!\n");
        }
    }
}

int8_t SystemManager_GetSystemComponentNumber( component_id_t component_id )
{
    FOR( uint8_t, i, System.profile->component_list.num_entries )
    {
        component_t * p_check = &System.profile->component_list.entries[i];
        if( p_check->ID == component_id ) return i;
    }
    return -1;
}

void SystemManager_RegisterStateProfileList( system_state_profile_list_t * p_state_profiles )
{
//    for( uint8_t i = 0; i < NUM_SYSTEM_STATES; i++ )
//    {
//        for( uint8_t j = 0; j < p_state_profiles[i]->routine.num_subactivities; j++ )
//        {
//            system_subactivity_id_t subactivity_id = p_state_profiles[i]->routine.subactivities[j];
//
//            component_id_t * p_component_id_to_assign = NULL;
//            for( uint8_t k = 0; k < p_state_profiles[i]->num_entries; k++ )
//            {
//                system_task_shelf_entry_id_t task_shelf_entry_id = p_state_profiles[i]->entries[k];
//                system_task_shelf_entry_t * p_task_shelf_entry = SystemFunctions.Get.TaskShelfEntry( task_shelf_entry_id );
//                if( task_shelf_entry == NULL ) continue;
//                for( uint8_t l = 0; l < p_task_shelf_entry->num_interrupts; l++ )
//                {
//                    system_task_t * p_task = p_task_shelf_entry->interrupts[l];
//                    if( p_task->handler_id == subactivity_id )
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
//                        system_task_t * p_task = task_shelf_entry->scheduled[l];
//                        if( p_task->handler_id == subactivity_id )
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
//                    system_subactivity_t * p_subactivity = SystemFunctions.Get.SubactivityMapEntry( subactivity_id );
//                    if( p_subactivity != NULL )
//#warning fix for multiple component
//                        p_subactivity->components.ids[0] = *component_id_to_assign;
//                    break;
//                }
//            }
//        }
//    }
}
void SystemManager_RegisterState( system_state_t state )
{
    if( System.status.state == state ) return;
    LOG_SYSTEM(SYSTEM_DEBUG, "Registering state: %s\n", SYSTEM_STATE_STRINGS[state]);
    
    if( System.status.state != STATE_NAME_UNKNOWN )
        SystemFunctions.Terminate.StateProfile( &System.profile->state_profiles[System.status.state] );
    System.status.prev_state = System.status.state;
    System.status.state = state;
    SystemFunctions.Instate.StateProfile( &System.profile->state_profiles[System.status.state] );
}

void SystemManager_RegisterExitState( system_state_t exit_state )
{
    System.status.exit_state = exit_state;
}

void SystemManager_RegisterActivity( system_activity_t activity )
{
    uint8_t id = activity;
    LOG_SYSTEM(SYSTEM_DEBUG, "Registering activity: %s(%d)\n", SYSTEM_ACTIVITY_STRINGS[id], id);
    System.status.activity = activity;
}
void SystemManager_RegisterSubactivity( system_subactivity_id_t subactivity )
{
    uint8_t id = subactivity;
    LOG_SYSTEM(SYSTEM_DEBUG, "Registering subactivity: %s(%d)\n", SYSTEM_SUBACTIVITY_ID_STRINGS[id], id);
    System.status.subactivity = subactivity;
}

void SystemManager_RegisterError( system_error_t error )
{
    uint8_t id = error;
    LOG_SYSTEM(SYSTEM_DEBUG, "%s error: %s(%d)\n", (id?"Registering":"Clearing"), error_strings[id], id);
    System.error.type = error;
}

void SystemManager_RegisterConsumption( system_consumption_t consumption )
{
    System.status.consumption_level = consumption;
}

void SystemManager_RegisterMinImmediateHandlePriority( system_task_priority_t priority )
{
    System.status.min_immediate_handle_priority = priority;
}

system_subactivity_t * SystemManager_GetSubactivityMapEntryById( system_subactivity_id_t entry_id )
{
    FOR( uint8_t, i, NUM_SYSTEM_SUBACTIVITY_ID )
    {
        system_subactivity_t * p_entry = &System.profile->subactivity_map.entry[i];
        if( p_entry == NULL ) continue;
        if( p_entry->ID == entry_id )
            return p_entry;
    }
    return NULL;
}
system_task_shelf_entry_t * SystemManager_GetTaskShelfEntryById( system_task_shelf_entry_id_t entry_id )
{
    FOR( uint8_t, i, MAX_TASK_SHELF_ENTRIES )
    {
        system_task_shelf_entry_t * p_entry = &System.profile->shelf.tasks[i];
        if( p_entry == NULL ) continue;
        if( p_entry->ID == entry_id ) return p_entry;
    }
    return NULL;
}

//system_task_t * SystemManager_GetTaskById( system_task_id_t task_id )
//{
//    return &System.profile->task_list.entries[task_id];
//}

system_task_t * SystemManager_GetTaskById( system_task_id_t task_id )
{
    FOR( uint8_t, i, NUM_SYSTEM_TASK_ID )
    {
        system_task_t * p_task = &System.profile->task_list.entries[i];
        if( p_task == NULL ) continue;
        if( p_task->ID == task_id ) return p_task;
    }
    return NULL;
}

os_task_data_t * SystemManager_GetTaskDataByComponentId( component_id_t component_id )
{
    FOR( uint8_t, i, NUM_SYSTEM_TASK_ID )
    {
        system_task_t * p_task = SystemFunctions.Get.TaskById((system_task_id_t)i);
        if( p_task == NULL ) continue;
        SystemFunctions.Get.ComponentNumber( component_id );
        ///TODO: Finish
    }
    return NULL;
}

component_id_t SystemManager_GetComponentIdFromPortPin( port_t port, pin_t pin )
{
    FOR( uint8_t, i, System.profile->component_list.num_entries )
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
    system_task_shelf_entry_t * p_entry = SystemFunctions.Get.TaskShelfEntry( entry_id );
    if( p_entry == NULL ) return;
    FOR( uint8_t, i, p_entry->num_interrupts )
        SystemFunctions.Perform.EnableTask( p_entry->interrupts[i] );
    FOR( uint8_t, i, p_entry->num_scheduled )
        SystemFunctions.Perform.EnableTask( p_entry->scheduled[i] );
}

void SystemManager_InstateStateProfile( system_state_profile_t * p_state_profile )
{
    uint8_t id = p_state_profile->state_id;
    LOG_SYSTEM(SYSTEM_DEBUG, "Instating state profile: %s(%d)\n", SYSTEM_STATE_STRINGS[id], id);
    
    if( SysIOCtl.p_system != NULL )
    {
        /* Enable active families */
        uint16_t active = 0;
        FOR( uint8_t, i, NUM_SYSTEM_COMPONENT_FAMILY )
        {
            system_family_t family = p_state_profile->families[i];
            SysIOCtlFunctions.EnableFamily( family );
            active |= ( 1 << family );
        }
        /* Disable inactive families */
        FOR( uint8_t, i, NUM_SYSTEM_COMPONENT_FAMILY )
        {
            if( active & ( 1 << i ) ) continue;
            SysIOCtlFunctions.DisableFamily( (system_family_t)i );
        }
        
        /* Finish component enables and disables */
        SysIOCtlFunctions.CompleteComponentTransients();
    }
    else
    {
        LOG_SYSTEM(SYSTEM_DEBUG, "SYSIOCTL not initialized yet.\n");
    }

    /* Enable tasks */
    FOR( uint8_t, i, p_state_profile->num_entries )
        SystemFunctions.Instate.TaskShelfEntry( p_state_profile->entries[i] );

    SystemFunctions.Perform.Routine( &p_state_profile->routine );
}

void SystemManager_TerminateStateProfile( system_state_profile_t * p_state_profile )
{
    uint8_t id = p_state_profile->state_id;
    LOG_SYSTEM(SYSTEM_DEBUG, "Terminating state profile: %s(%d)\n", SYSTEM_STATE_STRINGS[id], id);
    
    /* Enable tasks */
    FOR( uint8_t, i, p_state_profile->num_entries )
        SystemFunctions.Terminate.TaskShelfEntry( p_state_profile->entries[i] );
}

void SystemManager_TerminateTaskShelfEntry( system_task_shelf_entry_id_t entry_id )
{
    if( entry_id == SYSTEM_TASK_SHELF_ENTRY_ID_NONE ) return;
    
    uint8_t id = entry_id;
    LOG_SYSTEM(SYSTEM_DEBUG, "Terminating task shelf entry: %s(%d)\n", SYSTEM_TASK_SHELF_ENTRY_ID_STRINGS[id], id);
    system_task_shelf_entry_t * p_entry = SystemFunctions.Get.TaskShelfEntry( entry_id );
    if( p_entry == NULL ) return;
    FOR( uint8_t, i, p_entry->num_interrupts )
        SystemFunctions.Perform.DisableTask( p_entry->interrupts[i] );
    FOR( uint8_t, i, p_entry->num_scheduled )
        SystemFunctions.Perform.DisableTask( p_entry->scheduled[i] );
}

comm_host_t SystemManager_GetCommHostForComponentById( component_id_t component_id )
{
    int8_t component_number = SystemFunctions.Get.ComponentNumber( component_id );
    if( component_number < 0 )
        return NULL_HOST;
    
    component_t * p_component = &System.profile->component_list.entries[component_number];
    
    comm_host_t comm_host = { p_component->protocol };
#ifdef CHANNEL_I2C
    I2C_Channel * I2C_Channels[] = CHANNEL_I2C;
#endif
#ifdef CHANNEL_SPI
    SPI_Channel * SPI_Channels[] = CHANNEL_SPI;
#endif
    
    switch(p_component->protocol)
    {
#ifdef CHANNEL_I2C
        case COMPONENT_PROTOCOL_I2C:
            comm_host.i2c_host.address = p_component->addr;
            comm_host.i2c_host.device = I2C_Channels[p_component->route - 1];
            break;
#endif
#ifdef CHANNEL_SPI
        case COMPONENT_PROTOCOL_SPI:
            comm_host.spi_host.gpio.pin = p_component->pin;
            comm_host.spi_host.gpio.port = (GPIO_Port_TypeDef)p_component->port;
            comm_host.spi_host.device = SPI_Channels[p_component->route - 1];
            break;
#endif
        default:
            break;
    }
    
    return comm_host;
}

void SystemManager_InjectCommHostIntoTaskData( void_p_function_data_t* pp_data, component_id_t component_id )
{
    if( *pp_data == NULL ) return;
    int8_t component_number = SystemFunctions.Get.ComponentNumber(component_id);
    if( component_number >= 0 )
    { /* If component is included, populate communication host */
        System.registration.comm_hosts[component_number] = SystemFunctions.Get.CommHostForComponentById( component_id );
        System.registration.comm_hosts[component_number].generic_comm_host.buffer = *pp_data; // Slide argument to buffer part of comm_host
        *pp_data = &System.registration.comm_hosts[component_number];
    }
}
