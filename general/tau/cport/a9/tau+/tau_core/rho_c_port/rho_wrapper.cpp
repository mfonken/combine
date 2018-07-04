//
//  rho_wrapper.cpp
//  tau+
//
//  Created by Matthew Fonken on 3/26/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_wrapper.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

using namespace cv;

Rho::Rho( int width, int height ) : frame(height, width, CV_8UC3, Scalar(0,0,0))
{
    printf("Initializing Rho Utility: %dx%d & [%.3f, %.3f, %.3f]\n", width, height, RHO_K_TARGET, RHO_VARIANCE_NORMAL, RHO_VARIANCE_SCALE);
    size_t
        a = sizeof(redistribution_variables),
        b = sizeof(rho_selection_variables),
        c = sizeof(prediction_update_variables),
        d = sizeof(rho_c_utility)-sizeof(int)*C_FRAME_SIZE+(2*(FNL_RESIZE_W+FNL_RESIZE_H))*sizeof(density_t),
        e = a + b + c + d;
    printf("\tSizes: RedVar-%luB SelVars-%luB PredVars-%luB Rho-%lukB > Tot-%.3fkB\n", a, b, c, d>>10, ((double)e)/1024);
    this->width  = width;
    this->height = height;
    cimageInit(&image, width, height);
    pthread_mutex_init(&density_map_pair_mutex, NULL);
    pthread_mutex_init(&c_mutex, NULL);
    
    RhoFunctions.Init(&utility, width, height);
    BayesianFunctions.sys.init( &utility.sys );
    
    backgrounding_event = false;
}

void Rho::perform( cimage_t * img, PredictionPair * p )
{
    image = *img;
    
    pthread_mutex_lock(&density_map_pair_mutex);
    pthread_mutex_lock(&c_mutex);
    /* Core Rho Functions */
    RhoFunctions.Generate_Density_Map_Using_Interrupt_Model( &utility, image, backgrounding_event );
    RhoFunctions.Perform( &utility, backgrounding_event );
    backgrounding_event = false; // Generate background always and only once
    /* * * * * * * * * * */
    pthread_mutex_unlock(&density_map_pair_mutex);
    pthread_mutex_unlock(&c_mutex);
    
    p->x.primary.value           = utility.prediction_pair.y.primary.value;
    p->x.secondary.value         = utility.prediction_pair.y.secondary.value;
    p->y.primary.value           = utility.prediction_pair.x.primary.value;
    p->y.secondary.value         = utility.prediction_pair.x.secondary.value;
    p->x.probabilities.primary   = utility.prediction_pair.y.probabilities.primary;
    p->x.probabilities.secondary = utility.prediction_pair.y.probabilities.secondary;
    p->x.probabilities.alternate = utility.prediction_pair.y.probabilities.alternate;
    p->y.probabilities.primary   = utility.prediction_pair.x.probabilities.primary;
    p->y.probabilities.secondary = utility.prediction_pair.x.probabilities.secondary;
    p->y.probabilities.alternate = utility.prediction_pair.x.probabilities.alternate;
}
