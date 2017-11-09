//
//  startup.c
//  
//
//  Created by Matthew Fonken on 1/4/17.
//
//


#include "startup.h"

system_t sys;

void startup( system_t *s )
{
    uint8_t sys_default = SYSTEM_DEFAULT;
    *s = *( system_t * )&sys_default;
    
    Utilities_Init();
    Sensors_Init();
}

void Utilities_Init( void )
{
    SYSCLK_Init();
    Print_String( "System Clock Initialized.\r\n" );
    
    if( sys.system_controller )
    {
        SYSCTL_Init();
        Print_String( "System Controller Initialized.\r\n" );
    }
    if( sys.rf_controller )
    {
        /* TODO: Enable RF controller hardware if necessary */
        //Print_String( "RF Controller Initialized.\r\n" );
    }
}

void Sensors_Init( void )
{
    if( sys.imu )
    {
    	/* TODO: Enable IMU Hardware */
        //Print_String( "IMU Enabled.\r\n" );
    }
    
    if( sys.camera )
    {
        Camera_Init();
        Print_String( "Camera Initialized.\r\n" );
    }
    
    if( sys.touch )
    {
        Touch_Init();
        Print_String( "Touch Controller Initialized.\r\n" );
    }
}
