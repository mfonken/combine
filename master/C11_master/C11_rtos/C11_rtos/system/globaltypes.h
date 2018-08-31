//
//  globaltypes.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/25/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef globaltypes_h
#define globaltypes_h

#include "i2c_template.h"
#include "spi_template.h"

#include "kinetic_master.h"

#define NO_DATA 0xff

#define UNLOCK 0
#define LOCK 1

typedef void (*generic_function_t)(void);

typedef uint16_t event_id;
typedef uint8_t generic_id_t;

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
    state,
    tied;
void *
    instance;
} component_t;

typedef struct
{
    quaternion_t
    data;
    double
    confidence;
    uint32_t
    timestamp;
} orientation_data_t;

typedef struct
{
    kpoint_t
    data[3];
    double
    confidence[3];
    uint32_t
    timestamp;
} rho_data_t;

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
