/***********************************************************************************************//**
 * \file   mpu9250.c
 * \brief  IMU Control
 ***************************************************************************************************
 *      Author: Matthew Fonken
 **************************************************************************************************/

/* Own header */
#include "LSM9DS1.h"

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup imu
 * @{
 **************************************************************************************************/

/***************************************************************************************************
 Macros
 **************************************************************************************************/
#define sign(x) x > 0 ? 1:-1

/***************************************************************************************************
 Local Variables
 **************************************************************************************************/
LSM9DS1_t lsm9;

double magResolutions[4] = { 0.00014, 0.00029, 0.00043, 0.00058 };

/***************************************************************************************************
 Local Functions
 **************************************************************************************************/
/**************************************************************************//**
 * \brief IMU_GetRegister
 * \param[out] Return value from register
 * \param[in] reg Register to access
 *****************************************************************************/
uint8_t IMU_GetRegister( uint8_t reg )
{
	return I2C_Read_Reg( LSM9DS1_IMU_ADDR, reg);
}

uint8_t Mag_GetRegister( uint8_t reg )
{
	return I2C_Read_Reg( LSM9DS1_MAG_ADDR, reg);
}

/**************************************************************************//**
 * \brief IMU_SetRegister
 * \param[in] reg Register to access
 * \param[in] val Value to set
 *****************************************************************************/
void IMU_SetRegister( uint8_t reg, uint8_t val )
{
	uint8_t i2c_write_data[2];
	i2c_write_data[0] = reg;
	i2c_write_data[0] = val;
	I2C_Write( LSM9DS1_IMU_ADDR, i2c_write_data, 2 );
}

/***************************************************************************************************
 Initialization
 **************************************************************************************************/

/**************************************************************************//**
 * \brief Initialize IMU with local settings
 * \param[out] Initialization success
 *****************************************************************************/
void IMU_Init( LSM9DS1_t * imu )
{
    lsm9 = *imu;
	LSM9DS1_defaultInit( &imu->settings );

    imu->data.accel_bias[0] 	= ACCEL_BIAS_X;
    imu->data.accel_bias[1] 	= ACCEL_BIAS_Y;
    imu->data.accel_bias[2] 	= ACCEL_BIAS_Z;

    imu->data.gyro_bias[0] 	= GYRO_BIAS_X;
	imu->data.gyro_bias[1] 	= GYRO_BIAS_Y;
	imu->data.gyro_bias[2]	= GYRO_BIAS_Z;

	imu->data.mag_bias[0] 	= MAG_BIAS_X;
	imu->data.mag_bias[1] 	= MAG_BIAS_Y;
	imu->data.mag_bias[2] 	= MAG_BIAS_Z;

	uint32_t accel_res;
	switch ( XL_FS_DEFAULT )
	{
		default:
		case XL_FS_2G:
			accel_res = 2;
			break;
		case XL_FS_4G:
			accel_res = 4;
			break;
		case XL_FS_8G:
			accel_res = 8;
			break;
		case XL_FS_16G:
			accel_res = 16;
			break;
	}

	uint32_t gyro_res;
	switch ( GYRO_FS_DEFAULT )
	{
		default:
		case GYRO_FS_245DPS:
			gyro_res = 245;
			break;
		case GYRO_FS_500DPS:
			gyro_res = 500;
			break;
		case GYRO_FS_2000DPS:
			gyro_res = 2000;
			break;
	}

	double mag_res;
	switch ( MAG_FS_DEFAULT )
	{
		default:
		case MAG_FS_4GAUSS:
			mag_res = magResolutions[0];
			break;
		case MAG_FS_8GAUSS:
			mag_res = magResolutions[1];
			break;
		case MAG_FS_12GAUSS:
			mag_res = magResolutions[2];
			break;
		case MAG_FS_16GAUSS:
			mag_res = magResolutions[3];
			break;
	}

	imu->data.accel_res      = accel_res / LSM9DS1_IMU_ADC_MAX;
	imu->data.gyro_res 		= gyro_res  / LSM9DS1_IMU_ADC_MAX;
	imu->data.mag_res 		= mag_res;
}

bool IMU_Verify( LSM9DS1_t * imu )
{
	if(IMU_GetRegister(WHO_AM_I) != IMU_WHO_AM_I_VAL) return 0;
	if(Mag_GetRegister(WHO_AM_I_M) != MAG_WHO_AM_I_VAL) return 0;
	return 1;
}

/******************************************************************************
 * Motion Data Access
 *****************************************************************************/

/**************************************************************************//**
 * \brief Read IMU accel and gyro data
 * \param[in] read_data Array to store read data
 *****************************************************************************/
void IMU_Update_All( LSM9DS1_t * imu )
{
    IMU_Update_Accel( imu );
    IMU_Update_Gyro( imu );
    IMU_Update_Mag( imu );
    
    IMU_Update_Roll( imu );
    IMU_Update_Pitch( imu );
    IMU_Update_Yaw( imu );
}

void IMU_Update_Angles( LSM9DS1_t * imu )
{
    IMU_Update_Accel( imu );
    IMU_Update_Mag( imu );
    
    IMU_Update_Roll( imu );
    IMU_Update_Pitch( imu );
    IMU_Update_Yaw( imu );
}

void IMU_Update_Accel( LSM9DS1_t * imu )
{
    /* Combine low and high byte values */
    uint8_t                    i2c_read_data[6];
    I2C_Read_Regs( LSM9DS1_IMU_ADDR, XL_OUT, i2c_read_data, 6 );
    int16_t accel[3];
    accel[0] = ( i2c_read_data[1] << 8 ) | i2c_read_data[0];
    accel[1] = ( i2c_read_data[3] << 8 ) | i2c_read_data[2];
    accel[2] = ( i2c_read_data[5] << 8 ) | i2c_read_data[4];
    
    for( int i = 0; i < 3 ; i++ )
    {
        imu->data.accel[i] = accel[i] * imu->data.accel_res - imu->data.accel_bias[i];
    }
}

void IMU_Update_Gyro( LSM9DS1_t * imu )
{
    /* Combine low and high byte values */
    uint8_t                    i2c_read_data[6];
    I2C_Read_Regs( LSM9DS1_IMU_ADDR, G_OUT, i2c_read_data, 6 );
    int16_t gyro[3];
    gyro[0] = ( i2c_read_data[1] << 8 ) | i2c_read_data[0];
    gyro[1] = ( i2c_read_data[3] << 8 ) | i2c_read_data[2];
    gyro[2] = ( i2c_read_data[5] << 8 ) | i2c_read_data[4];
    
    for( int i = 0; i < 3 ; i++ )
    {
        imu->data.gyro[i]  = gyro[i]  * imu->data.gyro_res  - imu->data.gyro_bias[i];
    }
}

void IMU_Update_Mag( LSM9DS1_t * imu )
{
    /* Combine low and high byte values */
    uint8_t                    i2c_read_data[6];
    I2C_Read_Regs( LSM9DS1_MAG_ADDR, M_OUT, i2c_read_data, 6 );
    int16_t mag[3];
    mag[0] = ( i2c_read_data[1] << 8 ) | i2c_read_data[0];
    mag[1] = ( i2c_read_data[3] << 8 ) | i2c_read_data[2];
    mag[2] = ( i2c_read_data[5] << 8 ) | i2c_read_data[4];
    
    for( int i = 0; i < 3 ; i++ )
    {
        imu->data.mag[i]   = mag[i]   * imu->data.mag_res	  - imu->data.mag_bias[i];
    }
}

/******************************************************************************
 * Rotation Calculation
 *****************************************************************************/
/* See - http://www.nxp.com/files/sensors/doc/app_note/AN3461.pdf and
       - http://www.nxp.com/assets/documents/data/en/application-notes/AN4248.pdf
 */

/**************************************************************************//**
 * \brief Calculate roll angle (phi) from accelerometer data
 * \param[out] Return roll
 *****************************************************************************/
void IMU_Update_Roll( LSM9DS1_t * imu )
{
    /* AN4248: Eq. 13 */
    imu->data.roll = atan2( imu->data.accel[0], imu->data.accel[2] );
    
    /* AN3461: Eq. 37 */
//    double den = sqrt( ( ( imu->data.accel[1] * imu->data.accel[1] ) + ( imu->data.accel[2] * imu->data.accel[2] ) ) );
//    imu->data.roll = atan2( -imu->data.accel[0], den );
}

/**************************************************************************//**
 * \brief Calculate pitch angle (theta) from accelerometer data
 * \param[out] Return pitch
 *****************************************************************************/
void IMU_Update_Pitch( LSM9DS1_t * imu )
{
    /* AN4248: Eq. 14 */
    double den = ( imu->data.accel[0] * sin( imu->data.roll ) ) + ( imu->data.accel[2] * cos ( imu->data.roll ) );
    imu->data.pitch = atan2( -imu->data.accel[1], den );
    
    /* AN3461: Eq. 38 */
//    double den = sign( imu->data.accel[2] ) * sqrt( ( ( imu->data.accel[2] * imu->data.accel[2] ) + ( MU * ( imu->data.accel[0] * imu->data.accel[0] ) ) ) );
//    imu->data.pitch = atan2( imu->data.accel[1], den );
}

/**************************************************************************//**
 * \brief Calculate yaw angle (psi) from magnetometer data, pitch, and roll
 * \param[out] Return yaw
 *****************************************************************************/
 void IMU_Update_Yaw( LSM9DS1_t * imu )
{
	double Bx = imu->data.mag[1];
	double By = imu->data.mag[0];
	double Bz = -imu->data.mag[2];

    /* AN4248: Eq. 22 */
    double sin_phi   = sin( imu->data.roll );
    double sin_theta = sin( imu->data.pitch );
    double cos_phi   = cos( imu->data.roll );
    double cos_theta = cos( imu->data.pitch );
    double num = ( Bz * sin_phi ) - ( By * cos_phi );
    double den = ( Bx * cos_theta ) + ( By * ( sin_theta * sin_phi ) ) + ( Bz * ( sin_theta * cos_phi ) );
    imu->data.yaw = atan2( num, den );
}

/**************************************************************************//**
 * \brief Calculate roll angle (phi) error from accelerometer data
 * \param[out] Return roll error
 *****************************************************************************/
double IMU_Roll_Error_Get( LSM9DS1_t * imu )
{
    double sin_phi   = sin( imu->data.roll );
    double sin_theta = sin( imu->data.pitch );
    double cos_phi   = cos( imu->data.roll );
    double cos_theta = cos( imu->data.pitch );
    double cos_theta_cos_phi = cos_theta * cos_phi;
    double mu_sin_2_theta = MU * ( sin_theta * sin_theta );
    double factor = sqrt( ( cos_theta_cos_phi * cos_theta_cos_phi ) + mu_sin_2_theta );
    double num = sin_phi * ( cos_theta_cos_phi - factor );
    double den = ( cos_theta * ( sin_phi * sin_phi ) ) + ( cos_phi * factor );
    return atan2( num, den );
}

/**************************************************************************//**
 * \brief Get no gravitation acceleration from accelerometer data
 * \param[out] Return 3D vector of acceleration
 * \param[in] tba Tait-Bryan angles to transform by
 *****************************************************************************/
void IMU_Non_Grav_Get( LSM9DS1_t * imu, quaternion_t * q, vec3_t * ngacc )
{
    IMU_Update_All( imu );
    
    /* Create a vector of accelerometer values */
    vec3_t avec;
    avec.i = -imu->data.accel[0];
    avec.j = -imu->data.accel[1];
    avec.k = -imu->data.accel[2];

	vec3_t nvec;
	mat3x3_t m;
	Quaternion_To_Matrix( q, &m );
	Multiply_Vec_3x1( &m, &avec, &nvec );

	ang3_t a;
	a.x = atan2(2*(q->w*q->x+q->y*q->z),1-2*(q->x*q->x+q->y*q->y)) * 57.295779; //yaw
	a.y = asin(2*((q->w*q->y) - (q->z*q->x))) * 57.295779;// roll
	a.z = atan2(2*(q->w*q->z+q->x*q->y),1-2*(q->y*q->y+q->z*q->z)) * 57.295779; // pitch

	ngacc->i = nvec.i;
	ngacc->j = nvec.j;
	ngacc->k = nvec.k + 1; // Negate gravity
}

/******************************************************************************
 * Temperature
 *****************************************************************************/

/**************************************************************************//**
 * \brief Read temperature from register
 * \param[out] Return raw temperature data
 *****************************************************************************/
uint16_t IMU_ReadTemp( void )
{
	uint16_t tempurature;
	tempurature  = IMU_GetRegister( OUT_TEMP_H ) << 8;
	tempurature += IMU_GetRegister( OUT_TEMP_L );
	return tempurature;
}

/**************************************************************************//**
 * \brief Read temperature in Fahrenheit
 * \param[out] Return corrected temperature data as readable double
 *****************************************************************************/
double getTempF( void )
{
	return ( double )( ( IMU_ReadTemp() / 16 ) + 25 );;
}

/**************************************************************************//**
 * \brief Read temperature in Celsius
 * \param[out] Return corrected temperature data as readable double
 *****************************************************************************/
double getTempC( void )
{
    return ( double )( ( getTempF() * ( 5 / 9 ) ) - 32 );;
}

/** @} (end addtogroup imu) */
/** @} (end addtogroup Application) */
