//
//  webcam_utility.cpp
//  ots-proto
//
//  Created by Matthew Fonken on 3/8/22.
//

#include "webcam_utility.hpp"

using namespace cv;
using namespace std;

WebcamUtility::WebcamUtility(string n, int id, int width, int height)
: name(n), id(id), size(width, height)
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
    if(rate > 0) cam.set(CAP_PROP_FPS, rate);
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
    LOG_WU(DEBUG_1, "WebcamUtility::trigger\n");
    cam >> raw;
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
        OnFrame( frame );
}

std::string WebcamUtility::serialize()
{
    return this->name;
}

Mat WebcamUtility::GetFrame()
{
    return frame;
}
