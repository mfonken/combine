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

ImageUtility::ImageUtility( std::string n ) : cam(0), image(Size(CAM_WIDTH, CAM_HEIGHT), CV_8UC3, Scalar(0,0,0)), frame(Size(FNL_RESIZE_W, FNL_RESIZE_H), CV_8UC3, Scalar(0,0,0))
{
    this->name = n;
}

void ImageUtility::init()
{
    printf("Initializing Image Utility.\n");
    counter = 0;
    
    cam.set(CV_CAP_PROP_FRAME_WIDTH,  CAM_WIDTH);
    cam.set(CV_CAP_PROP_FRAME_HEIGHT, CAM_HEIGHT);
    cam.set(CV_CAP_PROP_FPS,          CAM_FRAME_RATE);
    
    size.width = FNL_RESIZE_W;
    size.height = FNL_RESIZE_H;
    
    if (!cam.isOpened()) cout << "cannot open camera" << endl;
    cam.read(image);
    
//    outframe = image;
    printf("Initializing Camera: (%d, %d) @ %d fps\n", image.cols, image.rows, (int)cam.get(CV_CAP_PROP_FPS));

    
#ifdef GREYSCALE
    Mat grey;
    cv::cvtColor(image, grey, cv::COLOR_BGR2GRAY);
    image = grey;
#endif
    
    live = true;
}

void ImageUtility::trigger()
{
    getBeacons();
}

std::string ImageUtility::serialize()
{
    return this->name;
}

Mat ImageUtility::getNextFrame()
{
    cam >> image;
    resize(image, frame, size, 1, 1);
    
#ifdef REDSCALE
    Mat bgr[3];
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

void ImageUtility::getBeacons()
{
    Mat out = getNextFrame();
    Mat threshout(Size(CAM_WIDTH, CAM_HEIGHT), CV_8UC3, Scalar(0,0,0));

#ifdef OPENCV_THRESHOLD
    cv::threshold(out, threshout, THRESHOLD_, 255, THRESH_BINARY);
#endif
    
    std::vector<KeyPoint> kps, gkps;
    kps = tra.detect(threshout, threshout);

    for(int i = 0; i < kps.size(); i++)
    {
        Point pt(kps.at(i).pt);
        if(   pt.x > DETECT_BORDER_OFFSET && pt.x < ( size.width  - DETECT_BORDER_OFFSET)
           && pt.y > DETECT_BORDER_OFFSET && pt.y < ( size.height - DETECT_BORDER_OFFSET))
        {
            Point opt(pt.x + TEXT_OFFSET_X, pt.y + TEXT_OFFSET_Y);
            putText(out, "+", opt, FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Vec3b(0,0,255), 3, 8);
            gkps.push_back(kps.at(i));
        }
    }

    if(gkps.size() >= 2)
    {
        invfisheye(&gkps.at(0).pt, FNL_RESIZE_W, FNL_RESIZE_H, STRENGTH, ZOOM);
        invfisheye(&gkps.at(1).pt, FNL_RESIZE_W, FNL_RESIZE_H, STRENGTH, ZOOM);
        if(gkps.at(1).pt.x < gkps.at(0).pt.x)
        {
            bea[0].x = gkps.at(1).pt.x;
            bea[0].y = gkps.at(1).pt.y;
            bea[1].x = gkps.at(0).pt.x;
            bea[1].y = gkps.at(0).pt.y;
        }
        else
        {
            bea[0].x = gkps.at(0).pt.x;
            bea[0].y = gkps.at(0).pt.y;
            bea[1].x = gkps.at(1).pt.x;
            bea[1].y = gkps.at(1).pt.y;
        }
    }
    outframe = threshout;
    drawOutframe();
}

void ImageUtility::drawOutframe()
{
    putText(outframe, "A", Point(bea[1].x, bea[1].y), FONT_HERSHEY_PLAIN, 2, Vec3b(0,55,255), 3);
    putText(outframe, "B", Point(bea[0].x, bea[0].y), FONT_HERSHEY_PLAIN, 2, Vec3b(0,255,55), 3);
#ifdef TILT_LINES
    double top_x, bot_x, lef_y, rig_y, tr = tan(0/*-bno.roll * DEG_TO_RAD*/);
    top_x = FNL_RESIZE_W/2 - FNL_RESIZE_H/2*tr;
    bot_x = FNL_RESIZE_W/2 + FNL_RESIZE_H/2*tr;
    lef_y = FNL_RESIZE_H/2 + FNL_RESIZE_W/2*tr;
    rig_y = FNL_RESIZE_H/2 - FNL_RESIZE_W/2*tr;
    
    line(outframe, Point(top_x,0), Point(bot_x,FNL_RESIZE_H), Vec3b(255,245,255));
    line(outframe, Point(0,lef_y), Point(FNL_RESIZE_W,rig_y), Vec3b(255,245,255));
#endif
}

Vec3b preset(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS);

bool ImageUtility::isLive()
{
    return live;
}

void drawPosition(double x, double y, double z)
{
    int width = 800, act_width = 600, height = 600, step = 50;
    Mat P(height, width, CV_8UC3, Scalar(0,0,0));
    
    const Vec3b white(255,255,255);
    /* Draw Grid */
    for(int x = 0; x <= act_width; x+=step) for(int y = 0; y < act_width; y++) P.at<Vec3b>(y,x) = white;
    for(int y = 0; y < height; y+=step) for(int x = 0; x < act_width; x++) P.at<Vec3b>(y,x) = white;
    
    int HORZ_INSET = 100;
    
    /* Draw Beacons */
    Point A(HORZ_INSET, 200), B(HORZ_INSET, height - 200);
    int cir_rad = 5;
    const Vec3b cir_col(0,0,255);
    circle(P, A, cir_rad, cir_col, 2, 8, 0);
    circle(P, B, cir_rad, cir_col, 2, 8, 0);
    
    line(P, Point(act_width, 0), Point(act_width, height), white, 3, 8, 0);
    for(int y = 0; y < height; y+=step*2) for(int x = act_width; x < width; x++) P.at<Vec3b>(y,x) = white;
    
    double unit_scale = (B.y-A.y)/D_FIXED;
    
    /* Draw Given (X,Y) */
    Point C( y * unit_scale + HORZ_INSET, height/2 + x * unit_scale - (B.y-A.y) );
    circle(P, C, 10, Vec3b(255,100,0), 2, 8, 0);
    
    /* Draw Given Z */
    double thisZ = height/2 - z * unit_scale;
    line(P, Point(act_width, thisZ), Point(width, thisZ), Vec3b(255,100,0), 3, 10, 0);
    
    //    imshow("Position", P);
}
