//
//  rho_wrapper.hpp
//  tau+
//
//  Created by Matthew Fonken on 3/26/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_wrapper_hpp
#define rho_wrapper_hpp

#include "tau_structures.hpp"
#include "rho_master.h"

class Rho
{
public:
    int                 width,
                        height;
    rho_utility       utility;
    pthread_mutex_t     c_mutex;
    pthread_mutex_t     density_map_pair_mutex;
    bool                backgrounding_event;
    
    Rho( int, int );
    void perform( cimage_t&, GlobalPacket * );
//    void perform( cv::Mat,   PredictionPair * );
    void Generate_Density_Map_Using_Interrupt_Model( cimage_t, bool );
};

#endif /* rho_wrapper_hpp */
