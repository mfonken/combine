//
//  rho_client.c
//  C12_rtos
//
//  Created by Matthew Fonken on 8/5/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_client.h"

void InitRho( comm_host_t * p_host, rho_setting_t * p_setting )
{
    SendRhoSetting( p_host, p_setting );
}

void SendRhoSetting( comm_host_t * p_host, rho_setting_t * p_setting )
{
    PerformCommEvent( RhoFunctions.GetSendEvent(p_host, p_setting) );
}

void ReceiveRhoPacket( comm_host_t * p_host, rho_t * p_rho )
{
    PerformCommEvent( RhoFunctions.GetReceiveEvent(p_host, &p_rho->packet) );
}
