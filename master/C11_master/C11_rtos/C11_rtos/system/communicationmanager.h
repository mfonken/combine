//
//  communicationmanager.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/13/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef communicationmanager_h
#define communicationmanager_h

#include <stdio.h>
#include "taumanager.h"

typedef struct
{
    
} comm_packet_t;

typedef struct
{
    void (*Init)(void);
    void (*Transmit)(comm_packet_t *);
    void (*Receive)(comm_packet_t *);
} comm_functions;

void CommuncationManagerInit(void);
void CommunicationManagerTransmit(comm_packet_t *);
void CommunicationManagerReceive(comm_packet_t *);

static comm_functions CommFunctions =
{
    .Init = CommunicationManagerInit,
    .Transmit = CommunicationManagerTransmit,
    .Receive = CommunicationManagerReceive
};

#endif /* communicationmanager_h */
