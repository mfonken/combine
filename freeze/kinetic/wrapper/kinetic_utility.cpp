//
//  kinetic_utility.cpp
//  ots-proto
//
//  Created by Matthew Fonken on 3/13/22.
//

#include "kinetic_utility.hpp"

KineticUtility::KineticUtility( kinetic_config_t * config, string n )
: name(n), config(config)
{
    if( pthread_mutex_init(&mutex, NULL) != 0 ) printf( "mutex init failed\n" );
    if( pthread_mutex_init(&point_data_mutex, NULL) != 0 ) printf( "mutex init failed\n" );
}

KineticUtility::~KineticUtility()
{
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&point_data_mutex);
}

void KineticUtility::init()
{
    KineticFunctions.Init(&kin, *config);
}

string KineticUtility::serialize()
{
    return this->name;
}

void KineticUtility::trigger()
{ LOCK(&mutex)//, &point_data_mutex)
    KineticFunctions.UpdatePosition( &kin, &n, &O, &A, &B );
}

void KineticUtility::UpdateIMUDate( vec3_t * n, quaternion_t * o )
{ LOCK(&mutex)
    Vector.copy3( n, &this->n );
    Quaternion.copy( o, &O );
}

void KineticUtility::UpdatePointData( kpoint_t * a, kpoint_t * b )
{ LOCK(&point_data_mutex)
    KPoint.copy( a, &A );
    KPoint.copy( b, &B );
}

vec3_t KineticUtility::GetPosition()
{ LOCK(&mutex)
    vec3_t pos =
    {
        kin.filters.position[0].value,
        kin.filters.position[1].value,
        kin.filters.position[2].value
    };
    return pos;
}
