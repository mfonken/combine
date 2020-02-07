//
//  communicationmanager.c
//  C12_rtos
//
//  Created by Matthew Fonken on 8/13/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "communicationmanager.h"

void CommunicationManagerInit(void)
{
    
}

void CommunicationManager_PerformEvent( comm_event_t event )
{
    switch( event.generic_event.protocol )
    {
        case COMM_I2C:
            PAPI.I2C.Perform(*(i2c_event_t *)&event );
            break;
        case COMM_SPI:
            PAPI.SPI.Perform(*(spi_event_t *)&event );
            break;
        case COMM_UART:
            //            performUARTEvent(*(uart_event_t *)&event, data );
            break;
        case COMM_BLE:
            //            performBLEEvent(*(ble_event_t *)&event, data );
            break;
        case COMM_SUB:
            //            performSubEvent(*(sub_event_t *)&event, data );
            break;
        default:
            break;
    }
}

void CommunicationManager_PerformTransmit( comm_packet_t * packet )
{
    
}
void CommunicationManager_PerformReceive( comm_packet_t * packet )
{
    
}
