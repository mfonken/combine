//
//  sysiocontroller.c
//  sensors
//
//  Created by Matthew Fonken on 7/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "sysiocontroller.h"

void SYSIOCTL_Init( system_master_t * p_system )
{
    LOG_IO_CTL(IO_CTL_DEBUG, "Initializing SYSIOCTL\n");
    memset( &SysIOCtl, 0, sizeof(SysIOCtl) );
    SysIOCtl.p_system = p_system;
    for( uint8_t i = 0; i < p_system->profile->component_list.num_entries; i++ )
    {
        component_t * p_component = &p_system->profile->component_list.entries[i];
        SysIOCtlFunctions.EnableFamily( p_component->family );
        SysIOCtlFunctions.InitComponent( p_component );
        SysIOCtlFunctions.Push( p_component );
        SysIOCtlFunctions.DisableFamily( p_component->family );
    }
    
    SysIOCtlFunctions.CompleteComponentTransients();
}

component_t * SYSIOCTL_GetComponent( component_id_t id )
{
    for( uint8_t i = 0; i < SYSIOCTL_NUM_FAMILIES; i++ )
    {
        for( uint8_t j = 0; j < SysIOCtl.tables.index[i]; j++ )
        {
            component_t * p_curr = &SysIOCtl.tables.component[i][j];
            if( p_curr->ID == id )
                return p_curr;
        }
    }
    return NULL;
}

void SYSIOCTL_TieComponent( component_id_t id, void * p_instance )
{
//    SysIOCtlFunctions.GetComponent(id)->instance = p_instance;
}
/*
uint8_t ID, uint8_t sequence_number, uint8_t comm_protocol,
shtp_client_comm_host * header,
shtp_client_buffer * buffer,
shtp_client_product_id * product,
shtp_client_output * output )
 */
void SYSIOCTL_InitComponent( component_t * p_component )
{
}

void SYSIOCTL_PushComponent( component_t * p_component )
{
    LOG_IO_CTL(IO_CTL_DEBUG, "Pushing component %s(0x%02x) to %s\n", p_component->name, p_component->ID, SYSTEM_COMPONENT_FAMILY_STRINGS[p_component->family]);
    SysIOCtl.tables.component[p_component->family][SysIOCtl.tables.index[p_component->family]++] = *p_component;
    if( SysIOCtl.tables.index[p_component->family] >= SYSIOCTL_MAX_COMPONENTS_PER_FAMILY )
    {
        SysIOCtl.tables.index[p_component->family]--;
        p_component->tied = false;
    }
    else p_component->tied = true;
}

void SYSIOCTL_EnableFamily( system_family_t family )
{
    if( family == COMPONENT_FAMILY_NONE ) return;
    if( family == COMPONENT_FAMILY_0 ) return;
    uint8_t i = 0;
    LOG_IO_CTL(IO_CTL_DEBUG, "Enabling family: %s\n", SYSTEM_COMPONENT_FAMILY_STRINGS[family]);
    for( ; i < SysIOCtl.tables.index[family]; i++)
        SysIOCtlFunctions.EnableComponent( &SysIOCtl.tables.component[family][i] );
    if( !i )
        LOG_IO_CTL(IO_CTL_DEBUG, "No components in family: %s\n", SYSTEM_COMPONENT_FAMILY_STRINGS[family]);
}
void SYSIOCTL_DisableFamily( system_family_t family )
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
        SysIOCtlFunctions.DisableComponent( &SysIOCtl.tables.component[family][i] );
    
    if( !i )
        LOG_IO_CTL(IO_CTL_DEBUG, "No components in family: %s\n", SYSTEM_COMPONENT_FAMILY_STRINGS[family]);
}

void SYSIOCTL_EnableComponent( component_t * p_component )
{
    if( p_component->state == COMPONENT_STATE_ON
       || p_component->state == COMPONENT_STATE_ENABLING ) return;
    LOG_IO_CTL(IO_CTL_DEBUG, "Enabling component: 0x%02x\n", p_component->ID);
    PAPI.GPIO.Set( (gpio_t){ p_component->port, p_component->pin } );
    p_component->state = COMPONENT_STATE_ENABLING;
}

void SYSIOCTL_DisableComponent( component_t * p_component )
{
    if( p_component->state == COMPONENT_STATE_OFF
       || p_component->state == COMPONENT_STATE_DISABLING ) return;
    LOG_IO_CTL(IO_CTL_DEBUG, "Disabling component: 0x%02x\n", p_component->ID);
    PAPI.GPIO.Clear( (gpio_t){ p_component->port, p_component->pin } );
    p_component->state = COMPONENT_STATE_DISABLING;
}

void SYSIOCTL_CompleteComponentTransients( void )
{
    for( uint8_t i = 1; i < NUM_SYSTEM_COMPONENT_ID; i++ )
    {
        component_t * p_component = SysIOCtlFunctions.GetComponent( i );
        switch( p_component->state )
        {
            case COMPONENT_STATE_ENABLING:
                p_component->state = COMPONENT_STATE_ON;
                break;
            case COMPONENT_STATE_DISABLING:
                p_component->state = COMPONENT_STATE_OFF;
                break;
            default:
                break;
        }
    }
}

generic_id_t SYSIOCTL_GenerateID( protocol_id_base_t base )
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
    .InitComponent = SYSIOCTL_InitComponent,
    .GetComponent = SYSIOCTL_GetComponent,
    .Tie = SYSIOCTL_TieComponent,
    .Push = SYSIOCTL_PushComponent,
    .EnableFamily = SYSIOCTL_EnableFamily,
    .DisableFamily = SYSIOCTL_DisableFamily,
    .EnableComponent = SYSIOCTL_EnableComponent,
    .DisableComponent = SYSIOCTL_DisableComponent,
    .GenerateID = SYSIOCTL_GenerateID,
    .CompleteComponentTransients = SYSIOCTL_CompleteComponentTransients,
//    .GetCallback = SYSIOCTL_Get_Component_Callback
};

