//
//  global_types.h
//  tau+
//
//  Created by Matthew Fonken on 8/22/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef global_types_h
#define global_types_h

#ifdef __cplusplus
extern "C" {
#endif

#include "test_setup.h"
#include <stdint.h>

static void cma( double new_val, double *avg, int num ) { *avg += ( new_val - *avg ) / (double)( num + 1 ); }

#ifdef __cplusplus
}
#endif
    
#endif /* global_types_h */
