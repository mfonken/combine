//
//  tau_utility.hpp
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef tau_utility_hpp
#define tau_utility_hpp

#define AVERAGE_COUNT 10

#include <stdio.h>
#include <stdint.h>
#include <string>

#include "rho_drawer_xprm.hpp"
#include "environment_master.h"
#include "image_utility.hpp"
#include "rho_wrapper.hpp"

#define MAX_COUNT               1000

class Tau : public TestInterface
{
public:
    int id;
    const char * name;
    virtual void init( void );
    virtual void trigger( void );
    virtual std::string serialize( void );
    
    Tau( const char * name, int width, int height, std::string f = "", int num = 0, std::string n = "ImageUtility" );
    virtual ~Tau();
    
    double perform( cv::Mat );
    double perform( cimage_t & );
    void updateThresh();
    void updatePrediction();
    
    int                 count,
                        accuracy_count;
    double              avg,
                        accuracy,
                        current_accuracy;
    
    kpoint_t            A, B;
    cimage_t            image;
    int                 thresh,
                        tick,
                        width,
                        height;
    ImageUtility        utility;
    Rho                 rho;
    GlobalPacket        packet;
    pthread_mutex_t     predictions_mutex,
                      * utility_mutex;
#ifdef __PSM__
    RhoDrawer           rho_drawer;
#endif
};

#endif /* tau_utility_hpp */
