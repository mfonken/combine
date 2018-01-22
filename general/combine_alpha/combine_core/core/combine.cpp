//
//  combine.cpp
//  combine_core
//
//  Created by Matthew Fonken on 1/21/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "combine.hpp"

string Combine::serialize()
{
    char kin_packet[MAX_BUFFER];
    
    double r[3], p[3];
    r[0] = kin.values.rotation[0];
    r[1] = kin.values.rotation[1];
    r[2] = kin.values.rotation[2];
    p[0] = kin.filters.position[0].value * SCALE;
    p[1] = kin.filters.position[1].value * SCALE;
    p[2] = kin.filters.position[2].value * SCALE;
    sprintf(kin_packet, "f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\r\n", r[0], r[1], r[2],  p[0], p[1], p[2] );
    return string(kin_packet);
}

void Combine::init()
{
    printf("Initializing IMU.\n");
    IMU.init( &bno );
    
    printf("Initializing Kinetic Utility.\n");
    Kinetic.init( &kin, width, height, FOCAL_LENGTH, D_FIXED );

}

void Combine::trigger()
{
    utility.getBeacons();
    ang3_t e, g = { bno.gyro[0], bno.gyro[1], bno.gyro[2] };
    IMU.update.orientation( &bno );
    e = { bno.pitch * DEG_TO_RAD, bno.roll * DEG_TO_RAD, bno.yaw * DEG_TO_RAD };
    vec3_t R = { bno.accel_raw[0], bno.accel_raw[1], bno.accel_raw[2] };
    Kinetic.updateRotation( &kin, &e, &g );
    Kinetic.updatePosition( &kin, &R, &utility.bea[1], &utility.bea[0] );
}
