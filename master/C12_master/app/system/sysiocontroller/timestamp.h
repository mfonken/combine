//
//  timestamp.h
//  sensors
//
//  Created by Matthew Fonken on 8/2/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef timestamp_h
#define timestamp_h

#if __OS__ == MICRIUM && defined(__MICRIUM__)
#include "systemmanager.h"

static double TIMESTAMP(void)
{
    return (double)OS.Timestamp() / 1000.0;
}
#else
#include <sys/time.h>
<<<<<<< Updated upstream

=======
#include <stdbool.h>
>>>>>>> Stashed changes
static double TIMESTAMP(void)
{
    struct timeval stamp;
    gettimeofday(&stamp, NULL);
    return stamp.tv_sec + stamp.tv_usec/1000000.0;
}
#endif

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
