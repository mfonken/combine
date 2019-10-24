//
//  combine.cpp
//  combine_core
//
//  Created by Matthew Fonken on 1/21/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "combine.hpp"

Combine::Combine(const char * n, Tau * t )
:
TestInterface(2, n)
{
    this->tau = t;
    this->name = n;
    this->width = t->width;
    this->height = t->height;
}

string Combine::Serialize()
{
#ifdef __KIN__
    char kin_packet[MAX_BUFFER];
    
    double r[3], p[3];
    r[0] = kin.values.rotation[0] * rotation_scale;
    r[1] = kin.values.rotation[1] * rotation_scale;
    r[2] = kin.values.rotation[2] * rotation_scale;
    p[0] = kin.values.position[0] * position_scale;
    p[1] = kin.values.position[1] * position_scale;
    p[2] = kin.values.position[2] * position_scale;

    sprintf(kin_packet, "f,%.10f,%.10f,%.10f,%.10f,%.10f,%.10f\r\n", r[0], r[1], r[2], p[0], p[1], p[2] );
//    printf("%s", kin_packet);
    return string(kin_packet);
#else
    return "";
#endif
}

void Combine::Init()
{
#ifdef __IMU__
    printf("Initializing IMU Utility.\n");
    IMUFunctions.init( &imu, DEFAULT_IMU_REMAP );
#endif

#ifdef __KIN__
    printf("Initializing Kinetic Utility.\n");
    KineticFunctions.Init( &kin, width, height, FOCAL_LENGTH, D_FIXED );
#endif
}

void Combine::Trigger()
{
#ifdef __IMU__
#ifdef __KIN__
    pthread_mutex_lock( &tau->predictions_mutex );
    kpoint_t A = (kpoint_t){tau->A.x, tau->A.y}, B = (kpoint_t){tau->B.x, tau->B.y};
    pthread_mutex_unlock( &tau->predictions_mutex );
    
    IMUFunctions.update.orientation( &imu );
    ang3_t e = { imu.pitch * DEG_TO_RAD, imu.roll * DEG_TO_RAD, imu.yaw * DEG_TO_RAD },
           g = { imu.gyro[0], imu.gyro[1], imu.gyro[2] };
    KineticFunctions.UpdateRotation( &kin, &e, &g );

    vec3_t n;
    KineticFunctions.Nongrav( &kin, &n );
    KineticFunctions.UpdatePosition( &kin, &n, B, A );
#else
    IMUFunctions.update.orientation( &imu );
    printf("[IMU] <%.4f, %.4f, %.4f>\n", imu.roll, imu.pitch, imu.yaw);
#endif
#endif
}
