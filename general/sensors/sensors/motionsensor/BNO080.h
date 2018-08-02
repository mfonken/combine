//
//  BNO080.h
//  sensors
//
//  Created by Matthew Fonken on 7/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef BNO080_h
#define BNO080_h

#include "shtp.h"
#include "i2c_template.h"

#define BNO080_DEFAULT_ADDRESS 0x4B
#define BNO080_UART_RVC_HEADER 0xAAAA

static shtp_client_t bno;

static void Enable_Rotation_Vector( uint32_t interval_ms )
{
    SHTPFunctions.SetActiveClient( &bno );
    SHTPFunctions.SetRotationVector( interval_ms );
}

#endif /* BNO080_h */
