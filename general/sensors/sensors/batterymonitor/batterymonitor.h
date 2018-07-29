//
//  batterymonitor.h
//  sensors
//
//  Created by Matthew Fonken on 7/26/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef batterymonitor_h
#define batterymonitor_h

#include <stdio.h>
#include "STC3100IQT.h"

typedef stc_t battery_monitor_t;

static uint8_t getControl(void)
{
    performI2CEvent(STCFunctions.getControlEvent(), i2c_data);
    return (uint8_t)*i2c_data;
}

typedef struct
{
    uint8_t(*getControl)(void);
} battery_monitor_functions;

static battery_monitor_functions BatteryMonitor =
{
    .getControl = getControl
};

#endif /* batterymonitor_h */
