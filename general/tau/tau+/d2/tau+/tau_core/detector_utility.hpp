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

#include "tau_structures.hpp"

#define KEYPOINTS_COLOR Scalar(0,0,255)

class RhoDetector
{
public:
    cv::SimpleBlobDetector::Params params;
    std::vector<cv::KeyPoint> keypoints;
    cv::Ptr<cv::SimpleBlobDetector> detector;
    
    int                 width,
    height,
    Cx, Cy,
    Q[4], QT;
    double              QF, FT;
    
    cv::Mat             image;
    
    RhoDetector( int, int );
    void perform( cv::Mat, PredictionPair * );
};

#endif /* detector_utility_hpp */
