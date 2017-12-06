//
//  imu_stubs.hpp
//  combine_core
//
//  Created by Matthew Fonken on 11/17/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef imu_stubs_h
#define imu_stubs_h

#ifdef __cplusplus
extern "C" {
#endif
    
//#define PACKET_DEBUG

#include <stdio.h>
#include "kinetic_types.h"
#include "matrix.h"

typedef struct
{
    double 	accel[3];
    double 	gyro[3];
    double 	mag[3];
    
    double  accel_res;
    double 	gyro_res;
    double 	mag_res;
    
    double 	accel_bias[3];
    double 	gyro_bias[3];
    double 	mag_bias[3];
    
    double	roll;
    double	pitch;
    double	yaw;
} imu_t;

typedef struct {
    
} LSM9DS1_cfg_t;

typedef struct
{
    imu_t               data;
    LSM9DS1_cfg_t       settings;
} LSM9DS1_t;
    
void Init_SERCOM();
void Read_SERCOM_IMU_Packet(LSM9DS1_t * imu);

void IMU_Init(LSM9DS1_t * a);
void IMU_Update_All(LSM9DS1_t * a);
void IMU_Normalize_All( LSM9DS1_t * imu );
void IMU_Normalize_Accel( LSM9DS1_t * imu );
void IMU_Normalize_Mag( LSM9DS1_t * imu );
void IMU_Update_Roll( LSM9DS1_t * imu );
void IMU_Update_Pitch( LSM9DS1_t * imu );
void IMU_Update_Yaw( LSM9DS1_t * imu );
double IMU_Roll_Error_Get( LSM9DS1_t * imu );
void IMU_Get_Non_Grav( LSM9DS1_t * imu, vec3_t * ngacc );

void tokenifyPacket( char * a, int l, double * d);
char** str_split(char* a_str, const char a_delim);
    
#ifdef __cplusplus
}
#endif
        
#endif /* imu_stubs_h */
