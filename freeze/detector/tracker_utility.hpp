//
//  tracker_utility.hpp
//  ots-proto
//
//  Created by Matthew Fonken on 4/3/22.
//

#ifndef tracker_utility_hpp
#define tracker_utility_hpp

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "kalman2d.h"
#include "environment_master.hpp"
#include <algorithm>

#define MAX_TRACKERS    2

#define TRACKER_PROCESS_NOISE   1e-5
#define TRACKER_STD_MEAS        1e-2

#define TRACKER_MAX_DELTA       100

//class Tracker
//{
//public:
//    kalman2d_t kalman;
//    bool claimed;
//}

class TrackerUtility
{
    string name;
    kalman2d_t trackers[MAX_TRACKERS] = {0};
    
public:
    TrackerUtility( string name = "TrackerUtil" );
    std::vector<cv::Point2f> Update(std::vector<cv::Point2f> pts);
    std::vector<cv::Point2f> UpdateTrack(std::vector<cv::Point2f> pts);
};

#endif /* tracker_utility_hpp */
