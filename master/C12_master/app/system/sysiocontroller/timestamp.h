//
//  timestamp.h
//  sensors
//
//  Created by Matthew Fonken on 8/2/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef timestamp_h
#define timestamp_h

#include <stdbool.h>
#include "os_interface.h"

double TIMESTAMP(void);
double SECONDSSINCE( double check );
bool ISTIMEDOUT( double check, double time_out );

typedef struct
{
    double (*Stamp)(void);
    double (*SecondsSince)(double);
    bool (*IsTimedOut)(double, double);
} time_functions;

extern time_functions Time;

#endif /* timestamp_h */
