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
#include "globaltypes.h"

#define COMM_ADDR_NONE 0xff
typedef enum
{
    COMM_NONE = 0,
    COMM_I2C,
    COMM_SPI,
    COMM_UART,
    COMM_BLE,
    COMM_SUB
} COMM;
typedef enum
{
    COMM_CHANNEL_NONE = 0,
    COMM_CHANNEL_PRIMARY,
    COMM_CHANNEL_SECONDARY
} COMM_CHANNEL;
typedef enum
{
    COMM_READ_REG = 1,
    COMM_WRITE_REG
} COMM_TYPE;

typedef struct
{
uint8_t
    type:8;
uint8_t
    reg,
    length,
    addr;
uint8_t
    channel;
} comm_event_t;

typedef struct
{
    
} comm_packet_t;

void CommunicationManagerInit(void);
void PerformCommunicationManagerEvent( comm_event_t event, uint8_t * data );
void PerformCommunicationManagerTransmit(comm_packet_t *);
void PerformCommunicationManagerReceive(void);

typedef struct
{
    void (*Event)( comm_event_t, uint8_t * );
    void (*Transmit)(comm_packet_t *);
    void (*Receive)(void);
} comm_perform_functions;

typedef struct
{
    void (*Init)(void);
    comm_perform_functions Perform;
} comm_functions;

static comm_functions CommFunctions =
{
    .Init = CommunicationManagerInit,
    .Perform.Event = PerformCommunicationManagerEvent,
    .Perform.Transmit = PerformCommunicationManagerTransmit,
    .Perform.Receive = PerformCommunicationManagerReceive
};

#endif /* communicationmanager_h */
