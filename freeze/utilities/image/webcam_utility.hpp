//
//  webcam_utility.hpp
//  ots-proto
//
//  Created by Matthew Fonken on 3/8/22.
//

#ifndef webcam_utility_hpp
#define webcam_utility_hpp

#include "environment_master.hpp"
#include "opencv_master.h"
#include "fisheye.h"

#define DEBUG_WU

#ifdef DEBUG_WU
#define LOG_WU(L, ...) LOG(L, "<ImageUtility> " __VA_ARGS__)
#else
#define LOG_WU(L, ...)
#endif

class WebcamUtility : public TestInterface
{
    int id;
    const std::string name;
    void init( void );
    string serialize( void );
    
    int rate;
    cv::Size size;
    cv::VideoCapture cam;
    
public:
    double frame_time_ms;
    cv::Mat raw, frame;
    pthread_mutex_t mutex;
    camera_intrinsics_t intrinsics;
    
    WebcamUtility(string n = "webcam", camera_intrinsics_t camera_intrinsics = {0}, int id = 0, int width = -1, int height = -1);
    ~WebcamUtility();
    
    void trigger( void );
    cv::Mat GetFrame();
    cv::Mat Undistort(cv::Mat);
    
    std::function<void(cv::Mat, double)> OnFrame;
};

#endif /* webcam_utility_hpp */
