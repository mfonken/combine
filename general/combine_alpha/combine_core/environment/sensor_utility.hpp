//
//  sensor_utility.hpp
//  combine_core
//
//  Created by Matthew Fonken on 1/21/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef sensor_utility_hpp
#define sensor_utility_hpp

#include <stdio.h>
#include "environment.hpp"

class Sensor
{
public:
    void * init( void *data );
    void * trigger( void *data );
};

#endif /* sensor_utility_hpp */
