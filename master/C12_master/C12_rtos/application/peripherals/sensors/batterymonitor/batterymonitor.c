//
//  batterymonitor.c
//  C12_rtos
//
//  Created by Matthew Fonken on 8/27/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include <stdio.h>
#include "batterymonitor.h"

void GetBatteryMonitorBasic( battery_monitor_basic_t * buffer )
{
    buffer->charge = STCFunctions.GetCharge();
    buffer->counter = STCFunctions.GetCounter();
    buffer->current = STCFunctions.GetCurrent();
    buffer->voltage = STCFunctions.GetVoltage();
}

void SetBatteryMonitorMode( uint8_t mode )
{
    
}
