//
//  imu_wrapper.h
//  IMU Math
//
//  Created by Matthew Fonken on 12/19/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef imu_wrapper_h
#define imu_wrapper_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdio.h>
#include <math.h>
    
#include "sercom.h"
    
#define DEFAULT_INTERFACE SERCOM
    
/* Generic messages enum */
#define MESSAGE_TOKEN_ID       -1
#define MESSAGE_TOKEN_INDEX     1
#define SYSTEM_CAL_TOKEN_INDEX  2
#define ACCEL_CAL_TOKEN_INDEX   3
#define GYRO_CAL_TOKEN_INDEX    4
#define MAG_CAL_TOKEN_INDEX     5
    
#define FULL_CALIBRATION        0b11
    
#define CALIBRATION_INDEX       4
#define CALIBRATED              1 << CALIBRATION_INDEX
#define ACTIVE_INDEX            3
#define ACTIVE                  1 << ACTIVE_INDEX
    
#define LSM9DS1
    
    typedef enum
    {
        SERCOM = 0,
        I2C,
        SPI
    } INTERFACE;
    
    typedef struct
    {
        INTERFACE interface;
        int       descriptor;
    } channel_t;
    
    typedef enum
    {
        NULL_PACKET_ID  = 0xff,
        MESSAGE_ID      = 'm',
        ORIENTATION_ID  = 'o'
    } packet_id_t;

    
    static char * state_action_strings[] = { "NULL", "CALIBRATING", "ACTIVATING", "RUNNING" };
    typedef enum
    {
        CALIBRATING = 1,
        ACTIVATING,
        RUNNING
    } state_action_t;
    
    typedef struct
    {
    uint16_t
        calibrated:1,
        active:1,
        sc:2,
        ac:2,
        gc:2,
        mc:2,
        padding:6;
    } status_register;
    
    typedef struct
    {
        state_action_t action;
        status_register status;
    } imu_state_t;
    
    typedef struct
    {
        double 	accel_raw[3];
        
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
        
        channel_t channel;
        imu_state_t state;
    } imu_t;
    
    typedef struct imu imu;
    struct imu
    {
        void (*update)(imu_t * imu);
        int (*init)(imu_t * imu);
    };
    extern const imu IMU;

    int Read_SERCOM_IMU_Packet( imu_t *, char * );
    
    void IMU_SERCOM_Init(imu_t * imu);
    void IMU_Update(imu_t * imu);
    
#ifdef __cplusplus
}
#endif

#endif /* imu_wrapper_h */
