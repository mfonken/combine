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

#include "systembridge.h"

static system_master_t System;

void InitSystemManager(void);
void PerformRoutine( system_activity_routine_t * );
void PerformRoutineSubactivities( system_subactivity_t *, uint8_t );
void PerformSubactivity( system_subactivity_t );
void PerformSystemRoutine( const system_activity_routine_t * );

void RegisterSystemState( system_state_t );
void RegisterSystemAction( system_action_t );
void RegisterSystemActivity( system_activity_t );
void RegisterSystemSubactivity( system_subactivity_t );
void RegisterSystemError( system_error_t );
void RegisterSystemConsumption( system_consumption_t );

#endif /* systemmanager_h */
