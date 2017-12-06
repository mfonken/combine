//
//  image_utilities.cpp
//  tau+
//
//  Created by Matthew Fonken on 10/22/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "image_utility.hpp"

using namespace cv;
using namespace std;

#ifdef HAS_CAMERA
VideoCapture cam(1);
#endif

image_test::image_test(int argc, char * argv[])
{
    no_file = true;
    Mat image,frame;
    counter = 0;
    num_frames = 1;
    
    file = IMAGE_ROOT;
    
    if(argc == 2)
    {
        std::string file(IMAGE_ROOT);
        file.append(argv[1]);
        file.append(".bmp");
        printf("opening file: %s\n", file.c_str());
        image = imread(file, IMREAD_COLOR );
        no_file = false;
        if( image.empty() )                      // Check for invalid input
        {
            cout <<  "Could not open or find the image" << std::endl ;
            return;
        }
    }
    else if(argc > 2)
    {
        no_file = false;
        num_frames = atoi(argv[1]);
        subdir = argv[2];
        printf("Using gif with %d frames.\n",num_frames);
        counter = num_frames;
        file.append("frames/");
        file.append(argv[2]);
        //        file.append("/1.png");
        printf("opening file: %s\n", file.c_str());
        image = imread(file+"/1.png", IMREAD_COLOR );
        if( image.empty() )                      // Check for invalid input
        {
            cout <<  "Could not open or find the image" << std::endl ;
            return;
        }
    }
    
    width = CAM_WIDTH, height = CAM_HEIGHT;
    size.width = FNL_RESIZE;
    size.height = FNL_RESIZE;
    
#ifdef HAS_CAMERA
    cam.set(CV_CAP_PROP_FRAME_WIDTH, width);
    cam.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    if (!cam.isOpened()) cout << "cannot open camera" << endl;
    cam.read(frame);
    printf("Initializing Camera: (%d, %d) - ", frame.cols, frame.rows);
#else
    resize(image,frame,size);
#endif
    
    width  = frame.cols;
    height = frame.rows;
    
#ifdef GREYSCALE
    Mat grey;
    cv::cvtColor(frame, grey, cv::COLOR_BGR2GRAY);
    frame = grey;
#endif
    
    live = false;
    
    if(no_file)printf("Using webcam.\n");
    else printf("Using file.\n");
}

void image_test::loop(char c)
{
    if (c == 03) printf("Next frame.\n");
    else if (c == 02)
    {
        counter -= 2;
        if( counter < 0 ) counter += num_frames;
        printf("Last frame. %d\n ", counter);
    }
    else if (c == ' ') live = !live;
}

Mat image_test::getNextFrame()
{
    Mat image, frame(height, width, CV_8UC3, Scalar(0,0,0));
    
    if(!no_file)
    {
        if(counter > 0)
        {
#ifdef IMG_DEBUG
            printf("\tReading file...\n");
#endif
            counter %= num_frames;
            counter++;
            image = imread(file + "/" + to_string( counter ) + ".png", IMREAD_COLOR );
            if( image.empty() )
            {
                cout <<  "Could not open or find the image " << file << std::endl ;
                return frame;
            }
        }
    }
    else
    {
#ifdef HAS_CAMERA
        cam >> frame;
        imshow("Original", frame);
#endif
    }
    
#ifndef HAS_CAMERA
    resize(image,frame,size);
#endif
    
#ifdef REDSCALE
    Mat bgr[3];   //destination array
    split(frame,bgr);
    frame = bgr[2];
#endif

#ifdef GREYSCALE
    Mat grey;
    cv::cvtColor(frame, grey, cv::COLOR_BGR2GRAY);
    frame = grey;
#endif
    
    return frame;
}

int image_test::getWidth()
{
    return width;
}
int image_test::getHeight()
{
    return height;
}
int image_test::getIteration()
{
    return iteration;
}
int image_test::getCounter()
{
    return counter;
}

bool image_test::isLive()
{
    return live;
}

Vec3b preset(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS);

