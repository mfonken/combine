//
//  global.h
//  sensors
//
//  Created by Matthew Fonken on 8/2/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef global_h
#define global_h

#include "i2c_template.h"
#include "spi_template.h"

typedef enum
{
    SYSTEM_SENSOR_MOTION_PRIMARY = 0x01
} SYSTEM_SENSOR;

typedef enum
{
    SYSTEM_COMM_I2C = 1,
    SYSTEM_COMM_SPI
} SYSTEM_COMM;

typedef enum
{
    SYSTEM_COMM_READ_REG = 1,
    SYSTEM_COMM_WRITE_REG
} SYSTEM_COMM_TYPE;

typedef SYSTEM_COMM system_comm_channel;
typedef SYSTEM_COMM_TYPE system_comm_type;

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
        default:
            break;
    }
}

#endif /* global_h */
