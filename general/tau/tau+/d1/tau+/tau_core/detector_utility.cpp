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

/* RHO PIXEL CHECK */
#define RPC(X) if(X&0xf0)
#define RPCB(X,Y) {RPC(X){Q##Y++;mapy[x]++;}}

#define FOR(L) for( int i = L; i > 0; --i )
#define FORA(L,A) for( int i = L; i > 0; --i, A[i] )
#define ZDIV(X,Y) (!Y?2<<10:X/Y)

#define RHO_DIM_INFLUENCE   0.1
#define RHO_K_TARGET_IND    0.3
#define RHO_K_TARGET        RHO_K_TARGET_IND+(10/sqrt(FNL_RESIZE_H)*RHO_DIM_INFLUENCE)          //0.3
#define RHO_VARIANCE_NORMAL sqrt(FNL_RESIZE_H)/5.0             //5
#define RHO_VARIANCE_SCALE  sqrt(FNL_RESIZE_H)/3.0//1.32        //20

using namespace cv;
using namespace std;

RhoDetector::RhoDetector( int width, int height )
{
    printf("Initializing RhoDetector: %dx%d & [%.3f, %.3f, %.3f]\n", width, height, RHO_K_TARGET, RHO_VARIANCE_NORMAL, RHO_VARIANCE_SCALE);
    this->width  = width;
    this->height = height;

    
    pthread_mutex_init(&density_map_pair_mutex, NULL);
    pthread_mutex_init(&c_mutex, NULL);
    
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
}

static void keyPointsToPredictionPair( KeyPoint a, KeyPoint b, PredictionPair * r )
{
    r->y.primary    = a.pt.x;
    r->y.secondary  = b.pt.x;
    r->x.primary    = a.pt.y;
    r->x.secondary  = b.pt.y;
}

void RhoDetector::perform( Mat M, PredictionPair * r )
{
    vector<KeyPoint> points;
    detector->detect( M, points );
    
    size_t num_points = points.size();
//    printf("Num keypoints: %zu\n", num_points );
    
    if( num_points < 2 ) return;
    keyPointsToPredictionPair( points.at(0), points.at(1), r );
    
    float total_size = 0;
    for( size_t i = 0; i < num_points; i++ )
        total_size += points.at(i).size;
    
    float a_cov = points.at(0).size/total_size;
    float b_cov = points.at(1).size/total_size;
    float c_cov = 1 - (a_cov + b_cov);
    
    r->x.probabilities.primary   = a_cov;
    r->y.probabilities.primary   = a_cov;
    r->x.probabilities.secondary = b_cov;
    r->y.probabilities.secondary = b_cov;
    r->x.probabilities.alternate = c_cov;
    r->y.probabilities.alternate = c_cov;

    keypoints = points;
    
    drawKeypoints( M, keypoints, M, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS );
}














