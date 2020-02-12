//
//  papi_interface.c
//  C12_rtos
//
//  Created by Matthew Fonken on 2/1/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#include "papi_interface.h"

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
void PAPIInterface_I2C_Init( i2c_event_t * data ) {}
void PAPIInterface_I2C_Enable( i2c_event_t * data ) {}
i2c_transfer_return_t PAPIInterface_I2C_Read( i2c_event_t * data ) { return false; }
i2c_transfer_return_t PAPIInterface_I2C_Write( i2c_event_t * data ) { return false; }
i2c_transfer_return_t PAPIInterface_I2C_Perform( i2c_event_t event ) { return false; }
#else
void PAPIInterface_I2C_Init( i2c_event_t * data )
{
    LOG_PAPI(PAPI_DEBUG, "Initializing I2C on channel \n");
    PAPI_SPECIFIC(PAPIInterface_I2C_Init)(data);
}

void PAPIInterface_I2C_Enable( i2c_event_t * data )
{
    LOG_PAPI(PAPI_DEBUG, "Enabling I2C on channel \n");
    PAPI_SPECIFIC(PAPIInterface_I2C_Enable)(data);
}

i2c_transfer_return_t PAPIInterface_I2C_Read( i2c_event_t * data )
{
    LOG_PAPI(PAPI_DEBUG, "Reading I2C on channel \n");
    i2c_transfer_return_t ret;
    if( data->length == 1 && data->reg != NO_REG)
        ret = PAPI_SPECIFIC(PAPIInterface_I2C_ReadRegister)(data);
    else
        ret = PAPI_SPECIFIC(PAPIInterface_I2C_Read)(data);
    return ret;
}

i2c_transfer_return_t PAPIInterface_I2C_Write( i2c_event_t * data )
{
    LOG_PAPI(PAPI_DEBUG, "Writing I2C on channel \n");
    i2c_transfer_return_t ret;
    if( data->length == 1 && data->reg != NO_REG)
        ret = PAPI_SPECIFIC(PAPIInterface_I2C_WriteRegister)(data);
    else
        ret = PAPI_SPECIFIC(PAPIInterface_I2C_Write)(data);
    return ret;
}

i2c_transfer_return_t PAPIInterface_I2C_Perform( i2c_event_t event )
{
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

#ifndef SPI_SERVICE
/// TODO look into "weak" across compilers
void PAPIInterface_SPI_Init( spi_event_t * data ) {}
void PAPIInterface_SPI_Enable( spi_event_t * data ) {}
spi_transfer_return_t PAPIInterface_SPI_Read( spi_event_t * data ) { return false; }
spi_transfer_return_t PAPIInterface_SPI_Write( spi_event_t * data ) { return false; }
spi_transfer_return_t PAPIInterface_SPI_Perform( spi_event_t event ) { return false; }
#else
void PAPIInterface_SPI_Init( spi_event_t * data )
{
    
}

void PAPIInterface_SPI_Enable( spi_event_t * data )
{
    
}

spi_transfer_return_t PAPIInterface_SPI_Read( spi_event_t * data )
{
    return false;
}

spi_transfer_return_t PAPIInterface_SPI_Write( spi_event_t * data )
{
    return false;
}

spi_transfer_return_t PAPIInterface_SPI_Perform( spi_event_t event )
{
//    switch(event.type)
//    {
//        case I2C_READ_REG_EVENT:
//            return PAPI.I2C.Read( &event );
//            break;
//        case I2C_WRITE_REG_EVENT:
//            return PAPI.I2C.Write( &event );
//            break;
//        default:
//            break;
//    }
    return false;
}
#endif

#ifndef USART_SERVICE
#else
#endif

#ifndef LEUART_SERVICE
#else
#endif
