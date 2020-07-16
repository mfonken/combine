//
//  systembehavior.h
//  C12_rtos
//
//  Created by Matthew Fonken on 8/11/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef systembehavior_h
#define systembehavior_h

#include <stdio.h>
#include "systemmanager.h"

//void SystemBehavior_PerformSelfCheck(void);
//void SystemBehavior_PerformConfirmInit(void);
//void SystemBehavior_PerformWaitForWake(void);
//void SystemBehavior_PerformProbeSend( system_task_id_t );
//void SystemBehavior_PerformProbeReceive( system_task_id_t );
//void SystemBehavior_PerformSchedulerSchedule( system_task_id_t, uint32_t );
//void SystemBehavior_PerformSchedulerDeschedule( system_task_id_t );
//void SystemBehavior_PerformInterrupterSend( system_task_id_t );
void SystemBehavior_PerformInterrupterReceive( system_task_id_t, hw_event_message_t );
void SystemBehavior_PerformInterrupterPerform( system_task_id_t );
void SystemBehavior_PerformComponentInterrupt( port_t, pin_t, hw_edge_t );

void InitTask( system_task_id_t );

typedef struct
{
//    void (*SelfCheck)(void);
//    void (*ConfirmInit)(void);
//    void (*WaitForWake)(void);
    void (*ComponentInterrupt)( port_t, pin_t, hw_edge_t);
//    system_probe_functions Probe;
//    system_scheduler_functions Scheduler;
    system_interrupter_functions Interrupter;
} system_behavior_perform_functions;

typedef struct
{
    void (*InitTask)( system_task_id_t );
    system_behavior_perform_functions Perform;
} system_behavior_functions;

static system_behavior_functions SystemBehavior =
{
//    .Perform.SelfCheck              = SystemBehavior_PerformSelfCheck,
//    .Perform.ConfirmInit            = SystemBehavior_PerformConfirmInit,
//    .Perform.WaitForWake            = SystemBehavior_PerformWaitForWake,
//    .Perform.Probe.Send             = SystemBehavior_PerformProbeSend,
//    .Perform.Probe.Receive          = SystemBehavior_PerformProbeReceive,
//    .Perform.Scheduler.Schedule     = SystemBehavior_PerformSchedulerSchedule,
//    .Perform.Scheduler.Deschedule   = SystemBehavior_PerformSchedulerDeschedule,
//    .Perform.Interrupter.Send       = SystemBehavior_PerformInterrupterSend,
    .Perform.Interrupter.Receive    = SystemBehavior_PerformInterrupterReceive,
    .Perform.Interrupter.Perform    = SystemBehavior_PerformInterrupterPerform,
    .Perform.ComponentInterrupt     = SystemBehavior_PerformComponentInterrupt
};

//static generic_function_t BehaviorScheduledTasks[] =
//{
//    NullFunction, /* SYSTEM_SCHEDULER_ID_NONE = 0 */
//    NullFunction, /* SYSTEM_SCHEDULER_ID_TAU_DATA_TRANFER */
//    TauManagerTick, /* SYSTEM_SCHEDULER_ID_TAU_PERFORM */
//    NullFunction /* SYSTEM_SCHEDULER_ID_TAU_PACKET_QUEUE */
//};
//
//static generic_function_t BehaviorInterruptTasks[] =
//{
//    NullFunction, /* SYSTEM_INTERRUPTER_ID_TAU_PERFORM */
//    NullFunction, /* SYSTEM_INTERRUPTER_ID_TAU_RHO_RECEIVE */
//    NullFunction, /* SYSTEM_INTERRUPTER_ID_TAU_PACKET_GENERATE */
//    NullFunction /* SYSTEM_INTERRUPTER_ID_HAPTIC_PACKET_GENERATE */
//};

#endif /* systembehavior_h */
