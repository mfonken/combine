//
//  motionsensor.c
//  sensors
//
//  Created by Matthew Fonken on 7/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include <stdio.h>
#include "motionsensor.h"

bool Init_IMU( imu_t * imu, component_id_t component_ID, uint8_t shtp_ID, comm_protocol protocol, imu_chip_t chip )
{
    imu->ID = component_ID;
    imu->client.ID = shtp_ID;
    imu->chip = chip;
    imu->sensor_specific_metadata = 0;
    IMUFunctions.GenerateClient( &imu->client, imu->ID, (void *)( &imu->client + sizeof(imu->client) ) );
    return true;
}

bool Set_IMU_RotVec( imu_t * imu )
{
    IMUFunctions.Start( &imu->client, SH2_SENSOR_REPORT_ROTATION_VECTOR, imu->interval, 0 );
    return true;
}
