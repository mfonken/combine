//
//  open.hpp
//  combine_core
//
//  Created by Matthew Fonken on 12/3/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef open_hpp
#define open_hpp

#include <stdio.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <features2d.hpp>

using namespace cv;
using namespace std;

class open_t
{
private:
    SimpleBlobDetector::Params params;
    
    vector<KeyPoint> keypoints;
    Ptr<SimpleBlobDetector> detector;
    
    void invertMat(Mat I, Mat O)
    {
        O = cv::Scalar::all(255) - I;
    }
    
public:
    open_t()
    {
        params.minDistBetweenBlobs = 10; //Minimum distance between blobs
        params.filterByCircularity = false;
        params.filterByConvexity = false;
        params.filterByInertia = false;
        
        params.filterByArea = true; // filter by area of the blob
        params.minArea = 50 ;// Minimum area of the blob
        params.maxArea = 100000; // Maximum area of the blob
        params.filterByColor = true;
        params.blobColor = 255;
        detector = SimpleBlobDetector::create(params);
    }
    
    std::vector<KeyPoint> detect( Mat M, Mat R)
    {
        detector->detect( M, keypoints);
        drawKeypoints( M, keypoints, R, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
        return keypoints;
    }
    std::vector<KeyPoint> getKeypoints()
    {
        return keypoints;
    }
};

#endif /* open_hpp */
