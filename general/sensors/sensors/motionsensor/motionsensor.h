//
//  motionsensor.h
//  sensors
//
//  Created by Matthew Fonken on 7/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef motionsensor_h
#define motionsensor_h

#include <stdint.h>
#include "BNO080.h"

#define USE_WATERFALL_EQUATIONS

typedef struct
{
    double x, y, z;
} triple_axis_t;

typedef struct
{
    triple_axis_t   accel[3];
    triple_axis_t   gyro[3];
    triple_axis_t   mag[3];
    
    double          accel_res;
    double          gyro_res;
    double          mag_res;
    
    triple_axis_t   accel_bias[3];
    triple_axis_t   gyro_bias[3];
    triple_axis_t   mag_bias[3];
    
    double          roll;
    double          pitch;
    double          yaw;
} imu_t;

struct imuUpdateops
{
    void (*all)(imu_t * imu);
    void (*accel)(imu_t * imu);
    void (*gyro)(imu_t * imu);
    void (*mag)(imu_t * imu);
    
    void (*roll)(imu_t * imu);
    void (*pitch)(imu_t * imu);
    void (*yaw)(imu_t * imu);
    
    void (*orientation)(imu_t * imu);
};

struct imuNormalizeops
{
    void (*all)(imu_t * imu);
    void (*accel)(imu_t * imu);
    void (*mag)(imu_t * imu);
};

typedef struct imu imu;
struct imu
{
    struct imuUpdateops update;
    struct imuNormalizeops normalize;
    int (*init)(imu_t * imu);
};

extern const imu IMU;

void imuUpdateAll(imu_t * imu);
void imuUpdateOrientation(imu_t * imu);
void imuNormalizeAll( imu_t * imu );
void imuNormalizeAccel( imu_t * imu );
void imuNormalizeMag( imu_t * imu );
void imuUpdateRoll( imu_t * imu );
void imuUpdatePitch( imu_t * imu );
void imuUpdateYaw( imu_t * imu );
double IMU_Roll_Error_Get( imu_t * imu );

double mag_cal[] = { -2.396, 38.040, 1.093 };
char line[BUFFER_LENGTH];

static int init( imu_t * imu )
{
    switch(imu->channel.interface)
    {
        default:
        case SERCOM:
            imu->channel.descriptor = Init_SERCOM_Default();
            return (int)SERCOM;
            break;
    }
    return -1;
}

const imu IMU =
{
    .init = init,
    
    .update.all = imuUpdateAll,
    .update.orientation = imuUpdateOrientation,
    .update.roll = imuUpdateRoll,
    .update.pitch = imuUpdatePitch,
    .update.yaw = imuUpdateYaw,
    
    .normalize.all = imuNormalizeAll,
    .normalize.accel = imuNormalizeAccel,
    .normalize.mag = imuNormalizeMag
};

void imuUpdateAll(imu_t * a)
{
    imuNormalizeAll(a);
    imuUpdateRoll(a);
    imuUpdatePitch(a);
    imuUpdateYaw(a);
}

void imuGetReadAllEvent( i)

static void imuNormalizeAll( imu_t * imu )
{
    imuNormalizeAccel(imu);
    imuNormalizeMag(imu);
}
static void imuNormalizeAccel( imu_t * imu )
{
    double x,y,z,n;
    x = imu->accel.x;
    y = imu->accel.y;
    z = imu->accel.z;
    
    n = sqrt( x*x + y*y + z*z );
    imu->accel.x /= n;
    imu->accel.y /= n;
    imu->accel.z /= n;
}
static void imuNormalizeMag( imu_t * imu )
{
    n = sqrt( imu->mag.x*imu->mag.x + imu->mag.y*imu->mag.y + imu->mag.z*imu->mag.z );
    imu->mag.x /= n;
    imu->mag.y /= n;
    imu->mag.z /= n;
}

static void imuUpdateRoll( imu_t * imu )
{
#ifdef USE_WATERFALL_EQUATIONS
    /* AN4248: Eq. 13 */
    imu->roll = atan2( imu->accel.y, imu->accel.z );
    
#else
    /* AN3461: Eq. 37 */
    double den = sqrt( ( ( imu->accel.y * imu->accel.y ) + ( imu->accel.z * imu->accel.z ) ) );
    imu->roll = atan2( -imu->accel.x, den );
#endif
}

/* Note: Requires updated roll */
static void imuUpdatePitch( imu_t * imu )
{
#ifdef USE_WATERFALL_EQUATIONS
    /* AN4248: Eq. 14 */
    double den = ( imu->accel.y * sin( imu->roll ) ) + ( imu->accel.z * cos ( imu->roll ) );
    imu->pitch = atan2( -imu->accel.x, den );
#else
    /* AN3461: Eq. 38 */
    double den = sign( imu->accel.z ) * sqrt( ( ( imu->accel.z * imu->accel.z ) + ( MU * ( imu->accel.x * imu->accel.x ) ) ) );
    imu->pitch = atan2( imu->accel.y, den );
#endif
}

/* Note: Requires updated pitch and roll */
static void imuUpdateYaw( imu_t * imu )
{
    /* NOTE: Magnetometer X and Y axes are switched between AN4248<>LSM9DS1 */
    ///TODO fix axes for BNO080
    /* AN4248: Eq. 22 */
    double sin_phi   = sin( imu->roll );
    double sin_theta = sin( imu->pitch );
    double cos_phi   = cos( imu->roll );
    double cos_theta = cos( imu->pitch );
    double num = ( imu->mag.z * sin_phi ) - ( imu->mag.y * cos_phi );
    double den = ( imu->mag.x * cos_theta ) + ( imu->mag.y * ( sin_theta * sin_phi ) ) + ( imu->mag.z * ( sin_theta * cos_phi ) );
    imu->yaw = -atan2( -num, den );
}

#endif /* motionsensor_h */
