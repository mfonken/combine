//
//  peripheralbridge.h
//  C12_rtos
//
//  Created by Matthew Fonken on 9/9/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef peripheralbridge_h
#define peripheralbridge_h

#include "communicationmanager.h"

static generic_comm_return_t (*PerformCommEvent)( comm_event_t ) = CommunicationManager_PerformEvent;
static i2c_transfer_return_t (*PerformI2CEvent)( i2c_event_t ) = PAPIInterface_I2C_Perform;
static spi_transfer_return_t (*PerformSPIEvent)( spi_event_t ) = PAPIInterface_SPI_Perform;

#endif /* peripheralbridge_h */
