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

#define MAX_TRACKERS    3

#define TRACKER_PROCESS_NOISE   5
#define TRACKER_STD_MEAS        1e-2

#define TRACKER_MAX_DELTA       100
#define TRACKER_MAX_LIFESPAN_S  1
//class Tracker
//{
//public:
//    kalman2d_t kalman;
//    bool claimed;
//}

class TrackerUtility
{
    string name;
    vector<kalman2d_t> trackers;
    
public:
    TrackerUtility( string name = "TrackerUtil" );
    std::vector<cv::Point2f> Update(std::vector<cv::Point2f> pts);
    std::vector<cv::Point2f> UpdateTrack(std::vector<cv::Point2f> pts);
    void reorder();
};

#endif /* tracker_utility_hpp */
