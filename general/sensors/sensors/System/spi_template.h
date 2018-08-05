//
//  spi_template.h
//  sensors
//
//  Created by Matthew Fonken on 8/5/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef spi_template_h
#define spi_template_h

#include <stdint.h>

//#define NO_REG 0xff
#define SPI_DATA_LEN 32
static uint8_t spi_data[SPI_DATA_LEN];

typedef enum
{
    SPI_READ_REG_EVENT = 1,
    SPI_WRITE_REG_EVENT
} spi_type_t;

typedef struct
{
spi_type_t
    type:8;
uint8_t
    reg,
    length;
} spi_event_t;

static uint8_t performSPIEvent( spi_event_t e, uint8_t * data )
{
    uint8_t len = 0;
    switch(e.type)
    {
        case SPI_READ_REG_EVENT:
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
        case SPI_WRITE_REG_EVENT:
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

#endif /* spi_template_h */
