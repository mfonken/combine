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

typedef struct
{
uint32_t
    charge,
    counter,
    current,
    voltage;
} battery_monitor_basic_t;

void GetBatteryMonitorBasic( battery_monitor_basic_t * );
void SetBatteryMonitorMode( uint8_t );

typedef struct
{
    void(*GetBasic)( battery_monitor_basic_t *);
    void (*Set)( uint8_t );
} battery_monitor_functions;

static battery_monitor_functions BatteryMonitor =
{
    .GetBasic = GetBatteryMonitorBasic,
    .Set = SetBatteryMonitorMode
};


#endif /* batterymonitor_h */
