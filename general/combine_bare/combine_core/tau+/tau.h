//
//  tau.h
//  tau+
//
//  Created by Matthew Fonken on 8/25/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#ifndef tau_h
#define tau_h

#include "test_setup.h"

#include <stdio.h>
#include "global_types.h"
#include "rho.h"
#include "sigma.h"
#include "statem.h"

void initTau( tau_t * t, uint16_t width, uint16_t height );
void deinitTau( tau_t * t );
void performTau( tau_t * t, double * times, cimage_t img );

#endif /* tau_h */

#ifdef __cplusplus
}
#endif
