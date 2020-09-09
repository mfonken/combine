//
//  application.h
//  C12_rtos
//
//  Created by Matthew Fonken on 1/18/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#ifndef application_h
#define application_h

#include "communicationmanager.h"
#include "application_types.h"
#include "application_debug.h"

typedef struct
{
bool
	component_a_gpio_state;
} application_config_t;

typedef struct
{
application_config_t
    config;
} application_buffers_t;

//typedef struct
//{
//
//} application_objects_t;

typedef struct
{
    application_buffers_t buffers;
//    application_objects_t objects;
} application_t;

static application_t App;
static system_profile_t Profile;

void InitializeMeta(void);
//

void Application_Init( void );
void Application_Start( void );
void Application_Tick( void );
void Application_InitComponent( component_t * );

typedef struct
{
    void (*Init)(void);
    void (*InitComponent)( component_t * );
    void (*Start)(void);
    void (*Tick)(void);
    void (*MotionState)( void * ); // bool);
} application_functions;

static application_functions AppFunctions =
{
    .Init = Application_Init,
    .InitComponent = Application_InitComponent,
    .Start = Application_Start,
    .Tick = Application_Tick
};

#endif /* application_h */
