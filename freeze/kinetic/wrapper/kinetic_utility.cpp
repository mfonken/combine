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
    if( pthread_mutex_init(&orienter_data_mutex, NULL) != 0 ) printf( "mutex init failed\n" );
    
    // Kinetic
    KineticFunctions.Init( &kin, config );
    
    // Orienter
    OrienterFunctions.Init( &orienter );
}

KineticUtility::~KineticUtility()
{
    pthread_mutex_destroy(&mutex);
    pthread_mutex_destroy(&point_data_mutex);
}

void KineticUtility::init()
{
//    KineticFunctions.Init( &kin, *config );
    OrienterFunctions.Init( &orienter );
};

string KineticUtility::serialize()
{
    return this->name;
}

void KineticUtility::trigger()
{ //LOCK(&mutex)
    LOG_KU(DEBUG_1, "trigger\n");
    KineticFunctions.UpdatePosition( &kin, &O, &A, &B );
}

void KineticUtility::UpdateIMUData( vec3_t * nong, vec3_t * ang )//, vec3_t * rate )
{ LOCK(&orienter_data_mutex)
    OrienterFunctions.Update( &orienter, ang );//, rate );
    ang3_t r;
    Vector3.copy3( (vec3_t *)&orienter.rotation, (vec3_t *)&r );
    Angle.degToRad( &r );
    Quaternion.fromEuler( &r, &O );
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
        position[0].value,
        position[1].value,
        position[2].value
    };
    return pos;
}

/***************************************************************************************************
 *  \brief  Initialize Filters for Kinetic Data
 **************************************************************************************************/
void KineticUtility::InitFilters()
{
    
//    Kalman.init(&position[0], 0.0, MOTION_MAX_KALMAN_LIFE, motion_uncertainty );
//    Kalman.init(&position[1], 0.0, MOTION_MAX_KALMAN_LIFE, motion_uncertainty );
//    Kalman.init(&position[2], 0.0, MOTION_MAX_KALMAN_LIFE, motion_uncertainty );
}

///* Calculation and filtering of nongraviation data */
//void KineticUtility::Nongrav( vec3_t * n )
//{
//    Quaternion.rotVec( n, &k->qd, &k->n );
//    
//    Kalman.update( &k->filters.position[1], k->r.i, k->n.i, ACCELERATION );
//    Kalman.update( &k->filters.position[0], k->r.j, k->n.j, ACCELERATION );
//    Kalman.update( &k->filters.position[2], k->r.k, k->n.k, ACCELERATION );
//    
//    k->values.position[0] = k->filters.position[0].value;
//    k->values.position[1] = k->filters.position[1].value;
//    k->values.position[2] = k->filters.position[2].value;
//}
