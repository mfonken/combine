/***********************************************************************************************//**
 * \file   i2c_sp.h
 * \brief  I2C Special Functions Header
 ***************************************************************************************************
 *      Author: Matthew Fonken
 **************************************************************************************************/

#ifndef SYSTEM_I2C_SP_H_
#define SYSTEM_I2C_SP_H_

#include <stdbool.h>

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup sp
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 *  \brief  Read I2C
 *  \param[out] Return read success
 *  \param[in] addr Address of slave
 *  \param[in] i2c_read_data Array to store read data in
 *  \param[in] i2c_read_data_length Number of bytes to read
 **************************************************************************************************/
bool I2C_Read( uint8_t addr, uint8_t *i2c_read_data, uint8_t i2c_read_data_length);

/***********************************************************************************************//**
 *  \brief  Read I2C
 *  \param[out] Return read success
 *  \param[in] addr Address of slave
 *  \param[in] reg Register to access
 *  \param[in] i2c_read_data Array to store read data in
 *  \param[in] i2c_read_data_length Number of bytes to read
 **************************************************************************************************/
bool I2C_Read_Reg( uint8_t addr, uint8_t reg, uint8_t *i2c_read_data, uint8_t i2c_read_data_length);

/***********************************************************************************************//**
 *  \brief  Write I2C
 *  \param[out] Return read success
 *  \param[in] addr Address of slave
 *  \param[in] i2c_write_data Array to write
 *  \param[in] i2c_read_data_length Number of bytes to write
 **************************************************************************************************/
bool I2C_Write( uint8_t addr, uint8_t *i2c_write_data, uint8_t i2c_write_data_length);

/** @} (end addtogroup sp) */
/** @} (end addtogroup Application) */

#endif /* SYSTEM_I2C_SP_H_ */
