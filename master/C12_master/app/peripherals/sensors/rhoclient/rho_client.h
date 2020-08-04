//
//  rho_client.h
//  C12_rtos
//
//  Created by Matthew Fonken on 8/5/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_client_h
#define rho_client_h

#include "../../../application/tau/kinetic/qmath/point.h"
#include "peripheralbridge.h"

#define RHO_DEFAULT_COMPONENT_PROTOCOL COMPONENT_PROTOCOLUART

typedef floating_t packet_data_type;
typedef float global_timestamp_t;
typedef struct
{
generic_id_t
    ID;
uint8_t
    includes,
    timestamp[sizeof(global_timestamp_t)];
} global_packet_header;

typedef struct
{
generic_id_t
    ID;
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

static void rho_get_confidence( rho_t * rho, floating_t confidence[3] )
{
    confidence[0] = (floating_t)rho->packet.probabilites.primary;
    confidence[1] = (floating_t)rho->packet.probabilites.secondary;
    confidence[2] = (floating_t)rho->packet.probabilites.alternate;
}

void InitRho( comm_host_t *, rho_setting_t * );
void SendRhoSetting( comm_host_t *, rho_setting_t * );
void ReceiveRhoPacket( comm_host_t *, rho_t * );

typedef struct
{
    void (*Init)( comm_host_t *, rho_setting_t * );
    void (*Send)( comm_host_t *, rho_setting_t * );
    void (*Receive)( comm_host_t *, rho_t * );
    comm_event_t (*GetSendEvent)( comm_host_t *, rho_setting_t * );
    comm_event_t (*GetReceiveEvent)( comm_host_t *, rho_packet_t * );
} rho_functions;

static comm_event_t RhoGetSendEvent( comm_host_t * p_host, rho_setting_t * p_settings ) { return (comm_event_t){ p_host, COMM_WRITE_REG, (uint8_t)sizeof(rho_setting_t), (uint8_t *)p_settings }; }
static comm_event_t RhoGetReceiveEvent( comm_host_t * p_host, rho_packet_t * p_packet ) { return (comm_event_t){ p_host, COMM_READ_REG, (uint8_t)sizeof(rho_packet_t), (uint8_t *)p_packet }; }

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
