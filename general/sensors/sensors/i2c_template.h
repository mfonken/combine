//
//  i2c_template.h
//  sensors
//
//  Created by Matthew Fonken on 7/26/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef i2c_template_h
#define i2c_template_h

#include <stdint.h>

#define NO_REG 0xff

#define MAX_DATA_LEN 32
static uint8_t i2c_data[MAX_DATA_LEN];

typedef enum
{
    I2C_READ_REG_EVENT = 0,
    I2C_WRITE_REG_EVENT
} I2C_type_t;

typedef struct
{
    I2C_type_t type:8;
    uint8_t
        addr,
        reg,
        length;
} i2c_event_t;

static uint8_t performI2CEvent(i2c_event_t e, uint8_t * data)
{
    uint8_t len = 0;
    switch(e.type)
    {
        case I2C_READ_REG_EVENT:
            if(e.length == 1)
            {
                //
            }
            else
            {
                if(e.reg == NO_REG)
                {
                    //
                }
                else
                {
                    //
                }
            }
            break;
        case I2C_WRITE_REG_EVENT:
            if(e.length == 1)
            {
                //
            }
            else
            {
                if(e.reg == NO_REG)
                {
                    //
                }
                else
                {
                    //
                }
            }
            break;
        default:
            break;
    }
    return len;
}

#endif /* i2c_template_h */
