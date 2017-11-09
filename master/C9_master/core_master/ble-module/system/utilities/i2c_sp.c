/***********************************************************************************************//**
 * \file   i2c_sp.c
 * \brief  I2C Special Functions
 ***************************************************************************************************
 *      Author: Matthew Fonken
 **************************************************************************************************/

/* Standard headers */
#include <stdbool.h>

/* em headers */
#include "em_i2c.h"

/* Own header */
#include "i2c_sp.h"

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
bool I2C_Read( uint8_t addr, uint8_t *i2c_read_data, uint8_t i2c_read_data_length)
{
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;

	seq.addr  = addr;
	seq.flags = I2C_FLAG_READ;
	/* Select location/length of data to be read */
	seq.buf[0].data = i2c_read_data;
	seq.buf[0].len  = i2c_read_data_length;

	ret = I2C_TransferInit(I2C0, &seq);
	while (ret == i2cTransferInProgress)
	{
		ret = I2C_Transfer(I2C0);
	}
	return ret;
}

/***********************************************************************************************//**
 *  \brief  Read I2C
 *  \param[out] Return read success
 *  \param[in] addr Address of slave
 *  \param[in] reg Register to access
 *  \param[in] i2c_read_data Array to store read data in
 *  \param[in] i2c_read_data_length Number of bytes to read
 **************************************************************************************************/
bool I2C_Read_Reg( uint8_t addr, uint8_t reg, uint8_t *i2c_read_data, uint8_t i2c_read_data_length)
{
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;

	seq.addr  = addr;
	seq.flags = I2C_FLAG_WRITE_READ;
	/* Select command to issue */
	seq.buf[0].data   = &reg;
	seq.buf[0].len    = 1;
	/* Select location/length of data to be read */
	seq.buf[1].data = i2c_read_data;
	seq.buf[1].len  = i2c_read_data_length;

	ret = I2C_TransferInit(I2C0, &seq);
	while (ret == i2cTransferInProgress)
	{
		ret = I2C_Transfer(I2C0);
	}
	return ret;
}

/***********************************************************************************************//**
 *  \brief  Write I2C
 *  \param[out] Return read success
 *  \param[in] addr Address of slave
 *  \param[in] i2c_write_data Array to write
 *  \param[in] i2c_read_data_length Number of bytes to write
 **************************************************************************************************/
bool I2C_Write( uint8_t addr, uint8_t *i2c_write_data, uint8_t i2c_write_data_length)
{
	I2C_TransferSeq_TypeDef    seq;
	I2C_TransferReturn_TypeDef ret;
	//uint8_t                    i2c_read_data[1];

	seq.addr  = addr;
	seq.flags = I2C_FLAG_WRITE;
	/* Select command to issue */
	seq.buf[0].data   = i2c_write_data;
	seq.buf[0].len    = i2c_write_data_length;

	ret = I2C_TransferInit(I2C0, &seq);
	while (ret == i2cTransferInProgress)
	{
		ret = I2C_Transfer(I2C0);
	}
	return ret;
}

/** @} (end addtogroup sp) */
/** @} (end addtogroup Application) */

