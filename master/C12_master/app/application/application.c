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
    SystemFunctions.Perform.EnableTask(TASK_ID_BLINK_GPIO_COMPONENT_A);//TASK_ID_MONITOR);//
}

void Application_Start( void )
{
	Application_Tick();
//    SystemFunctions.Perform.ExitState();
//
//    SystemFunctions.Register.State( STATE_NAME_ACTIVE );
//    SystemBehavior.Perform.ComponentInterrupt( BNO080_PORT, BNO080_PIN, HW_EDGE_FALLING );
}

void Application_Tick( void )
{
	OS_ERR  err;
	while(1)
	{
		LOG_SYSTEM(SYSTEM_DEBUG, "Tick.\n");
		OSTimeDlyHMSM(0,0,10,0, OS_OPT_TIME_HMSM_NON_STRICT, &err);
	    ASSERT(err == OS_ERR_NONE);
//		OS.DelayMs(10000);
	}
}


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
