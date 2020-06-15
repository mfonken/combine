/* File: CPT112S.h
 * Author: Matthew Fonken
 * Source: https://www.silabs.com/documents/public/data-sheets/CPT112S-DataSheet.pdf
 * Description:
    The CPT112S device, part of the TouchXpress family, is designed to quickly add capacitive touch via an I2C interface by eliminating the firmware complexity and reducing the development time for ca- pacitive sensing applications.
 */
#ifndef CPT112S_h
#define CPT112S_h

#include "peripheralbridge.h"
#include <stdio.h>
#include <stdint.h>

#define CPT_ADDR            0xe0 // dynamically set with configurator
#define CPT_PACKET_LEN      4 // bytes

typedef enum
{
    TOUCH_EVENT     = 0,
    RELEASE_EVENT   = 1,
    SLIDER_EVENT    = 2,
} cpt_event_t;

typedef struct
{
    uint32_t counter:4;
    cpt_event_t type:4;
    union
    {
        struct
        {
            uint8_t index;
            uint8_t RESERVED;
        };
        uint16_t slider;
    };
    uint32_t PADDING:8;
} cpt_packet_t;

static i2c_event_t CPTGetReadEvent( comm_host_t * host, cpt_packet_t * p) { return (i2c_event_t){ (i2c_host_t*)host, I2C_READ_REG_EVENT, NO_REG, CPT_PACKET_LEN, (uint8_t *)p }; }

typedef struct
{
    i2c_event_t (*GetReadEvent)(comm_host_t *, cpt_packet_t *);
} cpt_functions;

static cpt_functions CPTFunctions =
{
    .GetReadEvent = CPTGetReadEvent
};
#endif /* CPT112S_h */
