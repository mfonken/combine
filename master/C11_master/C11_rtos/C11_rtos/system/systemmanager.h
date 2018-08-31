//
//  systemmanager.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/6/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef systemmanager_h
#define systemmanager_h

#include <stdio.h>
#include "systemtypes.h"
#include "profilemanager.h"
#include "systembehavior.h"
#include "sysiocontroller.h"

#define DEFAULT_SYSTEM_STATE SYSTEM_STATE_IDLE
#define DEFAULT_SYSTEM_ACTION SYSTEM_ACTION_NONE
#define DEFAULT_SYSTEM_ACTIVITY SYSTEM_ACTIVITY_NONE
#define DEFAULT_SYSTEM_SUBACTIVITY SYSTEM_SUBACTIVITY_NONE
#define DEFAULT_SYSTEM_ERROR SYSTEM_ERROR_NONE
#define DEFAULT_SYSTEM_CONSUMPTION SYSTEM_CONSUMPTION_NONE
#define DEFAULT_SYSTEM_ACTIVITY SYSTEM_ACTIVITY_NONE

extern system_master_t System;

void InitSystemManager(system_profile_t *);

void PerformSystemManagerRoutine( system_activity_routine_t * );
void PerformSystemManagerRoutineSubactivities( system_subactivity_t *, uint8_t );
void PerformSystemManagerSubactivity( system_subactivity_t );
void PerformSystemManagerEnableProfileEntryState( system_profile_entry_t * );
void PerformSystemManagerDisableProfileEntryState( system_profile_entry_t * );

void RegisterSystemManangerTaskShelf( system_task_shelf_t * );
void RegisterSystemManangerSubactivityMap( system_subactivity_map_t );
void RegisterSystemManagerProfile( system_profile_t * );
void RegisterSystemManagerProfileEntry( system_profile_entry_t * );
void RegisterSystemManagerStateProfileList( system_state_profile_list_t * );
void RegisterSystemManagerState( system_state_t );
void RegisterSystemManagerActivity( system_activity_t );
void RegisterSystemManagerSubactivity( system_subactivity_t );
void RegisterSystemManagerError( system_error_t );
void RegisterSystemManagerConsumption( system_consumption_t );

system_subactivity_map_entry_t * GetSubactivityMapEntryById( system_subactivity_t );
system_task_shelf_entry_t * GetTaskShelfEntryById( system_task_shelf_entry_id_t );

void EnstateSystemManagerTaskShelfEntry( system_task_shelf_entry_id_t );
void EnstateSystemManagerStateProfile( system_state_profile_t * );

typedef struct
{
    void (*Routine)( system_activity_routine_t * );
    void (*Subactivities)( system_subactivity_t *, uint8_t );
    void (*Subactivity)( system_subactivity_t );
    void (*EnableProfileEntry)( system_profile_entry_t * );
    void (*DisableProfileEntry)( system_profile_entry_t * );
} system_perform_functions;
typedef struct
{
    void (*TaskShelf)( system_task_shelf_t * );
    void (*SubactivityMap)( system_subactivity_map_t );
    void (*Profile)( system_profile_t * );
    void (*ProfileEntry)( system_profile_entry_t * );
    void (*StateProfileList)( system_state_profile_list_t * );
    void (*State)( system_state_t );
    void (*Activity)( system_activity_t );
    void (*Subactivity)( system_subactivity_t );
    void (*Error)( system_error_t );
    void (*Consumption)( system_consumption_t );
} system_register_functions;
typedef struct
{
    void (*TaskShelfEntry)( system_task_shelf_entry_id_t );
    void (*StateProfile)( system_state_profile_t * );
} system_enstate_functions;

typedef struct
{
    system_subactivity_map_entry_t * (*SubactivityMapEntry)( system_subactivity_t );
    system_task_shelf_entry_t * (*TaskShelfEntry)( system_task_shelf_entry_id_t );
} system_get_functions;

typedef struct
{
    void (*Init)(system_profile_t *);
    system_perform_functions Perform;
    system_register_functions Registers;
    system_enstate_functions Enstate;
    system_get_functions Get;
} system_functions;

static system_functions SystemFunctions =
{
    .Init = InitSystemManager,
    .Perform.Routine = PerformSystemManagerRoutine,
    .Perform.Subactivities = PerformSystemManagerRoutineSubactivities,
    .Perform.Subactivity = PerformSystemManagerSubactivity,
    .Perform.EnableProfileEntry = PerformSystemManagerEnableProfileEntryState,
    .Perform.DisableProfileEntry = PerformSystemManagerEnableProfileEntryState,
    .Registers.TaskShelf = RegisterSystemManangerTaskShelf,
    .Registers.SubactivityMap = RegisterSystemManangerSubactivityMap,
    .Registers.Profile = RegisterSystemManagerProfile,
    .Registers.ProfileEntry = RegisterSystemManagerProfileEntry,
    .Registers.StateProfileList = RegisterSystemManagerStateProfileList,
    .Registers.State = RegisterSystemManagerState,
    .Registers.Activity = RegisterSystemManagerActivity,
    .Registers.Subactivity = RegisterSystemManagerSubactivity,
    .Registers.Error = RegisterSystemManagerError,
    .Registers.Consumption = RegisterSystemManagerConsumption,
    .Enstate.TaskShelfEntry = EnstateSystemManagerTaskShelfEntry,
    .Enstate.StateProfile = EnstateSystemManagerStateProfile,
    .Get.SubactivityMapEntry = GetSubactivityMapEntryById,
    .Get.TaskShelfEntry = GetTaskShelfEntryById
};
#endif /* systemmanager_h */
