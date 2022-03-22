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

static double TIMESTAMP(void)
{
    struct timeval stamp;
    gettimeofday(&stamp, NULL);
    return stamp.tv_sec + stamp.tv_usec/1000000.0;
}

static bool ISTIMEDOUT( double check, double time_out )
{
    return ( TIMESTAMP() - check ) > time_out;
}

#endif /* timestamp_h */
