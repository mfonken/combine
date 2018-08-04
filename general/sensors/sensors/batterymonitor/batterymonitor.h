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

static stc_functions BatteryMonitor =
{
    .SetMode             = STCSetMode,
    .GetMode             = STCGetMode,
    .SetControl          = STCSetControl,
    .GetControl          = STCGetControl,
    .GetIDE              = STCGetIDE,
    .GetRAM              = STCGetRAM,
    .GetCharge           = STCGetCharge,
    .GetCounter          = STCGetCounter,
    .GetCurrent          = STCGetCurrent,
    .GetVoltage          = STCGetVoltage,
    .GetTemperature      = STCGetTemperature,
    .SetModeEvent        = STCSetModeEvent,
    .GetModeEvent        = STCGetModeEvent,
    .SetControlEvent     = STCSetControlEvent,
    .GetControlEvent     = STCGetControlEvent,
    .GetIDEvent          = STCGetIDEvent,
    .GetRAMEvent         = STCGetRAMEvent,
    .GetChargeEvent      = STCGetChargeEvent,
    .GetCounterEvent     = STCGetCounterEvent,
    .GetCurrentEvent     = STCGetCurrentEvent,
    .GetVoltageEvent     = STCGetVoltageEvent,
    .GetTemperatureEvent = STCGetTemperatureEvent
};

#endif /* batterymonitor_h */
