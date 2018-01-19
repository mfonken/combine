//
//  sigma.h
//  tau+
//
//  Created by Matthew Fonken on 8/22/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#ifndef sigma_h
#define sigma_h
    
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "global_types.h"
#include "kmat.h"
    
void initSigma( sigma_t * s, uint16_t width, uint16_t height );
void performSigma( sigma_t * s, peak_list_pair_t * p, prediction_pair_t * d  );
void framePrediction( sigma_t * s, prediction_pair_t * d );
    
#endif /* sigma_h */

#ifdef __cplusplus
}
#endif
