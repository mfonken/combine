//
//  batterymonitor.c
//  C12_rtos
//
//  Created by Matthew Fonken on 8/27/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include <stdio.h>
#include "batterymonitor.h"

void GetBatteryMonitorBasicBase( comm_host_t * p_host )
{
    BatteryMonitor.GetBasic( p_host, p_host->generic_comm_host.buffer );
}

void GetBatteryMonitorBasic( comm_host_t * p_host, battery_monitor_basic_t * p_buffer )
{
    p_buffer->charge  = STC3100Functions.GetCharge(  p_host );
    p_buffer->counter = STC3100Functions.GetCounter( p_host );
    p_buffer->current = STC3100Functions.GetCurrent( p_host );
    p_buffer->voltage = STC3100Functions.GetVoltage( p_host );
}

void SetBatteryMonitorMode( comm_host_t * p_host )
{
    uint8_t mode = *(uint8_t *)p_host->generic_comm_host.buffer;
    printf("Setting to %d\n", mode);
}
