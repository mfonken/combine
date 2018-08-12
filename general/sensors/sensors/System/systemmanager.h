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

typedef struct
{
    system_state_t          state;
    system_action_t         action;
    system_activity_t       activity;
    system_subactivity_t    subactivity;
    system_error_t          error;
    system_consumption_t    consumption_level;
    system_task_shelf_t     shelf;
    system_state_profile_t *state_profile;
    system_profile_t       *profile;
    system_subactivity_map_t *subactivity_map;
} system_master_t;
static system_master_t System;

void InitSystemManager(void);

void PerformSystemManagerRoutine( system_activity_routine_t * );
void PerformSystemManagerRoutineSubactivities( system_subactivity_t *, uint8_t );
void PerformSystemManagerSubactivity( system_subactivity_t );

void RegisterSystemManangerSubactivityMap( system_subactivity_map_t * );
void RegisterSystemManagerProfile( system_profile_t * );
void RegisterSystemManagerStateProfile( system_state_t, system_state_profile_t );
void RegisterSystemManagerState( system_state_t );
void RegisterSystemManagerAction( system_action_t );
void RegisterSystemManagerActivity( system_activity_t );
void RegisterSystemManagerSubactivity( system_subactivity_t );
void RegisterSystemManagerError( system_error_t );
void RegisterSystemManagerConsumption( system_consumption_t );

system_task_shelf_entry_t GetTaskShelfEntryById( system_task_shelf_entry_id_t );

void EnstateSystemManagerTaskShelfEntry( system_task_shelf_entry_id_t );
void EnstateSystemManagerStateProfile( system_state_profile_t * );

typedef struct
{
    void (*Routine)( system_activity_routine_t * );
    void (*Subactivities)( system_state_t, system_state_profile_t );
    void (*Subactivity)( system_subactivity_t );
} system_perform_functions;
typedef struct
{
    void (*SubactivityMap)( system_subactivity_map_t * );
    void (*Profile)( system_profile_t * );
    void (*StateProfile)( system_state_t, system_state_profile_t );
    void (*State)( system_state_t );
    void (*Action)( system_action_t );
    void (*Activity)( system_activity_t );
    void (*Subactivity)( system_subactivity_t );
    void (*Error)( system_error_t );
    void (*Consumption)( system_consumption_t );
} system_register_functions;
typedef struct
{
    void (*TaskShelfEntry)( system_task_shelf_entry_id_t );
    void (*StateProfile)( system_subactivity_t );
} system_enstate_functions;

typedef struct
{
    void (*Init)(void);
    system_perform_functions Perform;
    system_register_functions Registers;
    system_enstate_functions Enstate;
} system_functions;

static system_functions SystemFunctions =
{
    .Init = InitSystemManager,
    .Perform.Routine = PerformSystemManagerRoutine,
    .Perform.Subactivities = PerformSystemManagerRoutineSubactivities,
    .Perform.Subactivity = PerformSystemManagerSubactivity,
    .Register.SubactivityMap = RegisterSystemManangerSubactivityMap,
    .Registers.Profile = RegisterSystemManagerProfile,
    .Registers.StateProfile = RegisterSystemManagerStateProfile,
    .Registers.State = RegisterSystemManagerState,
    .Registers.Action = RegisterSystemManagerAction,
    .Registers.Activity = RegisterSystemManagerActivity,
    .Registers.Subactivity = RegisterSystemManagerSubactivity,
    .Registers.Error = RegisterSystemManagerError,
    .Registers.Consumption = RegisterSystemManagerConsumption,
    .Enstate.TaskShelfEntry = EnstateSystemManagerTaskShelfEntry,
    .Enstate.StateProfile = EnstateSystemManagerStateProfile
};
#endif /* systemmanager_h */
