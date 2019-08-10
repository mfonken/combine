//
//  tau_test.cpp
//  tau+
//
//  Created by Matthew Fonken on 8/27/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "tau_test.hpp"
#include <string.h>

using namespace cv;
using namespace std;

Vec3b ap       (150, 255, 150);
Vec3b bp       (150, 150, 255);
Vec3b white    (255, 255, 255);
Vec3b red      (  0,   0, 255);
Vec3b blue     (255,   0,   0);

TauDraw::TauDraw( Tau * tau, Mat inframe )
#ifdef DRAW_RHO_MAPS
:
RMX(Size(tau->width,  RHO_MAPS_HEIGHT), CV_8UC3, white),
RMY(Size(tau->height, RHO_MAPS_HEIGHT), CV_8UC3, white)
#endif
{
    printf("Initializing Tau Drawer.\n");
    this->tau = tau;
    w = tau->width; h = tau->height;
    Mat N(h, w, CV_8UC3, Scalar(0));
    inframe.copyTo(N(Rect(0,0,w,h)));
    frame = N;
}

void TauDraw::draw(Mat M)
{
    M.copyTo(frame);
    putText(frame, "A", Point(tau->A.x, tau->A.y), FONT_HERSHEY_PLAIN, 2, Vec3b(0,55,150), 3);
    putText(frame, "B", Point(tau->B.x, tau->B.y), FONT_HERSHEY_PLAIN, 2, Vec3b(0,150,55), 3);
    
    double to = tan(tau->sys_omega), x = tau->A.x, y = tau->A.y;
    
    Point nu_A( 0, y+(x*to) ), nu_B( w, y-((w-x)*to) );
    Point up_A( x-(y*to), 0 ), up_B( x+((h-y)*to), h );
    line(frame, nu_A, nu_B, ap);
    line(frame, up_A, up_B, bp);
    
    x = w/2; y = h/2;
    Point h_A( 0, y+(x*to) ), h_B( w, y-((w-x)*to) );
    Point v_A( x-(y*to), 0 ), v_B( x+((h-y)*to), h );
    line(frame, h_A, h_B, white);
    line(frame, v_A, v_B, white);
}

void TauDraw::drawKalmans()
{
    string xks = tau->predictions.x.primary.toString();
    string yks = tau->predictions.x.secondary.toString();
    Mat dataframe(34, 960, CV_8UC3, Scalar(245,245,245));
    putText(dataframe, xks, Point(0,12), FONT_HERSHEY_PLAIN, 1, Scalar(15,15,15));
    putText(dataframe, yks, Point(0,28), FONT_HERSHEY_PLAIN, 1, Scalar(15,15,15));
    imshow("Kalman Data", dataframe);
}
