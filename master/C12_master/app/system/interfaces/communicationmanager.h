//
//  communicationmanager.h
//  C12_rtos
//
//  Created by Matthew Fonken on 8/13/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef communicationmanager_h
#define communicationmanager_h

#include "systembehavior.h"

typedef struct
{
    comm_host_t *
        host;
    uint8_t
        reg,
        length,
        *buffer;
} generic_comm_event_t;

typedef union
{
    generic_comm_event_t generic_comm_event;
    i2c_event_t i2c_event;
    spi_event_t spi_event;
} comm_event_t;

typedef struct
{
    uint8_t tmp;
} comm_packet_t;
typedef comm_packet_t * p_comm_packet_t;

void CommunicationManager_Init(void);
generic_comm_return_t CommunicationManager_PerformEvent( comm_event_t );
void CommunicationManager_PerformTransmit(comm_packet_t *);
void CommunicationManager_PerformReceive(comm_packet_t *);

typedef struct
{
    generic_comm_return_t (*Event)( comm_event_t );
    void (*Transmit)(comm_packet_t *);
    void (*Receive)(comm_packet_t *);
} comm_perform_functions;

typedef struct
{
    void (*Init)(void);
    comm_perform_functions Perform;
} comm_functions;

static const comm_functions CommFunctions =
{
    .Init = CommunicationManager_Init,
    .Perform.Event = CommunicationManager_PerformEvent,
    .Perform.Transmit = CommunicationManager_PerformTransmit,
    .Perform.Receive = CommunicationManager_PerformReceive
};

#endif /* communicationmanager_h */
