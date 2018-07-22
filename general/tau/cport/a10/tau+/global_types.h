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

#define DNI
#define LOG(...) printf(__VA_ARGS__)

#ifdef MAIN_DEBUG
#define LOG_MAIN(...) LOG("Main: " __VA_ARGS__)
#else
#define LOG_MAIN(...)
#endif

#ifdef TAU_DEBUG
#define LOG_TAU(...) LOG("Tau: " __VA_ARGS__)
#else
#define LOG_TAU(...)
#endif

#ifdef RHO_DEBUG
#define LOG_RHO(...) LOG("Rho: " __VA_ARGS__)
#else
#define LOG_RHO(...)
#endif

#ifdef STATEM_DEBUG
#define LOG_STATEM(...) LOG("State Machine: " __VA_ARGS__)
#else
#define LOG_STATEM(...)
#endif

#ifdef IMUT_DEBUG
#define LOG_IMUT(...) LOG("Image Utility: "n__VA_ARGS__)
#else
#define LOG_IMUT(...)
#endif

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
