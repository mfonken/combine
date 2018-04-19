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

#include "tau_test.hpp"

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
    
    cv::Size size;

public:
    image_test( int argc, const char * argv[] );
    void loop(char c);
    cv::Mat getNextFrame();
    int getWidth();
    int getHeight();
    int getIteration();
    int getCounter();
    bool isLive();
};

#endif /* image_utilities_hpp */
