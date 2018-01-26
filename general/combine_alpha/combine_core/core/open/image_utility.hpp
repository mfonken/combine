//
//  image_utilities.hpp
//  tau+
//
//  Created by Matthew Fonken on 10/22/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef image_utilities_hpp
#define image_utilities_hpp

#include "test_setup.h"

#include <stdio.h>
#include <stdint.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <unistd.h>

#include "unfisheye.hpp"

#include "open.hpp"
#include "kinetic.h"

//#define TILT_LINES

using namespace cv;

class ImageUtility
{
private:
    int iteration;
    int counter;
    int args;
    bool live;
    bool no_file;
    int num_frames;
    
    cv::Size size;
    open_t    tra;
    
public:
    VideoCapture cam;
    Mat outframe, frame, image;
    
    ImageUtility();
    void init();
    
    Mat getNextFrame();
    bool isLive();
    void getBeacons();
    void drawOutframe( Mat );
    
    kpoint_t bea[2];
};

bool thresh(uint8_t t, Vec3d p);

void drawPosition(double x, double y, double z);

//void initImg( cimage_t i, int w, int h );
//void MatToCimage( uint16_t width, uint16_t height, Mat mat, cimage_t img, int threshold );
//void cimageToBandW( uint16_t width, uint16_t height, cimage_t img, Mat mat );

#endif /* image_utilities_hpp */
