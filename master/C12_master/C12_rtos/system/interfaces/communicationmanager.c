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

generic_comm_return_t CommunicationManager_PerformEvent( comm_event_t event )
{
    generic_comm_return_t ret = false;
    switch( event.generic_comm_event.host->generic_comm_host.protocol )
    {
        case COMM_PROTOCOL_I2C:
            ret = (generic_comm_return_t)PAPI.I2C.Perform(*(i2c_event_t *)&event );
            break;
        case COMM_PROTOCOL_SPI:
            ret = (generic_comm_return_t)PAPI.SPI.Perform(*(spi_event_t *)&event );
            break;
        case COMM_PROTOCOL_UART:
            //            performUARTEvent(*(uart_event_t *)&event, data );
            break;
        case COMM_PROTOCOL_BLE:
            //            performBLEEvent(*(ble_event_t *)&event, data );
            break;
        case COMM_PROTOCOL_SUB:
            //            performSubEvent(*(sub_event_t *)&event, data );
            break;
        default:
            break;
    }
    return ret;
}

void CommunicationManager_PerformTransmit( comm_packet_t * packet )
{
    
}
void CommunicationManager_PerformReceive( comm_packet_t * packet )
{
    
}
