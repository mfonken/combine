//
//  rho_client.h
//  C11_rtos
//
//  Created by Matthew Fonken on 8/5/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_client_h
#define rho_client_h

#include <stdio.h>
#include <stdint.h>

#include "systemtypes.h"
#include "global.h"

#define RHO_DEFAULT_COMM_CHANNEL SYSTEM_COMM_UART

typedef uint16_t packet_data_type;
typedef float global_timestamp_t;
typedef struct
{
uint8_t
    ID,
    includes,
    timestamp[sizeof(global_timestamp_t)];
} global_packet_header;

typedef struct
{
packet_data_type
    a,
    b;
} rho_point;

typedef struct
{
packet_data_type
    primary,
    secondary,
    alternate;
} rho_probabilites;

typedef struct
{
global_packet_header
    header;
rho_point
    primary,
    secondary;
rho_probabilites
    probabilites;
} rho_packet;

typedef struct
{
uint8_t
    ID;
rho_packet
    packet;
} rho_t;

void ReceiveRhoPacket( rho_t * );

typedef struct
{
    void (*Receive)( rho_t * );
    comm_event_t (*GetReceiveEvent)(void);
} rho_functions;

static comm_event_t RhoGetReceiveEvent(void) { return (comm_event_t){ SYSTEM_COMM_READ_REG, NO_REG, sizeof(rho_packet), RHO_DEFAULT_COMM_CHANNEL }; }

static rho_functions RhoFunctions =
{
    .Receive = ReceiveRhoPacket,
    .GetReceiveEvent = RhoGetReceiveEvent
};

#endif /* rho_client_h */
