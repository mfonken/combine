//
//  application.h
//  C12_rtos
//
//  Created by Matthew Fonken on 1/18/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#ifndef application_h
#define application_h

#include "application_types.h"

#include "application_debug.h"
#include "taumanager.h"
//#include "communicationmanager.h"
#include "motionsensor.h"
#include "rho_client.h"
#include "batterymonitor.h"
#include "touchcontroller.h"
#include "hapticcontroller.h"

#include "C12_profile.h"
static system_profile_t GENERIC_PROFILE = PROFILE_TEMPLATE;

typedef struct
{
tau_config_t
    tau;
uint8_t
    battery_monitor_mode,
    haptic;
} application_config_t;

typedef struct
{
orientation_data_t
    orientation;
rho_data_t
    rho;
touch_data_t
    touch;
comm_packet_t
    packet_in,
    packet_out,
    sub_packet_in,
    sub_packet_out;
battery_monitor_basic_t
    battery;
application_config_t
    config;
} application_buffers_t;

typedef struct
{
imu_t
    IMU;
rho_t
    Rho;
tau_t
    Kinetic;
} application_objects_t;

typedef struct
{
    application_buffers_t buffers;
    application_objects_t objects;
} application_t;

static application_t App;

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
    OS.DelayMs(1000);
    SystemFunctions.Register.State( SYSTEM_STATE_ACTIVE );
    ComponentInterrupt( BNO080_PORT, BNO080_PIN, HW_EDGE_FALLING );
}

static void TickApplication( void )
{
    
COMPLETE_TASK }


/* [Meta] */


/* Rho In */
static void RhoInputHandler( comm_host_t * host )
{
    RhoFunctions.Receive( host, &App.objects.Rho );
    RhoPointToKPoint( &App.objects.Rho.packet.primary, &App.buffers.rho.data[0] );
    RhoPointToKPoint( &App.objects.Rho.packet.secondary, &App.buffers.rho.data[1] );
//    App.buffers.rho.confidence = App.objects.Rho.packet.probabilites.;
}

/* Motion Out */
static void MotionOutputHandler( imu_feature_t feature, uint32_t interval )
{
    IMUFunctions.Start( &App.objects.IMU.client, feature, interval, App.objects.IMU.chip );
}
/* Motion In */
static void MotionInputHandler( void )
{
    shtp_client_t * client = &App.objects.IMU.client;
    
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
                RotVecToQuaternion( &client->output.rotation_vector, &App.buffers.orientation.data );
                App.buffers.orientation.timestamp = client->output.rotation_vector.timestamp;
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
static void TouchInterruptHandler( comm_host_t * host )
{
    touch_data_t * ptr = &App.buffers.touch;
    touch_packet_t packet = TouchController.Read( host );
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
    void (*Rho)( comm_host_t * );
    void (*Motion)(void);
    void (*Host)(void);
    void (*Touch)( comm_host_t * );
} application_handler_input_functions;

typedef struct
{
    void (*Rho)( comm_host_t * );
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

static application_functions AppFunctions =
{
    .Init = InitApplication,
    .Start = StartApplication,
    .Tick = TickApplication,
//    .Interrupt.Queue    = ComponentInterruptQueuer,
//    .Interrupt.Handle   = ComponentInterruptHandler,
    .Handler.Input.Rho          = RhoInputHandler,
//    .Handler.Output.Rho         = RhoOutputHandler,
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
        SUBACTIVITY( APPLICATION_SUBACTIVITY_SELF_CHECK,                NULL,                                 NO_DATA ),

        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STANDARD_RHO_START,    RhoFunctions.Send,                    &App.objects.Rho.settings ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STANDARD_MOTION_START, IMUFunctions.RotVec,                  &App.objects.IMU ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STANDARD_START,        TauFunctions.Start,                   TAU_STATE_STANDARD ),

        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STANDARD_RHO_STOP,     RhoFunctions.Send,                    &App.objects.Rho.settings ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STANDARD_MOTION_STOP,  IMUFunctions.RotVec,                  &App.objects.IMU ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_TAU_STOP,                  TauFunctions.Stop,                    NO_DATA ),

        /* Initialization */
        SUBACTIVITY( APPLICATION_SUBACTIVITY_INIT_COMMUNICATION,        CommFunctions.Init,                   NO_DATA ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_INIT_SUB_VREG,             PAPI.Energy.InitDCDC,                 SUB_VREG_MV ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_INIT_COMPONENTS,           SysIOCtlFunctions.Init,               &System ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_INIT_TAU_CLIENT,           TauFunctions.Perform.Init,            NO_DATA ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_INIT_RHO_CLIENT,           RhoFunctions.Init,                    &App.objects.Rho.settings ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_INIT_CONFIRM,              BehaviorFunctions.Perform.ConfirmInit, NO_DATA ),
        
        SUBACTIVITY( APPLICATION_SUBACTIVITY_POLL_BATTERY_MONITOR,      BatteryMonitor.GetBasicBase,          &App.buffers.battery ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_ACTIVATE_BATTERY_MONITOR,  BatteryMonitor.Set,                   ACTIVE ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_DEACTIVATE_BATTERY_MONITOR, BatteryMonitor.Set,                  INACTIVE ),

        SUBACTIVITY( APPLICATION_SUBACTIVITY_TRANSMIT_HOST_PACKET,      CommFunctions.Perform.Transmit,       &App.buffers.packet_out ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_RECEIVE_HOST_PACKET,       CommFunctions.Perform.Receive,        &App.buffers.packet_in ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_TRANSMIT_SUB_RADIO_PACKET, CommFunctions.Perform.Transmit,       &App.buffers.sub_packet_out ),
        SUBACTIVITY( APPLICATION_SUBACTIVITY_RECEIVE_SUB_RADIO_PACKET,  CommFunctions.Perform.Receive,        &App.buffers.sub_packet_in ),
                
        //        SUBACTIVITY( APPLICATION_SUBACTIVITY_BATTERY_MONITOR_SLEEP, BatteryMonitor.Set, BATTERY_MONITOR_MODE_SLEEP ),
        //        SUBACTIVITY( APPLICATION_SUBACTIVITY_POLL_TIP, BatteryMonitor.GetBasic, &App.buffers.tip_data ),
        //        SUBACTIVITY( APPLICATION_SUBACTIVITY_TRIGGER_HAPTIC,            HapticFunctions.Trigger,              App.buffers.config.haptic, APPLICATION_COMPONENT_HAPTIC_PRIMARY ),
    };
    
    os_task_list_t task_list_initializer =
    (os_task_list_t)
    {/* Task name                                                  Function to call                Argument                     Priority                  Error */
        TASK(APPLICATION_COMBINE_GLOBAL,                           AppFunctions.Tick,              NULL,                        TASK_PRIORITY_CLASS_HIGH, &System.error.runtime    ),
        TASK(APPLICATION_SCHEDULER_ID_TAU_PERFORM,                 TauFunctions.Tick,              NULL,                        TASK_PRIORITY_CLASS_HIGH, &System.error.runtime    ),
        TASK(APPLICATION_SCHEDULER_ID_TAU_PACKET_QUEUE,            TauFunctions.Tick,              NULL,                        TASK_PRIORITY_CLASS_HIGH, &System.error.runtime    ),
        TASK(APPLICATION_SCHEDULER_ID_BATTERY_MONITOR_POLL,        BatteryMonitor.GetBasic,        &App.buffers.battery,        TASK_PRIORITY_CLASS_HIGH, &System.error.peripheral ),
        TASK(APPLICATION_INTERRUPTER_ID_TAU_PACKET_TRANSMIT,       CommFunctions.Perform.Transmit, &App.buffers.packet_out,     TASK_PRIORITY_CLASS_HIGH, &System.error.system     ),
        TASK(APPLICATION_INTERRUPTER_ID_TAU_PACKET_RECEIVE,        CommFunctions.Perform.Receive,  &App.buffers.packet_in,      TASK_PRIORITY_CLASS_HIGH, &System.error.system     ),
        TASK(APPLICATION_INTERRUPTER_ID_SUB_RADIO_PACKET_TRANSMIT, CommFunctions.Perform.Transmit, &App.buffers.sub_packet_out, TASK_PRIORITY_CLASS_HIGH, &System.error.system     ),
        TASK(APPLICATION_INTERRUPTER_ID_HAPTIC_TRIGGER,            HapticFunctions.Trigger,        &App.buffers.config.haptic,  TASK_PRIORITY_CLASS_HIGH, &System.error.peripheral ),
        TASK(APPLICATION_SCHEDULER_ID_MOTION_INTERRUPT,            IMUFunctions.Read,              &App.objects.IMU.client,     TASK_PRIORITY_CLASS_HIGH, &System.error.peripheral ),
        TASK(APPLICATION_SCHEDULER_ID_RHO_INTERRUPT,               RhoFunctions.Receive,           &App.objects.Rho,            TASK_PRIORITY_CLASS_HIGH, &System.error.peripheral ),
//        TASK(APPLICATION_SCHEDULER_ID_TOUCH_INTERRUPT,             ,                               ,                          TASK_PRIORITY_CLASS_HIGH, &System.error.peripheral ),
        TASK(APPLICATION_SCHEDULER_ID_TIP_POLL,                    AppFunctions.Tick,              NULL,                        TASK_PRIORITY_CLASS_HIGH, &System.error.peripheral )
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
