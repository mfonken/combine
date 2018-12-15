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

//#define USE_SHORTHAND_TYPES
#ifdef USE_SHORTHAND_TYPES
typedef uint8_t   uh;
typedef uint16_t  uw;
typedef uint32_t  ul;
typedef int8_t    sh;
typedef int16_t   sw;
typedef int32_t   sl;
#endif

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
//#define ALLOW_NEGATIVE_REDISTRIBUTION

#define BOUNDU(X,MAX)    ((X>MAX)?(MAX-1):X)         // Bound in upper range
#define BOUND(X,MIN,MAX) ((X<MIN)?MIN:BOUNDU(X,MAX)) // Bound in upper and lower range

static double timestamp(void)
{
    //    struct timeval stamp;
    //    gettimeofday(&stamp, NULL);
    //    return stamp.tv_sec + stamp.tv_usec/1000000.0;
    return 1; ///TODO: Implement local time
}

#endif /* rho_global_h */
