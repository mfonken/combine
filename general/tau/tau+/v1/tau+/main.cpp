//
//  main.cpp
//  tau+
//
//  Created by Matthew Fonken on 8/23/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include <stdio.h>
#include <stdint.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <string>

#include "global_types.h"
#include "tau_test.hpp"
#include "tau.h"
#include "rho.h"
#include <sys/time.h>

#define IMAGE_ROOT "/Users/matthewfonken/Desktop/marbl/xprm/Programs/Algorithms/Image Processing/tau/tau+/tau+/images/"

#define FRAME_DELAY_MS 30

#define CAM_WIDTH   640
#define CAM_HEIGHT  320

using namespace cv;
using namespace std;

int main( int argc, char * args[] )
{
    struct timeval stop, start;
    bool no_file = true;
    Mat image,frame;
    int gif_loop = -1;
    int NUM_GIF_FRAMES = 1;
    
    string subdir("old");
    if(argc == 2)
    {
        std::string file(IMAGE_ROOT);
        file.append(args[1]);
        file.append(".bmp");
        printf("opening file: %s\n", file.c_str());
        image = imread(file, IMREAD_COLOR );
        no_file = false;
        if( image.empty() )                      // Check for invalid input
        {
            cout <<  "Could not open or find the image" << std::endl ;
            return -1;
        }
    }
    else if(argc > 2)
    {
        no_file = false;
        NUM_GIF_FRAMES = atoi(args[1]);
        subdir = args[2];
        printf("Using gif with %d frames.\n",NUM_GIF_FRAMES);
        gif_loop = NUM_GIF_FRAMES;
        std::string file(IMAGE_ROOT);
        file.append("frames/");
        file.append(subdir);
        file.append("/1.png");
        image = imread(file, IMREAD_COLOR );
        if( image.empty() )                      // Check for invalid input
        {
            cout <<  "Could not open or find the image" << std::endl ;
            return -1;
        }
    }
    
    Size size(FNL_RESIZE,FNL_RESIZE);
    if(!no_file) resize(image,frame,size);
    
    int width = CAM_WIDTH, height = CAM_HEIGHT;
    VideoCapture cam(0);
    cam.set(CV_CAP_PROP_FRAME_WIDTH, width);
    cam.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    if (!cam.isOpened()) cout << "cannot open camera" << endl;
    
    if(no_file)
    {
        cam.read(frame);
        printf("Init-ing camera: (%d, %d)\n", frame.cols, frame.rows);
    }
    
    width  = frame.cols;
    height = frame.rows;
    
    printf("Using frame size %dx%d.\n", width, height);
    
    pixel_base_t **pixels;
    initImg(&pixels, width, height);
    
    density_map_pair_t      d;
    peak_list_pair_t        p;
    probability_list_pair_t r;
    prediction_pair_t       e;

    cv::Point textOrg(3, 18);
    int fontFace = FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.5;
    int thickness = 1;
    
    initTauA( width, height );

    
    bool live = false;
    for(int l=0;l<1;) {
        
        Mat out(height, width, CV_8UC3, Scalar(0,0,0));
        if(no_file) cam.read(frame);
        else if(gif_loop > 0)
        {
            gif_loop %= NUM_GIF_FRAMES;
            gif_loop++;
            std::string file(IMAGE_ROOT);
            file.append("frames/");
            file.append(subdir);
            file.append("/");
            file.append(to_string( gif_loop ));
            file.append(".png");
//            printf("Opening frame #%d.\n", gif_loop);
            image = imread(file, IMREAD_COLOR );
            if( image.empty() )                      // Check for invalid input
            {
                cout <<  "Could not open or find the image " << file << std::endl ;
                return -1;
            }
            resize(image,frame,size);
        }
        
        MatToCimage(width, height, frame, pixels);
        cimageToBandW( width, height, pixels, out );

        /* Tau A Start */
        gettimeofday(&start, NULL);
        performTauA(pixels, &e);
        gettimeofday(&stop, NULL);
        /* Tau A End */
        
        
        
        double seconds  = stop.tv_sec  - start.tv_sec;
        double useconds = stop.tv_usec - start.tv_usec;
        double diff = ((seconds) + useconds/1000000.0) + 0.0005;
        int fps  = 1/diff;
    
        string text = "FPS:" + to_string(fps);
        cv::putText(out, text, textOrg, fontFace, fontScale, Scalar::all(255), thickness,8);
        
        getRhoData(&d, &p, &r);
        drawTau(out, &d, &p, &r, &e);
        //if (waitKey(FRAME_DELAY_MS) >= 0) break;
        char c = waitKey(live?FRAME_DELAY_MS:50000);
        if (c == 03) printf("Next frame.\n");
        else if (c == 02)
        {
            gif_loop -= 2;
            if( gif_loop < 0 ) gif_loop += NUM_GIF_FRAMES;
            printf("Last frame. %d\n ", gif_loop);
        }
        else if (c == ' ') live = !live;
    }
    return 0;
}
