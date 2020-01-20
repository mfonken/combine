//
//  os_interface.h
//  C12_rtos
//
//  Created by Matthew Fonken on 9/2/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef os_interface_h
#define os_interface_h

#include "profilemanager.h"
#include "sysiocontroller.h"

void OSInterface_Init(void);
void OSInterface_Start(void);

void OSInterface_CreateTask( os_task_data_t * );
void OSInterface_ResumeTask( os_task_data_t * );
void OSInterface_SuspendTask( os_task_data_t * );
void OSInterface_DeleteTask( os_task_data_t * );

void OSInterface_CreateQueue( os_queue_data_t * );
qty_t OSInterface_DeleteQueue( os_queue_data_t * );
qty_t OSInterface_FlushQueue( os_queue_data_t * );
void OSInterface_PendQueue( os_queue_data_t * );
qty_t OSInterface_PendAbortQueue( os_queue_data_t * );
void OSInterface_PostQueue( os_queue_data_t * );

void OSInterface_CreateTimer( os_timer_data_t * );
bool OSInterface_DeleteTimer( os_timer_data_t * );
tick_t OSInterface_GetRemainTimer( os_timer_data_t * );
void OSInterface_SetTimer( os_timer_data_t * );
bool OSInterface_StartTimer( os_timer_data_t * );
os_state_t OSInterface_StartGetTimer( os_timer_data_t * );
bool OSInterface_StopTimer( os_timer_data_t * );


typedef struct
{
    void (*Create)( os_task_data_t * );
    void (*Resume)( os_task_data_t * );
    void (*Suspend)( os_task_data_t * );
    void (*Delete)( os_task_data_t * );
} system_os_interface_task_functions;

typedef struct
{
    void (*Create)( os_queue_data_t * );
    qty_t (*Delete)( os_queue_data_t * );
    qty_t (*Flush)( os_queue_data_t * );
    void (*Pend)( os_queue_data_t * );
    qty_t (*PendAbort)( os_queue_data_t * );
    void (*Post)( os_queue_data_t * );
} system_os_interface_queue_functions;

typedef struct
{
    void (*Create)( os_timer_data_t * );
    bool (*Delete)( os_timer_data_t * );
    tick_t (*GetRemain)( os_timer_data_t * );
    void (*Set)( os_timer_data_t * );
    bool (*Start)( os_timer_data_t * );
    os_state_t (*StartGet)( os_timer_data_t * );
    bool (*Stop)( os_timer_data_t * );
} system_os_interface_timer_functions;

typedef struct
{
    void (*Init)(void);
    void (*Start)(void);
    system_os_interface_task_functions Task;
    system_os_interface_queue_functions Queue;
    system_os_interface_timer_functions Timer;
} system_os_interface_functions;

static system_os_interface_functions OSFunctions =
{
    .Init = OSInterface_Init,
    .Start = OSInterface_Start,
    
    .Task.Create = OSInterface_CreateTask,
    .Task.Resume = OSInterface_ResumeTask,
    .Task.Suspend = OSInterface_SuspendTask,
    .Task.Delete = OSInterface_DeleteTask,
    
    .Queue.Create = OSInterface_CreateQueue,
    .Queue.Delete = OSInterface_DeleteQueue,
    .Queue.Flush = OSInterface_FlushQueue,
    .Queue.Pend = OSInterface_PendQueue,
    .Queue.PendAbort = OSInterface_PendAbortQueue,
    .Queue.Post = OSInterface_PostQueue,
    
    .Timer.Create = OSInterface_CreateTimer,
    .Timer.Delete = OSInterface_DeleteTimer,
    .Timer.GetRemain = OSInterface_GetRemainTimer,
    .Timer.Set = OSInterface_SetTimer,
    .Timer.Start = OSInterface_StartTimer,
    .Timer.StartGet = OSInterface_StartGetTimer,
    .Timer.Stop = OSInterface_StopTimer
    
};


#endif /* os_interface_h */
