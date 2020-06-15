//
//  point.h
//  combine_core
//
//  Created by Matthew Fonken on 1/13/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef point_h
#define point_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include "../../../../application/tau/kinetic/qmath/qmath_types.h"
    
    typedef struct
    {
        floating_t x, y, z, the, phi;
    } kpoint_t;
    
    typedef struct kpoint kpoint;
    struct kpoint
    {
        void   (*init)( kpoint_t *, floating_t, floating_t, floating_t, floating_t );
        floating_t (*dist)( kpoint_t * );
        floating_t (*disq)( kpoint_t * );
        floating_t (*angl)( kpoint_t * );
        floating_t (*anga)( kpoint_t * );
        void   (*copy)( kpoint_t *, kpoint_t * );
        void (*toVec3)( kpoint_t *, vec3_t * );
    };
    extern const kpoint KPoint;
    
#ifdef __cplusplus
}
#endif

#endif /* point_h */
