//
//  sigma_utility.hpp
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef sigma_utility_hpp
#define sigma_utility_hpp

#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "global_types.h"
#include "tau_structures.hpp"
#include "kalman_matrix_utility.hpp"

class Sigma
{
public:
    int                width;
    int                height;
    KalmanMatrixPair   kalman_matrix;
    
    Sigma(int, int);
    void perform( PeakListPair *, PredictionPair * );
    void framePrediction( PredictionPair * );
};

#endif /* sigma_utility_hpp */
