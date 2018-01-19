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
#include <sys/time.h>

#include "unfisheye.hpp"

#include "../kinetic/kinetic.h"

using namespace cv;

class image_test
{
private:
    int width;
    int height;
    int iteration;
    int counter;
    int args;
    bool live;
    bool no_file;
    int num_frames;
    
    std::string subdir;
    std::string file;
    
    cv::Size size;
    
public:
    image_test( int argc, char * argv[] );
    void loop(char c);
    cv::Mat getNextFrame();
    int getWidth();
    int getHeight();
    int getIteration();
    int getCounter();
    bool isLive();
};

bool thresh(uint8_t t, Vec3d p);

void drawPosition(double x, double y, double z);

//void initImg( cimage_t i, int w, int h );
//void MatToCimage( uint16_t width, uint16_t height, Mat mat, cimage_t img, int threshold );
//void cimageToBandW( uint16_t width, uint16_t height, cimage_t img, Mat mat );

#endif /* image_utilities_hpp */
