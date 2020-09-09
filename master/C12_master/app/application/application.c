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
//    SystemBehavior.Perform.ComponentInterrupt( BNO080_PORT, BNO080_PIN, HW_EDGE_FALLING );
}

void Application_Tick( void )
{
    
COMPLETE_TASK }


/* [Meta] */
void InitializeMeta(void)
{
    /* Override SYSIOCTL's component initializer for Application's */
    SysIOCtlFunctions.InitComponent = AppFunctions.InitComponent;
}

void Application_InitComponent( component_t * p_component )
{
    LOG_IO_CTL(IO_CTL_DEBUG, "Initializing component: %s(0x%02x)\n", p_component->name, p_component->ID);
    switch( p_component->ID)
    {
        default:
            break;
    }
}
