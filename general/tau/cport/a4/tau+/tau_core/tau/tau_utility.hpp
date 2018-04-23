//
//  tau_utility.hpp
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef tau_utility_hpp
#define tau_utility_hpp

#include <stdio.h>
#include <stdint.h>
#include <string>

#include "image_utility.hpp"
#include "environment_master.h"
#include "rho_wrapper.hpp"
#include "state_machine_utility.h"

#define MAX_COUNT   10000

class Tau : public TestInterface
{
public:
    int id;
    std::string name;
    virtual void init( void );
    virtual void trigger( void );
    virtual std::string serialize( void );
    
    Tau( std::string, ImageUtility *, int, int );
    double perform( cv::Mat );
    double perform( cimage_t * );
    void updateThresh();
    void updatePrediction();
    void updatePredictionFromPeaks();
    
    int                 count;
    double              avg;
    
    kpoint_t            A, B;
    cimage_t            image;
    int                 thresh;
    int                 width,
                        height;
    ImageUtility      * utility;
    Rho                 rho;
    PredictionPair      predictions;
    pthread_mutex_t     predictions_mutex;
};

#endif /* tau_utility_hpp */
