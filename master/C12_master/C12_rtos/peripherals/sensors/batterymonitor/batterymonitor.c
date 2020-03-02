//
//  batterymonitor.c
//  C12_rtos
//
//  Created by Matthew Fonken on 8/27/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include <stdio.h>
#include "batterymonitor.h"

void GetBatteryMonitorBasicBase( comm_host_t * host )
{
    BatteryMonitor.GetBasic( host, host->generic_comm_host.buffer );
}

void GetBatteryMonitorBasic( comm_host_t * host, battery_monitor_basic_t * buffer )
{
    buffer->charge = STCFunctions.GetCharge( host );
    buffer->counter = STCFunctions.GetCounter( host );
    buffer->current = STCFunctions.GetCurrent( host );
    buffer->voltage = STCFunctions.GetVoltage( host );
}

void SetBatteryMonitorMode( comm_host_t * host )
{
    uint8_t mode = *(uint8_t *)host->generic_comm_host.buffer;
    printf("Setting to %d\n", mode);
}
