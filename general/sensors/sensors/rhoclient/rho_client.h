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

#include "communicationmanager.h"

#define RHO_DEFAULT_COMM_CHANNEL COMM_UART

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
    ///
} rho_setting_t;

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
} rho_packet_t;

typedef struct
{
uint8_t
    ID;
rho_packet_t
    packet;
rho_setting_t
    settings;
} rho_t;

void InitRho( rho_setting_t * );
void SendRhoSetting( rho_setting_t * );
void ReceiveRhoPacket( rho_t * );

typedef struct
{
    void (*Init)( rho_setting_t * );
    void (*Send)( rho_setting_t * );
    void (*Receive)( rho_t * );
    comm_event_t (*GetSendEvent)(void);
    comm_event_t (*GetReceiveEvent)(void);
} rho_functions;

static comm_event_t RhoGetSendEvent(void) { return (comm_event_t){ COMM_WRITE_REG, NO_REG, sizeof(rho_setting_t), RHO_DEFAULT_COMM_CHANNEL }; }
static comm_event_t RhoGetReceiveEvent(void) { return (comm_event_t){ COMM_READ_REG, NO_REG, sizeof(rho_packet_t), RHO_DEFAULT_COMM_CHANNEL }; }

static rho_functions RhoFunctions =
{
    .Init = InitRho,
    .Send = SendRhoSetting,
    .Receive = ReceiveRhoPacket,
    .GetSendEvent = RhoGetSendEvent,
    .GetReceiveEvent = RhoGetReceiveEvent
};

static void RhoPointToKPoint( rho_point * r, kpoint_t * k )
{
    k->x = r->a;
    k->y = r->b;
}

#endif /* rho_client_h */
