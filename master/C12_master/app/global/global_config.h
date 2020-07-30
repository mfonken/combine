//
//  global_config.h
//  C12_rtos
//
//  Created by Matthew Fonken on 1/7/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#ifndef global_config_h
#define global_config_h

#include "global_log.h"

//#define __MICRIUM__
#define __PAPI__         EMLIB
#define __PLATFORM__     EFR32
#ifdef __MICRIUM__
#define __OS__             MICRIUM
#endif

#define __KALMAN__

#define INCLUDE_FULL_GAUSSIAN_1D

#define GLOBAL_LOG_LEVEL    DEBUG_1

#define KALMAN_DEBUG        DEBUG_1
#define KALMAN_DEBUG_2      DEBUG_1
#define SYSTEM_DEBUG        DEBUG_2
#define SYSTEM_DEBUG_2      DEBUG_1
#define SYSTEM_DEBUG_QUEUE  DEBUG_2

#define PAPI_DEBUG          DEBUG_2
#define OSI_DEBUG           DEBUG_2
#define IO_CTL_DEBUG        DEBUG_1
#define COMM_DEBUG          DEBUG_1

#ifdef KALMAN_DEBUG
#define LOG_KALMAN(L,...)       LOG(L,"<Kalman> " __VA_ARGS__)
#else
#define LOG_KALMAN(...)
#endif

#ifdef SYSTEM_DEBUG
#define LOG_SYSTEM(L,...)       LOG(L,"<System> " __VA_ARGS__)
#define LOG_SYSTEM_BARE(L,...)  LOG_BARE(L, __VA_ARGS__)
#else
#define LOG_SYSTEM(...)
#define LOG_SYSTEM_BARE(...)
#endif

#ifdef PAPI_DEBUG
#define LOG_PAPI(L,...)          LOG(L,"<PAP> " __VA_ARGS__)
#define LOG_PAPI_BARE(L,...)     LOG_BARE(L, __VA_ARGS__)
#else
#define LOG_PAPI(...)
#define LOG_PAPI_BARE(...)
#endif

#ifdef OSI_DEBUG
#define LOG_OSI(L,...)          LOG(L,"<OSI> " __VA_ARGS__)
#define LOG_OSI_BARE(L,...)     LOG_BARE(L, __VA_ARGS__)
#else
#define LOG_OSI(...)
#define LOG_OSI_BARE(...)
#endif

#ifdef IO_CTL_DEBUG
#define LOG_IO_CTL(L,...)       LOG(L,"<IO/CTL> " __VA_ARGS__)
#else
#define LOG_IO_CTL(...)
#endif

#ifdef COMM_DEBUG
#define LOG_COMM(L,...)       LOG(L,"<COMM> " __VA_ARGS__)
#define LOG_COMM_BARE(L,...)     LOG_BARE(L, __VA_ARGS__)
#else
#define LOG_COMM(...)
#define LOG_COMM_BARE(L,...)
#endif

#define FRAME_WIDTH         1280
#define FRAME_HEIGHT        800


#endif /* global_config_h */
