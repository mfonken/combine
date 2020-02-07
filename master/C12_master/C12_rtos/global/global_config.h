//
//  global_config.h
//  C12_rtos
//
//  Created by Matthew Fonken on 1/7/20.
//  Copyright © 2020 Matthew Fonken. All rights reserved.
//

#ifndef global_config_h
#define global_config_h

#include "genlog.h"

#define __KALMAN__

#define INCLUDE_FULL_GAUSSIAN_1D

#define GLOBAL_LOG_LEVEL    DEBUG_1

#define KALMAN_DEBUG        DEBUG_1
#define KALMAN_DEBUG_2      DEBUG_1
#define SYSTEM_DEBUG        DEBUG_2
#define PAPI_DEBUG          DEBUG_1
#define OSI_DEBUG           DEBUG_1
#define IO_CTL_DEBUG        DEBUG_1

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

#define FRAME_WIDTH         1280
#define FRAME_HEIGHT        800

#ifndef MAX
#define MAX(A,B)                ( ( A > B ) ? A : B )
#endif

#define AVG2(A,B)               ( ( A + B ) / 2. )
#define SWAP(A,B)               { typeof(A) temp = A; A = B; B = temp; }

#define BOUNDU(X,MAX)           ( ( X > MAX ) ? MAX : X )         // Bound in upper range
#define BOUND(X,MIN,MAX)        ( ( X < MIN ) ? MIN : BOUNDU( X, MAX ) ) // Bound in upper and lower range

#define FBOUND(X,MIN,MAX)       ( ( X < MIN ) ? MIN : ( ( X > MAX ) ? MAX : X ) )

#define SQUARE(X)               ( X * X )
#define DISTANCE_SQ(X,Y)        ( SQUARE(X) + SQUARE(Y) )
#define INRANGE(X,Y,T)          ( abs( X - Y ) < T )

#ifndef ZDIV
#define ZDIV_LNUM               ( 1 << 10 )
#define ZDIV(X,Y)               ( ( Y == 0 ) ? ( X == 0 ? 0 : ZDIV_LNUM ) : X / Y )
#endif

#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

#endif /* global_config_h */
