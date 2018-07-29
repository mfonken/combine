//
//  combine.cpp
//  combine_core
//
//  Created by Matthew Fonken on 1/21/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "combine.hpp"

Combine::Combine(const char * n, Tau * t, int width, int height )
{
    this->tau = t;
    this->name = n;
    this->width = width;
    this->height = height;
}

string Combine::serialize()
{  
    char kin_packet[MAX_BUFFER];
    
    double r[3], p[3];
    r[0] = kin.values.rotation[0];
    r[1] = kin.values.rotation[1];
    r[2] = kin.values.rotation[2];
    p[0] = kin.values.position[0] * SCALE;
    p[1] = kin.values.position[1] * SCALE;
    p[2] = kin.values.position[2] * SCALE;

    sprintf(kin_packet, "f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\r\n", r[0], r[1], r[2], p[0], p[1], -p[2] );
    return string(kin_packet);
}

void Combine::init()
{
    printf("Initializing IMU Utility.\n");
    IMU.init( &bno );
    
    printf("Initializing Kinetic Utility.\n");
    Kinetic.init( &kin, width, height, FOCAL_LENGTH, D_FIXED );
}

void Combine::trigger()
{
    IMU.update.orientation( &bno );
    ang3_t e = { bno.pitch * DEG_TO_RAD, bno.roll * DEG_TO_RAD, bno.yaw * DEG_TO_RAD },
           g = { bno.gyro[0], bno.gyro[1], bno.gyro[2] };
    Kinetic.updateRotation( &kin, &e, &g );
    
    vec3_t R = { bno.accel_raw[0], bno.accel_raw[1], bno.accel_raw[2] };
    Kinetic.updatePosition( &kin, &R, &tau->B, &tau->A );
}
