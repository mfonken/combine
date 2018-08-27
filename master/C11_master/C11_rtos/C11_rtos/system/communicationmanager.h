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
    COMM_UART
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

typedef struct
{
    void (*Init)(void);
    void (*Transmit)(comm_packet_t *);
    void (*Receive)(void);
} comm_functions;

void CommunicationManagerInit(void);
void CommunicationManagerTransmit(comm_packet_t *);
void CommunicationManagerReceive(void);

static comm_functions CommFunctions =
{
    .Init = CommunicationManagerInit,
    .Transmit = CommunicationManagerTransmit,
    .Receive = CommunicationManagerReceive
};

static void PerformCommEvent( comm_event_t event, uint8_t * data )
{
    switch( event.channel )
    {
        case COMM_I2C:
            performI2CEvent(*(i2c_event_t *)&event, data );
            break;
        case COMM_SPI:
            performSPIEvent(*(spi_event_t *)&event, data );
            break;
        case COMM_UART:
            //            performUARTEvent(*(uart_event_t *)&event, data );
            break;
        default:
            break;
    }
}

#endif /* communicationmanager_h */
