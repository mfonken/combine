//
//  detector_utility.cpp
//  tau+
//
//  Created by Matthew Fonken on 4/14/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "detector_utility.hpp"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>

using namespace cv;
using namespace std;

RhoDetector::RhoDetector( string name )
: name(name)
{
    params.minDistBetweenBlobs  = 10; //Minimum distance between blobs
    params.filterByCircularity  = false;
    params.filterByConvexity    = false;
    params.filterByInertia      = false;
    params.filterByArea         = true; // filter by area of the blob
    params.minArea              = 50 ;// Minimum area of the blob
    params.maxArea              = 100000; // Maximum area of the blob
    params.filterByColor        = true;
    params.blobColor            = 255;
    detector = SimpleBlobDetector::create(params);
    
    if( pthread_mutex_init(&mutex, NULL) != 0 )
        printf( "mutex init failed\n" );
}

//void RhoDetector::init( void )
//{
//
//}
//
//string RhoDetector::serialize()
//{
//    return this->name;
//}
//
//void RhoDetector::trigger()
//{
//
//}

static bool sizeSort( KeyPoint a, KeyPoint b ) { return a.size > b.size; }

//static void keyPointsToPredictionPair( KeyPoint a, KeyPoint b, PredictionPair * r )
//{
//    r->y.primary.value    = a.pt.x;
//    r->y.secondary.value  = b.pt.x;
//    r->x.primary.value    = a.pt.y;
//    r->x.secondary.value  = b.pt.y;
//}

void RhoDetector::perform( Mat M )
{
    vector<KeyPoint> points;
    detector->detect( M, points );
    
    size_t num_points = points.size();
    
    if( num_points < 2 ) return;
    sort( points.begin(), points.end(), sizeSort );
//    keyPointsToPredictionPair( points.at(0), points.at(1), r );
    
    float total_size = DEFAULT_COVERAGE;
    for( size_t i = 0; i < num_points; i++ )
        total_size += points.at(i).size;

//    float a_cov = points.at(0).size/total_size;
//    float b_cov = points.at(1).size/total_size;
//    float c_cov = 1 - ( a_cov + b_cov );
//
//    r->x.probabilities.primary   = a_cov;
//    r->y.probabilities.primary   = a_cov;
//    r->x.probabilities.secondary = b_cov;
//    r->y.probabilities.secondary = b_cov;
//    r->x.probabilities.alternate = c_cov;
//    r->y.probabilities.alternate = c_cov;

    keypoints = points;
    
    { LOCK(&mutex)
        pts.clear();
        for(int i = 0; i < keypoints.size(); i++)
        {
            pts.push_back(keypoints[i].pt);
            std::vector<cv::Point2f> pts;
        }
    }
}

void RhoDetector::calcCentroids(cv::Mat M)
{
//    centroids.clear();
//
//    Moments m = cv::moments(keypoints[i], false);
//    { mc[i] = Point2f( mu[i].m10/mu[i].m00 , mu[i].m01/mu[i].m00 ); }
//    centroids.push_back(keypoints)
}

void RhoDetector::draw(cv::Mat M)
{
    keypoints[0].size = 5;
    keypoints[1].size = 5;
    drawKeypoints( M, keypoints, M, KEYPOINTS_COLOR, DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
//    putText(M, "x", keypoints[0].pt, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
//    putText(M, "x", keypoints[1].pt, FONT_HERSHEY_COMPLEX, 1, Scalar(0, 255, 0));
}
