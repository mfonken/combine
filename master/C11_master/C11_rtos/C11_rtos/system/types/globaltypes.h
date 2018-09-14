//
//  globaltypes.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/25/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef globaltypes_h
#define globaltypes_h

/* Standard includes */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#define IS_VALID_PTR(X) ((long)X>=0x0000000100000000)

#define NO_DATA 0xff

#define UNLOCK 0
#define LOCK 1

typedef void (*generic_function_t)(void);

typedef uint16_t event_id;
typedef uint8_t generic_id_t;

typedef enum
{
    COMPONENT_STATE_OFF = 0x00,
    COMPONENT_STATE_ON = 0x01,
    COMPONENT_STATE_Z = 0x02,
    COMPONENT_STATE_INTERRUPT = 0x0a
} COMPONENT_STATE;

typedef struct
{
uint8_t
    macro,
    micro;
} component_id;

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
    tied;
COMPONENT_STATE
    state;
void *
    instance;
} component_t;

typedef struct
{
uint16_t
    buttons;
uint8_t
    slider;
uint8_t
    prev_slider;
uint32_t
    timestamp;
} touch_data_t;

#endif /* globaltypes_h */
