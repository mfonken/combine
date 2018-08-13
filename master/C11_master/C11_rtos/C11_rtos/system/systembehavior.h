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
void PerformBehaviorProbeSend( system_probe_id_t );
void PerformBehaviorProbeReceive( system_probe_id_t );
void PerformBehaviorSchedulerSchedule( system_scheduler_id_t );
void PerformBehaviorSchedulerDeschedule( system_scheduler_id_t );
void PerformBehaviorInterrupterSend( system_interrupter_id_t );
void PerformBehaviorInterrupterReceive( system_interrupter_id_t );
void PerformBehaviorInterrupterPerform( system_interrupter_id_t );

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

#endif /* systembehavior_h */
