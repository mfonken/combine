//
//  systembridge.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef systembridge_h
#define systembridge_h

#include "systemtypes.h"

#include "kinetic_master.h"

#include "C11_component_list.h"
#include "C11_system_routines.h"
#include "C11_Profile.h"
#include "sysiocontroller.h"

#include "batterymonitor.h"
#include "touchcontroller.h"
#include "motionsensor.h"

static void PerformComponentInit(void)
{
    SysIOCtlFunctions.Init( _C11_V1_component_list_, C11_V1_NUM_COMPONENTS );
}

static void PerformKineticInit(void)
{
    KineticFunctions.DefaultInit();
}

typedef struct
{
    void (*kinetic)(void);
    void (*components)(void);
} bridge_init_functions;

typedef struct
{
    bridge_init_functions init;
} bridge_functions;

static bridge_functions Bridge =
{
    .init.kinetic = PerformKineticInit,
    .init.components = PerformComponentInit
};

#endif /* systembridge_h */
