//
//  motionsensor.c
//  sensors
//
//  Created by Matthew Fonken on 7/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include <stdio.h>
#include "motionsensor.h"

bool Init_IMU( imu_t * p_imu, component_id_t component_ID, uint8_t shtp_ID, comm_protocol protocol, imu_chip_t chip )
{
    p_imu->ID = component_ID;
    p_imu->client.ID = shtp_ID;
    p_imu->chip = chip;
    p_imu->sensor_specific_metadata = 0;
    IMUFunctions.GenerateClient( &p_imu->client, p_imu->ID, (void *)( &p_imu->client + sizeof(p_imu->client) ) );
    return true;
}

bool Set_IMU_RotVec( imu_t * p_imu )
{
    IMUFunctions.Start( &p_imu->client, SH2_SENSOR_REPORT_ROTATION_VECTOR, p_imu->interval, 0 );
    return true;
}
