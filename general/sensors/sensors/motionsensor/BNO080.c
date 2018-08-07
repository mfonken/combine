//
//  BNO080.c
//  sensors
//
//  Created by Matthew Fonken on 7/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "BNO080.h"

bool BNO080EnableFeature( shtp_client_t * client, bno080_feature_t report_id, uint32_t interval_ms, uint32_t specific )
{
    SHTPFunctions.SetActiveClient( client );
    SHTPFunctions.SendFeatureCommand( report_id, interval_ms, specific );
    return true;
}

bool BNO080DisableFeature( shtp_client_t * client, bno080_feature_t report_id )
{
    SHTPFunctions.SetActiveClient( client );
    SHTPFunctions.SendFeatureCommand( report_id, 0, 0 );
    return true;
}

bool BNO080Refresh( shtp_client_t * client )
{
    SHTPFunctions.SetActiveClient( client );
    if(BNO080Functions.AttemptGet( BNO080_DEFAULT_GET_TIMEOUT ) )
    {
        return true;
    }
    else
    {
        
    }
    return false;
}

bool BNO080AttemptGet( uint32_t timeout )
{
    uint32_t timestart = timestamp();
    while(!ReceiveSHTPPacket())
        if(istimedout(timestart, timeout)) return false;
    return true;
}
