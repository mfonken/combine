//
//  sysiocontroller.c
//  sensors
//
//  Created by Matthew Fonken on 7/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "sysiocontroller.h"

void SYSIOCTL_Init( system_master_t * system )
{
    LOG_IO_CTL(IO_CTL_DEBUG, "Initializing SYSIOCTL\n");
    memset( &SysIOCtl, 0, sizeof(SysIOCtl) );
    SysIOCtl.system = system;
    for( uint8_t i = 0; i < system->profile->component_list.num_entries; i++ )
    {
        component_t * component = &system->profile->component_list.entries[i];
        SysIOCtlFunctions.EnableFamily( component->family );
        SysIOCtlFunctions.InitComponent( component );
        SysIOCtlFunctions.Push( component );
        SysIOCtlFunctions.DisableFamily( component->family );
    }
//    SysIOCtlFunctions.EnableFamily( SYSTEM_FAMILY_0 );
}

component_t * SYSIOCTL_Get_Component( component_id_t id )
{
    for( uint8_t i = 0; i < SYSIOCTL_NUM_FAMILIES; i++ )
    {
        for( uint8_t j = 0; j < SysIOCtl.tables.index[i]; j++ )
        {
            component_t * curr = &SysIOCtl.tables.component[i][j];
            if( curr->ID == id )
                return curr;
        }
    }
    return NULL;
}

void SYSIOCTL_Tie_Component( component_id_t id, void * instance )
{
    SYSIOCTL_Get_Component(id)->instance = instance;
}
/*
uint8_t ID, uint8_t sequence_number, uint8_t comm_protocol,
shtp_client_comm_host * header,
shtp_client_buffer * buffer,
shtp_client_product_id * product,
shtp_client_output * output )
 */
void SYSIOCTL_Init_Component( component_t * component )
{
}

void SYSIOCTL_Push_Component( component_t * component )
{
    LOG_IO_CTL(IO_CTL_DEBUG, "Pushing component: 0x%02x\n", component->ID);
    SysIOCtl.tables.component[component->family][SysIOCtl.tables.index[component->family]++] = *component;
    if( SysIOCtl.tables.index[component->family] >= SYSIOCTL_MAX_COMPONENTS_PER_FAMILY )
    {
        SysIOCtl.tables.index[component->family]--;
        component->tied = false;
    }
    else component->tied = true;
}

void SYSIOCTL_Enable_Family( system_family_t family )
{
    if( family == COMPONENT_FAMILY_NONE ) return;
    if( family == COMPONENT_FAMILY_0 ) return;
    uint8_t i = 0;
    LOG_IO_CTL(IO_CTL_DEBUG, "Enabling family: %s\n", SYSTEM_COMPONENT_FAMILY_STRINGS[family]);
    for( ; i < SysIOCtl.tables.index[family]; i++)
        SYSIOCTL_Enable_Component( &SysIOCtl.tables.component[family][i] );
    if( !i )
        LOG_IO_CTL(IO_CTL_DEBUG, "No components in family: %s\n", SYSTEM_COMPONENT_FAMILY_STRINGS[family]);
}
void SYSIOCTL_Disable_Family( system_family_t family )
{
    if( family == COMPONENT_FAMILY_NONE ) return;
    if( family == COMPONENT_FAMILY_0 )
    {
        LOG_IO_CTL(IO_CTL_DEBUG, "Rejecting disable of FAMILY_0\n");
        return;
    }
    uint8_t i = 0;
    LOG_IO_CTL(IO_CTL_DEBUG, "Disabling %s\n", SYSTEM_COMPONENT_FAMILY_STRINGS[family]);
    for( ; i < SysIOCtl.tables.index[family]; i++)
        SYSIOCTL_Disable_Component( &SysIOCtl.tables.component[family][i] );
    if( !i )
        LOG_IO_CTL(IO_CTL_DEBUG, "No components in family: %s\n", SYSTEM_COMPONENT_FAMILY_STRINGS[family]);
}

void GPIO_Set( uint8_t port, uint8_t pin, uint8_t status )
{
    
}

void SYSIOCTL_Enable_Component( component_t * component )
{
    if( component->state == COMPONENT_STATE_ON ) return;
    LOG_IO_CTL(IO_CTL_DEBUG, "Enabling component: 0x%02x\n", component->ID);
    GPIO_Set( component->pin, component->port, 1 );
    component->state = COMPONENT_STATE_ON;
}

void SYSIOCTL_Disable_Component( component_t * component )
{
    if( component->state == COMPONENT_STATE_OFF ) return;
    LOG_IO_CTL(IO_CTL_DEBUG, "Disabling component: 0x%02x\n", component->ID);
    GPIO_Set( component->pin, component->port, 0 );
    component->state = COMPONENT_STATE_OFF;
}

generic_id_t SYSIOCTL_Generate_ID( protocol_id_base_t base )
{
    switch( base )
    {
        case PROTOCOL_ID_SH2:
            if(SysIOCtl.protocol_id_base_indeces.sh2 == PROTOCOL_ID_NULL)
                SysIOCtl.protocol_id_base_indeces.sh2 = PROTOCOL_ID_SH2;
            else if(SysIOCtl.protocol_id_base_indeces.sh2 >= PROTOCOL_ID_SH2_MAX )
                return PROTOCOL_ID_NULL;
            return SysIOCtl.protocol_id_base_indeces.sh2++;
            break;
        case PROTOCOL_ID_RHO:
            if(SysIOCtl.protocol_id_base_indeces.rho == PROTOCOL_ID_NULL)
                SysIOCtl.protocol_id_base_indeces.rho = PROTOCOL_ID_RHO;
            else if(SysIOCtl.protocol_id_base_indeces.rho >= PROTOCOL_ID_RHO_MAX )
                return PROTOCOL_ID_NULL;
            return SysIOCtl.protocol_id_base_indeces.rho++;
            break;
        default:
            break;
    }
    return PROTOCOL_ID_NULL;
}

sysioctl_functions SysIOCtlFunctions =
{
    .Init = SYSIOCTL_Init,
    .InitComponent = SYSIOCTL_Init_Component,
    .Get = SYSIOCTL_Get_Component,
    .Tie = SYSIOCTL_Tie_Component,
    .Push = SYSIOCTL_Push_Component,
    .EnableFamily = SYSIOCTL_Enable_Family,
    .DisableFamily = SYSIOCTL_Disable_Family,
    .EnableComponent = SYSIOCTL_Enable_Component,
    .DisableComponent = SYSIOCTL_Disable_Component,
    .GenerateID = SYSIOCTL_Generate_ID,
//    .GetCallback = SYSIOCTL_Get_Component_Callback
};

