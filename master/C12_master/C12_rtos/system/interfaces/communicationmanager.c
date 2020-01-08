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

void PerformCommunicationManagerEvent( comm_event_t event, uint8_t * data )
{
    switch( event.channel )
    {
        case COMM_I2C:
            performI2CEvent(*(i2c_event_t *)&event, data );
            break;
        case COMM_SPI:
            performSPIEvent(*(spi_event_t *)&event, data );
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

void PerformCommunicationManagerTransmit( comm_packet_t * packet )
{
    
}
void PerformCommunicationManagerReceive( comm_packet_t * packet )
{
    
}
