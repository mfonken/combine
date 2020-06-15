//
//  timestamp.h
//  sensors
//
//  Created by Matthew Fonken on 8/2/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef timestamp_h
#define timestamp_h

#include <sys/time.h>

static double TIMESTAMP(void)
{
    struct timeval stamp;
    gettimeofday(&stamp, NULL);
    return stamp.tv_sec + stamp.tv_usec/1000000.0;
}

static double SECONDSSINCE( double check )
{
    return TIMESTAMP() - check;
}

static bool ISTIMEDOUT( double check, double time_out )
{
    double diff = SECONDSSINCE(check);
//    printf("∆%f c%f t%f\n", diff, check, time_out);
    return (diff > time_out);
}

#endif /* timestamp_h */
