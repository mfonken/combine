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
#include "hapticcontroller.h"
#include "motionsensor.h"
#include "rho_client.h"
#include "batterymonitor.h"
#include "touchcontroller.h"

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
//
void Application_Init( void );
void Application_Start( void );
void Application_Tick( void );
void Application_IMUSetState( TEMPLATED_TYPE(bool) );
void Application_InitComponent( component_t * );
void RhoInputHandler( comm_host_t * );
void RhoOutputHandler( comm_host_t * );
void MotionOutputHandler( imu_feature_t, uint32_t );
void MotionInputHandler( void );
void HostOutputHandler( comm_packet_t * );
void HostInputHandler( comm_packet_t * );
void TouchInterruptHandler( comm_host_t * );

typedef struct
{
    void (*Rho)( comm_host_t * );
    void (*Motion)(void);
    void (*Host)( comm_packet_t * );
    void (*Touch)( comm_host_t * );
} application_handler_input_functions;

typedef struct
{
    void (*Rho)( comm_host_t * );
    void (*Motion)( imu_feature_t, uint32_t );
    void (*Host)( comm_packet_t * );
} application_handler_output_functions;

typedef struct
{
    application_handler_input_functions Input;
    application_handler_output_functions Output;
} application_handler_functions;

typedef struct
{
    void (*Init)(void);
    void (*InitComponent)( component_t * );
    void (*Start)(void);
    void (*Tick)(void);
    void (*MotionState)(bool);
    application_handler_functions Handler;
} application_functions;

static application_functions AppFunctions =
{
    .Init = Application_Init,
    .InitComponent = Application_InitComponent,
    .Start = Application_Start,
    .Tick = Application_Tick,
    .MotionState = Application_IMUSetState,
    .Handler.Input.Rho          = RhoInputHandler,
    .Handler.Output.Rho         = RhoOutputHandler,
    .Handler.Input.Motion       = MotionInputHandler,
    .Handler.Output.Motion      = MotionOutputHandler,
    .Handler.Input.Host         = HostInputHandler,
    .Handler.Output.Host        = HostOutputHandler,
    .Handler.Input.Touch        = TouchInterruptHandler,
};

#endif /* application_h */
