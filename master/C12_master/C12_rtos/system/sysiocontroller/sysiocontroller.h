//
//  sysiocontroller.h
//  sensors
//
//  Created by Matthew Fonken on 7/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef sysiocontroller_h
#define sysiocontroller_h

/* System includes */
#include "systemtypes.h"

#define SYSIOCTL_NUM_CS_FAMILY 8
#define SYSIOCTL_NUM_FAMILIES NUM_SYSTEM_COMPONENT_FAMILY//sizeof(system_family_t)*8
#define SYSIOCTL_MAX_COMPONENTS_PER_FAMILY 6
#define SYSIOCTL_MAX_COMPONENTS SYSIOCTL_NUM_FAMILIES * SYSIOCTL_MAX_COMPONENTS_PER_FAMILY
#define PROTOCOL_ID_NULL 0

typedef enum
{
    PROTOCOL_ID_SH2 = 0x10,
    PROTOCOL_ID_SH2_MAX = 0x20,
    PROTOCOL_ID_RHO = 0x21,
    PROTOCOL_ID_RHO_MAX = 0x30
} PROTOCOL_ID_BASE, protocol_id_base_t;

typedef struct
{
generic_id_t
    sh2,
    rho;
} protocol_id_base_index_t;

typedef struct
{
    component_t component[SYSIOCTL_NUM_FAMILIES][SYSIOCTL_MAX_COMPONENTS_PER_FAMILY];
    uint8_t index[SYSIOCTL_MAX_COMPONENTS_PER_FAMILY];
    component_id_t interrupt[SYSIOCTL_NUM_FAMILIES][SYSIOCTL_MAX_COMPONENTS_PER_FAMILY];
} sysioctl_tables;

void SYSIOCTL_Init( system_master_t * );
component_t * SYSIOCTL_Get_Component( component_id_t );
void SYSIOCTL_Tie_Component( component_id_t, void * );
void SYSIOCTL_Init_Component( component_t * );
void SYSIOCTL_Push_Component( component_t * );
void SYSIOCTL_Enable_Family( system_family_t );
void SYSIOCTL_Disable_Family( system_family_t );
void SYSIOCTL_Enable_Component( component_t * );
void SYSIOCTL_Disable_Component( component_t * );
generic_id_t SYSIOCTL_Generate_ID( protocol_id_base_t );
//void * SYSIOCTL_Get_Component_Callback( component_t * );

typedef struct
{
    void (*Init)( system_master_t * );
    component_t * (*Get)( component_id_t );
    void (*Tie)( component_id_t, void * );
    void (*InitComponent)( component_t * );
    void (*Push)( component_t * );
    void (*EnableFamily)( system_family_t );
    void (*DisableFamily)( system_family_t );
    void (*EnableComponent)( component_t * );
    void (*DisableComponent)( component_t * );
    generic_id_t (*GenerateID)( protocol_id_base_t );
//    void * (*GetCallback)( component_t * );
} sysioctl_functions;

extern sysioctl_functions SysIOCtlFunctions;

typedef struct
{
protocol_id_base_index_t
    protocol_id_base_indeces;
system_master_t *
    system;
sysioctl_tables
    tables;
} sysioctl_t;

extern sysioctl_t SysIOCtl;

#endif /* sysiocontroller_h */
