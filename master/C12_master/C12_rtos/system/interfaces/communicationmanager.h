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

/* Port spoof */
#define PORT0 0
#define PORTA 0
#define PORTB 0
#define PORTC 0
#define PORTD 0
#define PORTF 0

#define INTERNAL 0xff

#define COMM_PORT_NONE 0xff
#define COMM_ADDR_NONE 0xff

typedef enum
{
    COMM_NONE = 0,
    COMM_I2C,
    COMM_SPI,
    COMM_UART,
    COMM_BLE,
    COMM_SUB
} COMM_PROTOCOL, comm_protocol;

typedef enum
{
    COMM_ROUTE_NONE = 0,
    COMM_ROUTE_PRIMARY,
    COMM_ROUTE_SECONDARY
} COMM_ROUTE;

typedef enum
{
    COMM_READ_REG = 1,
    COMM_WRITE_REG
} COMM_TYPE;

//typedef enum
//{
//    COMM_CHAN_I2C = 0x01,
//    COMM_CHAN_SPI = 0x02
//} comm_channel;

typedef struct
{
    comm_protocol
        protocol:8;
    uint8_t
        reg,
        addr,
        length,
        *buffer;
} generic_comm_event_t;

typedef union
{
    i2c_event_t i2c_event;
    spi_event_t spi_event;
    generic_comm_event_t generic_event;
} comm_event_t;

typedef struct
{
    
} comm_packet_t;

void CommunicationManagerInit(void);
void CommunicationManager_PerformEvent( comm_event_t );
void CommunicationManager_PerformTransmit(comm_packet_t *);
void CommunicationManager_PerformReceive(comm_packet_t *);

typedef struct
{
    void (*Event)( comm_event_t );
    void (*Transmit)(comm_packet_t *);
    void (*Receive)(comm_packet_t *);
} comm_perform_functions;

typedef struct
{
    void (*Init)(void);
    comm_perform_functions Perform;
} comm_functions;

static comm_functions CommFunctions =
{
    .Init = CommunicationManagerInit,
    .Perform.Event = CommunicationManager_PerformEvent,
    .Perform.Transmit = CommunicationManager_PerformTransmit,
    .Perform.Receive = CommunicationManager_PerformReceive
};

#endif /* communicationmanager_h */
