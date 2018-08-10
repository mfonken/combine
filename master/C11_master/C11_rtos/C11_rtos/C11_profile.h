//
//  C11_profile.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/9/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef C11_profile_h
#define C11_profile_h

#include "systemtypes.h"

#define SYSTEM_SENSOR_TOUCH_PRIMARY_ENTRY_STATE SYSTEM_PROFILE_ENTRY_STATE_ENABLED
#define SYSTEM_SENSOR_TOUCH_PRIMARY_ENTRY_DIRECTION SYSTEM_PROFILE_ENTRY_DIRECTION_INPUT
#define SYSTEM_SENSOR_TOUCH_PRIMARY_ENTRY_TYPE SYSTEM_PROFILE_ENTRY_TYPE_INTERRUPT
#define SYSTEM_SENSOR_TOUCH_PRIMARY_INTERRUPT_ENTRY { \
	{	SYSTEM_SENSOR_TOUCH_PRIMARY_ENTRY_STATE, \
		SYSTEM_SENSOR_TOUCH_PRIMARY_ENTRY_DIRECTION, \
		SYSTEM_SENSOR_TOUCH_PRIMARY_ENTRY_TYPE, }, \
	SYSTEM_SENSOR_TOUCH_PRIMARY, \
	SYSTEM_ACTIVITY_QUEUE_TOUCH_INTERRUPT }
#define SYSTEM_TASK_SHELF_ENTRY_SENSOR_TOUCH_PRIMARY_ENTRY { \
	SYSTEM_SENSOR_TOUCH_PRIMARY_TASKS \
	1, /* Number of interrupts */ \
	{ /* Interrupts */ \
		SYSTEM_SENSOR_TOUCH_PRIMARY_INTERRUPT_ENTRY \
	}, \
	0, /* Number of scheduled tasks */ \
	{ /* Sheduled Tasks */ \
	//
	}, } \

/* Registry of all available system tasks */
static system_task_shelf_t SystemTaskShelf[] =
{
	SYSTEM_TASK_SHELF_ENTRY_SENSOR_TOUCH_PRIMARY_ENTRY
};

static system_state_profile_t _C11_StartupStateProfile = 
{
	{ SYSTEM_FAMILY_A, SYSTEM_FAMILY_B }, /* Families */
	{ /* Tasks */
		SYSTEM_TASK_SHELF_ENTRY_SENSOR_TOUCH_PRIMARY_ENTRY,

	}
};

static sys_profile_t Profile =
{
	
};

#endif /* C11_profile_h */
