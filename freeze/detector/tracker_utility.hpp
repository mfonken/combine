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
#include "opencv2/video/tracking.hpp"
#include <features2d.hpp>
#include "environment_master.hpp"

#define MAX_TRACKERS    5

#define TRACKER_MAX_KALMAN_LIFE     10.0
#define TRACKER_VALUE_UNCERTAINTY   0.01
#define TRACKER_BIAS_UNCERTAINTY    0.003
#define TRACKER_SENSOR_UNCERTAINTY  0.02

class TrackerUtility
{
    string name;
//    kalman_uncertainty_c tracker_uncertainty;
    std::vector<cv::KalmanFilter> trackers;
    
public:
    TrackerUtility( string name = "TrackerUtil" );
    void Update(std::vector<cv::Point2f> pts);
};

#endif /* tracker_utility_hpp */
