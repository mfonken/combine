//
//  globaltypes.h
//  C12_rtos
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

#include "global_config.h"

#ifndef MAX
#define MAX(A,B)                ( ( A > B ) ? A : B )
#endif

#define AVG2(A,B)               ( ( A + B ) / 2. )
#define SWAP(A,B)               { typeof(A) temp = A; A = B; B = temp; }

#define BOUNDU(X,MAX)           ( ( X > MAX ) ? MAX : X )         // Bound in upper range
#define BOUND(X,MIN,MAX)        ( ( X < MIN ) ? MIN : BOUNDU( X, MAX ) ) // Bound in upper and lower range

#define FBOUND(X,MIN,MAX)       ( ( X < MIN ) ? MIN : ( ( X > MAX ) ? MAX : X ) )

#define SQUARE(X)               ( X * X )
#define DISTANCE_SQ(X,Y)        ( SQUARE(X) + SQUARE(Y) )
#define INRANGE(X,Y,T)          ( abs( X - Y ) < T )

#ifndef ZDIV
#define ZDIV_LNUM               ( 1 << 10 )
#define ZDIV(X,Y)               ( ( Y == 0 ) ? ( X == 0 ? 0 : ZDIV_LNUM ) : X / Y )
#endif

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#define MAX_COMPONENTS 3

#define NAME_STRINGIFY(x,y) x ## _ ## y
#define NAME_BUILDER(x,y) NAME_STRINGIFY(x,y)

//#define SUBACTIVITY_ADVANCED(ID, PTR, ARGS, COMPONENT) { ID, (void(*)(void*))PTR, (void*)ARGS, (component_id_t)COMPONENT }
#define SUBACTIVITY(ID, PTR, ARGS) { ID, (void(*)(void*))PTR, (void*)ARGS, 0 }

//#define COMPONENT_ID(A,B) (component_id_t){ A, B } //( ( A << 8 ) & 0xff00 | ( B & 0x00ff ) )

#warning Should global def be checked here or in implementation?
#define PLATFORM_SPECIFIC(NAME) NAME_BUILDER(__PLATFORM__, NAME)
#define OS_SPECIFIC(NAME) NAME_BUILDER(__OS__, NAME)
#define PAPI_SPECIFIC(NAME) NAME_BUILDER(__PAPI__, NAME)
#define APP_SPECIFIC(NAME) NAME_BUILDER(APPLICATION, NAME)

#define TEMPLATED_TYPE(T) void*
#define TEMPLATED_PROTOTYPE(function_name, T) void function_name(void*)
#define TEMPLATED_CALLBACK(function_name, T, name, content) \
void function_name(TEMPLATED_TYPE(T) NAME_BUILDER(name,T)) { \
    T name = (T)NAME_BUILDER(name,T); \
    content; }
#define DO_NOTHING

#define CALL_OS_PARENT(X) OS_SPECIFIC(__LINE__)(X)

#define IS_VALID_PTR(X) ((long)X>=0x0000000100000000)

#define NO_DATA NULL

#define APP_LOCK 1
#define APP_UNLOCK 0

#define NO_REG 0xff

#define FOR( TYPE, NAME, LEN ) for( TYPE NAME = 0; NAME < LEN; NAME++ )

typedef void (*void_handler_t)(void);
typedef void_handler_t generic_function_t;
typedef void * void_p_function_data_t;
typedef void (*void_p_function_t)(void_p_function_data_t);

typedef uint16_t event_id;
typedef int8_t generic_id_t;
typedef uint16_t qty_t;
typedef uint32_t tick_t;
typedef float frequency_t;
typedef double floating_t;

#warning Use Micrium versions
#ifndef OS_STATE
typedef enum
{
    OS_TMR_STATE_UNUSED = 0,
    OS_TMR_STATE_STOPPED,
    OS_TMR_STATE_RUNNING,
    OS_TMR_STATE_COMPLETED,
} os_state_t;
#endif

#ifndef OS_TMR_OPT
typedef enum
{
    OS_OPT_TMR_NONE = 0,  /* No option selected                                   */
    OS_OPT_TMR_ONE_SHOT,  /* Timer will not auto restart when it expires          */
    OS_OPT_TMR_PERIODIC   /* Timer will     auto restart when it expires          */
} tmr_opt_t;
#endif

//typedef enum
//{
//    COMPONENT_STATE_OFF = 0x00,
//    COMPONENT_STATE_ON = 0x01,
//    COMPONENT_STATE_Z = 0x02,
//    COMPONENT_STATE_INTERRUPT = 0x0a
//} COMPONENT_STATE;

typedef enum
{
    COMPONENT_TYPE_NONE = 0x00,
    COMPONENT_TYPE_SENSOR,
    COMPONENT_TYPE_DRIVER
} COMPONENT_TYPE;

typedef enum
{
    COMPONENT_ACTIVITY_NONE = 0x00,
    COMPONENT_ACTIVITY_PROBE,
    COMPONENT_ACTIVITY_SCHEDULE,
    COMPONENT_ACTIVITY_INTERRUPT,
    COMPONENT_ACTIVITY_TRIGGER
} COMPONENT_ACTIVITY;

typedef enum
{
    INTERRUPT_ACTION_IGNORE = 0x00,
    INTERRUPT_ACTION_QUEUE,
    INTERRUPT_ACTION_IMMEDIATE
} INTERRUPT_ACTION;

//typedef struct
//{
//uint8_t
//    macro,
//    micro;
//} component_id_t;
typedef uint8_t component_id_t;

typedef struct
{
char *
    name;
component_id_t
    ID;
uint8_t
    family,
    protocol,
    route,
    addr,
    port,
    pin,
    tied;
COMPONENT_STATE
    state;
void *
    instance;
} component_t;

typedef uint32_t port_t, pin_t;

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

typedef enum
{
    HW_EDGE_RISING = 0,
    HW_EDGE_FALLING,
} HW_EDGE_T, hw_edge_t;

typedef struct
{
    port_t port;
    pin_t pin;
    hw_edge_t edge;
    generic_id_t handle_id;
} hw_event_message_t;

//typedef struct
//{
//component_id_t
//    ids[MAX_COMPONENTS];
//uint8_t
//    num;
//} component_id_list_t;

#define DEFAULT_QUEUE_TIMEOUT_MS 100
#define DEFAULT_QUEUE_MAX_QTY 50

#endif /* globaltypes_h */
