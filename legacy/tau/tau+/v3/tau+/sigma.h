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
    
extern kmat_t kmat;
extern prediction_pair_t loc_k;
    
void initSigma(     uint16_t w, uint16_t h );
void getSigmaData(  kmat_pair_t * m );
void performSigma( peak_list_pair_t * p, prediction_pair_t * d );
void framePrediction(                    prediction_pair_t * d );
    
#endif /* sigma_h */

#ifdef __cplusplus
}
#endif
