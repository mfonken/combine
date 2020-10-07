//
//  os_interface.c
//  C12_rtos
//
//  Created by Matthew Fonken on 9/2/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include <osinterface.h>

bool TaskHasValidTimer( os_task_data_t * p_task_data )
{
    return (
#ifdef OS_CFG_DBG_EN
    		p_task_data->tmr.NamePtr != NULL &&
#endif
			p_task_data->period != 0 );
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

uint32_t OSInterface_Timestamp( void )
{
    LOG_OSI(OSI_DEBUG, "Getting timestamp.\n");
    return (uint32_t)OS_SPECIFIC(OSInterface_Timestamp);
}

void OSInterface_CreateTask( os_task_data_t * p_task_data )
{
    if( p_task_data->ID == SYSTEM_ACTION_ID_NONE ) return;
    LOG_OSI(OSI_DEBUG, "Creating OS Task: %s\n", SYSTEM_TASK_ID_STRINGS[(uint8_t)p_task_data->ID]);
    OS_SPECIFIC(OSInterface_CreateTask)( p_task_data );
    
    if( TaskHasValidTimer( p_task_data ) )
    {
        os_timer_data_t timer_data = TIMER_FROM_SCHEDULED_TASK( p_task_data );
        LOG_OSI_BARE(OSI_DEBUG, " - with schedule of %.2fHz\n", TICK_TO_HZ(timer_data.period));
        OS_SPECIFIC(OSInterface_CreateTimer)( &timer_data );
    }
//    LOG_OSI_BARE(OSI_DEBUG, "\n");
}

void OSInterface_ResumeTask( os_task_data_t * p_task_data )
{
    LOG_OSI(OSI_DEBUG, "Resuming OS Task: %s", SYSTEM_TASK_ID_STRINGS[(uint8_t)p_task_data->ID]);
    OS_SPECIFIC(OSInterface_ResumeTask)( p_task_data );
    
    if( TaskHasValidTimer( p_task_data ) )
    {
        os_timer_data_t timer_data = TIMER_FROM_SCHEDULED_TASK( p_task_data );
        LOG_OSI_BARE(OSI_DEBUG, " with schedule of %.2fHz", TICK_TO_HZ(timer_data.period));
        OS_SPECIFIC(OSInterface_StartTimer)( &timer_data );
    }
    LOG_OSI_BARE(OSI_DEBUG, "\n");
}

void OSInterface_SuspendTask( os_task_data_t * p_task_data )
{
    LOG_OSI(OSI_DEBUG, "Suspending OS Task: %s", SYSTEM_TASK_ID_STRINGS[(uint8_t)p_task_data->ID]);
    OS_SPECIFIC(OSInterface_SuspendTask)( p_task_data );
    
    if( TaskHasValidTimer( p_task_data ) )
    {
        LOG_OSI_BARE(OSI_DEBUG, " with schedule");
        os_timer_data_t timer_data = TIMER_FROM_SCHEDULED_TASK( p_task_data );
        timer_data.p_callback = 0u;
        OS_SPECIFIC(OSInterface_StopTimer)( &timer_data );
    }
    LOG_OSI_BARE(OSI_DEBUG, "\n");
}

void OSInterface_DeleteTask( os_task_data_t * p_task_data )
{
    LOG_OSI(OSI_DEBUG, "Deleting OS Task: %s", SYSTEM_TASK_ID_STRINGS[(uint8_t)p_task_data->ID]);
    OS_SPECIFIC(OSInterface_DeleteTask)( p_task_data );
    
    if( TaskHasValidTimer( p_task_data ) )
    {
        os_timer_data_t timer_data = TIMER_FROM_SCHEDULED_TASK( p_task_data );
        OS_SPECIFIC(OSInterface_DeleteTimer)( &timer_data );
    }
    LOG_OSI_BARE(OSI_DEBUG, "\n");
}

/* QUEUES */
void OSInterface_CreateQueue( os_queue_data_t * p_queue_data )
{
    LOG_OSI(OSI_DEBUG, "Creating OS Queue: %s\n", SYSTEM_QUEUE_ID_STRINGS[p_queue_data->ID]);
    OS_SPECIFIC(OSInterface_CreateQueue)( p_queue_data );
}

qty_t OSInterface_DeleteQueue( os_queue_data_t * p_queue_data )
{
    LOG_OSI(OSI_DEBUG, "Deleting OS Queue: %s\n", SYSTEM_QUEUE_ID_STRINGS[p_queue_data->ID]);
    return OS_SPECIFIC(OSInterface_DeleteQueue)( p_queue_data );
}

qty_t OSInterface_FlushQueue( os_queue_data_t * p_queue_data )
{
    LOG_OSI(OSI_DEBUG, "Flushing OS Queue: %s\n", SYSTEM_QUEUE_ID_STRINGS[p_queue_data->ID]);
    return OS_SPECIFIC(OSInterface_FlushQueue)( p_queue_data );
}

void OSInterface_PendQueue( os_queue_data_t * p_queue_data )
{
    LOG_OSI(OSI_DEBUG, "Pending OS Queue: %s\n", SYSTEM_QUEUE_ID_STRINGS[p_queue_data->ID]);
    OS_SPECIFIC(OSInterface_PendQueue)( p_queue_data );
}

qty_t OSInterface_PendAbortQueue( os_queue_data_t * p_queue_data )
{
    LOG_OSI(OSI_DEBUG, "Aborting pending OS queue: %s\n", SYSTEM_QUEUE_ID_STRINGS[p_queue_data->ID]);
    return OS_SPECIFIC(OSInterface_PendAbortQueue)( p_queue_data );
}

void OSInterface_PostQueue( os_queue_data_t * p_queue_data )
{
    LOG_OSI(OSI_DEBUG, "Posting OS Task: %s\n", SYSTEM_QUEUE_ID_STRINGS[p_queue_data->ID]);
    OS_SPECIFIC(OSInterface_PostQueue)( p_queue_data );
}

/* Timers */
void OSInterface_CreateTimer( os_timer_data_t * p_timer_data )
{
    LOG_OSI(OSI_DEBUG, "Creating OS Timer: %s - %.3fHz\n", SYSTEM_TIMER_ID_STRINGS[p_timer_data->ID], TICK_TO_HZ(p_timer_data->period));
    OS_SPECIFIC(OSInterface_CreateTimer)( p_timer_data );
}

bool OSInterface_DeleteTimer( os_timer_data_t * p_timer_data )
{
    LOG_OSI(OSI_DEBUG, "Deleting OS Timer: %s\n", SYSTEM_TIMER_ID_STRINGS[p_timer_data->ID]);
    return OS_SPECIFIC(OSInterface_DeleteTimer)( p_timer_data );
}

tick_t OSInterface_GetRemainTimer( os_timer_data_t * p_timer_data )
{
    LOG_OSI(OSI_DEBUG, "Getting remaining time of OS Timer: %s\n", SYSTEM_TIMER_ID_STRINGS[p_timer_data->ID]);
    return OS_SPECIFIC(OSInterface_GetRemainTimer)( p_timer_data );
}

void OSInterface_SetTimer( os_timer_data_t * p_timer_data )
{
    LOG_OSI(OSI_DEBUG, "Setting OS Timer: %s - %.3fHz\n", SYSTEM_TIMER_ID_STRINGS[p_timer_data->ID], TICK_TO_HZ(p_timer_data->period));
    OS_SPECIFIC(OSInterface_SetTimer)( p_timer_data );
}

bool OSInterface_StartTimer( os_timer_data_t * p_timer_data )
{
    LOG_OSI(OSI_DEBUG, "Starting OS Timer: %s - %.3fHz\n", SYSTEM_TIMER_ID_STRINGS[p_timer_data->ID], TICK_TO_HZ(p_timer_data->period));
    return OS_SPECIFIC(OSInterface_StartTimer)( p_timer_data );
}

os_state_t OSInterface_StartGetTimer( os_timer_data_t * p_timer_data )
{
    LOG_OSI(OSI_DEBUG, "Getting start time of OS Timer: %s\n", SYSTEM_TIMER_ID_STRINGS[p_timer_data->ID]);
	return OS_SPECIFIC(OSInterface_StartGetTimer)( p_timer_data );
}

bool OSInterface_StopTimer( os_timer_data_t * p_timer_data )
{
    LOG_OSI(OSI_DEBUG, "Stopping OS Timer: %s\n", SYSTEM_TIMER_ID_STRINGS[p_timer_data->ID]);
    return OS_SPECIFIC(OSInterface_StopTimer)( p_timer_data );
}
