//
//  imu_utility.hpp
//  ots-proto
//
//  Created by Matthew Fonken on 3/12/22.
//

#ifndef imu_utility_h
#define imu_utility_h

#include "environment_master.hpp"
#include "imu_wrapper.h"

#define DEBUG_IMU

#ifdef DEBUG_IMU
#define LOG_IMU(L, ...) LOG(L, "<IMUUtility> " __VA_ARGS__)
#else
#define LOG_IMU(L, ...)
#endif

class IMUUtility : public TestInterface
{
    int id;
    const std::string name;
    void init( void );
    string serialize( void );
    
public:
    typedef struct
    {
        double roll;
        double pitch;
        double yaw;
        
        double accel[3];
        double gravity[3];
    } imu_data_t;
    
    SERCOM_Channel * channel;
    pthread_mutex_t mutex;
    imu_t imu;
    
    IMUUtility( SERCOM_Channel * channel = NULL, string n = "imu" );
    ~IMUUtility();
    
    void trigger( void );
    imu_data_t FetchIMUData( void );
};

#endif /* imu_utility_h */
