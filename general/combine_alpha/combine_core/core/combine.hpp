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
#include "open_master.h"
//#include "environment_master.h"

#define MAX_BUFFER 256
#define SCALE 100

class Combine// : public TestInterface
{
public:
    Combine();
    virtual void init( void );
    virtual void trigger( void );
    virtual string serialize( void );
    ImageUtility utility;
private:
    int         width,
                height;
    kpoint_t    bea[2];
    imu_t       bno;
    kinetic_t   kin;
    
};

#endif /* combine_hpp */
