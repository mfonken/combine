//
//  papi_interface.c
//  C12_rtos
//
//  Created by Matthew Fonken on 2/1/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#include "papi_interface.h"

#define I2C_SERVICE

#ifndef DCDC_SERVICE
void PAPIInterface_DCDC_Init( uint16_t mV ) {}
#else
void PAPIInterface_DCDC_Init( uint16_t mV )
{
    LOG_PAPI(PAPI_DEBUG, "Starting DCDC at %.3fV.\n", (double)mV / 1000. );
    PAPI_SPECIFIC(PAPIInterface_DCDC_Init)(mV);
}
#endif

#ifndef I2C_SERVICE
bool PAPIInterface_I2C_Init( i2c_event_t * event ) { return true; }
void PAPIInterface_I2C_Enable( i2c_event_t * event ) {}
i2c_transfer_return_t PAPIInterface_I2C_Read( i2c_event_t * event ) { return false; }
i2c_transfer_return_t PAPIInterface_I2C_Write( i2c_event_t * event ) { return false; }
i2c_transfer_return_t PAPIInterface_I2C_Perform( i2c_event_t event ) { return false; }
#else
bool PAPIInterface_I2C_Init( i2c_event_t * event )
{
    LOG_PAPI(PAPI_DEBUG, "Initializing I2C on channel %p:0x%02x\n", event->host->device, event->host->address);
    return PAPI_SPECIFIC(PAPIInterface_I2C_Init)(event);
}

void PAPIInterface_I2C_Enable( i2c_event_t * event )
{
    LOG_PAPI(PAPI_DEBUG, "Enabling I2C on channel %p:0x%02x\n", event->host->device, event->host->address);
    PAPI_SPECIFIC(PAPIInterface_I2C_Enable)(event);
}

i2c_transfer_return_t PAPIInterface_I2C_Read( i2c_event_t * event )
{
    LOG_PAPI(PAPI_DEBUG, "Reading I2C on channel %p:0x%02x:0x%02x\n", event->host->device, event->host->address, event->reg);
    i2c_transfer_return_t ret;
    if( event->length == 1 && event->reg != NO_REG)
        ret = PAPI_SPECIFIC(PAPIInterface_I2C_ReadRegister)(event);
    else
        ret = PAPI_SPECIFIC(PAPIInterface_I2C_Read)(event);
    return ret;
}

i2c_transfer_return_t PAPIInterface_I2C_Write( i2c_event_t * event )
{
    LOG_PAPI(PAPI_DEBUG, "Writing I2C on channel %p:0x%02x:0x%02x\n", event->host->device, event->host->address, event->reg);
    i2c_transfer_return_t ret;
    if( event->length == 1 && event->reg != NO_REG)
        ret = PAPI_SPECIFIC(PAPIInterface_I2C_WriteRegister)(event);
    else
        ret = PAPI_SPECIFIC(PAPIInterface_I2C_Write)(event);
    return ret;
}

i2c_transfer_return_t PAPIInterface_I2C_Perform( i2c_event_t event )
{
    LOG_PAPI(PAPI_DEBUG, "Performing I2C event on channel %p:0x%02x\n", event.host->device, event.host->address);
    switch(event.command)
    {
        case I2C_READ_REG_EVENT:
            return PAPI.I2C.Read( &event );
            break;
        case I2C_WRITE_REG_EVENT:
            return PAPI.I2C.Write( &event );
            break;
        default:
            break;
    }
    return false;
}
#endif

#ifndef SPI_SERVICE
/// TODO look into "weak" across compilers
bool PAPIInterface_SPI_Init( spi_event_t * event ) { return true; }
void PAPIInterface_SPI_Enable( spi_event_t * event ) {}
spi_transfer_return_t PAPIInterface_SPI_Read( spi_event_t * event ) { return false; }
spi_transfer_return_t PAPIInterface_SPI_Write( spi_event_t * event ) { return false; }
spi_transfer_return_t PAPIInterface_SPI_Perform( spi_event_t event ) { return false; }
#else
bool PAPIInterface_SPI_Init( spi_event_t * event )
{
    LOG_PAPI(PAPI_DEBUG, "Initializing SPI on channel %d\n", event->host.device);
    return PAPI_SPECIFIC(PAPIInterface_SPI_Init)(event);
}

void PAPIInterface_SPI_Enable( spi_event_t * event )
{
    LOG_PAPI(PAPI_DEBUG, "Enabling SPI on channel %d\n", event->host.device);
    PAPI_SPECIFIC(PAPIInterface_SPI_Enable)(event);
}

spi_transfer_return_t PAPIInterface_SPI_Read( spi_event_t * event )
{
    LOG_PAPI(PAPI_DEBUG, "Reading SPI on channel %d\n", event->host.device);
    return PAPI_SPECIFIC(PAPIInterface_SPI_Read)(event);
}

spi_transfer_return_t PAPIInterface_SPI_Write( spi_event_t * event )
{
    LOG_PAPI(PAPI_DEBUG, "Writing SPI on channel %d\n", event->host.device);
    return PAPI_SPECIFIC(PAPIInterface_SPI_Write)(event);
}

spi_transfer_return_t PAPIInterface_SPI_Perform( spi_event_t event )
{
    LOG_PAPI(PAPI_DEBUG, "Performing SPI event on channel %d\n", event->host.device);
    switch(event.type)
    {
        case I2C_READ_REG_EVENT:
            return PAPI.I2C.Read( &event );
            break;
        case I2C_WRITE_REG_EVENT:
            return PAPI.I2C.Write( &event );
            break;
        default:
            break;
    }
    return false;
}
#endif

#ifndef USART_SERVICE
#else
#endif

#ifndef LEUART_SERVICE
#else
#endif
