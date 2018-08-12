//
//  systembridge.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef systembridge_h
#define systembridge_h

#include "systemtypes.h"
#include "systemmanager.h"
#incldue "profilemanager.h"
#include "sysiocontroller.h"

typedef struct
{
    void (*Self)(void);
    void (*Communication)(void);
    void (*Components)(void);
    void (*TauClient)(void);
    void (*Kinetic)(void);
} bridge_init_functions;

typedef enum
{
    BRIDGE_PROBE_ID_NONE = 0,
    BRIDGE_PROBE_ID_HOST,
    BRIDGE_PROBE_ID_RHO,
    BRIDGE_PROBE_ID_TIP,
    BRIDGE_PROBE_ID_BATTERY_MONITOR,
} BRIDGE_PROBE_ID;
typedef BRIDGE_PROBE_ID bridge_probe_id_t;

typedef struct
{
    void (*Send)(bridge_probe_id_t);
    void (*Receive)(bridge_probe_id_t);
} bridge_probe_functions;

typedef struct
{
    void (*Init)(void);
    void (*Fetch)(void);
    void (*Perform)(void);
    void (*Update)(void);
    void (*Store)(void);
} bridge_profile_functions;

typedef enum
{
    BRIDGE_SCHEDULER_ID_NONE = 0,
    BRIDGE_SCHEDULER_ID_TAU_RHO_RECEIVE,
    BRIDGE_SCHEDULER_ID_TAU_PACKET_QUEUE,
} BRIDGE_SCHEDULER_ID;
typedef BRIDGE_SCHEDULER_ID bridge_scheduler_id_t;

typedef struct
{
    void (*Schedule)(bridge_scheduler_id_t);
    void (*Deschedule)(bridge_scheduler_id_t);
} bridge_scheduler_functions;

typedef enum
{
    BRIDGE_INTERRUPTER_ID_NONE = 0,
    BRIDGE_INTERRUPTER_ID_TAU_RHO_RECEIVE,
    BRIDGE_INTERRUPTER_ID_TAU_KINETIC_PERFORM,
    BRIDGE_INTERRUPTER_ID_TAU_PACKET_GENERATE,
    BRIDGE_INTERRUPTER_ID_HAPTIC_PACKET_GENERATE,
} BRIDGE_INTERRUPTER_ID;
typedef BRIDGE_INTERRUPTER_ID bridge_interrupter_id_t;

typedef struct
{
    void (*Send)(bridge_interrupter_id_t);
    void (*Receive)(bridge_interrupter_id_t);
    void (*Perform)(bridge_interrupter_id_t);
} bridge_interrupter_functions;

typedef struct
{
    void (*SelfCheck)(void);
    void (*ConfirmInit)(void);
    void (*WaitForWake)(void);
} bridge_perform_functions;

typedef struct
{
    bridge_init_functions init;
    bridge_perform_functions perform;
    bridge_probe_functions probe;
    bridge_scheduler_functions scheduler;
    bridge_interrupter_functions interrupter;
    bridge_profile_functions profile;
} bridge_functions;

static bridge_functions Bridge =
{
    .Init.Self = InitBridge,
    .Init.Communication = ,
    .Init.Components = SysIOFunctions.Init,
    .Init.TauClient = ,
    .Init.Kinetic = KineticFunctions.DefaultInit,
    .Perform.SelfCheck = ,
    .Perform.ConfirmInit = ,
    .Perform.WaitForWake = ,
    .Perform.Probe.Send = ,
    .Perform.Probe.Receive = ,
    .Perform.Scheduler.Schedule = ,
    .Perform.Scheduler.Deschedule = ,
    .Perform.Interrupter.Send = ,
    .Perform.Interrupter.Receive =
    .Profile.Init = ProfileFunctions.Init,
    .Profile.Fetch = ProfileFunctions.Fetch,
    .Profile.Perform = ProfileFunctions.Perform,
    .Profile.Update = ProfileFunctions.Update,
    .Profile.Store = ProfileFunctions.Store
};

static system_subactivity_map_t BridgeSubactivityMap =
{
    { Bridge.Perform.SelfCheck, NULL }, /* SYSTEM_SUBACTIVITY_SELF_CHECK = 0 */
    { Bridge.Init.Communication, NULL }, /* SYSTEM_SUBACTIVITY_INIT_COMMUNICATION */
    { Bridge.Init.Components, NULL }, /* SYSTEM_SUBACTIVITY_INIT_COMPONENTS */
    { Bridge.Init.TauClient, NULL }, /* SYSTEM_SUBACTIVITY_INIT_TAU_CLIENT */
    { Bridge.Init.Kinetic, NULL }, /* SYSTEM_SUBACTIVITY_INIT_KINETIC */
    { Bridge.Perform.ConfirmInit, NULL }, /* SYSTEM_SUBACTIVITY_INIT_CONFIRM */
    { Bridge.Probe.Send, BRIDGE_PROBE_ID_HOST }, /* SYSTEM_SUBACTIVITY_SEND_HOST_PROBE */
    { Bridge.Probe.Receive, BRIDGE_PROBE_ID_HOST }, /* SYSTEM_SUBACTIVITY_RECEIVE_HOST_PROBE */
    { Bridge.Probe.Send, BRIDGE_PROBE_ID_RHO }, /* SYSTEM_SUBACTIVITY_SEND_RHO_PROBE */
    { Bridge.Probe.Receive, BRIDGE_PROBE_ID_RHO }, /* SYSTEM_SUBACTIVITY_RECEIVE_RHO_PROBE */
    { Bridge.Profile.Fetch, NULL },/* SYSTEM_SUBACTIVITY_PROFILE_FETCH */
    { Bridge.Profile.Perform, NULL },/* SYSTEM_SUBACTIVITY_PROFILE_PERFORM */
    { Bridge.Profile.Update, NULL },/* SYSTEM_SUBACTIVITY_PROFILE_UPDATE */
    { Bridge.Profile.Store, NULL },/* SYSTEM_SUBACTIVITY_PROFILE_STORE */
    { Bridge.Scheduler.Schedule, BRIDGE_SCHEDULER_ID_TAU_RHO_RECEIVE }, /* SYSTEM_SUBACTIVITY_TAU_SCHEDULE_RHO_RECEIVER */
    { Bridge.Scheduler.Schedule, BRIDGE_SCHEDULER_ID_TAU_PACKET_QUEUE },/* SYSTEM_SUBACTIVITY_TAU_SCHEDULE_PACKET_QUEUER */
    { Bridge.Scheduler.deschedule, BRIDGE_SCHEDULER_ID_TAU_RHO_RECEIVE }, /* SYSTEM_SUBACTIVITY_TAU_DESCHEDULE_RHO_RECEIVER */
    { Bridge.Scheduler.deschedule, BRIDGE_SCHEDULER_ID_TAU_PACKET_QUEUE }, /* SYSTEM_SUBACTIVITY_TAU_DESCHEDULE_PACKET_QUEUER */
    { Bridge.Interrupter.Receiver, BRIDGE_INTERRUPTER_ID_TAU_RHO_RECEIVE },/* SYSTEM_SUBACTIVITY_TAU_RECEIVE_RHO_PACKET */
    { Bridge.Interrupter.Perform, BRIDGE_INTERRUPTER_ID_TAU_KINETIC_PERFORM }, /* SYSTEM_SUBACTIVITY_TAU_PERFORM_KINETIC */
    { Bridge.Interrupter.Send, BRIDGE_INTERRUPTER_ID_TAU_PACKET_GENERATE }, /* SYSTEM_SUBACTIVITY_TAU_GENERATE_PACKET */
    { Bridge.Probe.Send, BRIDGE_PROBE_ID_TIP }, /* SYSTEM_SUBACTIVITY_TIP_SEND_PROBE */
    { Bridge.Probe.Send, BRIDGE_PROBE_ID_BATTERY_MONITOR }, /* SYSTEM_SUBACTIVITY_BATTERY_MONITOR_SEND_PROBE */
    { Bridge.Interrupter.Send, BRIDGE_INTERRUPTER_ID_HAPTIC_PACKET_GENERATE }, /* SYSTEM_SUBACTIVITY_HAPTIC_SEND_PACKET */
    { Bridge.Perform.WaitForWake, NULL }/* SYSTEM_SUBACTIVITY_WAIT_FOR_WAKE */
};

static void InitBridge(void)
{
    SystemFunctions.Register.SubactivityMap( &BridgeSubactivityMap );
}

#endif /* systembridge_h */
