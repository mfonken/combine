//
//  timestamp.h
//  tau+
//
//  Created by Matthew Fonken on 3/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef timestamp_h
#define timestamp_h

#ifdef __cplusplus
extern "C" {
#endif

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

double TIMESTAMP(TIME_UNIT unit);
double TIMESTAMP_MS(void);
bool ISTIMEDOUT( double check, double time_out, TIME_UNIT unit );

#ifdef __cplusplus
}
#endif

#endif /* timestamp_h */
