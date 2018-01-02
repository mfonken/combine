//
//  unfisheye.c
//  combine_core
//
//  Created by Matthew Fonken on 12/27/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "unfisheye.hpp"
#include <highgui.h>

using namespace cv;

double strength = 0.1, zoom = 0.5;
int strength_slider = 28, zoom_slider = 11, max_strength = 100, max_zoom = 20;

void quickUnfisheye(Mat I, Mat O)
{
    cvCreateTrackbar( "Strength", "Original", &strength_slider, max_strength );
    cvCreateTrackbar( "Zoom", "Original", &zoom_slider, max_zoom );
    strength = (double) strength_slider / 10;
    zoom = (double) zoom_slider / 10;
    
    double w = I.cols, h = I.rows;
    double wh = w/2, hh = h/2;
    int x, y = 0, x_, y_, xn, yn;
    double theta_zoom, r;
    
    double correctionRadius_ = (1 / qdist(fabs(wh), fabs(hh))) * strength;
    for(yn = -hh, y = 0; y < h; yn++, y++ )
    {
        for(xn = -wh, x = 0; x < w; xn++, x++ )
        {
            r = qdist(fabs(xn), fabs(yn)) * correctionRadius_;
            theta_zoom = qatanr_r(r) * zoom;
            x_ = wh + theta_zoom * xn;
            y_ = hh + theta_zoom * yn;
            if(INR(x_, 0, w) && INR(y_, 0, h))
                O.at<Vec3b>(y,x) = I.at<Vec3b>(y_,x_);
        }
    }
    putText(O, "Strength: " + std::to_string(strength), cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
    putText(O, "Zoom: " + std::to_string(zoom), cvPoint(30,48), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
}

void unfisheye(Mat I, Mat O)
{
    cvCreateTrackbar( "Strength", "Original", &strength_slider, max_strength );
    cvCreateTrackbar( "Zoom", "Original", &zoom_slider, max_zoom );
    
    strength = (double) strength_slider / 10;
    zoom = (double) zoom_slider / 10;
    
    double w = I.cols, h = I.rows;
    double wh = w/2, hh = h/2;
    Vec3b c = {0,0,0};
    int x, y = 0, x_, y_, xn, yn;
    double theta, r;
    
#ifdef QUICK
    double correctionRadius_ = 1 / sqrt((wh*wh)+(hh*hh)) * strength;
    for(yn = -h/2, y = 0; y < h; yn++, y++ )
    {
        for(xn = -w/2, x = 0; x < w; xn++, x++ )
        {
            r = sqrt((xn*xn)+(yn*yn)) * correctionRadius_;
            theta = qatanr_r(r) * zoom;
            x_ = wh + theta * xn;// * zoom;
            y_ = hh + theta * yn;// * zoom;
            O.at<Vec3b>(y,x) = I.at<Vec3b>(y_,x_);
        }
    }
#endif
#ifndef QUICK
#ifdef ALT_QUICK
    double correctionRadius_ = (1 / qdist(fabs(wh), fabs(hh))) * strength, thetaz;
    for(yn = -h/2, y = 0; y < h; yn++, y++ )
    {
        for(xn = -w/2, x = 0; x < h; xn++, x++ )
        {
            r = qdist(fabs(xn), fabs(yn)) * correctionRadius_;
            thetaz = qatanr_r(r) * zoom;
            x_ = wh + thetaz * xn;
            y_ = hh + thetaz * yn;
            O.at<Vec3b>(y,x) = I.at<Vec3b>(y_,x_);
        }
    }
#else
    double correctionRadius = sqrt((wh*wh)+(hh*hh)) / strength;
    for(yn = -h/2, y = 0; y < h; yn++, y++ )
    {
        for(xn = -w/2, x = 0; x < w; xn++, x++ )
        {
            r = sqrt((xn*xn)+(yn*yn)) / correctionRadius;
            if(!r) theta = 1;
            else theta = atan(r)/r;
            x_ = wh + theta * xn * zoom;
            y_ = hh + theta * yn * zoom;
            if(INR(x_, 0,w) && INR(y_, 0,h))
                O.at<Vec3b>(y,x) = I.at<Vec3b>(y_,x_);
        }
    }
#endif
#endif
    putText(O, "Strength: " + std::to_string(strength), cvPoint(30,30), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
    putText(O, "Zoom: " + std::to_string(zoom), cvPoint(30,48), FONT_HERSHEY_COMPLEX_SMALL, 0.8, cvScalar(200,200,250), 1, CV_AA);
}
