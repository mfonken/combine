//
//  papi_interface.h
//  C12_rtos
//
//  Created by Matthew Fonken on 2/1/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#ifndef papi_interface_h
#define papi_interface_h

#include "timestamp.h"

void PAPIInterface_DCDC_Init( uint16_t );

void PAPIInterace_GPIO_Set( gpio_t );
void PAPIInterace_GPIO_Clear( gpio_t );

bool PAPIInterface_I2C_Init( i2c_event_t * );
void PAPIInterface_I2C_Enable( i2c_event_t * );
void PAPIInterface_I2C_Disable( i2c_event_t * );
i2c_transfer_return_t PAPIInterface_I2C_Read( i2c_event_t * );
i2c_transfer_return_t PAPIInterface_I2C_Write( i2c_event_t * );
i2c_transfer_return_t PAPIInterface_I2C_Perform( i2c_event_t );

bool PAPIInterface_SPI_Init( spi_event_t * );
void PAPIInterface_SPI_Enable( spi_event_t * );
void PAPIInterface_SPI_Disable( spi_event_t * );
spi_transfer_return_t PAPIInterface_SPI_Read( spi_event_t * );
spi_transfer_return_t PAPIInterface_SPI_Write( spi_event_t * );
spi_transfer_return_t PAPIInterface_SPI_Perform( spi_event_t );

typedef struct
{
    void (*InitDCDC)( uint16_t );
} system_papi_interface_energy_functions;

typedef struct
{
    void (*Set)( gpio_t );
    void (*Clear)( gpio_t );
} system_papi_interface_gpio_functions;

typedef struct
{
    bool (*Init)( i2c_event_t * );
    void (*Enable)( i2c_event_t * );
    void (*Disable)( i2c_event_t * );
    i2c_transfer_return_t (*Read)( i2c_event_t * );
    i2c_transfer_return_t (*Write)( i2c_event_t * );
    i2c_transfer_return_t (*Perform)( i2c_event_t );
} system_papi_interface_i2c_functions;

typedef struct
{
    bool (*Init)( spi_event_t * );
    void (*Enable)( spi_event_t * );
    void (*Disable)( spi_event_t * );
    spi_transfer_return_t (*Read)( spi_event_t * );
    spi_transfer_return_t (*Write)( spi_event_t * );
    spi_transfer_return_t (*Perform)( spi_event_t );
} system_papi_interface_spi_functions;

typedef struct
{
    system_papi_interface_energy_functions Energy;
    system_papi_interface_gpio_functions GPIO;
    system_papi_interface_i2c_functions I2C;
    system_papi_interface_spi_functions SPI;
} system_papi_interface_functions;

static system_papi_interface_functions PAPI =
{
    .Energy.InitDCDC = PAPIInterface_DCDC_Init,
    
    .GPIO.Set = PAPIInterace_GPIO_Set,
    .GPIO.Clear = PAPIInterace_GPIO_Clear,
    
    .I2C.Init = PAPIInterface_I2C_Init,
    .I2C.Enable = PAPIInterface_I2C_Enable,
    .I2C.Read = PAPIInterface_I2C_Read,
    .I2C.Write = PAPIInterface_I2C_Write,
    .I2C.Perform = PAPIInterface_I2C_Perform,
    
    .SPI.Init = PAPIInterface_SPI_Init,
    .SPI.Enable = PAPIInterface_SPI_Enable,
    .SPI.Read = PAPIInterface_SPI_Read,
    .SPI.Write = PAPIInterface_SPI_Write,
    .SPI.Perform = PAPIInterface_SPI_Perform
};

#endif /* papi_interface_h */
