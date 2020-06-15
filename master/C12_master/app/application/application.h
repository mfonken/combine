//
//  application.h
//  C12_rtos
//
//  Created by Matthew Fonken on 1/18/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#ifndef application_h
#define application_h

#include "../application/application_types.h"
#include "../application/debug/application_debug.h"
#include "../application/tau/taumanager.h"
#include "motionsensor.h"
#include "rho_client.h"
#include "batterymonitor.h"
#include "touchcontroller.h"
#include "hapticcontroller.h"

#include "C12_profile.h"

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
static system_profile_t Profile;

void InitializeMeta(void);

static void Application_Init( void )
{
    InitializeMeta();
}

static void Application_Start( void )
{
    SystemFunctions.Perform.ExitState();
    OS.DelayMs(1000);
    SystemFunctions.Register.State( STATE_NAME_ACTIVE );
    SystemBehavior.Perform.ComponentInterrupt( BNO080_PORT, BNO080_PIN, HW_EDGE_FALLING );
}

static void Application_Tick( void )
{
    
COMPLETE_TASK }


/* [Meta] */
static void Application_InitComponent( component_t * component )
{
    LOG_IO_CTL(IO_CTL_DEBUG, "Initializing component: %s(0x%02x)\n", component->name, component->ID);
    generic_id_t ID = PROTOCOL_ID_NULL;
    switch( component->ID)
    {
        case COMPONENT_ID_MOTION_SENSOR:
            ID = SysIOCtlFunctions.GenerateID(PROTOCOL_ID_SH2);
            if( ID != PROTOCOL_ID_NULL )
            {
                IMUFunctions.Init( &App.objects.IMU, component->ID, ID, component->protocol, IMU_CHIP_BNO080 );
            }
            break;
        
        case COMPONENT_ID_BATTERY_MONITOR:
            break;
        case COMPONENT_ID_BLE_RADIO:
            break;
//        case COMPONENT_ID_RHO:
//            ID = SysIOCtlFunctions.GenerateID(PROTOCOL_ID_RHO);
//            if( ID != PROTOCOL_ID_NULL )
//                RhoFunctions.Init( &(rho_setting_t){ ID } );
//            break;
        default:
            break;
    }
}

/* Rho In */
static void RhoInputHandler( comm_host_t * host )
{
    RhoFunctions.Receive( host, &App.objects.Rho );
    RhoPointToKPoint( &App.objects.Rho.packet.primary, &App.buffers.rho.data[0] );
    RhoPointToKPoint( &App.objects.Rho.packet.secondary, &App.buffers.rho.data[1] );
    rho_get_confidence( &App.objects.Rho, App.buffers.rho.confidence );
}

/* Rho In */
static void RhoOutputHandler( comm_host_t * host )
{
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

typedef struct
{
    application_handler_input_functions Input;
    application_handler_output_functions Output;
} application_handler_functions;

typedef struct
{
    void (*Init)(void);
    void (*InitComponent)(component_t *);
    void (*Start)(void);
    void (*Tick)(void);
    application_handler_functions Handler;
} application_functions;

static application_functions AppFunctions =
{
    .Init = Application_Init,
    .InitComponent = Application_InitComponent,
    .Start = Application_Start,
    .Tick = Application_Tick,
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
    /* Override SYSIOCTL's component initializer for Application's */
    SysIOCtlFunctions.InitComponent = AppFunctions.InitComponent;

    system_profile_t profile = PROFILE_TEMPLATE;
    memcpy( &Profile, &profile, sizeof(system_profile_t));
    
    SystemFunctions.Init( &Profile );
}

#endif /* application_h */
