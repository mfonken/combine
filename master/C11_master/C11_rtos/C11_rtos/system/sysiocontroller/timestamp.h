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

static uint32_t timestamp(void)
{
    struct timeval now;
    gettimeofday( &now, NULL );
    return (now.tv_sec + now.tv_usec/1000000.0);
};
static uint32_t timesince( uint32_t time ) { return timestamp() - time; }
static bool istimedout( uint32_t time, uint32_t timeout ) { return timesince(time) > timeout; }
#endif /* timestamp_h */
