//
//  rho_client.c
//  C11_rtos
//
//  Created by Matthew Fonken on 8/5/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_client.h"

void ReceiveRhoPacket( rho_t * rho )
{
    PerformCommEvent(RhoFunctions.GetReceiveEvent(), (uint8_t*)&rho->packet);
}
