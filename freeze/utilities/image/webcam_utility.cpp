//
//  webcam_utility.cpp
//  ots-proto
//
//  Created by Matthew Fonken on 3/8/22.
//

#include <opencv2/calib3d.hpp>

#include "webcam_utility.hpp"
#include "timestamp.h"
#include "fisheye.h"

using namespace cv;
using namespace std;

WebcamUtility::WebcamUtility(string n, camera_intrinsics_t camera_intrinsics, int id, int width, int height)
: name(n), id(id), size(width, height), intrinsics(camera_intrinsics)
{
    if( pthread_mutex_init(&mutex, NULL) != 0 )
        printf( "mutex init failed\n" );
}

WebcamUtility::~WebcamUtility()
{
    pthread_mutex_destroy(&mutex);
}

void WebcamUtility::init()
{
    while (!cam.isOpened())
    {
        sleep(1);
        LOG_WU(DEBUG_2, "Could not open or find camera.\n");
        cam = VideoCapture(id);
    }
    if(size.width > 0) cam.set(CAP_PROP_FRAME_WIDTH, size.width);
    if(size.height > 0) cam.set(CAP_PROP_FRAME_HEIGHT, size.height);
//    if(rate > 0) cam.set(CAP_PROP_FPS, rate);
    cam.read(raw);
    size.width = raw.cols;
    size.height = raw.rows;
    
    LOG_WU(DEBUG_2, "Initializing Camera: %dx%d @ %d fps.\n", raw.cols, raw.rows, (int)cam.get(CAP_PROP_FPS));
    
#ifdef GREYSCALE
    Mat grey;
    cv::cvtColor(image, grey, cv::COLOR_BGR2GRAY);
    image = grey;
#endif
}

void WebcamUtility::trigger()
{
    LOG_WU(DEBUG_1, "trigger\n");
    cam >> raw;
//    raw = Undistort(raw);
    frame_time_ms = TIMESTAMP(TIME_MS);
    if(raw.cols == 0) return;
    LOCK(&mutex);
    resize(raw, frame, size, 1, 1);
#ifdef REDSCALE
    Mat bgr[3];
    split(frame, bgr);
    frame = bgr[2];
#endif
    
#ifdef GREYSCALE
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
#endif
    
    if( OnFrame != NULL )
        OnFrame( frame, frame_time_ms );
}

std::string WebcamUtility::serialize()
{
    return this->name;
}

Mat WebcamUtility::GetFrame()
{
    return frame;
}

Mat WebcamUtility::Undistort(Mat m)
{
    Mat K = Mat(3, 3, CV_64FC1, &intrinsics.K);
    Mat D = Mat(4, 1, CV_64FC1, &intrinsics.D);
    
    Mat map1, map2;
    initUndistortRectifyMap(K, D, cv::Mat(), K, m.size(), CV_32FC1, map1, map2);
    //create undistorted image
    Mat u;
    remap(m, u, map1, map2, cv::INTER_LINEAR);
    return u;
}
