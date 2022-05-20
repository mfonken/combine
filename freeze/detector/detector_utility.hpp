//
//  detector_utility.hpp
//  tau+
//
//  Created by Matthew Fonken on 4/14/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef detector_utility_hpp
#define detector_utility_hpp

#include <stdio.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <features2d.hpp>
#include "environment_master.hpp"
#include "tracker_utility.hpp"
#include <algorithm>

#define KEYPOINTS_COLOR Scalar(0,0,255)
#define BLOBS_COLOR Scalar(255,255,0)
#define DEFAULT_COVERAGE        60

class RhoDetector// : public TestInterface
{
//    int id;
    const std::string name;
//    void init( void );
//    string serialize( void );
    
public:
    cv::SimpleBlobDetector::Params params;
    std::vector<cv::KeyPoint> keypoints;
    cv::Ptr<cv::SimpleBlobDetector> detector;
    TrackerUtility tracker;
    pthread_mutex_t pts_mutex;
    
    int threshold_value;
    
//    int Cx;
//    int Cy;
//    int Q[4];
//    int QT;
//    double QF, FT;
    
    cv::Mat image;
    std::vector<cv::Point2f> pts;
    std::vector<cv::Point2f> centroids;
    
    RhoDetector( string name = "DetectorUtil" );
//    void trigger( void );
    void perform( cv::Mat );
    void calcCentroids(cv::Mat M);
    void draw(cv::Mat);
    std::vector<cv::Point2f> GetKeypoints();
};

#endif /* detector_utility_hpp */
