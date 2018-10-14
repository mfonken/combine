//
//  systembehavior.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/11/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef systembehavior_h
#define systembehavior_h

#include <stdio.h>

#include "systemtypes.h"

void PerformBehaviorSelfCheck(void);
void PerformBehaviorConfirmInit(void);
void PerformBehaviorWaitForWake(void);
void PerformBehaviorProbeSend( system_task_id_t );
void PerformBehaviorProbeReceive( system_task_id_t );
void PerformBehaviorSchedulerSchedule( system_task_id_t, uint32_t );
void PerformBehaviorSchedulerDeschedule( system_task_id_t );
void PerformBehaviorInterrupterSend( system_task_id_t );
void PerformBehaviorInterrupterReceive( system_task_id_t );
void PerformBehaviorInterrupterPerform( system_task_id_t );

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
    .Perform.SelfCheck = PerformBehaviorSelfCheck,
    .Perform.ConfirmInit = PerformBehaviorConfirmInit,
    .Perform.WaitForWake = PerformBehaviorWaitForWake,
    .Perform.Probe.Send = PerformBehaviorProbeSend,
    .Perform.Probe.Receive = PerformBehaviorProbeReceive,
    .Perform.Scheduler.Schedule = PerformBehaviorSchedulerSchedule,
    .Perform.Scheduler.Deschedule = PerformBehaviorSchedulerDeschedule,
    .Perform.Interrupter.Send = PerformBehaviorInterrupterSend,
    .Perform.Interrupter.Receive = PerformBehaviorInterrupterReceive,
    .Perform.Interrupter.Perform = PerformBehaviorInterrupterPerform
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
