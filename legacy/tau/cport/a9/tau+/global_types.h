//
//  global_types.h
//  tau+
//
//  Created by Matthew Fonken on 8/22/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef global_types_h
#define global_types_h

#include <stdint.h>
#include "test_setup.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef unsigned char   pixel_base_t;
    typedef struct
    {
        int width, height;
        pixel_base_t * pixels;
    } cimage_t;

    typedef float global_timestamp_t;
    typedef struct
    {
        unsigned char ID,
            includes,
            timestamp[sizeof(global_timestamp_t)];
    } GlobalPacketHeader;
    
    typedef struct
    {
        GlobalPacketHeader header;
        uint32_t padding;
        double
            px,
            py,
            sx,
            sy,
            pp,
            sp,
            ap;
    } GlobalPacket;
    
#ifdef __cplusplus
}
#endif
    
#endif /* global_types_h */
