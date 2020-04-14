//
//  combine.hpp
//  combine_core
//
//  Created by Matthew Fonken on 1/21/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef combine_hpp
#define combine_hpp

#include <stdio.h>
#include <string>

#include "kinetic_master.h"
#include "utility_master.h"
#include "environment_master.h"
#include "tau_master.h"

#define MAX_BUFFER 256

#define DEFAULT_ROTATION_SCALE 1
#define DEFAULT_POSITION_SCALE 100

static remap_array_t DEFAULT_IMU_REMAP =
{
    { 0, 0, 0 }, /* Pitch */
    { 1, 0, 1 }, /* Roll  */
    { 2, 0, 1 }, /* Yaw   */
    { 3, 0, 1 }, /* Linear Acceleration X */
    { 4, 0, 1 }, /* Linear Acceleration Y */
    { 5, 0, 1 }, /* Linear Acceleration Z */
};

class Combine : public TestInterface
{
public:
    void Init( void );
    void Trigger( void );
    string Serialize( void );
    
    Combine(const char *, Tau * );
    
private:
    double      rotation_scale = DEFAULT_ROTATION_SCALE,
                position_scale = DEFAULT_POSITION_SCALE;
    
    int         width,
                height;
    Tau       * tau;
#ifdef __KIN__
    kinetic_t   kin;
#endif
#ifdef __IMU__
    imu_t       imu;
#endif
};

#endif /* combine_hpp */
