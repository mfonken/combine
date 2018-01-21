//
//  sensor_utility.cpp
//  combine_core
//
//  Created by Matthew Fonken on 1/21/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "sensor_utility.hpp"

void * IMU_THREAD( void *data )
{
    environment_t edat = *( ( environment_t * ) data );
    printf("Initializing IMU.\n");
    IMU.init( &edat.bno );
    
    printf("Initializing Kinetic Utility.\n");
    Kinetic.init( &edat.kin, edat.width, edat.height, FOCAL_LENGTH, D_FIXED );
    
    ang3_t e, g = { edat.bno.gyro[0], edat.bno.gyro[1], edat.bno.gyro[2] };
    
    while(1)
    {
        IMU.update.orientation( &edat.bno );
        e = { edat.bno.pitch * DEG_TO_RAD, edat.bno.roll * DEG_TO_RAD, edat.bno.yaw * DEG_TO_RAD };
        vec3_t R = { edat.bno.accel_raw[0], edat.bno.accel_raw[1], edat.bno.accel_raw[2] };
        Kinetic.updateRotation( &edat.kin, &e, &g );
        Kinetic.updatePosition( &edat.kin, &R, &edat.bea[1], &edat.bea[0] );
    }
}
