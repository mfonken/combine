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
    pthread_mutex_destroy(&mutex);
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
    LOG_IMU(DEBUG_1, "IMUUtility::trigger\n");
    IMU.update.orientation(&imu);
}

std::string IMUUtility::serialize()
{
    return this->name;
}

IMUUtility::imu_data_t IMUUtility::FetchIMUData( void )
{ LOCK(&mutex)
    IMUUtility::imu_data_t data =
    {
        imu.roll,
        imu.pitch,
        imu.yaw,
        
        imu.accel_raw[0],
        imu.accel_raw[1],
        imu.accel_raw[2]
    };
    return data;
}
