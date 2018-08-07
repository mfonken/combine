//
//  system_types.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/6/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef system_types_h
#define system_types_h

#include <stdint.h>
#include "i2c_template.h"
#include "spi_template.h"

typedef enum
{
    SYSTEM_FAMILY_0 = 0x00, /* Always on */
    SYSTEM_FAMILY_A,
    SYSTEM_FAMILY_B,
    SYSTEM_FAMILY_C,
    SYSTEM_FAMILY_D
} SYSTEM_FAMILY;

typedef enum
{
    SYSTEM_COMM_NONE = 0,
    SYSTEM_COMM_I2C,
    SYSTEM_COMM_SPI,
    SYSTEM_COMM_UART
} SYSTEM_COMM;

typedef enum
{
    SYSTEM_COMM_CHANNEL_NONE = 0x00,
    SYSTEM_COMM_CHANNEL_PRIMARY,
    SYSTEM_COMM_CHANNEL_SECONDARY
} SYSTEM_COMM_CHANNEL;

typedef enum
{
    SYSTEM_COMM_READ_REG = 1,
    SYSTEM_COMM_WRITE_REG
} SYSTEM_COMM_TYPE;

typedef SYSTEM_COMM system_comm_channel;
typedef SYSTEM_COMM_TYPE system_comm_type;

#define SYSTEM_COMM_ADDR_NONE 0xff

typedef struct
{
    system_comm_type
type:8;
    uint8_t
    reg,
    length,
    addr;
    system_comm_channel
    channel;
} comm_event_t;

static void performCommEvent( comm_event_t event, uint8_t * data )
{
    switch( event.channel )
    {
        case SYSTEM_COMM_I2C:
            performI2CEvent(*(i2c_event_t *)&event, data );
            break;
        case SYSTEM_COMM_SPI:
            performSPIEvent(*(spi_event_t *)&event, data );
            break;
        case SYSTEM_COMM_UART:
            //            performUARTEvent(*(uart_event_t *)&event, data );
            break;
        default:
            break;
    }
}

typedef enum
{
    SYSTEM_SENSOR_MOTION_PRIMARY    = 0x50,
    SYSTEM_SENSOR_MOTION_SECONDARY  = 0x51,
    SYSTEM_SENSOR_RHO_MODULE_PRIMARY = 0x60,
    SYSTEM_SENSOR_RHO_MODULE_SECONDARY = 0x61,
    SYSTEM_SENSOR_TOUCH_PRIMARY = 0x70,
    SYSTEM_SENSOR_TOUCH_SECONDARY = 0x71,
    SYSTEM_SENSOR_TIP_PRIMARY = 0x80,
    SYSTEM_SENSOR_TIP_SECONDARY = 0x81,
    SYSTEM_SENSOR_TIP_ALTERNATE = 0x82,
    SYSTEM_SENSOR_BATTERY_MONITOR_PRIMARY = 0x90
} SYSTEM_SENSOR;

typedef enum
{
    SYSTEM_DRIVER_HAPTIC_PRIMARY    = 0x50,
    SYSTEM_DRIVER_HAPTIC_SECONDARY  = 0x51,
    SYSTEM_DRIVER_REGULATOR_1V5  = 0x61
} SYSTEM_DRIVER;

typedef enum
{
    SYSTEM_COMPONENT_SENSOR = 0x02,
    SYSTEM_COMPONENT_DRIVER = 0x03
} SYSTEM_COMPONENT;

typedef struct
{
uint8_t
    macro,
    micro;
} component_id;

typedef enum
{
    COMPONENT_STATE_OFF = 0x00,
    COMPONENT_STATE_ON = 0x01,
    COMPONENT_STATE_Z = 0x02,
    COMPONENT_STATE_INTERRUPT = 0x0a
} COMPONENT_STATE;

typedef uint8_t sys_family_t;
typedef struct
{
component_id
    ID;
uint8_t
    family,
    comm,
    chann,
    addr,
    port,
    pin,
    state,
    tied;
void *
    instance;
} sys_component_t;

#endif /* system_types_h */
