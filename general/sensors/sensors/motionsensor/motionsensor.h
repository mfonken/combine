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
#include <stdint.h>
#include <stdbool.h>
#include "BNO080.h"

#define IMU_DEFAULT_COMM_CHANNEL SYSTEM_COMM_SPI

typedef enum
{
    IMU_CHIP_BNO080 = 1
} IMU_CHIP;

typedef IMU_CHIP imu_chip_t;
typedef uint8_t imu_feature_t;

typedef struct
{
    shtp_client_t client;
    imu_chip_t chip;
    rotation_vector_t rotation;
    uint32_t sensor_specific_metadata;
} imu_t;

bool Init_IMU( imu_t *, uint8_t, system_comm_channel, imu_chip_t );

typedef struct
{
    bool (*Init)( imu_t *, uint8_t, system_comm_channel, imu_chip_t );
    bool (*Start)( shtp_client_t *, imu_feature_t, uint32_t, uint32_t );
    bool (*Stop)( shtp_client_t *, imu_feature_t );
    bool (*Refresh)( shtp_client_t * );
} imu_functions;

static const imu_functions IMUFunctions =
{
    .Init = Init_IMU,
    .Start = BNO080EnableFeature,
    .Stop = BNO080DisableFeature,
    .Refresh = BNO080Refresh
};

#endif /* motionsensor_h */

