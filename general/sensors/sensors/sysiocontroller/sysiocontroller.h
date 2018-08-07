//
//  sysiocontroller.h
//  sensors
//
//  Created by Matthew Fonken on 7/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef sysiocontroller_h
#define sysiocontroller_h

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "system_types.h"

#define SYSIOCTL_NUM_CS_FAMILY 8
#define SYSIOCTL_NUM_FAMILIES sizeof(sys_family_t)*8
#define SYSIOCTL_MAX_COMPONENTS_PER_FAMILY 6
#define SYSIOCTL_MAX_COMPONENTS SYSIOCTL_NUM_FAMILIES * SYSIOCTL_MAX_COMPONENTS_PER_FAMILY

typedef struct
{
    sys_component_t component[SYSIOCTL_NUM_FAMILIES][SYSIOCTL_MAX_COMPONENTS_PER_FAMILY];
    uint8_t index[SYSIOCTL_MAX_COMPONENTS_PER_FAMILY];
    component_id interrupt[SYSIOCTL_NUM_FAMILIES][SYSIOCTL_MAX_COMPONENTS_PER_FAMILY];
} sysioctl_tables;

void SYSIOCTL_Init( sys_component_t *, uint8_t );
sys_component_t * SYSIOCTL_Get_Component( component_id );
void SYSIOCTL_Tie_Component( component_id, void * );
void SYSIOCTL_Push_Component( sys_component_t );
void SYSIOCTL_Enable_Family( sys_family_t );
void SYSIOCTL_Disable_Family( sys_family_t );
void SYSIOCTL_Enable_Component( sys_component_t );
void SYSIOCTL_Disable_Component( sys_component_t );

typedef struct
{
    void (*Init)( sys_component_t *, uint8_t );
    sys_component_t * (*Get)( component_id );
    void (*Tie)( component_id, void * );
    void (*Push)( sys_component_t );
    void (*EnableFamily)( sys_family_t );
    void (*DisableFamily)( sys_family_t );
    void (*EnableComponent)( sys_component_t );
    void (*DisableComponent)( sys_component_t );
} sysioctl_functions;

static sysioctl_functions SysIOCtlFunctions =
{
    .Init = SYSIOCTL_Init,
    .Get = SYSIOCTL_Get_Component,
    .Tie = SYSIOCTL_Tie_Component,
    .Push = SYSIOCTL_Push_Component,
    .EnableFamily = SYSIOCTL_Enable_Family,
    .DisableFamily = SYSIOCTL_Disable_Family,
    .EnableComponent = SYSIOCTL_Enable_Component,
    .DisableComponent = SYSIOCTL_Disable_Component
};

typedef struct
{
sysioctl_tables
    tables;
} sysioctl_t;

static sysioctl_t SysIOCtl;

#endif /* sysiocontroller_h */
