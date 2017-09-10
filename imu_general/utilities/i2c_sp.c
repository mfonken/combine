/***********************************************************************************************//**
* \file   i2c_sp.c
* \brief  I2C Special Functions
***************************************************************************************************
*      Author: Matthew Fonken
**************************************************************************************************/

/* Standard headers */
#include <stdbool.h>

/* Own header */
#include "i2c_sp.h"

const char * I2C_DEV_FILE_NAME = "/dev/i2c-0";

int file = -1;

bool I2C_Init()
{
    if( file != -1 )
    {
        int file = open(I2C_DEV_FILE_NAME, O_RDWR);
        if (file == -1)
        {
            perror(I2C_DEV_FILE_NAME);
            exit(1);
        }
        return 1;
    }
    else
    {
        perror("I2C is already initialized");
        return 0;
    }
        
}

bool I2C_Deinit( )
{
    return close(file);
}

bool I2C_Read( uint8_t addr, uint8_t *i2c_read_data, uint8_t i2c_read_data_length)
{
    ioctl(file, I2C_SLAVE, addr);
    return read(file,i2c_read_data,i2c_read_data_length);
}

bool I2C_Read_Reg( uint8_t addr, uint8_t reg, uint8_t *i2c_read_data, uint8_t i2c_read_data_length)
{
    ioctl(file, I2C_SLAVE, addr);
    if( write(file, &reg, 1) != 1 ) return 0;
    return read(file, i2c_read_data, i2c_read_data_length);
}

bool I2C_Write( uint8_t addr, uint8_t *i2c_write_data, uint8_t i2c_write_data_length)
{
    ioctl(file, I2C_SLAVE, addr);
    return write(file, i2c_write_data, i2c_write_data_length);
}

