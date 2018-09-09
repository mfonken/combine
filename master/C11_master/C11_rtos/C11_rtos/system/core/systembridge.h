//
//  systembridge.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef systembridge_h
#define systembridge_h

#include "C11_profile.h"
#include "systemtasks.h"
#include "systemhandlers.h"

static void InitializeMeta(void)
{
    SystemFunctions.Registers.SubactivityMap(
    &(system_subactivity_map_t)
    {
        //         { SYSTEM_SUBACTIVITY_SELF_CHECK, .function.blank = (void(*)(void))BehaviorFunctions.Perform.SelfCheck, NO_DATA },

        { SYSTEM_SUBACTIVITY_HANDLE_MOTION_EVENT, .function.blank = (void(*)(void))HandlerFunctions.Input.Motion, NO_DATA },
        { SYSTEM_SUBACTIVITY_HANDLE_RHO_EVENT, .function.blank = (void(*)(void))HandlerFunctions.Input.Rho, NO_DATA },

        { SYSTEM_SUBACTIVITY_TAU_STANDARD_RHO_START, .function.pointer = (void(*)(void *))RhoFunctions.Send, .data.pointer = &System.objects.Rho.settings },
        { SYSTEM_SUBACTIVITY_TAU_STANDARD_MOTION_START, .function.pointer = (void(*)(void *))IMUFunctions.RotVec, .data.pointer = &System.objects.IMU },
        { SYSTEM_SUBACTIVITY_TAU_STANDARD_START, .function.byte = (void(*)(uint8_t))TauFunctions.Start, .data.byte = TAU_STATE_STANDARD },

        { SYSTEM_SUBACTIVITY_TAU_STANDARD_RHO_STOP, .function.pointer = (void(*)(void *))RhoFunctions.Send, .data.pointer = &System.objects.Rho.settings },
        { SYSTEM_SUBACTIVITY_TAU_STANDARD_MOTION_STOP, .function.pointer = (void(*)(void *))IMUFunctions.RotVec, .data.pointer = &System.objects.IMU },
        { SYSTEM_SUBACTIVITY_TAU_STOP, .function.blank = (void(*)(void))TauFunctions.Stop, NO_DATA },

        //             { SYSTEM_SUBACTIVITY_BATTERY_MONITOR_SLEEP, .function.byte = (void(*)(uint8_t))BatteryMonitor.Set, BATTERY_MONITOR_MODE_SLEEP },

        { SYSTEM_SUBACTIVITY_RECEIVE_HOST_PACKET, .function.blank = (void(*)(void))CommFunctions.Perform.Receive, NO_DATA },


        //             SYSTEM_SUBACTIVITY_SELF_CHECK,
        //             SYSTEM_SUBACTIVITY_WAIT_FOR_WAKE,

        /* Initialization */
        { SYSTEM_SUBACTIVITY_INIT_COMMUNICATION, .function.blank = (void(*)(void))CommFunctions.Init, .data.byte = NO_DATA },
        { SYSTEM_SUBACTIVITY_INIT_COMPONENTS, .function.pointer = (void(*)(void *))SysIOCtlFunctions.Init, .data.pointer = &System },
        { SYSTEM_SUBACTIVITY_INIT_TAU_CLIENT, .function.blank = (void(*)(void))TauFunctions.Perform.Init, .data.byte = NO_DATA },
        { SYSTEM_SUBACTIVITY_INIT_RHO_CLIENT, .function.pointer = (void(*)(void *))RhoFunctions.Init, .data.pointer = &System.objects.Rho.settings },
        //         { SYSTEM_SUBACTIVITY_INIT_CONFIRM, .function.blank   = (void(*)(void))BehaviorFunctions.Perform.ConfirmInit, .data.byte = NO_DATA },

        { SYSTEM_SUBACTIVITY_BATTERY_MONITOR_ACTIVE, .function.byte = (void(*)(uint8_t))BatteryMonitor.Set, .data.byte = System.buffers.battery_monitor_mode },

        /* Profile */
        //    SYSTEM_SUBACTIVITY_PROFILE_FETCH,
        //    SYSTEM_SUBACTIVITY_PROFILE_PERFORM,
        //    SYSTEM_SUBACTIVITY_PROFILE_UPDATE,
        //    SYSTEM_SUBACTIVITY_PROFILE_STORE,

        /* Probes */
        //             { SYSTEM_SUBACTIVITY_SEND_HOST_PROBE,  },
        //             { SYSTEM_SUBACTIVITY_RECEIVE_HOST_PROBE, .function.blank = (void(*)(void))CommFunctions.Receive, NO_DATA },

        /* Handles */
        { SYSTEM_SUBACTIVITY_HANDLE_MOTION_EVENT, .function.blank = (void(*)(void))HandlerFunctions.Input.Motion, NO_DATA },
        { SYSTEM_SUBACTIVITY_HANDLE_RHO_EVENT, .function.blank = (void(*)(void))HandlerFunctions.Input.Rho, NO_DATA },

        { SYSTEM_SUBACTIVITY_POLL_BATTERY_MONITOR, .function.pointer = (void(*)(void *))BatteryMonitor.GetBasic, .data.pointer = &System.buffers.battery },
        //             { SYSTEM_SUBACTIVITY_POLL_TIP, .function.pointer = (void(*)(void *))BatteryMonitor.GetBasic, .data.pointer = &System.buffers.tip_data },

        { SYSTEM_SUBACTIVITY_TRIGGER_HAPTIC, .function.byte = (void(*)(uint8_t))HapticFunctions.Trigger, .data.byte = System.buffers.haptic },

        { SYSTEM_SUBACTIVITY_TRANSMIT_HOST_PACKET, .function.pointer = (void(*)(void *))CommFunctions.Perform.Transmit, .data.pointer = &System.buffers.packet_out },
        { SYSTEM_SUBACTIVITY_RECEIVE_HOST_PACKET, .function.pointer = (void(*)(void *))CommFunctions.Perform.Receive, .data.pointer = &System.buffers.packet_in },
        { SYSTEM_SUBACTIVITY_TRANSMIT_SUB_RADIO_PACKET, .function.pointer = (void(*)(void *))CommFunctions.Perform.Transmit, .data.pointer = &System.buffers.sub_packet_out },
        { SYSTEM_SUBACTIVITY_RECEIVE_SUB_RADIO_PACKET, .function.pointer = (void(*)(void *))CommFunctions.Perform.Receive, .data.pointer = &System.buffers.sub_packet_in },

        /* Tau */
        //             { SYSTEM_SUBACTIVITY_TAU_STANDARD_RHO_START, .function.byte = (void(*)(uint8_t))RhoFunctions.Send, .data.byte = RHO_START },
        { SYSTEM_SUBACTIVITY_TAU_STANDARD_RHO_STOP, .function.pointer = (void(*)(void *))IMUFunctions.RotVec, .data.pointer = &System.objects.IMU },
        { SYSTEM_SUBACTIVITY_TAU_STANDARD_START, .function.byte = (void(*)(uint8_t))TauFunctions.Start, .data.byte = TAU_STATE_START },

        /* Sleep/Idle */
        //             { SYSTEM_SUBACTIVITY_TAU_STANDARD_RHO_STOP, .function.byte = (void(*)(uint8_t))RhoFunctions.Send, .data.byte = RHO_STOP },
        { SYSTEM_SUBACTIVITY_TAU_STANDARD_MOTION_STOP, .function.byte = (void(*)(uint8_t))IMUFunctions.RotVec, .data.pointer = &System.objects.IMU },
        { SYSTEM_SUBACTIVITY_TAU_STOP, .function.byte = (void(*)(uint8_t))TauFunctions.Stop, .data.byte = TAU_STATE_IDLE }
    });
    SystemFunctions.Registers.OSTaskList(
    &(os_task_list_t)
    {
        SCHEDULER_TAU_PERFORM_OS_TASK(TauFunctions.Tick),
//        SCHEDULER_TAU_PACKET_QUEUE_OS_TASK(nullptr),
//        SCHEDULER_TIP_POLL_OS_TASK(nullptr),
        SCHEDULER_BATTERY_MONITOR_POLL_OS_TASK(BatteryMonitor.GetBasic),
        INTERRUPTER_TAU_PACKET_TRANSMIT_OS_TASK(CommFunctions.Perform.Transmit),
        INTERRUPTER_TAU_PACKET_RECEIVE_OS_TASK(CommFunctions.Perform.Receive),
        INTERRUPTER_SUB_RADIO_PACKET_TRANSMIT_OS_TASK(CommFunctions.Perform.Transmit),
        INTERRUPTER_HAPTIC_TRIGGER_OS_TASK(HapticFunctions.Trigger),
        SCHEDULER_MOTION_INTERRUPT_OS_TASK(IMUFunctions.Read),
        SCHEDULER_RHO_INTERRUPT_OS_TASK(RhoFunctions.Receive),
        SCHEDULER_TOUCH_INTERRUPT_OS_TASK(CommFunctions.Perform.Transmit)
    });
    SystemFunctions.Init( &Profile );
}

void InitializeHardware(void)
{
}

void InitializeSystem(void)
{
    //    RTOS_ERR  err;
    //
    //    CMU_ClockEnable(cmuClock_PRS, true);
    //
    //    BSP_CPUInit();                                                /* Initialize CPU and make all interrupts Kernel Aware. */
    //    //system already initialized by enter_DefaultMode_from_RESET
    //    //BSP_SystemInit();                                           /* Initialize System.                                   */
    //
    //    // MTM: Not needed anymore
    //    //OS_ConfigureTickTask(&tickTaskCfg);
    //
    //    OSInit(&err);                                                 /* Initialize the Kernel.                               */
    //    /*   Check error code.                                  */
    //    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
    //

    InterfaceFunctions.OS.Create( &(os_task_data_t)COMBINE_API_OS_TASK(NULL) );
    //
    //    /*   Check error code.                                  */
    //    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
    //
    //    OSStart(&err);                                                /* Start the kernel.                                    */
    //    /*   Check error code.                                  */
    //    APP_RTOS_ASSERT_DBG((RTOS_ERR_CODE_GET(err) == RTOS_ERR_NONE), 1);
    //
    //    return (1);
}

#endif /* systembridge_h */



