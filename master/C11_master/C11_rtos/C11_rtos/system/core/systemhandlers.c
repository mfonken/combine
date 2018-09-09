//
//  systemhandlers.c
//  C11_rtos
//
//  Created by Matthew Fonken on 8/22/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "systemhandlers.h"

void ComponentInterruptQueuer( void )
{
    
}
void ComponentInterruptHandler( component_id ID )
{
    if( ID.macro == SYSTEM_COMPONENT_TYPE_SENSOR )
    {
        switch( ID.micro )
        {
            case SYSTEM_SENSOR_MOTION_PRIMARY:
            case SYSTEM_SENSOR_MOTION_SECONDARY:
                HandlerFunctions.Input.Motion();
                break;
            case SYSTEM_SENSOR_RHO_MODULE_PRIMARY:
            case SYSTEM_SENSOR_RHO_MODULE_SECONDARY:
                HandlerFunctions.Input.Rho();
                break;
            case SYSTEM_SENSOR_TOUCH_PRIMARY:
            case SYSTEM_SENSOR_TOUCH_SECONDARY:
                HandlerFunctions.Input.Touch(ID);
                break;
            case SYSTEM_SENSOR_TIP_PRIMARY:
            case SYSTEM_SENSOR_TIP_SECONDARY:
            case SYSTEM_SENSOR_TIP_ALTERNATE:
                ///
                break;
            case SYSTEM_SENSOR_BATTERY_MONITOR_PRIMARY:
                ///
                break;
            default:
                break;
        }
    }
}

/* [Meta] Global buffers */

/* Rho Out */
void RhoOutputHandler( void )
{
    RhoFunctions.Send( &System.objects.Rho.settings );
}
/* Rho In */
void RhoInputHandler( void )
{
    RhoFunctions.Receive( &System.objects.Rho );
    RhoPointToKPoint( &System.objects.Rho.packet.primary, &System.buffers.rho.data[0] );
    RhoPointToKPoint( &System.objects.Rho.packet.secondary, &System.buffers.rho.data[1] );
//    System.buffers.rho.confidence = System.objects.Rho.packet.probabilites.;
}

/* Motion Out */
void MotionOutputHandler( imu_feature_t feature, uint32_t interval )
{
    IMUFunctions.Start( &System.objects.IMU.client, feature, interval, System.objects.IMU.chip );
}
/* Motion In */
void MotionInputHandler( void )
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
void HostOutputHandler( void )
{
    
}
/* BLE In */
void HostInputHandler( void )
{
    // State change?
//    CommFunctions.Perform.Receive();
}

/* Sub Out */
void SubRadioOutputHandler( void )
{
    
}

/* Touch In */
void TouchInterruptHandler( component_id ID )
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

/* Battery Monitor In */
