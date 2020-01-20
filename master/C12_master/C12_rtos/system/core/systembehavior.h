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

void SystemBehavior_PerformSelfCheck(void);
void SystemBehavior_PerformConfirmInit(void);
void SystemBehavior_PerformWaitForWake(void);
void SystemBehavior_PerformProbeSend( system_task_id_t );
void SystemBehavior_PerformProbeReceive( system_task_id_t );
void SystemBehavior_PerformSchedulerSchedule( system_task_id_t, uint32_t );
void SystemBehavior_PerformSchedulerDeschedule( system_task_id_t );
void SystemBehavior_PerformInterrupterSend( system_task_id_t );
void SystemBehavior_PerformInterrupterReceive( component_t * );//system_task_id_t );
void SystemBehavior_PerformInterrupterPerform( system_task_id_t );

void InitProfileEntry( system_profile_entry_t * );

typedef struct
{
    void (*SelfCheck)(void);
    void (*ConfirmInit)(void);
    void (*WaitForWake)(void);
    system_probe_functions Probe;
    system_scheduler_functions Scheduler;
    system_interrupter_functions Interrupter;
} behavior_perform_functions;

typedef struct
{
    void (*InitEntry)( system_profile_entry_t * );
    behavior_perform_functions Perform;
} behavior_functions;

static behavior_functions BehaviorFunctions =
{
    .Perform.SelfCheck              = SystemBehavior_PerformSelfCheck,
    .Perform.ConfirmInit            = SystemBehavior_PerformConfirmInit,
    .Perform.WaitForWake            = SystemBehavior_PerformWaitForWake,
    .Perform.Probe.Send             = SystemBehavior_PerformProbeSend,
    .Perform.Probe.Receive          = SystemBehavior_PerformProbeReceive,
    .Perform.Scheduler.Schedule     = SystemBehavior_PerformSchedulerSchedule,
    .Perform.Scheduler.Deschedule   = SystemBehavior_PerformSchedulerDeschedule,
    .Perform.Interrupter.Send       = SystemBehavior_PerformInterrupterSend,
    .Perform.Interrupter.Receive    = SystemBehavior_PerformInterrupterReceive,
    .Perform.Interrupter.Perform    = SystemBehavior_PerformInterrupterPerform
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
