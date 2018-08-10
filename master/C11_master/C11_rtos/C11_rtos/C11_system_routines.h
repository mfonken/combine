//
//  C11_system_routines.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef C11_system_routines_h
#define C11_system_routines_h

#include "systemtypes.h"

static const system_activity_routine_t _C11_V1_startup_routine =
{
    SYSTEM_ACTIVITY_STARTUP,
    7, {
        SYSTEM_SUBACTIVITY_SELF_CHECK,
        SYSTEM_SUBACTIVITY_INIT_COMMUNICATION,
        SYSTEM_SUBACTIVITY_INIT_COMPONENTS,
        SYSTEM_SUBACTIVITY_INIT_TAU_CLIENT,
        SYSTEM_SUBACTIVITY_INIT_KINETIC,
        SYSTEM_SUBACTIVITY_INIT_CONFIRM
    }
};
static const system_activity_routine_t _C11_V1_probe_host_routine =
{
    SYSTEM_ACTIVITY_PROBE_HOST,
    3, {
        SYSTEM_SUBACTIVITY_SEND_HOST_PROBE,
        SYSTEM_SUBACTIVITY_RECEIVE_HOST_PROBE
    }
};
static const system_activity_routine_t _C11_V1_probe_rho_routine =
{
    SYSTEM_ACTIVITY_PROBE_RHO,
    2, {
        SYSTEM_SUBACTIVITY_SEND_RHO_PROBE,
        SYSTEM_SUBACTIVITY_RECEIVE_RHO_PROBE
    }
};

static const system_activity_routine_t _C11_V1_tau_init_routine =
{
    SYSTEM_ACTIVITY_TAU_INIT,
    4, {
        SYSTEM_SUBACTIVITY_TAU_SCHEDULE_RHO_RECEIVER,
        SYSTEM_SUBACTIVITY_TAU_SCHEDULE_PACKET_QUEUER,
        SYSTEM_SUBACTIVITY_TAU_DESCHEDULE_RHO_RECEIVER,
        SYSTEM_SUBACTIVITY_TAU_DESCHEDULE_PACKET_QUEUER
    }
};
static const system_activity_routine_t _C11_V1_tau_standard_routine =
{
    SYSTEM_ACTIVITY_TAU_STANDARD,
    7, {
        
    }
};

SYSTEM_ACTIVITY_TAU_AUGMENTED,
SYSTEM_ACTIVITY_TAU_MINIMAL,
SYSTEM_ACTIVITY_SLEEP,

#endif /* C11_system_routines_h */
