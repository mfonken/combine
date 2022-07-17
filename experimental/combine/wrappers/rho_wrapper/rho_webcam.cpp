//
//  rho_webcam.cpp
//  ots-proto
//
//  Created by Matthew Fonken on 7/16/22.
//

#include "rho_webcam.hpp"

int id;
const std::string name;

RhoWebcamUtility::RhoWebcamUtility(string n, camera_intrinsics_t * camera_intrinsics, int id, int width, int height)
: cam("webcam", camera_intrinsics, id, width, height),
rho_wrapper(width, height)
{
    if( pthread_mutex_init(&mutex, NULL) != 0 )
        printf( "mutex init failed\n" );
    cimageInit( cimage, width, height );
}
void RhoWebcamUtility::init( void )
{
    
}

std::string RhoWebcamUtility::serialize()
{
    return this->name;
}

void RhoWebcamUtility::trigger( void )
{LOCK(&mutex)
    cam.trigger();
    
    { LOCK(&cam.mutex)
        cimageFromMat( cam.frame, cimage );
    }
    
    rho_wrapper.Perform( cimage, &packet );
}
