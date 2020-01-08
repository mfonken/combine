//
//  systemhandlers.h
//  C12_rtos
//
//  Created by Matthew Fonken on 8/22/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef systemhandlers_h
#define systemhandlers_h

#include "systemmanager.h"

#define MAX_HANDLER_QUEUE 10

void ComponentInterruptQueuer( void );
void ComponentInterruptHandler( component_id );
void RhoOutputHandler( void );
void RhoInputHandler( void );
void MotionOutputHandler( imu_feature_t, uint32_t );
void MotionInputHandler( void );
void HostOutputHandler( void );
void HostInputHandler( void );
void SubRadioOutputHandler( void );
void TouchInterruptHandler( component_id );

typedef struct
{
    void (*Rho)(void);
    void (*Motion)(void);
    void (*Host)(void);
    void (*Touch)( component_id );
} handler_input_functions;

typedef struct
{
    void (*Rho)(void);
    void (*Motion)( imu_feature_t, uint32_t );
    void (*Host)(void);
    void (*SubRadio)(void);
} handler_output_functions;

typedef struct
{
    void (*Queue)(void);
    void (*Handle)(component_id);
} handler_interrupt_functions;

typedef struct
{
    handler_interrupt_functions Interrupt;
    handler_input_functions Input;
    handler_output_functions Output;
} handler_functions;

static handler_functions HandlerFunctions =
{
    .Interrupt.Queue    = ComponentInterruptQueuer,
    .Interrupt.Handle   = ComponentInterruptHandler,
    .Input.Rho          = RhoInputHandler,
    .Output.Rho         = RhoOutputHandler,
    .Input.Motion       = MotionInputHandler,
    .Output.Motion      = MotionOutputHandler,
    .Input.Host         = HostInputHandler,
    .Output.Host        = HostOutputHandler,
    .Input.Touch        = TouchInterruptHandler,
    .Output.SubRadio    = SubRadioOutputHandler
};

typedef union
{
    component_id component_ID;
    event_id event_ID;
} handler_id;

typedef struct
{
    uint8_t queue_index;
    handler_id queue[MAX_HANDLER_QUEUE];
} handler_t;

static handler_t Handler = {0};

#endif /* systemhandlers_h */
