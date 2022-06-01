//
//  timestamp.h
//  tau+
//
//  Created by Matthew Fonken on 3/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef timestamp_h
#define timestamp_h

#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>

#define TIMESTAMP_IS_SEC    0
#define TIMESTAMP_IS_MS     1
#define TIMESTAMP_IS_US     2

#ifndef TIMESTAMP_UNITS
#define TIMESTAMP_UNITS TIME_MS
#endif

typedef enum
{
    TIME_SEC = 0,
    TIME_MS,
    TIME_US
//    TIME_NS
} TIME_UNIT;

static double TIMESTAMP(TIME_UNIT unit)
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

static bool ISTIMEDOUT( double check, double time_out, TIME_UNIT unit )
{
    return ( TIMESTAMP(unit) - check ) > time_out;
}

#endif /* timestamp_h */
