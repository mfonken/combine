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
    
    threshold_value = 127;
    
    if( pthread_mutex_init(&pts_mutex, NULL) != 0 )
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

//static int KeyPointBrightness(Mat M, KeyPoint k)
//{
//    int b = 0, c = 0;
//    int l = (int)(k.size * 0.707);
//    for(int yi = 0, y = max((int)(k.pt.y - l/2), 0); yi < l && y < M.rows; yi++, y++ )
//    {
//        for(int xi = 0, x = max((int)(k.pt.x - l/2), 0); xi < l && x < M.cols; xi++, x++ )
//        {
//            auto p = M.at<Vec3b>(y, x);
//            auto pb = p[0] + p[2];
//            b += (pb - b) / ++c;
//        }
//    }
//    return b;
//}
//
//static vector<int> GetBrightnessOfKeyPoints(Mat M, vector<KeyPoint> kpts)
//{
//    vector<int> brightnesses;
//    for(auto &k : kpts)
//    {
//        int b = KeyPointBrightness(M, k);
//        brightnesses.push_back(b);
//        k.size = b;
//    }
//    return brightnesses;
//}

void RhoDetector::perform( Mat M )
{
    vector<KeyPoint> points;
    Mat Mb;
    detector->detect( M, points );
    
    size_t num_points = points.size();
    
    if( num_points < 2 ) return;
//    vector<int> brightnesses = GetBrightnessOfKeyPoints(M, points);
    sort( points.begin(), points.end(), sizeSort );
    
//    float total_size = DEFAULT_COVERAGE;
//    for( size_t i = 0; i < num_points; i++ )
//        total_size += points.at(i).size;

    keypoints.clear();
    keypoints.push_back(points[0]);
    keypoints.push_back(points[1]);
    
    std::vector<cv::Point2f> pts_;
    for(int i = 0; i < keypoints.size(); i++)
    {
        pts_.push_back(keypoints[i].pt);
    }
    { LOCK(&pts_mutex)
        pts = tracker.UpdateTrack(pts_);
        
        for(int i = 0; i < pts.size(); i++)
        {
            keypoints[i].pt.x = std::clamp(pts[i].x, 0.0f, (float)M.cols);
            keypoints[i].pt.y = std::clamp(pts[i].y, 0.0f, (float)M.rows);
        }
    }
}

#define radius 10
void RhoDetector::draw(cv::Mat M)
{
    if(keypoints.size() == 0) return;
    
    vector<Point2f> pts;
    drawKeypoints( M, keypoints, M, BLOBS_COLOR, DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
    for(int i = 0; i < keypoints.size(); i++)
    {
        keypoints[i].size = radius;
        pts.push_back(keypoints[i].pt);
    }
    drawKeypoints( M, keypoints, M, KEYPOINTS_COLOR, DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
    
//    vector<Point2f> u_pts = tracker.Update(pts);
//    int markerSize = 10;
//    for(int i = 0; i < std::min((int)pts.size(), (int)MAX_TRACKERS); i++)
//    {
//        drawMarker(M, pts[i], Scalar(0, 255, 255), cv::MARKER_SQUARE, markerSize, 2);
//        drawMarker(M, u_pts[i], Scalar(0, 255, 0), cv::MARKER_STAR, markerSize*2, 2);
//    }
//
//    putText(M, "x", keypoints[0].pt, FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0));
//    putText(M, "x", keypoints[1].pt, FONT_HERSHEY_DUPLEX, 1, Scalar(0, 255, 0));
}
