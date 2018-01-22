//
//  sensor_utility.cpp
//  combine_core
//
//  Created by Matthew Fonken on 1/21/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "sensor_utility.hpp"

void * Sensor::init( void *data )
{
    environment_t edat = *( ( environment_t * ) data );
    pthread_mutex_lock(&edat.lock);
    
    pthread_mutex_unlock(&edat.lock);
    return NULL;
}

void * Sensor::trigger( void *data )
{
    environment_t edat = *( ( environment_t * ) data );
    pthread_mutex_lock(&edat.lock);
    
    pthread_mutex_unlock(&edat.lock);
    return NULL;
}
