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

ImageUtility::ImageUtility() : cam(0)
{
    Mat image, frame;
    counter = 0;
    
    width = CAM_WIDTH;
    height = CAM_HEIGHT;
    size.width = FNL_RESIZE_W;
    size.height = FNL_RESIZE_H;
    cam.set(CV_CAP_PROP_FRAME_WIDTH, width);
    cam.set(CV_CAP_PROP_FRAME_HEIGHT, height);
    
    
    
    if (!cam.isOpened()) cout << "cannot open camera" << endl;
    cam.read(frame);
    Mat temp(size, CV_8UC3, Scalar(0,0,0));
    outframe = temp;
    printf("Initializing Camera: (%d, %d)\n", frame.cols, frame.rows);
    
    width  = size.width;
    height = size.height;
    
#ifdef GREYSCALE
    Mat grey;
    cv::cvtColor(frame, grey, cv::COLOR_BGR2GRAY);
    frame = grey;
#endif
    
    live = true;
}

void ImageUtility::loop(char c)
{
    if (c == 03) printf("Next frame.\n");
    else if (c == 02)
    {
        counter -= 2;
        if( counter < 0 ) counter += num_frames;
        printf("Last frame. %d\n ", counter);
    }
    else if (c == ' ') live = !live;
    usleep(live?FRAME_DELAY_MS:60000);
//    waitKey(live?FRAME_DELAY_MS:60000);
}

Mat ImageUtility::getNextFrame()
{
    Mat image(size, CV_8UC3, Scalar(0,0,0)), frame(size, CV_8UC3, Scalar(0,0,0)), temp(size, CV_8UC3, Scalar(0,0,0));

    cam >> image;
    resize(image, frame, size, 1, 1);
//        invfisheye(temp,frame);
//    imshow("Original", frame);
    
#ifdef REDSCALE
    Mat bgr[3];   //destination array
    split(frame,bgr);
    frame = bgr[2];
//    imshow("R", bgr[2]);
//    imshow("G", bgr[1]);
//    imshow("B", bgr[0]);
#endif
    
#ifdef GREYSCALE
    Mat grey;
    cv::cvtColor(frame, grey, cv::COLOR_BGR2GRAY);
    frame = grey;
#endif
    
    return frame;
}

int ImageUtility::getWidth()
{
    return width;
}
int ImageUtility::getHeight()
{
    return height;
}
int ImageUtility::getIteration()
{
    return iteration;
}
int ImageUtility::getCounter()
{
    return counter;
}
bool ImageUtility::isLive()
{
    return live;
}

Vec3b preset(BRIGHTNESS,BRIGHTNESS,BRIGHTNESS);

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


void ImageUtility::getBeacons()
{
    Mat frame = getNextFrame();
#ifdef OPENCV_THRESHOLD
    Mat threshed_frame(Size(frame.cols, frame.rows), CV_8UC3, Scalar(0,0,0)),
    out(Size(frame.cols, frame.rows), CV_8UC3, Scalar(0,0,0));
    cv::threshold(frame, threshed_frame, THRESHOLD_, 255, THRESH_BINARY);
    //                frame = threshed_frame;
#endif
    //                imshow("Frame", threshed_frame);
    
    std::vector<KeyPoint> kps;
    kps = tra.detect(threshed_frame, out);
    
#define TEXT_OFFSET_X 0//-26
#define TEXT_OFFSET_Y 0// 18
#define DETECT_BORDER_OFFSET 10
    std::vector<KeyPoint> gkps;
    for(int i = 0; i < kps.size(); i++)
    {
        Point pt(kps.at(i).pt);
        if(   pt.x > DETECT_BORDER_OFFSET && pt.x < ( width - DETECT_BORDER_OFFSET)
           && pt.y > DETECT_BORDER_OFFSET && pt.y < ( height - DETECT_BORDER_OFFSET))
        {
            Point opt(pt.x + TEXT_OFFSET_X, pt.y + TEXT_OFFSET_Y);
            putText(out, "+", opt, FONT_HERSHEY_SCRIPT_SIMPLEX, 2, Vec3b(0,0,255), 3, 8);
            gkps.push_back(kps.at(i));
        }
    }
    
    //                printf("Found %lu good keypoints\n", gkps.size());
    
#define STRENGTH 2.0
#define ZOOM     1.1
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
    /*************************/
#ifdef ITERATIONS
    //                times[l] = t[0]+t[1]+t[2];
#endif
    
#ifdef TILT_LINES
    double top_x, bot_x, lef_y, rig_y, tr = tan(-bno.roll * DEG_TO_RAD);
    top_x = FNL_RESIZE_W/2 - FNL_RESIZE_H/2*tr;
    bot_x = FNL_RESIZE_W/2 + FNL_RESIZE_H/2*tr;
    lef_y = FNL_RESIZE_H/2 + FNL_RESIZE_W/2*tr;
    rig_y = FNL_RESIZE_H/2 - FNL_RESIZE_W/2*tr;
    
    line(out, Point(top_x,0), Point(bot_x,FNL_RESIZE_H), Vec3b(255,245,255));
    line(out, Point(0,lef_y), Point(FNL_RESIZE_W,rig_y), Vec3b(255,245,255));
#endif
    
#ifdef SHOW_IMAGES
    putText(out, "A", Point(bea[1].x, bea[1].y), FONT_HERSHEY_PLAIN, 2, Vec3b(0,55,255), 3);
    putText(out, "B", Point(bea[0].x, bea[0].y), FONT_HERSHEY_PLAIN, 2, Vec3b(0,255,55), 3);
//    imshow("Out", out);
#endif
    outframe = out;
    loop(' ');
}

