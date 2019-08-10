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

#define SIDEBAR_WIDTH 25
#define DENSITY_SCALE 1
#define MAX_BLOBS 10

#define INR(X,H)     INRLH(X,0,H)
#define INRLH(X,L,H) ((X>=H)?H-1:((X<L)?L:X))
#define OP_ALIGN(X,Y) (Y-X)

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
    int w, h, W, H;
    
    TauDraw( Tau * tau, cv::Mat );
    void drawDensitiesOnFrame(Mat);
    void drawDensityGraph(Mat M);
    void drawDensityMaps(Mat M);
    Vec3b densityColor( int v );
    int pixelDensity( Vec3b p );
    Vec3b hsv2bgr(Vec3b hsv);
    void drawKalmans();
};

#endif /* tau_test_hpp */
