//
//  application.h
//  C12_rtos
//
//  Created by Matthew Fonken on 1/18/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#ifndef application_h
#define application_h

#include "application.h"
#include "systembehavior.h"
#include "C12_profile.h"

system_subactivity_map_t global_subactivity_map = { 0 };
os_task_list_t global_task_list = { 0 };
os_queue_list_t global_queue_list = { 0 };

void InitializeMeta(void);

static void InitApplication( void )
{
    InitializeMeta();
}

static void StartApplication( void )
{
    SystemFunctions.Perform.ExitState();
    OSFunctions.DelayMs(1000);
    SystemFunctions.Register.State( SYSTEM_STATE_ACTIVE );
    ComponentInterrupt( BNO080_PORT, BNO080_PIN, HW_EDGE_FALLING );
}

static void TickApplication( void )
{
COMPLETE_TASK
}


/* [Meta] Global buffers */

/* Rho Out */
static void RhoOutputHandler( void )
{
    RhoFunctions.Send( &System.objects.Rho.settings );
}

/* Rho In */
static void RhoInputHandler( void )
{
    RhoFunctions.Receive( &System.objects.Rho );
    RhoPointToKPoint( &System.objects.Rho.packet.primary, &System.buffers.rho.data[0] );
    RhoPointToKPoint( &System.objects.Rho.packet.secondary, &System.buffers.rho.data[1] );
//    System.buffers.rho.confidence = System.objects.Rho.packet.probabilites.;
}

/* Motion Out */
static void MotionOutputHandler( imu_feature_t feature, uint32_t interval )
{
    IMUFunctions.Start( &System.objects.IMU.client, feature, interval, System.objects.IMU.chip );
}
/* Motion In */
static void MotionInputHandler( void )
{
    shtp_client_t * client = &System.objects.IMU.client;
    
    if( IMUFunctions.Refresh( client ) )
    {
        switch( client->output.type )
        {
            case SH2_SENSOR_REPORT_ROTATION_VECTOR:
            case SH2_SENSOR_REPORT_GRAVITY:
            case SH2_SENSOR_REPORT_GAME_ROTATION_VECTOR:
            case SH2_SENSOR_REPORT_GEOMAGNETIC_ROTATION_VECTOR:
            case SH2_SENSOR_REPORT_STABILIZED_ROTATION_VECTOR:
            case SH2_SENSOR_REPORT_STABILIZED_GAME_ROTATION_VECTOR:
            case SH2_SENSOR_REPORT_GYRO_INTEGRATED_ROTATION_VECTOR:
                RotVecToQuaternion( &client->output.rotation_vector, &System.buffers.orientation.data );
                System.buffers.orientation.timestamp = client->output.rotation_vector.timestamp;
                break;
            default:
                break;
        }
    }
}

/* BLE Out */
static void HostOutputHandler( void )
{
    
}
/* BLE In */
static void HostInputHandler( void )
{
    // State change?
//    CommFunctions.Perform.Receive();
}

/* Sub Out */
static void SubRadioOutputHandler( void )
{
    
}

/* Touch In */
static void TouchInterruptHandler( component_id_t ID )
{
    touch_data_t * ptr = &System.buffers.touch;
//    component_t * component = SysIOCtlFunctions.Get( ID );
    touch_packet_t packet = TouchController.Read();
    switch(packet.type)
    {
        case TOUCH_EVENT:
            ptr->buttons |= ( 1 << packet.index );
            break;
        case SLIDER_EVENT:
            ptr->slider = packet.slider;
            break;
        case RELEASE_EVENT:
            ptr->buttons &= ~( 1 << packet.index );
            ptr->prev_slider = ptr->slider;
            ptr->slider = TOUCH_SLIDER_NULL;
            break;
        default:
            break;
    }
}


//#define MAX_HANDLER_QUEUE 10
//
//void ComponentInterruptQueuer( component_id_t );
//void ComponentInterruptHandler( component_id_t );
//void RhoOutputHandler( void );
//void RhoInputHandler( void );
//void MotionOutputHandler( imu_feature_t, uint32_t );
//void MotionInputHandler( void );
//void HostOutputHandler( void );
//void HostInputHandler( void );
//void SubRadioOutputHandler( void );
//void TouchInterruptHandler( component_id_t );
//
typedef struct
{
    void (*Rho)(void);
    void (*Motion)(void);
    void (*Host)(void);
    void (*Touch)( component_id_t );
} application_handler_input_functions;

typedef struct
{
    void (*Rho)(void);
    void (*Motion)( imu_feature_t, uint32_t );
    void (*Host)(void);
    void (*SubRadio)(void);
} application_handler_output_functions;

//typedef struct
//{
//    void (*Queue)(void);
//    void (*Handle)(component_id_t);
//} handler_interrupt_functions;

typedef struct
{
//    handler_interrupt_functions Interrupt;
    application_handler_input_functions Input;
    application_handler_output_functions Output;
} application_handler_functions;

typedef struct
{
    void (*Init)(void);
    void (*Start)(void);
    void (*Tick)(void);
    application_handler_functions Handler;
} application_functions;

static application_functions App =
{
    .Init = InitApplication,
    .Start = StartApplication,
    .Tick = TickApplication,
//    .Interrupt.Queue    = ComponentInterruptQueuer,
//    .Interrupt.Handle   = ComponentInterruptHandler,
    .Handler.Input.Rho          = RhoInputHandler,
    .Handler.Output.Rho         = RhoOutputHandler,
    .Handler.Input.Motion       = MotionInputHandler,
    .Handler.Output.Motion      = MotionOutputHandler,
    .Handler.Input.Host         = HostInputHandler,
    .Handler.Output.Host        = HostOutputHandler,
    .Handler.Input.Touch        = TouchInterruptHandler,
    .Handler.Output.SubRadio    = SubRadioOutputHandler,
};

void InitializeMeta(void)
{
    system_subactivity_map_t subactivity_map_initializer = (system_subactivity_map_t)
    {
        SUBACTIVITY( APPLICATION_SUBACTIVITY_SELF_CHECK,                  NULL,                               NO_DATA ),

        ////SUBACTIVITY( APPLICATION_SUBACTIVITY_HANDLE_MOTION_EVENT,         HandlerFunctions.Input.Motion,      NO_DATA ),
        ////SUBACTIVITY( APPLICATION_SUBACTIVITY_HANDLE_RHO_EVENT,            HandlerFunctions.Input.Rho,         NO_DATA ),

        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STANDARD_RHO_START,      RhoFunctions.Send,                  &System.objects.Rho.settings ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STANDARD_MOTION_START,   IMUFunctions.RotVec,                &System.objects.IMU ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STANDARD_START,          TauFunctions.Start,                 TAU_STATE_STANDARD ),

        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STANDARD_RHO_STOP,       RhoFunctions.Send,                  &System.objects.Rho.settings ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STANDARD_MOTION_STOP,    IMUFunctions.RotVec,                &System.objects.IMU ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STOP,                    TauFunctions.Stop,                  NO_DATA ),

        //             SUBACTIVITY( APPLICATION_SUBACTIVITY_BATTERY_MONITOR_SLEEP, BatteryMonitor.Set, BATTERY_MONITOR_MODE_SLEEP ),

        SUBACTIVITY( APPLICATION_SUBACTIVITY_RECEIVE_HOST_PACKET,       CommFunctions.Perform.Receive,        NO_DATA ),


        //             APPLICATION_SUBACTIVITY_SELF_CHECK,
        //             APPLICATION_SUBACTIVITY_WAIT_FOR_WAKE,

        /* Initialization */
        SUBACTIVITY( APPLICATION_SUBACTIVITY_INIT_COMMUNICATION,        CommFunctions.Init,                   NO_DATA ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_INIT_COMPONENTS,           SysIOCtlFunctions.Init,               &System ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_INIT_TAU_CLIENT,           TauFunctions.Perform.Init,            NO_DATA ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_INIT_RHO_CLIENT,           RhoFunctions.Init,                    &System.objects.Rho.settings ),
        //         SUBACTIVITY( APPLICATION_SUBACTIVITY_INIT_CONFIRM, BehaviorFunctions.Perform.ConfirmInit, .data.byte = NO_DATA ),

        SUBACTIVITY( APPLICATION_SUBACTIVITY_BATTERY_MONITOR_ACTIVE,    BatteryMonitor.Set,                   System.buffers.config.battery_monitor_mode ),

        /* Profile */
        //    APPLICATION_SUBACTIVITY_PROFILE_FETCH,
        //    APPLICATION_SUBACTIVITY_PROFILE_PERFORM,
        //    APPLICATION_SUBACTIVITY_PROFILE_UPDATE,
        //    APPLICATION_SUBACTIVITY_PROFILE_STORE,

        /* Probes */
        //        SUBACTIVITY( APPLICATION_SUBACTIVITY_SEND_HOST_PROBE,  ),
        //        SUBACTIVITY( APPLICATION_SUBACTIVITY_RECEIVE_HOST_PROBE, .function.blank = (void(*)(void))CommFunctions.Receive, NO_DATA ),

        /* Handles */
        ////SUBACTIVITY( APPLICATION_SUBACTIVITY_HANDLE_MOTION_EVENT,       HandlerFunctions.Input.Motion,        NO_DATA ),
        ////SUBACTIVITY( APPLICATION_SUBACTIVITY_HANDLE_RHO_EVENT,          HandlerFunctions.Input.Rho,           NO_DATA ),

        SUBACTIVITY( APPLICATION_SUBACTIVITY_POLL_BATTERY_MONITOR,      BatteryMonitor.GetBasic,              &System.buffers.battery ),
        //             SUBACTIVITY( APPLICATION_SUBACTIVITY_POLL_TIP, BatteryMonitor.GetBasic, &System.buffers.tip_data ),

        SUBACTIVITY( APPLICATION_SUBACTIVITY_TRIGGER_HAPTIC,            HapticFunctions.Trigger,              System.buffers.config.haptic ),

        SUBACTIVITY( APPLICATION_SUBACTIVITY_TRANSMIT_HOST_PACKET,      CommFunctions.Perform.Transmit,       &System.buffers.packet_out ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_RECEIVE_HOST_PACKET,       CommFunctions.Perform.Receive,        &System.buffers.packet_in ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_TRANSMIT_SUB_RADIO_PACKET, CommFunctions.Perform.Transmit,       &System.buffers.sub_packet_out ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_RECEIVE_SUB_RADIO_PACKET,  CommFunctions.Perform.Receive,        &System.buffers.sub_packet_in ),

        /* Tau */
        //             SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STANDARD_RHO_START, RhoFunctions.Send, .data.byte = RHO_START ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STANDARD_RHO_STOP,     IMUFunctions.RotVec,                  &System.objects.IMU ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STANDARD_START,        TauFunctions.Start,                   TAU_STATE_START ),

        /* Sleep/Idle */
        //             SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STANDARD_RHO_STOP, RhoFunctions.Send, .data.byte = RHO_STOP ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STANDARD_MOTION_STOP,  IMUFunctions.RotVec,                  &System.objects.IMU ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STOP,                  TauFunctions.Stop,                    TAU_STATE_IDLE )
    };
    
    os_task_list_t task_list_initializer =
    (os_task_list_t)
    {/* Task name                                                  Function to call                Variable to fill                Priority                  Error */
        TASK(APPLICATION_COMBINE_GLOBAL,                           App.Tick,                       NULL,                           TASK_PRIORITY_CLASS_HIGH, &System.error.runtime    ),
        TASK(APPLICATION_SCHEDULER_ID_TAU_PERFORM,                 TauFunctions.Tick,              NULL,                           TASK_PRIORITY_CLASS_HIGH, &System.error.runtime    ),
        TASK(APPLICATION_SCHEDULER_ID_TAU_PACKET_QUEUE,            TauFunctions.Tick,              NULL,                           TASK_PRIORITY_CLASS_HIGH, &System.error.runtime    ),
        TASK(APPLICATION_SCHEDULER_ID_BATTERY_MONITOR_POLL,        BatteryMonitor.GetBasic,        &System.buffers.battery,        TASK_PRIORITY_CLASS_HIGH, &System.error.peripheral ),
        TASK(APPLICATION_INTERRUPTER_ID_TAU_PACKET_TRANSMIT,       CommFunctions.Perform.Transmit, &System.buffers.packet_out,     TASK_PRIORITY_CLASS_HIGH, &System.error.system     ),
        TASK(APPLICATION_INTERRUPTER_ID_TAU_PACKET_RECEIVE,        CommFunctions.Perform.Receive,  &System.buffers.packet_in,      TASK_PRIORITY_CLASS_HIGH, &System.error.system     ),
        TASK(APPLICATION_INTERRUPTER_ID_SUB_RADIO_PACKET_TRANSMIT, CommFunctions.Perform.Transmit, &System.buffers.sub_packet_out, TASK_PRIORITY_CLASS_HIGH, &System.error.system     ),
        TASK(APPLICATION_INTERRUPTER_ID_HAPTIC_TRIGGER,            HapticFunctions.Trigger,        &System.buffers.config.haptic,  TASK_PRIORITY_CLASS_HIGH, &System.error.peripheral ),
        TASK(APPLICATION_SCHEDULER_ID_MOTION_INTERRUPT,            IMUFunctions.Read,              &System.objects.IMU.client,     TASK_PRIORITY_CLASS_HIGH, &System.error.peripheral ),
        TASK(APPLICATION_SCHEDULER_ID_RHO_INTERRUPT,               RhoFunctions.Receive,           &System.objects.Rho,            TASK_PRIORITY_CLASS_HIGH, &System.error.peripheral ),
//        TASK(APPLICATION_SCHEDULER_ID_TOUCH_INTERRUPT,           ,                               ,                           TASK_PRIORITY_CLASS_HIGH, &System.error.peripheral ),
        TASK(APPLICATION_SCHEDULER_ID_TIP_POLL,                    App.Tick,                       NULL,                           TASK_PRIORITY_CLASS_HIGH,   &System.error.peripheral )
    };
    
    os_queue_list_t queue_list_initializer =
    (os_queue_list_t)
    {
        QUEUE( APPLICATION_QUEUE_ID_HW_INTERRUPTS,                  DEFAULT_QUEUE_TIMEOUT_MS, DEFAULT_QUEUE_MAX_QTY, &System.error.interrupt ),
        QUEUE( APPLICATION_SYSTEM_QUEUE_ID_COMM_MESSAGES,           DEFAULT_QUEUE_TIMEOUT_MS, DEFAULT_QUEUE_MAX_QTY, &System.error.messaging ),
        QUEUE( APPLICATION_SYSTEM_QUEUE_ID_RUNTIME_MESSAGES,        DEFAULT_QUEUE_TIMEOUT_MS, DEFAULT_QUEUE_MAX_QTY, &System.error.messaging ),
        QUEUE( APPLICATION_SYSTEM_QUEUE_ID_APPLICATION_MESSAGES,    DEFAULT_QUEUE_TIMEOUT_MS, DEFAULT_QUEUE_MAX_QTY, &System.error.messaging ),
    };
    
    memcpy(&global_subactivity_map, &subactivity_map_initializer, sizeof(system_subactivity_map_t));
    memcpy(&global_task_list, &task_list_initializer, sizeof(os_task_list_t));
    memcpy(&global_queue_list, &queue_list_initializer, sizeof(os_queue_list_t));
    
    SystemFunctions.Register.SubactivityMap(&global_subactivity_map);
    SystemFunctions.Register.TaskList(&global_task_list);
    SystemFunctions.Register.QueueList(&global_queue_list);
    
    SystemFunctions.Init( &GENERIC_PROFILE );
}

#endif /* application_h */
