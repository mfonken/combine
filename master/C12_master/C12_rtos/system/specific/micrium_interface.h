//
//  micrium_interface.h
//  C12_rtos
//
//  Created by Matthew Fonken on 1/18/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#ifndef micrium_interface_h
#define micrium_interface_h

#include "globaltypes.h"
#include <unistd.h>

/// SPOOF START
#ifndef __MICRIUM__
typedef void            (*OS_TASK_PTR)(void *p_arg);
typedef void            * OS_TCB, * OS_Q, * OS_TMR, * OS_TMR_CALLBACK_PTR;
typedef const char      CPU_CHAR;
typedef unsigned char   OS_PRIO, OS_ERR, RTOS_ERR;
typedef unsigned int    CPU_STK, CPU_STK_SIZE, OS_TICK, CPU_TS;
typedef unsigned short  OS_MSG_QTY, OS_OPT, OS_MSG_SIZE;


#ifndef OSTaskDel
static void OSTaskDel (OS_TCB *p_tcb, RTOS_ERR *p_err) {}
#endif
#define COMPLETE_TASK OSTaskDel((OS_TCB *)0, &System.error.runtime);

#define OS_CFG_TICK_RATE_HZ 10000
#define DEFAULT_STACK_SIZE (100 / sizeof(CPU_STK))

#endif /* __MICRIUM__ */
/// SPOOF END

#define HZ_TO_TICK(X)   ( (OS_TICK)( (double)X * (double)OS_CFG_TICK_RATE_HZ ) )             /// <--- Double Check
#define TICK_TO_HZ(X)   ( (double)X / (double)OS_CFG_TICK_RATE_HZ )                          /// <--- Double Check
#define MS_TO_TICK(X)   ( (OS_TICK)( ( (double)X / 1000. ) * (double)OS_CFG_TICK_RATE_HZ ) ) /// <--- Double Check

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
    union
    {
        struct basic_timer
        {
            OS_TICK dly;
            OS_TICK period;
            OS_OPT opt;
            OS_TMR tmr;
        } schedule;
        struct basic_interrupt
        {
            INTERRUPT_ACTION action;
            component_id_t component_id;
        } interrupt;
    } event_data;
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
} micrium_os_timer_data_t,  MICRIUM_OS_TIMER_DATA_T;

#define TASK_ADV( ID_, PTR_, ARGS_, PRIORITY_, SIZE_, Q_SIZE_, OS_TICK_, EXTENSION_, OS_OPTIONS_, ERROR_/*, TIMER_DELAY_MS_, TIMER_HZ_, TIMER_OPT_*/ ) \
{ \
    (generic_id_t)ID_, \
    (CPU_CHAR*)task_id_strings[ID_], \
    (OS_TASK_PTR)PTR_, \
    (void*)ARGS_, \
    (OS_PRIO)PRIORITY_, \
    (CPU_STK_SIZE)SIZE_ / DEFAULT_TASK_STACK_LIMIT_FACTOR, \
    (CPU_STK_SIZE)SIZE_, \
    (OS_MSG_QTY)Q_SIZE_, \
    (OS_TICK)OS_TICK_, \
    (void*)EXTENSION_, \
    (OS_OPT)OS_OPTIONS_, \
    (OS_ERR*)ERROR_/*, \
    { MS_TO_TICK(TIMER_DELAY_MS_), HZ_TO_TICK(TIMER_HZ_), TIMER_OPT_ }*/ \
}

#define TASK( ID_, PTR_, ARGS_, PRIORITY_, ERROR_ ) \
TASK_ADV( ID_, PTR_, ARGS_, PRIORITY_, DEFAULT_STACK_SIZE, 0u, 0u, 0u, DEFAULT_TASK_OS_OPTIONS, ERROR_/*, 0u, 0u, 0u*/ )

#define QUEUE_ADV( ID_, MAX_QTY_, TIMEOUT_MS_, ERROR_, OPT_ ) \
{ \
    (generic_id_t)ID_, \
    (CPU_CHAR*)queue_id_strings[ID_], \
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

//#define SCHEDULED_TASK( ID_, PTR_, ARGS_, PRIORITY_, ERROR_, TIMER_DELAY_, TIMER_HZ_, TIMER_OPT_ ) \
//TASK_ADV( ID_, PTR_, ARGS_, PRIORITY_, DEFAULT_STACK_SIZE, 0u, 0u, 0u, DEFAULT_TASK_OS_OPTIONS, ERROR_, TIMER_DELAY_MS_, TIMER_HZ_, TIMER_OPT_ )

#define TIMER_FROM_SCHEDULED_TASK(p_task_data) \
{ \
    p_task_data->ID, \
    p_task_data->event_data.schedule.tmr, \
    p_task_data->p_name, \
    p_task_data->event_data.schedule.dly, \
    p_task_data->event_data.schedule.period, \
    p_task_data->event_data.schedule.opt, \
    p_task_data->p_task, \
    p_task_data->p_arg, \
    p_task_data->p_err \
}

#ifndef __MICRIUM__

static void MICRIUM_OSInterface_Init( void ) {}
static void MICRIUM_OSInterface_Start( void ) {}
static void MICRIUM_OSInterface_DelayMs( uint32_t ms ) { usleep(ms * 1000); }
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
    RTOS_ERR  err;

    CMU_ClockEnable(cmuClock_PRS, true);

    BSP_CPUInit();                                                / * Initialize CPU and make all interrupts Kernel Aware. * /
    //system already initialized by enter_DefaultMode_from_RESET
    //BSP_SystemInit();                                           / * Initialize System.                                   * /

    // MTM: Not needed anymore
    //OS_ConfigureTickTask(&tickTaskCfg);

    OSInit(&err);                                                 /  * Initialize the Kernel.                               * /
    /  *   Check error code.                                  * /
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
    
    / *   Check error code.                                  * /
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}

static inline void MICRIUM_OSInterface_Start( void )
{
    RTOS_ERR  err;
    
    OSStart(&err);        / * Start the kernel.                                    * /
    
    / *   Check error code.                                  * /
    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
}
 
 static inline void MICRIUM_OSInterface_Delay( uint32_t ms )
 {
     RTOS_ERR  err;
     
     void  OSTimeDly (MS_TO_TICK(ms), OS_OPT_TIME_DLY, &err);
 }

static inline void MICRIUM_OSInterface_CreateTask( micrium_os_task_data_t * task_data )
{
    OSTaskCreate(task_data->p_tcb,
                 task_data->p_name,
                 task_data->p_task,
                 task_data->p_arg,
                 task_data->prio,
                 task_data->p_stk_base,
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
    OSTaskResume(task_data->p_tcb,
                 task_data->p_err);
}

static inline void MICRIUM_OSInterface_SuspendTask( micrium_os_task_data_t * task_data )
{
    OSTaskSuspend(task_data->p_tcb,
                  task_data->p_err);
}

static inline void MICRIUM_OSInterface_DeleteTask( micrium_os_task_data_t * task_data )
{
    OSTaskDel(task_data->p_tcb,
              task_data->p_err);
}


/ * OS Queue * /
static inline void MICRIUM_OSInterface_CreateQueue( micrium_os_queue_data_t * queue_data )
{
    OSQCreate(queue_data->p_q,
              queue_data->p_name,
              queue_data->max_qty,
              queue_data->p_err);
}

static inline void MICRIUM_OSInterface_DeleteQueue( micrium_os_queue_data_t * queue_data )
{
    OSQDel(queue_data->p_q,
           queue_data->opt,
           queue_data->p_err);
}

static inline void MICRIUM_OSInterface_FlushQueue( micrium_os_queue_data_t * queue_data )
{
    OSQFlush(queue_data->p_q,
             queue_data->p_err);
}

static inline void MICRIUM_OSInterface_PendQueue( micrium_os_queue_data_t * queue_data )
{
    OSQPend(queue_data->p_q,
            queue_data->timeout,
            queue_data->opt,
            queue_data->p_msg_size,
            queue_data->p_ts,
            queue_data->p_err);
}

static inline void MICRIUM_OSInterface_PendAbortQueue( micrium_os_queue_data_t * queue_data )
{
    OSQPendAbort(queue_data->p_q,
                 queue_data->opt,
                 queue_data->p_err);
}
static inline void MICRIUM_OSInterface_PostQueue( micrium_os_queue_data_t * queue_data )
{
    OSQPost(queue_data->p_q,
            queue_data->p_void,
            *queue_data->p_msg_size,
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

static inline CPU_STATE MICRIUM_OSInterface_StartGetTimer( micrium_os_timer_data_t * timer_data )
{
   return OSTmrStartGet(timer_data->p_tmr,
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

