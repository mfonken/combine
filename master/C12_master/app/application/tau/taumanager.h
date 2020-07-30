//
//  taumanager.h
//  C12_rtos
//
//  Created by Matthew Fonken on 8/13/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef taumanager_h
#define taumanager_h

#include "kinetic_master.h"
#include "global_types.h"

#define TAU_MAX_ACTIONS_PER_STATE 3

typedef enum
{
    TAU_STATE_IDLE = 0,
    TAU_STATE_START,
    TAU_STATE_STANDARD,
    TAU_STATE_AUGMENTED,
    TAU_STATE_MINIMAL,
    TAU_STATE_END,
    NUM_TAU_STATES
} TAU_STATE, tau_state_t;

typedef enum
{
    TAU_ACTION_NONE = 0,
    TAU_ACTION_INIT,
    TAU_ACTION_START,
    TAU_ACTION_PAUSE,
    TAU_ACTION_STOP,
    TAU_ACTION_PERFORM_KINEITC,
    TAU_ACTION_GENERATE_PACKET,
    TAU_ACTION_ANALYZE_CYCLE,
    NUM_TAU_ACTIONS,
} TAU_ACTION, tau_action_t;

void RegisterTauManagerState( tau_state_t );
void TauManagerTick(void);

void TauManagerInit(void);
void TauManagerDoNothing(void);
void TauManagerStart(void);
void TauManagerPause(void);
void TauManagerStop(void);
void TauManagerPerformKinetic(void);
void TauManagerGeneratePacket(void);
void TauManagerAnalyzeCycle(void);

typedef struct
{
    void (*Init)(void);
    void (*DoNothing)(void);
    void (*Kinetic)(void);
    void (*GeneratePacket)(void);
    void (*AnalyzeCycle)(void);
} tau_perform_functions;

typedef struct
{
    void (*RegisterState)(tau_state_t);
    void (*Tick)(void);
    void (*Start)(void);
    void (*Pause)(void);
    void (*Stop)(void);
    tau_perform_functions Perform;
} tau_functions;

static tau_functions TauFunctions =
{
    .RegisterState = RegisterTauManagerState,
    .Tick = TauManagerTick,
    .Start = TauManagerStart,
    .Pause = TauManagerPause,
    .Stop = TauManagerStop,
    .Perform.Init = TauManagerInit,
    .Perform.DoNothing = TauManagerDoNothing,
    .Perform.Kinetic = TauManagerPerformKinetic,
    .Perform.GeneratePacket = TauManagerGeneratePacket,
    .Perform.AnalyzeCycle = TauManagerAnalyzeCycle,
};

typedef struct
{
uint8_t
    len;
tau_action_t
    actions[TAU_MAX_ACTIONS_PER_STATE];
} tau_state_action_entry_t;
typedef tau_state_action_entry_t tau_state_action_list_t[NUM_TAU_STATES];

static tau_state_action_list_t TauStateActionList =
{
    { /* STATE_IDLE */
        1,
        {
            TAU_ACTION_NONE
        }
    },
    { /* STATE_START */
        2,
        {
            TAU_ACTION_INIT,
            TAU_ACTION_START
        }
    },
    { /* STATE_STANDARD */
        3,
        {
            TAU_ACTION_PERFORM_KINEITC,
            TAU_ACTION_GENERATE_PACKET,
            TAU_ACTION_ANALYZE_CYCLE
        }
    },
    { /* STATE_AUGMENTED */
        3,
        {
            TAU_ACTION_PERFORM_KINEITC,
            TAU_ACTION_GENERATE_PACKET,
            TAU_ACTION_ANALYZE_CYCLE
        }
    },
    { /* STATE_MINIMAL */
        2,
        {
            TAU_ACTION_PERFORM_KINEITC,
            TAU_ACTION_GENERATE_PACKET
        }
    },
    { /* STATE_END */
        2,
        {
            TAU_ACTION_PAUSE,
            TAU_ACTION_STOP
        }
    }
};

typedef generic_function_t tau_action_list_t[NUM_TAU_ACTIONS];

static tau_action_list_t TauActionList =
{
    TauManagerDoNothing,/* ACTION_NONE */
    TauManagerInit, /* ACTION_INIT */
    TauManagerStart, /* ACTION_START */
    TauManagerPause, /* ACTION_PAUSE */
    TauManagerStop, /* ACTION_STOP */
    TauManagerPerformKinetic, /* ACTION_PERFORM_KINEITC */
    TauManagerGeneratePacket, /* ACTION_GENERATE_PACKET */
    TauManagerAnalyzeCycle, /* ACTION_ANALYZE_CYCLE */
};

typedef struct
{
double
    value,
    confidence;
} tau_data_element_t;

typedef struct
{
uint8_t
    ID;
tau_state_t
    state;
uint32_t
    timestamp;
tau_data_element_t
    data[3];
} tau_packet_t;

typedef struct
{
uint8_t
    ID;
tau_state_t
    state;
tau_state_action_entry_t
    tasks;
uint32_t
    timestamp;
} tau_header_t;


typedef struct
{
uint8_t
    packet:1,
    kinetic:1,
    reserved:6;
} tau_locks;

typedef struct
{
tau_header_t
    header;
orientation_data_t
    orientation;
rho_data_t
    rho;
kinetic_t
    kinetic;
tau_packet_t
    packet;
tau_locks
    locks;
} tau_t;

typedef struct
{
uint32_t
    orientation_interval,
    rho_interval;
} tau_config_t;

static tau_data_element_t TauManagerGetElementFromKalman( kalman_filter_t * kalman )
{
    /* Assume total doubt */
    tau_data_element_t e = { kalman->value, 0. };
    e.confidence = kalman->K[0];
    return e;
}

#endif /* taumanager_h */
