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
#include "systembehavior.h"
#include "profilemanager.h"
#include "sysiocontroller.h"
#include "kinetic_master.h"
#include "communicationmanager.h"
#include "taumanager.h"

static void InitBridge( system_profile_t * profile )
{
    SystemFunctions.Init();
    SystemFunctions.Registers.SubactivityMap(
     (system_subactivity_map_t) { {
        { .function.blank = BehaviorFunctions.Perform.SelfCheck, .data.byte = NO_DATA }, /* SELF_CHECK = 0 */
        { .function.blank = CommFunctions.Init, .data.byte = NO_DATA }, /* INIT_COMMUNICATION */
        { .function.pointer = (void(*)(void *))SysIOCtlFunctions.Init, .data.pointer = Profile.components }, /* INIT_COMPONENTS */
        { .function.blank = TauFunctions.Init, .data.byte = NO_DATA }, /* INIT_TAU_CLIENT */
        { .function.blank = KineticFunctions.DefaultInit, .data.byte = NO_DATA }, /* INIT_KINETIC */
        { .function.blank = BehaviorFunctions.Perform.ConfirmInit, .data.byte = NO_DATA }, /* INIT_CONFIRM */
        { .function.byte = (void(*)(uint8_t))BehaviorFunctions.Perform.Probe.Send, SYSTEM_PROBE_ID_HOST }, /* SEND_HOST_PROBE */
        { .function.byte = (void(*)(uint8_t))BehaviorFunctions.Perform.Probe.Receive, SYSTEM_PROBE_ID_HOST }, /* RECEIVE_HOST_PROBE */
        { .function.byte = (void(*)(uint8_t))BehaviorFunctions.Perform.Probe.Send, SYSTEM_PROBE_ID_RHO }, /* SEND_RHO_PROBE */
        { .function.byte = (void(*)(uint8_t))BehaviorFunctions.Perform.Probe.Receive, SYSTEM_PROBE_ID_RHO }, /* RECEIVE_RHO_PROBE */
        { .function.blank = ProfileFunctions.Fetch, .data.byte = NO_DATA },/* PROFILE_FETCH */
        { .function.blank = ProfileFunctions.Perform, .data.byte = NO_DATA },/* PROFILE_PERFORM */
        { .function.blank = ProfileFunctions.Update, .data.byte = NO_DATA },/* PROFILE_UPDATE */
        { .function.blank = ProfileFunctions.Store, .data.byte = NO_DATA },/* PROFILE_STORE */
        { .function.byte = (void(*)(uint8_t))BehaviorFunctions.Perform.Scheduler.Schedule, SYSTEM_SCHEDULER_ID_TAU_RHO_RECEIVE }, /* TAU_SCHEDULE_RHO_RECEIVER */
        { .function.byte = (void(*)(uint8_t))BehaviorFunctions.Perform.Scheduler.Schedule, SYSTEM_SCHEDULER_ID_TAU_PACKET_QUEUE },/* TAU_SCHEDULE_PACKET_QUEUER */
        { .function.byte = (void(*)(uint8_t))BehaviorFunctions.Perform.Scheduler.Deschedule, SYSTEM_SCHEDULER_ID_TAU_RHO_RECEIVE }, /* TAU_DESCHEDULE_RHO_RECEIVER */
        { .function.byte = (void(*)(uint8_t))BehaviorFunctions.Perform.Scheduler.Deschedule, SYSTEM_SCHEDULER_ID_TAU_PACKET_QUEUE }, /* TAU_DESCHEDULE_PACKET_QUEUER */
        { .function.byte = (void(*)(uint8_t))BehaviorFunctions.Perform.Interrupter.Receive, SYSTEM_INTERRUPTER_ID_TAU_RHO_RECEIVE },/* TAU_RECEIVE_RHO_PACKET */
        { .function.byte = (void(*)(uint8_t))BehaviorFunctions.Perform.Interrupter.Perform, SYSTEM_INTERRUPTER_ID_TAU_KINETIC_PERFORM }, /* TAU_PERFORM_KINETIC */
        { .function.byte = (void(*)(uint8_t))BehaviorFunctions.Perform.Interrupter.Send, SYSTEM_INTERRUPTER_ID_TAU_PACKET_GENERATE }, /* TAU_GENERATE_PACKET */
        { .function.byte = (void(*)(uint8_t))BehaviorFunctions.Perform.Probe.Send, SYSTEM_PROBE_ID_TIP }, /* TIP_SEND_PROBE */
        { .function.byte = (void(*)(uint8_t))BehaviorFunctions.Perform.Probe.Send, SYSTEM_PROBE_ID_BATTERY_MONITOR }, /* BATTERY_MONITOR_SEND_PROBE */
        { .function.byte = (void(*)(uint8_t))BehaviorFunctions.Perform.Interrupter.Send, SYSTEM_INTERRUPTER_ID_HAPTIC_PACKET_GENERATE }, /* HAPTIC_SEND_PACKET */
        { .function.blank = BehaviorFunctions.Perform.WaitForWake, .data.byte = NO_DATA } /* WAIT_FOR_WAKE */
    } } );
    ProfileFunctions.Init( profile );
}

#endif /* systembridge_h */
