//
//  tau_test.hpp
//  tau+
//
//  Created by Matthew Fonken on 8/27/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef tau_test_hpp
#define tau_test_hpp

#include "tau_structures.hpp"
#include "opencv_master.h"
#include "tau_utility.hpp"

#define SIDEBAR_WIDTH 25
#define DENSITY_SCALE 1
#define MAX_BLOBS 10

#define INR(X,H)     INRLH(X,0,H)
#define INRLH(X,L,H) ((X>=H)?H-1:((X<L)?L:X))
#define OP_ALIGN(X,Y) (Y-X)

//#define DRAW_RHO_MAPS
#define RHO_MAPS_HEIGHT 200
#define RHO_MAPS_INSET 10

#define inseta_ 10
#define insetb_ 50
#define fontsize_ 1.4
#define fontcolora_ Vec3b(100,10,0)
#define fontcolorb_ Vec3b(0,100,10)
#define fontcolorc_ Vec3b(10,0,100)

using namespace cv;

class TauDrawer : public Tau
{
public:
    pthread_mutex_t drawer_mutex;
    
    cv::Mat image, frame, RMX, RMY, probabilities;
//    Tau * tau;
    int w, h, W, H;
    
    TauDrawer( const char * name, int width, int height, std::string f = "", int num = 0 );
    Mat& GetDensitiesFrame(void);
    void DrawDensityGraph(Mat);
    void DrawDensityMaps(Mat);
    void DrawRhoProbabilities(Mat);
    Vec3b densityColor( int );
    int pixelDensity( Vec3b );
    Vec3b hsv2bgr(Vec3b);
    void drawKalmans();
};

#endif /* tau_test_hpp */
