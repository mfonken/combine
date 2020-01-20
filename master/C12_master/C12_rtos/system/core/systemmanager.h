//
//  systemmanager.h
//  C12_rtos
//
//  Created by Matthew Fonken on 8/6/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef systemmanager_h
#define systemmanager_h

//#include "systemhandlers.h"
#include "os_interface.h"

#define DEFAULT_SYSTEM_STATE SYSTEM_STATE_IDLE
#define DEFAULT_SYSTEM_ACTION SYSTEM_ACTION_NONE
#define DEFAULT_SYSTEM_ACTIVITY SYSTEM_ACTIVITY_NONE
#define DEFAULT_SYSTEM_SUBACTIVITY SYSTEM_SUBACTIVITY_NONE
#define DEFAULT_SYSTEM_ERROR SYSTEM_ERROR_NONE
#define DEFAULT_SYSTEM_CONSUMPTION SYSTEM_CONSUMPTION_NONE
#define DEFAULT_SYSTEM_ACTIVITY SYSTEM_ACTIVITY_NONE

extern system_master_t System;

void SystemManager_Init(system_profile_t *);

void SystemManager_PerformRoutine( system_activity_routine_t * );
void SystemManager_PerformRoutineSubactivities( system_subactivity_t *, uint8_t );
void SystemManager_PerformSubactivity( system_subactivity_t );
void SystemManager_PerformEnableProfileEntryState( system_profile_entry_t * );
void SystemManager_PerformDisableProfileEntryState( system_profile_entry_t * );
void SystemManager_PerformExitState( void );

void SystemManager_RegisterTaskList( os_task_list_t * );
void SystemManager_RegisterTaskShelf( system_task_shelf_t * );
void SystemManager_RegisterSubactivityMap( system_subactivity_map_t * );
void SystemManager_RegisterProfile( system_profile_t * );
void SystemManager_RegisterProfileEntry( system_profile_entry_t *, bool );
void SystemManager_RegisterStateProfileList( system_state_profile_list_t * );
void SystemManager_RegisterState( system_state_t );
void SystemManager_RegisterExitState( system_state_t );
void SystemManager_RegisterActivity( system_activity_t );
void SystemManager_RegisterSubactivity( system_subactivity_t );
void SystemManager_RegisterError( system_error_t );
void SystemManager_RegisterConsumption( system_consumption_t );

system_subactivity_map_entry_t * SystemManager_GetSubactivityMapEntryById( system_subactivity_t );
system_task_shelf_entry_t * SystemManager_GetTaskShelfEntryById( system_task_shelf_entry_id_t );
os_task_data_t * SystemManager_GetTaskDataById( system_task_id_t );
os_task_data_t * SystemManager_GetTaskDataByComponentId( component_id_t );
int8_t SystemManager_GetSystemComponentNumber( component_id_t );
component_id_t SystemManager_GetComponentIdFromPortPin( port_t, pin_t );
//void_handler_t GetHandlerByComponent( component_t * );

void SystemManager_InstateTaskShelfEntry( system_task_shelf_entry_id_t );
void SystemManager_InstateStateProfile( system_state_profile_t * );

void SystemManager_TerminateTaskShelfEntry( system_task_shelf_entry_id_t );
void SystemManager_TerminateStateProfile( system_state_profile_t * );

typedef struct
{
    void (*Routine)( system_activity_routine_t * );
    void (*Subactivities)( system_subactivity_t *, uint8_t );
    void (*Subactivity)( system_subactivity_t );
    void (*EnableProfileEntry)( system_profile_entry_t * );
    void (*DisableProfileEntry)( system_profile_entry_t * );
    void (*ExitState)( void );
} system_perform_functions;
typedef struct
{
    void (*TaskList)( os_task_list_t * );
    void (*TaskShelf)( system_task_shelf_t * );
    void (*SubactivityMap)( system_subactivity_map_t * );
    void (*Profile)( system_profile_t * );
    void (*ProfileEntry)( system_profile_entry_t *, bool );
    void (*StateProfileList)( system_state_profile_list_t * );
    void (*State)( system_state_t );
    void (*ExitState)( system_state_t );
    void (*Activity)( system_activity_t );
    void (*Subactivity)( system_subactivity_t );
    void (*Error)( system_error_t );
    void (*Consumption)( system_consumption_t );
} system_register_functions;
typedef struct
{
    void (*TaskShelfEntry)( system_task_shelf_entry_id_t );
    void (*StateProfile)( system_state_profile_t * );
} system_instate_functions;

typedef struct
{
    system_subactivity_map_entry_t * (*SubactivityMapEntry)( system_subactivity_t );
    system_task_shelf_entry_t * (*TaskShelfEntry)( system_task_shelf_entry_id_t );
    os_task_data_t * (*TaskById)( system_task_id_t );
    os_task_data_t * (*TaskByComponent)( component_id_t );
//    void* (*HandlerByComponentId)( component_id_t );
    int8_t (*ComponentNumber)( component_id_t );
    component_id_t (*ComponentIdFromPortPin)( port_t, pin_t );
} system_get_functions;

typedef struct
{
    void (*Init)(system_profile_t *);
    system_perform_functions Perform;
    system_register_functions Register;
    system_instate_functions Instate, Terminate;
    system_get_functions Get;
} system_functions;

static system_functions SystemFunctions =
{
    .Init                       = SystemManager_Init,
    .Perform.Routine            = SystemManager_PerformRoutine,
    .Perform.Subactivities      = SystemManager_PerformRoutineSubactivities,
    .Perform.Subactivity        = SystemManager_PerformSubactivity,
    .Perform.EnableProfileEntry = SystemManager_PerformEnableProfileEntryState,
    .Perform.DisableProfileEntry = SystemManager_PerformDisableProfileEntryState,
    .Perform.ExitState          = SystemManager_PerformExitState,
    
    .Register.TaskList          = SystemManager_RegisterTaskList,
    .Register.TaskShelf         = SystemManager_RegisterTaskShelf,
    .Register.SubactivityMap    = SystemManager_RegisterSubactivityMap,
    .Register.Profile           = SystemManager_RegisterProfile,
    .Register.ProfileEntry      = SystemManager_RegisterProfileEntry,
    .Register.StateProfileList  = SystemManager_RegisterStateProfileList,
    .Register.State             = SystemManager_RegisterState,
    .Register.ExitState         = SystemManager_RegisterExitState,
    .Register.Activity          = SystemManager_RegisterActivity,
    .Register.Subactivity       = SystemManager_RegisterSubactivity,
    .Register.Error             = SystemManager_RegisterError,
    .Register.Consumption       = SystemManager_RegisterConsumption,
    .Instate.TaskShelfEntry     = SystemManager_InstateTaskShelfEntry,
    .Instate.StateProfile       = SystemManager_InstateStateProfile,
    
    .Terminate.TaskShelfEntry   = SystemManager_TerminateTaskShelfEntry,
    .Terminate.StateProfile     = SystemManager_TerminateStateProfile,
    
    .Get.SubactivityMapEntry    = SystemManager_GetSubactivityMapEntryById,
    .Get.TaskShelfEntry         = SystemManager_GetTaskShelfEntryById,
    .Get.TaskById               = SystemManager_GetTaskDataById,
    .Get.TaskByComponent        = SystemManager_GetTaskDataByComponentId,
    .Get.ComponentNumber        = SystemManager_GetSystemComponentNumber,
//    .Get.HandlerByComponentId   = GetHandlerByComponentId
    .Get.ComponentIdFromPortPin = SystemManager_GetComponentIdFromPortPin
};

#endif /* systemmanager_h */
