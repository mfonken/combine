//
//  rho_global.h
//  rho_client
//
//  Created by Matthew Fonken on 10/22/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_global_h
#define rho_global_h

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef double          floating_t;
typedef uint8_t         byte_t;
typedef uint16_t        index_t;
typedef uint16_t        density_t;
typedef int16_t         sdensity_t;
typedef int16_t         variance_t;
typedef uint32_t        density_2d_t;
//typedef uint32_t        register_t;
typedef register_t *    address_t;
typedef floating_t      timestamp_t;
typedef uint8_t         capture_t;
typedef volatile bool   flag_t;

#define byte_t_max          ( (sizeof(byte_t)       << 3 ) - 1 )
#define index_t_max         ( (sizeof(index_t)      << 3 ) - 1 )
#define density_t_max       ( (sizeof(density_t)    << 3 ) - 1 )
#define density_2d_t_max    ( (sizeof(density_2d_t) << 3 ) - 1 )

#define MAX(A,B)        ((A>B)?A:B)

#define SWAP(A,B)      ({typeof(A) temp = A; A = B; B = temp;})
//#define ALLOW_NEGATIVE_REDISTRIBUTION

#define BOUNDU(X,MAX)    ((X>MAX)?(MAX-1):X)         // Bound in upper range
#define BOUND(X,MIN,MAX) ((X<MIN)?MIN:BOUNDU(X,MAX)) // Bound in upper and lower range

#define FBOUND(X,MIN,MAX) ((X<MIN)?MIN:((X>MAX)?MAX:X))

#define USE_INTERRUPT_MODEL

//#define RHO_DEBUG
//#define STATEM_DEBUG
//#define KALMAN_DEBUG

#ifndef LOG
#define LOG(...) printf(__VA_ARGS__)
#endif

#ifdef RHO_DEBUG
#define LOG_RHO(...) LOG("<Rho> " __VA_ARGS__)
#else
#define LOG_RHO(...)
#endif

#ifdef STATEM_DEBUG
#define LOG_STATEM(...) LOG("<State> " __VA_ARGS__)
#else
#define LOG_STATEM(...)
#endif

#ifdef KALMAN_DEBUG
#define LOG_KALMAN(...) LOG("<Kalman> " __VA_ARGS__)
#else
#define LOG_KALMAN(...)
#endif

#endif /* rho_global_h */
