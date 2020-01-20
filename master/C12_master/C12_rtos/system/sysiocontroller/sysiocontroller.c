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
    SysIOCtlFunctions.EnableFamily( SYSTEM_FAMILY_0 );
}

component_t * SYSIOCTL_Get_Component( component_id_t id )
{
    for( uint8_t i = 0; i < SYSIOCTL_NUM_FAMILIES; i++ )
    {
        for( uint8_t j = 0; j < SysIOCtl.tables.index[i]; j++ )
        {
            component_t * curr = &SysIOCtl.tables.component[i][j];
            if( curr->ID.macro == id.macro
               && curr->ID.micro == id.micro )
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
uint8_t ID, uint8_t sequence_number, uint8_t comm_channel,
shtp_client_header * header,
shtp_client_buffer * buffer,
shtp_client_product_id * product,
shtp_client_output * output )
 */
void SYSIOCTL_Init_Component( component_t * component )
{
    LOG_IO_CTL(IO_CTL_DEBUG, "Initializing component: %s(0x%02x)\n", component_type_strings[component->ID.macro], component->ID.micro);
    generic_id_t ID = PROTOCOL_ID_NULL;
    if( component->ID.macro == COMPONENT_TYPE_SENSOR )
    {
//        switch( component->ID.micro)
//        {
//            case SYSTEM_SENSOR_MOTION_PRIMARY:
//            case SYSTEM_SENSOR_MOTION_SECONDARY:
//                ID = SysIOCtlFunctions.GenerateID(PROTOCOL_ID_SH2);
//                if( ID != PROTOCOL_ID_NULL )
//                {
//                    IMUFunctions.GenerateClient( &SysIOCtl.system->objects.IMU.client, ID );
//                    IMUFunctions.Init( &SysIOCtl.system->objects.IMU, component->ID, ID, COMM_CHAN_I2C, IMU_CHIP_BNO080 );
//                }
//                break;
//            case SYSTEM_SENSOR_RHO_MODULE_PRIMARY:
//            case SYSTEM_SENSOR_RHO_MODULE_SECONDARY:
//                ID = SysIOCtlFunctions.GenerateID(PROTOCOL_ID_RHO);
//                if( ID != PROTOCOL_ID_NULL )
//                    RhoFunctions.Init( &(rho_setting_t){ ID } );
//                break;
//            case SYSTEM_SENSOR_TOUCH_PRIMARY:
//            case SYSTEM_SENSOR_TOUCH_SECONDARY:
//                break;
//            case SYSTEM_SENSOR_TIP_PRIMARY:
//            case SYSTEM_SENSOR_TIP_SECONDARY:
//            case SYSTEM_SENSOR_TIP_ALTERNATE:
//                break;
//            case SYSTEM_SENSOR_BATTERY_MONITOR_PRIMARY:
//                break;
//            default:
//                break;
//        }
    }
}

void SYSIOCTL_Push_Component( component_t * component )
{
    LOG_IO_CTL(IO_CTL_DEBUG, "Pushing component: 0x%02x\n", component->ID.micro);
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
    if( family == SYSTEM_FAMILY_NONE ) return;
    if( family == SYSTEM_FAMILY_0 ) return;
    uint8_t i = 0;
    LOG_IO_CTL(IO_CTL_DEBUG, "Enabling family: %s\n", family_strings[family]);
    for( ; i < SysIOCtl.tables.index[family]; i++)
        SYSIOCTL_Enable_Component( &SysIOCtl.tables.component[family][i] );
    if( !i )
        LOG_IO_CTL(IO_CTL_DEBUG, "No components in family: %s\n", family_strings[family]);
}
void SYSIOCTL_Disable_Family( system_family_t family )
{
    if( family == SYSTEM_FAMILY_NONE ) return;
    if( family == SYSTEM_FAMILY_0 )
    {
        LOG_IO_CTL(IO_CTL_DEBUG, "Rejecting disable of FAMILY_0\n");
        return;
    }
    uint8_t i = 0;
    LOG_IO_CTL(IO_CTL_DEBUG, "Disabling %s\n", family_strings[family]);
    for( ; i < SysIOCtl.tables.index[family]; i++)
        SYSIOCTL_Disable_Component( &SysIOCtl.tables.component[family][i] );
    if( !i )
        LOG_IO_CTL(IO_CTL_DEBUG, "No components in family: %s\n", family_strings[family]);
}

void GPIO_Set( uint8_t port, uint8_t pin, uint8_t status )
{
    
}

void SYSIOCTL_Enable_Component( component_t * component )
{
    if( component->state == COMPONENT_STATE_ON ) return;
    LOG_IO_CTL(IO_CTL_DEBUG, "Enabling component: 0x%02x\n", component->ID.micro);
    GPIO_Set( component->pin, component->port, 1 );
    component->state = COMPONENT_STATE_ON;
}

void SYSIOCTL_Disable_Component( component_t * component )
{
    if( component->state == COMPONENT_STATE_OFF ) return;
    LOG_IO_CTL(IO_CTL_DEBUG, "Disabling component: 0x%02x\n", component->ID.micro);
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
