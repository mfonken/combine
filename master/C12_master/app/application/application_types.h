//
//  application_types.h
//  C12_rtos
//
//  Created by Matthew Fonken on 1/18/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#ifndef application_types_h
#define application_types_h

#include "global_types.h"

#define SUB_VREG_MV 2200
#define ACTIVE      (1)
#define INACTIVE    (0)

#define INTERRUPT_CHANNEL QUEUE_ID_HW_INTERRUPTS
#ifndef COMPONENT_FAMILY_0
#define APPLICATION_COMPONENT_FAMILY_0 -1
#endif

#define APPLICATION_COMPONENT_NONE COMPONENT_ID_NONE
#define APPLICATION_ACTION_ID_NONE TASK_ACTION_NONE
#define APPLICATION_TASK_SHELF_ENTRY_ID_NONE TASK_SHELF_ID_NONE

typedef application_comm_protocol_t comm_protocol;

#endif /* application_types_h */
