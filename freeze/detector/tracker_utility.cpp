//
//  tracker_utility.cpp
//  ots-proto
//
//  Created by Matthew Fonken on 4/3/22.
//

#include "tracker_utility.hpp"

using namespace cv;
using namespace std;

TrackerUtility::TrackerUtility(string name)
: name( name )
{
}

//void TrackerUtility::UpdateActive()
//{
//    for( int i = 0; i < MAX_TRACKERS; i++ )
//    {
//        Kalman.init( &trackers[i], , TRACKER_MAX_KALMAN_LIFE, tracker_uncertainty );
//    }
//}

void TrackerUtility::Update(vector<Point2f> pts)
{
//    for( int i = 0; i < MAX_TRACKERS; i++ )
//    {
//        kalman_t * tr = &trackers[i];
//
//        // Check if uninitialzed
//        if( tr->timestamp == 0 )
//        {
//            Kalman.init( tr, , TRACKER_MAX_KALMAN_LIFE, tracker_uncertainty );
//        }
//    }
}
