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
    
    pthread_mutex_t     density_map_pair_mutex;
    pthread_mutex_t     c_mutex;
    
    RhoDetector( int, int );
    void perform( cv::Mat, PredictionPair * );
    //std::vector<cv::KeyPoint> detect( cv::Mat );
//    void updatePredictions( PredictionPair * );
};

#endif /* detector_utility_hpp */
