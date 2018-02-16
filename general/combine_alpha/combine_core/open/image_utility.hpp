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

#include "environment_master.h"

#include "unfisheye.hpp"
#include "open.hpp"
#include "kinetic.h"

#define TILT_LINES

using namespace cv;

class ImageUtility : public TestInterface
{
public:
    int id;
    std::string name;
    virtual void init( void );
    virtual void trigger( void );
    virtual string serialize( void );
    
    VideoCapture cam;
    Mat outframe, frame, image;
    
    ImageUtility( std::string );
    ImageUtility( std::string, std::string, int, int);
    ImageUtility( std::string, std::string, int, int, int );
    
    void initFile();
    void initCamera();
    
    Mat getNextImage();
    Mat getImage();
    void loop(char c);
    
    Mat getNextFrame();
    bool isLive();
    void getBeacons();
    void drawOutframe();
    
    kpoint_t bea[2];
    open_t    tra;
    
private:
    std::string file, subdir;
    int iteration;
    int counter;
    int args;
    bool live;
    bool has_file;
    int num_frames;
    
    cv::Size size;
    
};

bool thresh(uint8_t t, Vec3d p);

void drawPosition(double x, double y, double z);

#endif /* image_utilities_hpp */
