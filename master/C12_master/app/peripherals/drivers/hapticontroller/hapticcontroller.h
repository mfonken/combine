//
//  hapticcontroller.h
//  C12_rtos
//
//  Created by Matthew Fonken on 8/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef hapticcontroller_h
#define hapticcontroller_h

#include "DRV2605.h"

typedef enum
{
    HAPTIC_TRIGGER_TYPE_NONE = 0,
    HAPTIC_TRIGGER_TYPE_SHORT,
    HAPTIC_TRIGGER_TYPE_MEDIUM,
    HAPTIC_TRIGGER_TYPE_LONG,
} HAPTIC_TRIGGER_TYPE, haptic_trigger_t;

static void HapticControllerTrigger( comm_host_t * host )
{
    haptic_trigger_t trigger_type = (haptic_trigger_t)host->i2c_host.buffer;
    //TODO: Finish...
}

typedef struct
{
    void (*Waveform)(comm_host_t *, uint8_t, uint8_t);
    void (*Library)(comm_host_t *, uint8_t);
    void (*Mode)(comm_host_t *, uint8_t);
    void (*RealtimeValue)(comm_host_t *, uint8_t);
    void (*ERM)(comm_host_t *);
    void (*LRA)(comm_host_t *);
} haptic_set_functions;

typedef struct
{
    void (*Trigger)(comm_host_t *);
    bool (*Start)(comm_host_t *);
    void (*Go)(comm_host_t *);
    void (*Stop)(comm_host_t *);
    uint8_t (*Read)(comm_host_t *, uint8_t);
    void (*Write)(comm_host_t *, uint8_t, uint8_t);
    haptic_set_functions Set;
    
    i2c_event_t (*GetReadEvent)(comm_host_t *, uint8_t, uint8_t*);
    i2c_event_t (*GetWriteEvent)(comm_host_t *, uint8_t, uint8_t*);
} haptic_functions;

static haptic_functions HapticFunctions =
{
    .Trigger = HapticControllerTrigger,
    .Start = DRV2605_Start,
    .Go = DRV2605_Go,
    .Stop = DRV2605_Stop,
    .Read = DRV2605_ReadRegister,
    .Write = DRV2605_WriteRegister,
    .Set.Waveform = DRV2605_SetWaveform,
    .Set.Library = DRV2605_SelectLibrary,
    .Set.Mode = DRV2605_SetMode,
    .Set.RealtimeValue = DRV2605_SetRealtimeValue,
    .Set.ERM = DRV2605_UseERM,
    .Set.LRA = DRV2605_UseLRA,
    .GetReadEvent = DRV2605_GetReadEvent,
    .GetWriteEvent = DRV2605_GetWriteEvent
};

#endif /* hapticcontroller_h */
