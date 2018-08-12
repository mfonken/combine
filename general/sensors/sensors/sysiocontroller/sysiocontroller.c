//
//  sysiocontroller.c
//  sensors
//
//  Created by Matthew Fonken on 7/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "sysiocontroller.h"

void SYSIOCTL_Init( system_component_t * list )
{
    uint8_t num_components = sizeof(list) / sizeof(list[0]);
    memset( &SysIOCtl, 0, sizeof(SysIOCtl) );
    for( uint8_t i = 0; i < num_components; i++ ) SYSIOCTL_Push_Component( list[i] );
}

system_component_t * SYSIOCTL_Get_Component( component_id id )
{
    for( uint8_t i = 0; i < SYSIOCTL_NUM_FAMILIES; i++ )
    {
        for( uint8_t j = 0; j < SysIOCtl.tables.index[i]; j++ )
        {
            system_component_t * curr = &SysIOCtl.tables.component[i][j];
            if( curr->ID.macro == id.macro
               && curr->ID.micro == id.micro )
                return curr;
        }
    }
    
    return NULL;
}

void SYSIOCTL_Tie_Component( component_id id, void * instance )
{
    SYSIOCTL_Get_Component(id)->instance = instance;
}

void SYSIOCTL_Push_Component( system_component_t component )
{
    SysIOCtl.tables.component[component.family][SysIOCtl.tables.index[component.family]++] = component;
    if( SysIOCtl.tables.index[component.family] >= SYSIOCTL_MAX_COMPONENTS_PER_FAMILY )
    {
        SysIOCtl.tables.index[component.family]--;
        component.tied = false;
    }
    else component.tied = true;
}

void SYSIOCTL_Enable_Family( system_family_t family )
{
    for( uint8_t i = 0; i < SysIOCtl.tables.index[family]; i++)
    {
        SYSIOCTL_Enable_Component( SysIOCtl.tables.component[family][i] );
    }
}
void SYSIOCTL_Disable_Family( system_family_t family )
{
    for( uint8_t i = 0; i < SysIOCtl.tables.index[family]; i++)
    {
        SYSIOCTL_Disable_Component( SysIOCtl.tables.component[family][i] );
    }
}

void GPIO_Set( uint8_t port, uint8_t pin, uint8_t status )
{
    
}

void SYSIOCTL_Enable_Component( system_component_t component )
{
    GPIO_Set( component.pin, component.port, 1 );
}

void SYSIOCTL_Disable_Component( system_component_t component )
{
    GPIO_Set( component.pin, component.port, 0 );
}

