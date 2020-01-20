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

void SystemBehavior_PerformProbeSend( system_task_id_t id )
{
}
void SystemBehavior_PerformProbeReceive( system_task_id_t id )
{
}
void SystemBehavior_PerformSchedulerSchedule( system_task_id_t id, uint32_t data )
{
}
void SystemBehavior_PerformSchedulerDeschedule( system_task_id_t id )
{
    //Deschedule BehaviorScheduledTasks[id]
}
void SystemBehavior_PerformInterrupterSend( system_task_id_t id )
{
    //Enable BehaviorInterruptTasks[id]
}
void SystemBehavior_PerformInterrupterReceive( system_task_id_t task_id )
{
    bool handled = false;
    os_task_data_t * task_data = SystemFunctions.Get.TaskById( task_id );
    if( task_data == NULL ) return;
    switch( task_data->event_data.interrupt.action )
    {
        case  INTERRUPT_ACTION_IMMEDIATE:
            OSFunctions.Task.Resume( task_data );
        case INTERRUPT_ACTION_IGNORE:
        default:
            handled = true;
            break;
        case INTERRUPT_ACTION_QUEUE:
            break;
    }
    if( !handled )
    {
        os_queue_data_t queue_data;
//        message_t message = App.Get.MessageForComponent( component->ID );
        OSFunctions.Queue.Post( &queue_data );
    }
    
//    os_task_data_t * task = SystemFunctions.Get.TaskByComponent( component );
//    void_handler_t handler = SystemFunctions.Get.HandlerByComponentId( component->ID );
//    if(handler == NULL) return;
//    handler();
    //Handle BehaviorInterruptTasks[id]
}
void SystemBehavior_PerformInterrupterPerform( system_task_id_t id )
{
    //Disable BehaviorInterruptTasks[id]
}

//void InitProfileEntry( system_profile_entry_t * entry )
//{
//}
