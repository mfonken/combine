//
//  tau_test.hpp
//  tau+
//
//  Created by Matthew Fonken on 8/27/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef tau_test_hpp
#define tau_test_hpp

#include "test_setup.h"

#include "utility_functions.hpp"

#include "tau_structures.hpp"
#include "rho_utility.hpp"

#include <stdio.h>
#include <stdint.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>

#include "tau_utility.hpp"


//#define DRAW_RHO_MAPS
#define RHO_MAPS_HEIGHT 100
#define RHO_MAPS_INSET 10

using namespace cv;
//
class TauDraw
{
public:
    cv::Mat image, frame, RMX, RMY;
    Tau * tau;
    int w, h;
    
    TauDraw( Tau * tau, cv::Mat );
    void draw(Mat);
    void drawKalmans();
};

#endif /* tau_test_hpp */
