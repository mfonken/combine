//
//  systembehavior.c
//  C11_rtos
//
//  Created by Matthew Fonken on 8/11/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "systembehavior.h"

void SystemBehaviorTask( )
{
}

void PerformBehaviorSelfCheck(void)
{
}
void PerformBehaviorConfirmInit(void)
{
}
void PerformBehaviorWaitForWake(void)
{
}
void PerformBehaviorProbeSend( system_task_id_t id )
{
}
void PerformBehaviorProbeReceive( system_task_id_t id )
{
}
void PerformBehaviorSchedulerSchedule( system_task_id_t id, uint32_t data )
{
}

void PerformBehaviorSchedulerDeschedule( system_task_id_t id )
{
    //Deschedule BehaviorScheduledTasks[id]
}
void PerformBehaviorInterrupterSend( system_task_id_t id )
{
    //Enable BehaviorInterruptTasks[id]
}
void PerformBehaviorInterrupterReceive( system_task_id_t id )
{
    //Handle BehaviorInterruptTasks[id]
}
void PerformBehaviorInterrupterPerform( system_task_id_t id )
{
    //Disable BehaviorInterruptTasks[id]
}

void InitProfileEntry( system_profile_entry_t * entry )
{
}
