//
//  hapticcontroller.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef hapticcontroller_h
#define hapticcontroller_h

#include "drv2605.h"

typedef enum
{
    HAPTIC_TRIGGER_TYPE_NONE = 0,
    HAPTIC_TRIGGER_TYPE_SHORT,
    HAPTIC_TRIGGER_TYPE_MEDIUM,
    HAPTIC_TRIGGER_TYPE_LONG,
} HAPTIC_TRIGGER_TYPE, haptic_trigger_t;

static void HapticControllerTrigger( haptic_trigger_t type )
{
    
}

typedef struct
{
    void (*Waveform)(uint8_t, uint8_t);
    void (*Library)(uint8_t);
    void (*Mode)(uint8_t);
    void (*RealtimeValue)(uint8_t);
    void (*ERM)(void);
    void (*LRA)(void);
} haptic_set_functions;

typedef struct
{
    void (*Trigger)(haptic_trigger_t);
    bool (*Start)(void);
    void (*Go)(void);
    void (*Stop)(void);
    uint8_t (*Read)(uint8_t);
    void (*Write)(uint8_t, uint8_t);
    haptic_set_functions Set;
    
    i2c_event_t (*GetReadEvent)(uint8_t);
    i2c_event_t (*GetWriteEvent)(uint8_t);
} haptic_functions;

static haptic_functions HapticFunctions =
{
    .Trigger = HapticControllerTrigger,
    .Start = DRV2605Start,
    .Go = DRV2605Go,
    .Stop = DRV2605Stop,
    .Read = DRV2605ReadRegister,
    .Write = DRV2605WriteRegister,
    .Set.Waveform = DRV2605SetWaveform,
    .Set.Library = DRV2605SelectLibrary,
    .Set.Mode = DRV2605SetMode,
    .Set.RealtimeValue = DRV2605SetRealtimeValue,
    .Set.ERM = DRV2605UseERM,
    .Set.LRA = DRV2605UseLRA,
    .GetReadEvent = DRV2605GetReadEvent,
    .GetWriteEvent = DRV2605GetWriteEvent
};

#endif /* hapticcontroller_h */
