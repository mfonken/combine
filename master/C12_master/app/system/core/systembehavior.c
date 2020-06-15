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
    if( task->ACTION != TASK_ACTION_INTERRUPT ) return;
    INTERRUPT_ACTION action = task->PRIORITY <= System.status.min_immediate_handle_priority
        ? INTERRUPT_ACTION_IMMEDIATE
        : INTERRUPT_ACTION_QUEUE;
    switch( action )
    {
        case INTERRUPT_ACTION_IMMEDIATE:
            if( task != NULL )
                OS.Task.Resume( &task->os_task_data );
        case INTERRUPT_ACTION_IGNORE:
        default:
            handled = true;
            break;
        case INTERRUPT_ACTION_QUEUE:
            /* Intentionally empty */
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

void SystemBehavior_PerformInterrupterPerform( system_task_id_t task_id )
{
    system_task_t * task = SystemFunctions.Get.TaskById( task_id );
    if( task != NULL )
        OS.Task.Resume( &task->os_task_data );
}

void SystemBehavior_PerformComponentInterrupt( port_t port, pin_t pin, hw_edge_t edge )
{
    component_id_t component_id = SystemFunctions.Get.ComponentIdFromPortPin( port, pin );
    int8_t component_number = SystemFunctions.Get.ComponentNumber( component_id );
    if( component_number >= 0 )
    {
        uint16_t interrupts_triggered = 0;
        hw_event_message_t message = { port, pin, edge };
        
#warning Fix to handle task array
        for( system_task_id_t i = 0; i < NUM_SYSTEM_TASK_ID; i++ )
        {
            if( System.registration.component_tasks[component_number][i] )
            {
                message.handle_id = i;
                SystemBehavior.Perform.Interrupter.Receive( i, message );
                interrupts_triggered++;
                /* Intentional continue for multiple triggers */
            }
        }
        LOG_SYSTEM(SYSTEM_DEBUG, "Triggered %d interrupts for component event %s.\n", interrupts_triggered, COMPONENT_ID_STRINGS[component_id]);
    }
}
