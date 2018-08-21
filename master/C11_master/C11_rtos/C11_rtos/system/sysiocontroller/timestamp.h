//
//  timestamp.h
//  sensors
//
//  Created by Matthew Fonken on 8/2/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef timestamp_h
#define timestamp_h

static uint32_t timestamp(void) { return 0; };
static uint32_t timesince( uint32_t time ) { return timestamp() - time; }
static bool istimedout( uint32_t time, uint32_t timeout ) { return timesince(time) > timeout; }
#endif /* timestamp_h */
