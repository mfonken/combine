/***********************************************************************************************//**
 * \file   i2c_sp.h
 * \brief  I2C Special Functions Header
 ***************************************************************************************************
 *      Author: Matthew Fonken
 **************************************************************************************************/

#ifndef SYSTEM_I2C_SP_H_
#define SYSTEM_I2C_SP_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

#ifdef NONRASPI
#include "../tests/test_stubs.h"
#else
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#endif

extern int file;

bool I2C_Init( void );
bool I2C_Deinit( void );
bool I2C_Read( uint8_t addr, uint8_t *i2c_read_data, uint8_t i2c_read_data_length);
bool I2C_Read_Reg( uint8_t addr, uint8_t reg, uint8_t *i2c_read_data, uint8_t i2c_read_data_length);
bool I2C_Write( uint8_t addr, uint8_t *i2c_write_data, uint8_t i2c_write_data_length);

#ifdef __cplusplus
}
#endif
    
#endif /* SYSTEM_I2C_SP_H_ */
