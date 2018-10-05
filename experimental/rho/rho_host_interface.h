//
//  rho_host_interface.h
//  rho_client
//
//  Created by Matthew Fonken on 9/29/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_host_interface_h
#define rho_host_interface_h

#include "rho_types.h"
#include "stm32_interface.h"

static inline bool TransmitPacket( packet_t * packet )
{
    return (bool)STMUartTxDMA( packet, sizeof(packet_t));
}

static inline uint16_t ReceivePacket( packet_t * packet )
{
    return STMUartRxDMA( packet );
}

#endif /* rho_host_interface_h */
