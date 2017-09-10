/***********************************************************************************************//**
 * \file   LSM9DS1.h
 * \brief  IMU Control Header
 ***************************************************************************************************
 *      Author: Matthew Fonken
 **************************************************************************************************/

#ifndef LSM9DS1_H_
#define LSM9DS1_H_

/* Standard headers */
#include <stdbool.h>
#include <stddef.h>
#include <math.h>

#include "../utilities/i2c_sp.c"

/* Included types header */
#include "LSM9DS1_types.h"
#include "LSM9DS1_regs.c"
#include "../kinetic/kinetic_types.h"

/* Math headers */
#include "../kinetic/matrix.c"

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup imu
 * @{
 **************************************************************************************************/

//extern uint8_t accel[3];
//extern uint8_t accel_bias[3];
//extern uint8_t gyro[3];
//extern uint8_t gyro_bias[3];
//extern uint8_t mag[3];
//extern uint8_t mag_bias[3];

#define ACCEL_BIAS_X 	0
#define ACCEL_BIAS_Y	0
#define ACCEL_BIAS_Z	0

#define GYRO_BIAS_X		0
#define GYRO_BIAS_Y		0
#define GYRO_BIAS_Z		0

#define MAG_BIAS_X	   0// 0.007
#define MAG_BIAS_Y	   0//	0.016
#define MAG_BIAS_Z	   0//-0.012

#define MU				0.1

/***************************************************************************************************
 Local Structures
 **************************************************************************************************/


/***************************************************************************************************
 Local Functions
 **************************************************************************************************/

/**************************************************************************//**
 * \brief Get IMU Register
 * \param[out] Return value from register
 * \param[in] reg Register to access
 *****************************************************************************/
uint8_t IMU_GetRegister( uint8_t );

/**************************************************************************//**
 * \brief Set IMU Register
 * \param[in] reg Register to access
 * \param[in] val Value to set
 *****************************************************************************/
void IMU_SetRegister(   uint8_t, uint8_t );

/**************************************************************************//**
 * \brief Reset Local Settings to Default
 ******************************************************************************
 * NOTE: This does not physically set on the IMU, just the local variable
 *****************************************************************************/
void IMU_Default(       LSM9DS1_t * );

/**************************************************************************//**
 * \brief Initialize IMU with local settings
 * \param[out] Initialization success
 *****************************************************************************/
void IMU_Init(          LSM9DS1_t * );
void IMU_Update_All(    LSM9DS1_t * );
void IMU_Update_Angles( LSM9DS1_t * );
void IMU_Update_Accel(  LSM9DS1_t * );
void IMU_Update_Gyro(   LSM9DS1_t * );
void IMU_Update_Mag(    LSM9DS1_t * );
void IMU_Update_Roll(   LSM9DS1_t *);
void IMU_Update_Pitch(  LSM9DS1_t * );
void IMU_Update_Yaw(    LSM9DS1_t * );
double IMU_Roll_Error_Get( LSM9DS1_t * );

/**************************************************************************//**
 * \brief Get no gravitation acceleration from accelerometer data
 * \param[out] Return 3D vector of acceleration
 * \param[in] tba Tait-Bryan angles to transform by
 *****************************************************************************/
void IMU_Non_Grav_Get( LSM9DS1_t * imu, quaternion_t * q, vec3_t * ngacc );

/**************************************************************************//**
 * \brief Read temperature from register
 * \param[out] Return raw temperature data
 *****************************************************************************/
uint16_t IMU_ReadTemp( void );

/**************************************************************************//**
 * \brief Read temperature in Fahrenheit
 * \param[out] Return corrected temperature data as readable double
 *****************************************************************************/
double getTempF( void );

/**************************************************************************//**
 * \brief Read temperature in Celsius
 * \param[out] Return corrected temperature data as readable double
 *****************************************************************************/
double getTempC( void );

/** @} (end addtogroup imu) */
/** @} (end addtogroup Application) */

#endif /* IMU_H_ */
