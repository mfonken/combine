//
//  kinetic_utility.hpp
//  ots-proto
//
//  Created by Matthew Fonken on 3/13/22.
//

#ifndef kinetic_utility_hpp
#define kinetic_utility_hpp

#include "environment_master.hpp"
#include "kinetic.h"
#include "orienter.h"

#include <sstream>

#define DEBUG_KU

#ifdef DEBUG_KU
#define LOG_KU(L, ...) LOG(L, "<KineticUtility> " __VA_ARGS__)
#else
#define LOG_KU(L, ...)
#endif

class KineticUtility : public TestInterface
{
    const std::string name;
    void  init( void );
    
    kinetic_t kin;
    kinetic_config_t * config;
    kalman_t position[3];
    vec3_t n;
    quaternion_t O;
    kpoint_t A;
    kpoint_t B;
    
public:
    string serialize( void );
    pthread_mutex_t mutex;
    pthread_mutex_t point_data_mutex;
    pthread_mutex_t orienter_data_mutex;
    orienter_t orienter;
    
    KineticUtility( kinetic_config_t * config, string n = "kinetic" );
    ~KineticUtility();
    
    void trigger( void );
    void UpdateIMUData( vec3_t * nong, vec3_t * ang );//, quaternion_t * o );
    void UpdatePointData( kpoint_t * a, kpoint_t * b );
    vec3_t GetPosition();
    void InitFilters();
};


#endif /* kinetic_utility_hpp */
