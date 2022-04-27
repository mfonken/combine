//
//  tracker_utility.cpp
//  ots-proto
//
//  Created by Matthew Fonken on 4/3/22.
//

#include "tracker_utility.hpp"

using namespace cv;
using namespace std;

double static dist(double a, double b)
{
    return sqrt(pow(a,2) + pow(b,2));
}

TrackerUtility::TrackerUtility(string name)
: name( name )
{
}

vector<Point2f> TrackerUtility::Update(vector<Point2f> pts)
{
    vector<Point2f> r_pts;
    for( int i = 0; i < std::min((int)pts.size(), (int)MAX_TRACKERS); i++ )
    {
        kalman2d_t * tr = &trackers[i];
        
        // Check if uninitialzed
        if( tr->t == 0 )
        {
            Kalman2D.init( tr, TRACKER_PROCESS_NOISE, TRACKER_X_STD_MEAS, TRACKER_Y_STD_MEAS );
            tr->state.px = pts[i].x;
            tr->state.py = pts[i].y;
            r_pts.push_back(Point2f(pts[i].x, pts[i].y));
        }
        else
        {
            Kalman2D.predict( tr );
            Kalman2D.update( tr, (floatp[]){ (floatp)pts[i].x, (floatp)pts[i].y } );
            r_pts.push_back(Point2f(tr->state.px, tr->state.py));
        }
    }
    return r_pts;
}

vector<Point2f> TrackerUtility::UpdateTrack(vector<Point2f> pts)
{
    vector<Point2f> r_pts;
    vector<Point2f> matched(pts.size(), Point2f(-1.0, -1.0));
    for(const auto& pt : pts)
    {
        double min_d = MAXFLOAT;
        int min_i = -1;
        for( int i = 0; i < MAX_TRACKERS; i++ )
        {
            if(matched[i].x >= 0) continue;
            kalman2d_t * tr = &trackers[i];
            
            // Check if uninitialzed
            if( tr->t == 0 )
                continue;
            
            physical_2d_t p;
            Kalman2D.test( tr, (floatp*)&p.px, true );
            double d = dist(pt.x - p.px, pt.y - p.py);
            if(d > TRACKER_MAX_DELTA)
                continue;
            
            if(d < min_d)
            {
                bool claimed = false;
                for( int ii = 0; ii < matched.size(); ii++ )
                {
                    if(i == ii)
                    {
                        claimed = true;
                        break;
                    }
                }
                if(claimed)
                    continue;
                min_d = d;
                min_i = i;
            }
        }
        if(min_d == MAXFLOAT)
        {
            for( int i = 0; i < MAX_TRACKERS; i++ )
            {
                kalman2d_t * tr = &trackers[i];
                if( tr->t == 0 )
                {
                    Kalman2D.init( tr, TRACKER_PROCESS_NOISE, TRACKER_X_STD_MEAS, TRACKER_Y_STD_MEAS );
                    tr->state.px = pt.x;
                    tr->state.py = pt.y;
                    matched[i] = Point2f(pt.x, pt.y);
                    break;
                }
            }
        }
        else if(min_i >= 0)
        {
            matched[min_i] = Point2f(pt.x, pt.y);
        }
    }
    
    for( int i = 0; i < matched.size(); i++ )
    {
        if( matched[i].x >= 0 )
        {
            kalman2d_t * tr = &trackers[i];
            floatp p[2] = { matched[i].x, matched[i].y };
            Kalman2D.predict( tr );
            Kalman2D.update( tr, p );
            r_pts.push_back( Point2f(tr->state.px, tr->state.py) );
        }
    }
    return r_pts;
}
