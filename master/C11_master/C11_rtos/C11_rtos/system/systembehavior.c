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
void PerformBehaviorProbeSend( system_probe_id_t id )
{
}
void PerformBehaviorProbeReceive( system_probe_id_t id )
{
}
void PerformBehaviorSchedulerSchedule( system_scheduler_id_t id, uint32_t data )
{
}

void PerformBehaviorSchedulerDeschedule( system_scheduler_id_t id )
{
    //Deschedule BehaviorScheduledTasks[id]
}
void PerformBehaviorInterrupterSend( system_interrupter_id_t id )
{
    //Enable BehaviorInterruptTasks[id]
}
void PerformBehaviorInterrupterReceive( system_interrupter_id_t id )
{
    //Handle BehaviorInterruptTasks[id]
}
void PerformBehaviorInterrupterPerform( system_interrupter_id_t id )
{
    //Disable BehaviorInterruptTasks[id]
}

void InitProfileEntry( system_profile_entry_t * entry )
{
    if( entry->header.state == SYSTEM_PROFILE_ENTRY_STATE_ENABLED)
    {
        switch( entry->header.type )
        {
            case SYSTEM_PROFILE_ENTRY_TYPE_INTERRUPT:
                switch( entry->header.direction )
            {
                case SYSTEM_PROFILE_ENTRY_DIRECTION_INPUT:
                    break;
                case SYSTEM_PROFILE_ENTRY_DIRECTION_OUTPUT:
                    break;
                default:
                    break;
            }
                break;
            case SYSTEM_PROFILE_ENTRY_TYPE_SCHEDULED:
                switch( entry->header.direction )
            {
                case SYSTEM_PROFILE_ENTRY_DIRECTION_INPUT:
                    break;
                case SYSTEM_PROFILE_ENTRY_DIRECTION_OUTPUT:
                    break;
                default:
                    break;
            }
                break;
            default:
                break;
        }
    }
}
