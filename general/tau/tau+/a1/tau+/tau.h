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

void initTauA( tau_t * t, uint16_t width, uint16_t height );
void performTauA( tau_t * t, double * times, pixel_base_t ** img );

#endif /* tau_h */

#ifdef __cplusplus
}
#endif
