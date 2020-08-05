//
//  micrium_interface.h
//  C12_rtos
//
//  Created by Matthew Fonken on 1/18/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#ifndef micrium_interface_h
#define micrium_interface_h

#include "global_types.h"

#ifdef __MICRIUM__
#include <os.h>
#ifdef __GECKO__
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"
#endif
#else
#include <unistd.h>
typedef void            (*OS_TASK_PTR)(void *p_arg);
typedef void            * OS_TCB, * OS_Q, * OS_TMR, * OS_TMR_CALLBACK_PTR;
typedef const char      CPU_CHAR;
typedef unsigned char   OS_PRIO, OS_ERR, RTOS_ERR;
typedef unsigned int    CPU_STK, CPU_STK_SIZE, OS_TICK, CPU_TS;
typedef unsigned short  OS_MSG_QTY, OS_OPT, OS_MSG_SIZE;

#endif /* __MICRIUM__ */

//#ifndef OSTaskDel
//void OSTaskDel(OS_TCB *p_tcb, OS_ERR *p_err) {}
//#endif

#define DEFAULT_TASK_STACK_LIMIT_FACTOR 10u

#ifdef OSTaskDel
#define COMPLETE_TASK OSTaskDel((OS_TCB *)0, &System.error.runtime);
#else
#define COMPLETE_TASK
#endif
#define DEFAULT_STACK_SIZE (100 / sizeof(CPU_STK))
#define DEFAULT_QUEUE_SIZE 1

/// SPOOF END

#define HZ_TO_TICK(X)   ( (OS_TICK)( (double)X * (double)OS_CFG_TICK_RATE_HZ ) )             /// <--- Double Check
#define TICK_TO_HZ(X)   ( (double)X / (double)OS_CFG_TICK_RATE_HZ )                          /// <--- Double Check
#define MS_TO_TICK(X)   ( (OS_TICK)( ( (double)X / 1000. ) * (double)OS_CFG_TICK_RATE_HZ ) ) /// <--- Double Check
#define TICK_TO_MS(X)   ( (CPU_TS)( ( (double)X * 1000. ) / (double)OS_CFG_TICK_RATE_HZ ) )/// <--- Double Check

typedef struct
{
    generic_id_t   ID;
    CPU_CHAR      *p_name; /* Name */
    OS_TASK_PTR    p_task; /* Pointer to task */
    void          *p_arg; /* Argument */
    OS_PRIO        prio; /* Priority */
    CPU_STK_SIZE   stk_limit; /* Stack limit */
    CPU_STK_SIZE   stk_size; /* Stack size */
    OS_MSG_QTY     q_size; /* Max messages that can be received through queue */
    OS_TICK        time_quanta; /* Clock ticks for time quanta during round robin */
    void          *p_ext; /* Task control block extension for extended data during context switch */
    OS_OPT         opt; /* Task specific options */
    OS_ERR        *p_err; /* Pointer to error receiver */
    OS_TICK        dly;
    OS_TICK        period;
    OS_TMR         tmr;
    OS_OPT         tmr_opt;
    OS_TCB         tcb; /* Operating System: Task Control Block */
    CPU_STK        stk_base[DEFAULT_STACK_SIZE]; /* Stack base address */
} micrium_os_task_data_t, MICRIUM_OS_TASK_DATA_T;

typedef struct
{
    generic_id_t   ID;
    CPU_CHAR      *p_name;
    OS_MSG_QTY     max_qty;
    OS_ERR        *p_err;
    OS_OPT         opt;
    OS_TICK        timeout;
    OS_MSG_SIZE    msg_size;
    CPU_TS        *p_ts;
    void          *p_void;
    OS_Q           q;
} micrium_os_queue_data_t, MICRIUM_OS_QUEUE_DATA_T;

typedef struct
{
    generic_id_t        ID;
    OS_TMR              *p_tmr;            /* Pointer to timer     */
    CPU_CHAR            *p_name;           /* Name of timer, ASCII */
    OS_TICK              dly;              /* Initial delay        */
    OS_TICK              period;           /* Repeat period        */
    OS_OPT               opt;              /* Options              */
    OS_TMR_CALLBACK_PTR  p_callback;       /* Fnct to call at 0    */
    void                *p_callback_arg;   /* Arg. to callback     */
    OS_ERR              *p_err;
    OS_TASK_PTR         *p_task;
} micrium_os_timer_data_t,  MICRIUM_OS_TIMER_DATA_T;

#define TASK_ADV( ID_, PTR_, ARGS_, PRIORITY_, SIZE_, Q_SIZE_, OS_TICK_, EXTENSION_, OS_OPTIONS_, ERROR_, COMPONENTS_, NUM_COMPONENTS_, PERIOD_HZ_, ACTION_ ) \
{ \
    (application_task_id_t)ID_, \
    (CPU_CHAR*)TASK_ID_STRINGS[ID_], \
    (OS_TASK_PTR)PTR_, \
    (void*)ARGS_, \
    (OS_PRIO)PRIORITY_, \
    (CPU_STK_SIZE)SIZE_ / DEFAULT_TASK_STACK_LIMIT_FACTOR, \
    (CPU_STK_SIZE)SIZE_, \
    (OS_MSG_QTY)Q_SIZE_, \
    (OS_TICK)OS_TICK_, \
    (void*)EXTENSION_, \
    (OS_OPT)OS_OPTIONS_, \
    (OS_ERR*)ERROR_, \
    COMPONENTS_, \
    NUM_COMPONENTS_, \
    HZ_TO_TICK(PERIOD_HZ_), \
    ACTION_, \
}

#define TASK( ID_, PTR_, ARGS_, PRIORITY_, ERROR_, COMPONENTS_, NUM_COMPONENTS_, PERIOD_HZ_, ACTION_ ) \
TASK_ADV( ID_, PTR_, ARGS_, PRIORITY_, DEFAULT_STACK_SIZE, 0u, 0u, 0u, DEFAULT_TASK_OS_OPTIONS, ERROR_, COMPONENTS_, NUM_COMPONENTS_, PERIOD_HZ_, ACTION_/*, 0u, 0u, 0u*/ )

#define QUEUE_ADV( ID_, MAX_QTY_, TIMEOUT_MS_, ERROR_, OPT_ ) \
{ \
    (generic_id_t)ID_, \
    (CPU_CHAR*)QUEUE_ID_STRINGS[ID_], \
    (OS_MSG_QTY)MAX_QTY_, \
    (OS_ERR*)ERROR_, \
    (OS_OPT)OPT_, \
    (OS_TICK)MS_TO_TICK(TIMEOUT_MS_), \
    0 \
}

#ifndef OS_OPT_PEND_BLOCKING
#define OS_OPT_PEND_BLOCKING 0
#endif
#define DEFAULT_OS_Q_OPT OS_OPT_PEND_BLOCKING

#define QUEUE( ID_, MAX_QTY_, TIMEOUT_MS_, ERROR_ ) \
    QUEUE_ADV( ID_, MAX_QTY_, TIMEOUT_MS_, ERROR_, DEFAULT_OS_Q_OPT )

#define TIMER_FROM_SCHEDULED_TASK(p_task_data) \
(os_timer_data_t){ \
    p_task_data->ID, \
    &p_task_data->tmr, \
    p_task_data->p_name, \
    0, \
    p_task_data->period, \
    p_task_data->opt, \
    TimerArgumentIntermediary, \
    p_task_data->p_arg, \
    p_task_data->p_err, \
    &p_task_data->p_task \
}

#ifndef __MICRIUM__
/* Stubs */
static void MICRIUM_OSInterface_Init( void ) {}
static void MICRIUM_OSInterface_Start( void ) {}
static void MICRIUM_OSInterface_DelayMs( uint32_t ms ) { usleep(ms * 1000); }
static CPU_TS MICRIUM_OSInterface_Timestamp( void ) { return 0; } //(uint32_t)TIMESTAMP(); }
static void MICRIUM_OSInterface_CreateTask( micrium_os_task_data_t * task_data ) {}
static void MICRIUM_OSInterface_ResumeTask( micrium_os_task_data_t * task_data ) {}
static void MICRIUM_OSInterface_SuspendTask( micrium_os_task_data_t * task_data ) {}
static void MICRIUM_OSInterface_DeleteTask( micrium_os_task_data_t * task_data ) {}
static void MICRIUM_OSInterface_CreateQueue( micrium_os_queue_data_t * queue_data ) {}
static qty_t MICRIUM_OSInterface_DeleteQueue( micrium_os_queue_data_t * queue_data ) { return 0; }
static qty_t MICRIUM_OSInterface_FlushQueue( micrium_os_queue_data_t * queue_data ) { return 0; }
static void MICRIUM_OSInterface_PendQueue( micrium_os_queue_data_t * queue_data ) {}
static qty_t MICRIUM_OSInterface_PendAbortQueue( micrium_os_queue_data_t * queue_data ) { return 0; }
static void MICRIUM_OSInterface_PostQueue( micrium_os_queue_data_t * queue_data ) {}
static void MICRIUM_OSInterface_CreateTimer( micrium_os_timer_data_t * timer_data ) {}
static bool MICRIUM_OSInterface_DeleteTimer( micrium_os_timer_data_t * timer_data ) { return 0; }
static tick_t MICRIUM_OSInterface_GetRemainTimer( micrium_os_timer_data_t * timer_data ) { return 0; }
static void MICRIUM_OSInterface_SetTimer( micrium_os_timer_data_t * timer_data ) {}
static bool MICRIUM_OSInterface_StartTimer( micrium_os_timer_data_t * timer_data ) { return 0; }
static os_state_t MICRIUM_OSInterface_StartGetTimer( micrium_os_timer_data_t * timer_data ) { return 0; }
static bool MICRIUM_OSInterface_StopTimer( micrium_os_timer_data_t * timer_data ) { return 0; }

#else

static inline void MICRIUM_OSInterface_Init( void )
{
    OS_ERR  err;
    
    /* Initialize the Kernel.                               */
    OSInit(&err);
    
    /*   Check error code.                                  */
    ASSERT(err == OS_ERR_NONE);
}

static inline void MICRIUM_OSInterface_Start( void )
{
	OS_ERR  err;
    
    /* Start the kernel.                                    */
    OSStart(&err);
    
    /* Check error code.                                    */
    ASSERT(err == OS_ERR_NONE);
}
 
static inline void MICRIUM_OSInterface_DelayMs( uint32_t ms )
{
	OS_ERR  err;

    OSTimeDly(MS_TO_TICK(ms), OS_OPT_TIME_DLY, &err);

    ASSERT(err == OS_ERR_NONE);
}

static inline CPU_TS MICRIUM_OSInterface_Timestamp( void )
{
	OS_ERR  err;

    OS_TICK ticks = OSTimeGet(&err);

    ASSERT(err == OS_ERR_NONE);
    
    return TICK_TO_MS(ticks);
}

static inline void MICRIUM_OSInterface_CreateTask( micrium_os_task_data_t * task_data )
{
    OSTaskCreate(&task_data->tcb,
                 task_data->p_name,
                 task_data->p_task,
                 task_data->p_arg,
                 task_data->prio,
                 task_data->stk_base,
                 task_data->stk_limit,
                 task_data->stk_size,
                 task_data->q_size,
                 task_data->time_quanta,
                 task_data->p_ext,
                 task_data->opt,
                 task_data->p_err);
    task_data->p_task(task_data->p_arg);
}

static inline void MICRIUM_OSInterface_ResumeTask( micrium_os_task_data_t * task_data )
{
    OSTaskResume(&task_data->tcb,
                 task_data->p_err);
}

static inline void MICRIUM_OSInterface_SuspendTask( micrium_os_task_data_t * task_data )
{
    OSTaskSuspend(&task_data->tcb,
                  task_data->p_err);
}

static inline void MICRIUM_OSInterface_DeleteTask( micrium_os_task_data_t * task_data )
{
    OSTaskDel(&task_data->tcb,
              task_data->p_err);
}


/* OS Queue */
static inline void MICRIUM_OSInterface_CreateQueue( micrium_os_queue_data_t * queue_data )
{
    OSQCreate(&queue_data->q,
              queue_data->p_name,
              queue_data->max_qty,
              queue_data->p_err);
}

static inline OS_OBJ_QTY MICRIUM_OSInterface_DeleteQueue( micrium_os_queue_data_t * queue_data )
{
    return OSQDel(&queue_data->q,
           queue_data->opt,
           queue_data->p_err);
}

static inline OS_MSG_QTY MICRIUM_OSInterface_FlushQueue( micrium_os_queue_data_t * queue_data )
{
    return OSQFlush(&queue_data->q,
             queue_data->p_err);
}

static inline void MICRIUM_OSInterface_PendQueue( micrium_os_queue_data_t * queue_data )
{
    OSQPend(&queue_data->q,
            queue_data->timeout,
            queue_data->opt,
            &queue_data->msg_size,
            queue_data->p_ts,
            queue_data->p_err);
}

static inline OS_OBJ_QTY MICRIUM_OSInterface_PendAbortQueue( micrium_os_queue_data_t * queue_data )
{
    return OSQPendAbort(&queue_data->q,
                 queue_data->opt,
                 queue_data->p_err);
}
static inline void MICRIUM_OSInterface_PostQueue( micrium_os_queue_data_t * queue_data )
{
    OSQPost(&queue_data->q,
            queue_data->p_void,
            queue_data->msg_size,
            queue_data->opt,
            queue_data->p_err);
}

static inline void MICRIUM_OSInterface_CreateTimer( micrium_os_timer_data_t * timer_data )
{
    OSTmrCreate(timer_data->p_tmr,
                timer_data->p_name,
                timer_data->dly,
                timer_data->period,
                timer_data->opt,
                timer_data->p_callback,
                timer_data->p_callback_arg,
                timer_data->p_err);
}

static inline bool MICRIUM_OSInterface_DeleteTimer( micrium_os_timer_data_t * timer_data )
{
    return OSTmrDel(timer_data->p_tmr,
                    timer_data->p_err);
}

static inline tick_t MICRIUM_OSInterface_GetRemainTimer( micrium_os_timer_data_t * timer_data )
{
    return OSTmrRemainGet(timer_data->p_tmr,
                          timer_data->p_err);
}

static inline void MICRIUM_OSInterface_SetTimer( micrium_os_timer_data_t * timer_data )
{
    OSTmrSet(timer_data->p_tmr,
             timer_data->dly,
             timer_data->period,
             timer_data->p_callback,
             timer_data->p_callback_arg,
             timer_data->p_err);
}

static inline bool MICRIUM_OSInterface_StartTimer( micrium_os_timer_data_t * timer_data )
{
    return OSTmrStart(timer_data->p_tmr,
                      timer_data->p_err);
}

static inline OS_STATE MICRIUM_OSInterface_StartGetTimer( micrium_os_timer_data_t * timer_data )
{
   return OSTmrStateGet(timer_data->p_tmr,
                        timer_data->p_err);
}

static inline bool MICRIUM_OSInterface_StopTimer( micrium_os_timer_data_t * timer_data )
{
    return OSTmrStop(timer_data->p_tmr,
                     timer_data->opt,
                     timer_data->p_callback_arg,
                     timer_data->p_err);
}
#endif /* __MICRIUM__ */

#endif /* micrium_interface_h */

