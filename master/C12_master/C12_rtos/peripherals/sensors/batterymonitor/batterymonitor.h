//
//  batterymonitor.h
//  sensors
//
//  Created by Matthew Fonken on 7/26/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef batterymonitor_h
#define batterymonitor_h

#include "STC3100IQT.h"

typedef struct
{
uint32_t
    charge,
    counter,
    current,
    voltage;
} battery_monitor_basic_t;

void GetBatteryMonitorBasicBase( comm_host_t * );
void GetBatteryMonitorBasic( comm_host_t *, battery_monitor_basic_t * );
void SetBatteryMonitorMode( comm_host_t * );

typedef struct
{
    void (*GetBasicBase)( comm_host_t * );
    void (*GetBasic)( comm_host_t *, battery_monitor_basic_t * );
    void (*Set)( comm_host_t * );
} battery_monitor_functions;

static battery_monitor_functions BatteryMonitor =
{
    .GetBasicBase = GetBatteryMonitorBasicBase,
    .GetBasic = GetBatteryMonitorBasic,
    .Set = SetBatteryMonitorMode
};


#endif /* batterymonitor_h */
