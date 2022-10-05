//
//  imu_utility.cpp
//  ots-proto
//
//  Created by Matthew Fonken on 3/12/22.
//

#include <stdio.h>
#include "imu_utility.hpp"

IMUUtility::IMUUtility(SERCOM_Channel * channel, string n)
{
    this->channel = channel;
}

IMUUtility::~IMUUtility()
{
//    pthread_mutex_destroy(&mutex);
}

void IMUUtility::init()
{
    LOG_IMU(DEBUG_2, "Initializing IMU: %s @ %d\n", channel->port, channel->baud);
    IMU.init(&imu, channel);
    
    if( pthread_mutex_init(&mutex, NULL) != 0 )
        printf( "mutex init failed\n" );
}

void IMUUtility::trigger()
{ LOCK(&mutex)
    LOG_IMU(DEBUG_1, "trigger\n");
//    IMU.update.all(&imu);
    IMU.update.orientation(&imu);
}

std::string IMUUtility::serialize()
{
    return this->name;
}

IMUUtility::imu_data_t IMUUtility::FetchIMUData( void )
{
    IMUUtility::imu_data_t data =
    {
        // Timestamp
        imu.timestamp_ns,
        
        // Orientation
        imu.roll,
        imu.pitch,
        imu.yaw,
        
        // Accel
        imu.accel[0],
        imu.accel[1],
        imu.accel[2],
        
        // Gyro
        imu.gyro[0],
        imu.gyro[1],
        imu.gyro[2],
        
        // Gravity
        imu.gravity[0],
        imu.gravity[1],
        imu.gravity[2]
    };
    return data;
}
