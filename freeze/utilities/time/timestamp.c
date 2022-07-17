//
//  timestamp.c
//  tau+
//
//  Created by Matthew Fonken on 3/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "timestamp.h"

double TIMESTAMP(TIME_UNIT unit)
{
    struct timeval stamp;
    gettimeofday(&stamp, NULL);
    double t = 0;
    if( unit == TIME_SEC )
        t = stamp.tv_sec + stamp.tv_usec * 1.0e-6;
    else if( unit == TIME_MS )
        t = stamp.tv_sec * 1.0e3 + stamp.tv_usec * 1.0e-3;
    else if( unit == TIME_US )
        t = stamp.tv_sec * 1.0e6 + stamp.tv_usec;
//    else if( unit == TIME_NS )
//        t = stamp.tv_sec * 1.0e9 + stamp.tv_usec * 1.0e3;
    return t;
}

double TIMESTAMP_MS()
{
    return TIMESTAMP(TIME_MS);
}

bool ISTIMEDOUT( double check, double time_out, TIME_UNIT unit )
{
    return ( TIMESTAMP(unit) - check ) > time_out;
}
