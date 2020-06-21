//
//  application.c
//  app
//
//  Created by Matthew Fonken on 6/20/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#include "application.h"

void Application_Init( void )
{
    InitializeMeta();
}

void Application_Start( void )
{
    SystemFunctions.Perform.ExitState();
    OS.DelayMs(1000);
    SystemFunctions.Register.State( STATE_NAME_ACTIVE );
    SystemBehavior.Perform.ComponentInterrupt( BNO080_PORT, BNO080_PIN, HW_EDGE_FALLING );
}

void Application_Tick( void )
{
    
COMPLETE_TASK }


/* [Meta] */
void InitializeMeta(void)
{
    /* Override SYSIOCTL's component initializer for Application's */
    SysIOCtlFunctions.InitComponent = AppFunctions.InitComponent;

    system_profile_t profile = PROFILE_TEMPLATE;
    memcpy( &Profile, &profile, sizeof(system_profile_t));
    
    SystemFunctions.Init( &Profile );
}

void Application_InitComponent( component_t * component )
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
void RhoInputHandler( comm_host_t * host )
{
    RhoFunctions.Receive( host, &App.objects.Rho );
    RhoPointToKPoint( &App.objects.Rho.packet.primary, &App.buffers.rho.data[0] );
    RhoPointToKPoint( &App.objects.Rho.packet.secondary, &App.buffers.rho.data[1] );
    rho_get_confidence( &App.objects.Rho, App.buffers.rho.confidence );
}

/* Rho In */
void RhoOutputHandler( comm_host_t * host )
{
}

/* Motion Out */
void MotionOutputHandler( imu_feature_t feature, uint32_t interval )
{
    IMUFunctions.Start( &App.objects.IMU.client, feature, interval, App.objects.IMU.chip );
}
/* Motion In */
void MotionInputHandler( void )
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
void HostOutputHandler( void )
{
    
}

/* BLE In */
void HostInputHandler( void )
{
    // State change?
//    CommFunctions.Perform.Receive();
}

///* Sub Out */
//void SubRadioOutputHandler( void )
//{
//
//}

/* Touch In */
void TouchInterruptHandler( comm_host_t * host )
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
