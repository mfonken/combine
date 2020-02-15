//
//  rho_client.c
//  C12_rtos
//
//  Created by Matthew Fonken on 8/5/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_client.h"

void InitRho( comm_host_t * host, rho_setting_t * setting )
{
    SendRhoSetting( host, setting );
}

void SendRhoSetting( comm_host_t * host, rho_setting_t * setting )
{
    PerformCommEvent( RhoFunctions.GetSendEvent(host, setting) );
}

void ReceiveRhoPacket( comm_host_t * host, rho_t * rho )
{
    PerformCommEvent( RhoFunctions.GetReceiveEvent(host, &rho->packet) );
}
