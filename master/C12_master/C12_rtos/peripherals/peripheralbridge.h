//
//  peripheralbridge.h
//  C12_rtos
//
//  Created by Matthew Fonken on 9/9/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef peripheralbridge_h
#define peripheralbridge_h

#include "globaltypes.h"
#include "communicationmanager.h"

static void (*PerformCommEvent)( comm_event_t, uint8_t * ) = PerformCommunicationManagerEvent;

#endif /* peripheralbridge_h */
