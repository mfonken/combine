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
static bool brightenssSort( keypoint_ext_t a, keypoint_ext_t b ) { return a.brightness > b.brightness; }

static int KeyPointBrightness(Mat M, KeyPoint k)
{
    int b = 0, c = 0;
    int l = (int)(k.size * 0.707);
    for(int yi = 0, y = max((int)(k.pt.y - l/2), 0); yi < l && y < M.rows; yi++, y++ )
    {
        for(int xi = 0, x = max((int)(k.pt.x - l/2), 0); xi < l && x < M.cols; xi++, x++ )
        {
            Vec3b p = M.at<Vec3b>(y, x);
            int pb = ((int)p[0] + (int)p[2]) / 2;
            b += (pb - b) / ++c;
//            M.at<Vec3b>(y, x) = Vec3b(b, b, b);
        }
    }
    return b;
}
//
static vector<keypoint_ext_t> GetBrightnessOfKeyPoints(Mat M, vector<KeyPoint> kpts, int min_size, int n = 100)
{
    vector<keypoint_ext_t> brightnesses;
    for(auto &k : kpts)
    {
        if(k.size < min_size) continue;
        int b = KeyPointBrightness(M, k);
        keypoint_ext_t ke = { k, b };
        brightnesses.push_back(ke);
        if(--n <= 0) break;
    }
    return brightnesses;
}

void RhoDetector::perform( Mat M )
{
    vector<KeyPoint> points;
//    Mat Mb;
    detector->detect( M, points );
    
    size_t num_points = points.size();
    
    if( num_points < 2 ) return;
//    sort( points.begin(), points.end(), sizeSort );
    vector<keypoint_ext_t> kps = GetBrightnessOfKeyPoints(M, points, DETECTION_MIN_SIZE);
    if(kps.size() < 2) return;
    sort( kps.begin(), kps.end(), brightenssSort );
//    sort( points.begin(), points.end(), sizeSort );
    
//    float total_size = DEFAULT_COVERAGE;
//    for( size_t i = 0; i < num_points; i++ )
//        total_size += points.at(i).size;

    keypoints.clear();
    keypoints.push_back(kps[0].kp);
    keypoints.push_back(kps[1].kp);
    
    std::vector<cv::Point2f> pts_;
    int n = 2;
    for(auto &kp : kps)
    {
        pts_.push_back(kp.kp.pt);
        if(--n <= 0) break;
    }
    { LOCK(&pts_mutex)
        pts = tracker.UpdateTrack(pts_);

        t_points.clear();
        for(int i = 0; i < pts.size() && i < keypoints.size(); i++)
        {
            keypoints[i].pt.x = std::clamp(pts[i].x, 0.0f, (float)M.cols);
            keypoints[i].pt.y = std::clamp(pts[i].y, 0.0f, (float)M.rows);
            
            invfisheye(&pts[i], M.cols, M.rows, UNFISHEYE_SCALE, 1.0);
            
            pts[i].x = std::clamp(pts[i].x, 0.0f, (float)M.cols);
            pts[i].y = std::clamp(pts[i].y, 0.0f, (float)M.rows);
            t_points.push_back(Point2f(pts[i].x, pts[i].y));
        }
    }
}

#define radius 10
void RhoDetector::draw(cv::Mat M)
{
    LOG_DET(DEBUG_1, "draw - %d\n", (int)keypoints.size());
    if(keypoints.size() == 0) return;
    for(int i = 0; i < keypoints.size(); i++)
    {
        if( keypoints[i].size <= 0 )
            keypoints[i].size = 0.01;
        else if( keypoints[i].size > 1000 )
            keypoints[i].size = 1000;
    }
    
    char c = 'a';
    vector<Point2f> pts;
    drawKeypoints( M, keypoints, M, BLOBS_COLOR, DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
    for(int i = 0; i < keypoints.size(); i++)
    {
        keypoints[i].size = radius;
        pts.push_back(keypoints[i].pt);
        
        line( M, (Point)t_points[i], (Point)keypoints[i].pt, Scalar(0, 25, 200));
        circle( M, (Point)t_points[i], 3, Scalar(0, 50, 255), -1);
        string s{c++};
        putText(M, s, (Point)t_points[i], FONT_HERSHEY_DUPLEX, 1.0, Scalar(0, 50, 255));
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
