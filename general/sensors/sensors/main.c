//
//  main.c
//  sensors
//
//  Created by Matthew Fonken on 7/26/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include <stdio.h>
#include "global.h"

#include "batterymonitor.h"
#include "touchcontroller.h"
#include "motionsensor.h"

int main(int argc, const char * argv[]) {
    stc_reg_ctrl_t data = BatteryMonitor.GetControl();
    
    imu_t imu;
    IMUFunctions.Init( &imu, SYSTEM_SENSOR_MOTION_PRIMARY, IMU_CHIP_BNO080 );
    IMUFunctions.Start( &imu.client, BNO080_ORIENTATION_OUTPUT_GYRO_ROTATION_VECTOR, 50, 0 );
    
    printf("Battery monitor control register 0x%02x\n", *(uint8_t*)&data);
    printf("Touch controller read 0x%02x\n", TouchController.Read().index);
    return 0;
}
