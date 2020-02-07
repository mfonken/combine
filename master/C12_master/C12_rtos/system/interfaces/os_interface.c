//
//  os_interface.c
//  C12_rtos
//
//  Created by Matthew Fonken on 9/2/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "os_interface.h"

bool TaskHasValidTimer( os_task_data_t * task_data )
{
    return (task_data->event_data.schedule.tmr != NULL
        && task_data->event_data.schedule.period != 0);
}

void OSInterface_Init(void)
{
    LOG_OSI(OSI_DEBUG, "Initializing OS.\n");
    OS_SPECIFIC(OSInterface_Init)();
}

void OSInterface_Start(void)
{
    LOG_OSI(OSI_DEBUG, "Starting OS.\n");
    OS_SPECIFIC(OSInterface_Start)();
}

void OSInterface_DelayMs( uint32_t ms )
{
    LOG_OSI(OSI_DEBUG, "Delaying OS Task for %dms\n", ms);
    OS_SPECIFIC(OSInterface_DelayMs)( ms );
}

void OSInterface_CreateTask( os_task_data_t * task_data )
{
    if( task_data->ID == SYSTEM_ACTION_ID_NONE ) return;
    LOG_OSI(OSI_DEBUG, "Creating OS Task: %s", task_id_strings[(uint8_t)task_data->ID]);
    OS_SPECIFIC(OSInterface_CreateTask)(task_data);
    
    if( TaskHasValidTimer(task_data) )
    {
        os_timer_data_t timer_data = TIMER_FROM_SCHEDULED_TASK( task_data );
        LOG_OSI_BARE(OSI_DEBUG, " with schedule of %.2fHz", TICK_TO_HZ(timer_data.period));
        OS_SPECIFIC(OSInterface_CreateTimer)( &timer_data );
    }
    LOG_OSI_BARE(OSI_DEBUG, "\n");
}

void OSInterface_ResumeTask( os_task_data_t * task_data )
{
    LOG_OSI(OSI_DEBUG, "Resuming OS Task: %s", task_id_strings[(uint8_t)task_data->ID]);
    OS_SPECIFIC(OSInterface_ResumeTask)(task_data);
    
    if( TaskHasValidTimer(task_data) )
    {
        os_timer_data_t timer_data = TIMER_FROM_SCHEDULED_TASK( task_data );
        LOG_OSI_BARE(OSI_DEBUG, " with schedule of %.2fHz", TICK_TO_HZ(timer_data.period));
        OS_SPECIFIC(OSInterface_StartTimer)( &timer_data );
    }
    LOG_OSI_BARE(OSI_DEBUG, "\n");
}

void OSInterface_SuspendTask( os_task_data_t * task_data )
{
    LOG_OSI(OSI_DEBUG, "Suspending OS Task: %s", task_id_strings[(uint8_t)task_data->ID]);
    OS_SPECIFIC(OSInterface_SuspendTask)(task_data);
    
    if( TaskHasValidTimer(task_data) )
    {
        LOG_OSI_BARE(OSI_DEBUG, " with schedule");
        os_timer_data_t timer_data = TIMER_FROM_SCHEDULED_TASK( task_data );
        timer_data.p_callback = 0u;
        OS_SPECIFIC(OSInterface_StopTimer)( &timer_data );
    }
    LOG_OSI_BARE(OSI_DEBUG, "\n");
}

void OSInterface_DeleteTask( os_task_data_t * task_data )
{
    LOG_OSI(OSI_DEBUG, "Deleting OS Task: %s", task_id_strings[(uint8_t)task_data->ID]);
    OS_SPECIFIC(OSInterface_DeleteTask)(task_data);
    
    if( TaskHasValidTimer(task_data) )
    {
        os_timer_data_t timer_data = TIMER_FROM_SCHEDULED_TASK( task_data );
        OS_SPECIFIC(OSInterface_DeleteTimer)( &timer_data );
    }
    LOG_OSI_BARE(OSI_DEBUG, "\n");
}

/* QUEUES */
void OSInterface_CreateQueue( os_queue_data_t * queue_data )
{
    LOG_OSI(OSI_DEBUG, "Creating OS Queue: %s\n", queue_id_strings[queue_data->ID]);
    OS_SPECIFIC(OSInterface_CreateQueue)(queue_data);
}

qty_t OSInterface_DeleteQueue( os_queue_data_t * queue_data )
{
    LOG_OSI(OSI_DEBUG, "Deleting OS Queue: %s\n", queue_id_strings[queue_data->ID]);
    return OS_SPECIFIC(OSInterface_DeleteQueue)(queue_data);
}

qty_t OSInterface_FlushQueue( os_queue_data_t * queue_data )
{
    LOG_OSI(OSI_DEBUG, "Flushing OS Queue: %s\n", queue_id_strings[queue_data->ID]);
    return OS_SPECIFIC(OSInterface_FlushQueue)(queue_data);
}

void OSInterface_PendQueue( os_queue_data_t * queue_data )
{
    LOG_OSI(OSI_DEBUG, "Pending OS Queue: %s\n", queue_id_strings[queue_data->ID]);
    OS_SPECIFIC(OSInterface_PendQueue)(queue_data);
}

qty_t OSInterface_PendAbortQueue( os_queue_data_t * queue_data )
{
    LOG_OSI(OSI_DEBUG, "Aborting pending OS queue: %s\n", queue_id_strings[queue_data->ID]);
    return OS_SPECIFIC(OSInterface_PendAbortQueue)(queue_data);
}

void OSInterface_PostQueue( os_queue_data_t * queue_data )
{
    LOG_OSI(OSI_DEBUG, "Posting OS Task: %s\n", queue_id_strings[queue_data->ID]);
    OS_SPECIFIC(OSInterface_PostQueue)(queue_data);
}

/* Timers */
void OSInterface_CreateTimer( os_timer_data_t * timer_data )
{
    LOG_OSI(OSI_DEBUG, "Creating OS Timer: %s - %.3fHz\n", timer_id_strings[timer_data->ID], TICK_TO_HZ(timer_data->period));
    OS_SPECIFIC(OSInterface_CreateTimer)(timer_data);
}

bool OSInterface_DeleteTimer( os_timer_data_t * timer_data )
{
    LOG_OSI(OSI_DEBUG, "Deleting OS Timer: %s\n", timer_id_strings[timer_data->ID]);
    return OS_SPECIFIC(OSInterface_DeleteTimer)(timer_data);
}

tick_t OSInterface_GetRemainTimer( os_timer_data_t * timer_data )
{
    LOG_OSI(OSI_DEBUG, "Getting remaining time of OS Timer: %s\n", timer_id_strings[timer_data->ID]);
    return OS_SPECIFIC(OSInterface_GetRemainTimer)(timer_data);
}

void OSInterface_SetTimer( os_timer_data_t * timer_data )
{
    LOG_OSI(OSI_DEBUG, "Setting OS Timer: %s - %.3fHz\n", timer_id_strings[timer_data->ID], TICK_TO_HZ(timer_data->period));
    OS_SPECIFIC(OSInterface_SetTimer)(timer_data);
}

bool OSInterface_StartTimer( os_timer_data_t * timer_data )
{
    LOG_OSI(OSI_DEBUG, "Starting OS Timer: %s - %.3fHz\n", timer_id_strings[timer_data->ID], TICK_TO_HZ(timer_data->period));
    return OS_SPECIFIC(OSInterface_StartTimer)(timer_data);
}

os_state_t OSInterface_StartGetTimer( os_timer_data_t * timer_data )
{
    LOG_OSI(OSI_DEBUG, "Getting start time of OS Timer: %s\n", timer_id_strings[timer_data->ID]);
    return OS_SPECIFIC(OSInterface_StartGetTimer)(timer_data);
}

bool OSInterface_StopTimer( os_timer_data_t * timer_data )
{
    LOG_OSI(OSI_DEBUG, "Stopping OS Timer: %s\n", timer_id_strings[timer_data->ID]);
    return OS_SPECIFIC(OSInterface_StopTimer)(timer_data);
}
