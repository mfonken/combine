//
//  systembehavior.c
//  C12_rtos
//
//  Created by Matthew Fonken on 8/11/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "systembehavior.h"

void SystemBehaviorTask( )
{
}

void SystemBehavior_PerformSelfCheck(void)
{
}

void SystemBehavior_PerformConfirmInit(void)
{
}

void SystemBehavior_PerformWaitForWake(void)
{
}

void SystemBehavior_PerformProbeSend( system_task_id_t task_id )
{
}

void SystemBehavior_PerformProbeReceive( system_task_id_t task_id )
{
}

void SystemBehavior_PerformSchedulerSchedule( system_task_id_t task_id, uint32_t data )
{
}

void SystemBehavior_PerformSchedulerDeschedule( system_task_id_t task_id )
{
    //Deschedule BehaviorScheduledTasks[id]
}

void SystemBehavior_PerformInterrupterSend( system_task_id_t task_id )
{
    //Enable BehaviorInterruptTasks[id]
//    os_task_data_t * task_data = SystemFunctions.Get.TaskById( task_id );
}

void SystemBehavior_PerformInterrupterReceive( system_task_id_t task_id, hw_event_message_t message )
{
    bool handled = false;
    system_task_t * task = SystemFunctions.Get.TaskById( task_id );
    if( task == NULL ) return;
    switch( task->ACTION )
    {
        case INTERRUPT_ACTION_IMMEDIATE:
            OS.Task.Resume( &task->os_task_data );
        case INTERRUPT_ACTION_IGNORE:
        default:
            handled = true;
            break;
        case INTERRUPT_ACTION_QUEUE:
            break;
    }
    if( !handled )
    {
        os_queue_data_t * queue_data = &System.profile->queue_list.entries[INTERRUPT_CHANNEL];
        queue_data->p_void = (void*)&message;
        queue_data->msg_size = sizeof(hw_event_message_t);
        OS.Queue.Post( queue_data );
    }
}
void SystemBehavior_PerformInterrupterPerform( system_task_id_t id )
{
    //Disable BehaviorInterruptTasks[id]
}

//void InitTask( system_task_id_t id )
//{
//}
