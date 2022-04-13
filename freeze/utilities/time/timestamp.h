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
#define TIMESTAMP_UNITS TIMESTAMP_IS_MS
#endif

static double TIMESTAMP(void)
{
    struct timeval stamp;
    gettimeofday(&stamp, NULL);
#if TIMESTAMP_UNITS == TIMESTAMP_IS_SEC
    return stamp.tv_sec + stamp.tv_usec * 0.000001;
#elif TIMESTAMP_UNITS == TIMESTAMP_IS_MS
    return stamp.tv_sec * 1000.0 + stamp.tv_usec * 0.001;
#elif TIMESTAMP_UNITS == TIMESTAMP_IS_US
    return stamp.tv_sec * 1000000.0 + stamp.tv_usec;
#endif
}

static bool ISTIMEDOUT( double check, double time_out )
{
    return ( TIMESTAMP() - check ) > time_out;
}

#endif /* timestamp_h */
