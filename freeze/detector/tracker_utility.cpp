//
//  tracker_utility.cpp
//  ots-proto
//
//  Created by Matthew Fonken on 4/3/22.
//

#include "tracker_utility.hpp"

using namespace cv;
using namespace std;

static double dist(double a, double b)
{
    return sqrt(pow(a,2) + pow(b,2));
}


static vector<int> min_path(vector<vector<double>> g, int xl, int yl )
{
    double min_cost = MAXFLOAT;
    vector<int> min_path(yl);
    vector<int> xs(xl);
    bool valid = false;
    for( int x = 0; x < xl; x++ ) xs[x] = x;
    do
    {
        double cost = 0;
        for( int y = 0; y < yl; y++ )
            cost += g[y][xs[y]];
        if( cost < min_cost )
        {
            min_cost = cost;
            for( int x = 0; x < xl; x++ )
                min_path[x] = xs[x];
            valid = true;
        }
    } while ( std::next_permutation(xs.begin(),xs.end()) );
    return valid ? min_path : vector<int>();
}

TrackerUtility::TrackerUtility(string name)
: name( name ), trackers(MAX_TRACKERS)
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
            Kalman2D.init( tr, TRACKER_PROCESS_NOISE, TRACKER_STD_MEAS, TRACKER_STD_MEAS );
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
    vector<Point2f> r_pts(pts.size());
    vector<Point2f> matched(pts.size(), Point2f(-1.0, -1.0));
    
    vector<vector<double>> graph;
    
    for( int ti = 0; ti < MAX_TRACKERS; ti++ )
    { // Reset un-updated trackers
        kalman2d_t * tr = &trackers[ti];
        if( tr->t == 0 )
            continue;
        double lifespan_s = TIMESTAMP(TIME_SEC) - tr->t;
        if( lifespan_s > TRACKER_MAX_LIFESPAN_S )
            tr->t = 0;
    }
    
    for( int pi = 0; pi < pts.size(); pi++ )
    {
        Point2f pt = pts[pi];
        vector<double> ds(MAX_TRACKERS);
        double d = MAXFLOAT;
        bool claimed = false;
        for( int ti = 0; ti < MAX_TRACKERS; ti++ )
        {
            kalman2d_t * tr = &trackers[ti];
            d = MAXFLOAT;
            if( tr->t > 0)
            {
                physical_2d_t p;
                Kalman2D.test( tr, (floatp*)&p.px, true );
                d = dist(pt.x - p.px, pt.y - p.py);
                if( d > TRACKER_MAX_DELTA )
                    d = MAXFLOAT;
                else
                    claimed = true;
            }
            ds[ti] = d;
        }
        if( !claimed )
        {
            for( int ti = 0; ti < MAX_TRACKERS; ti++ )
            { // Initialize next tracker
                kalman2d_t * tr = &trackers[ti];
                if( tr->t == 0 )
                {
                    Kalman2D.init( tr, TRACKER_PROCESS_NOISE, TRACKER_STD_MEAS, TRACKER_STD_MEAS );
                    tr->state.px = pt.x;
                    tr->state.py = pt.y;
                    ds[ti] = 0;
                    break;
                }
            }
        }
        graph.push_back(ds);
    }
    
    vector<int> match = min_path(graph, MAX_TRACKERS, (int)pts.size());
    for( int pi = 0; pi < match.size(); pi++ )
    {
        Point2f pt = pts[pi];
        floatp p[2] = { pt.x, pt.y };
        int ti = match[pi];
        kalman2d_t * tr = &trackers[ti];
        Kalman2D.predict( tr );
        Kalman2D.update( tr, p );
        r_pts[ti] = Point2f(tr->state.px, tr->state.py);
//        printf("%d ", match[pi]);
    }
//    printf("\n");
    return r_pts;
}

void TrackerUtility::reorder()
{
    std::sort(trackers.begin(), trackers.end(), [](kalman2d_t a, kalman2d_t b) {
        if( a.t > 0)
        {
            double da = sqrt( a.state.px * a.state.px + a.state.py * a.state.py );
            double db = sqrt( b.state.px * b.state.px + b.state.py * b.state.py );
            return da < db;
        }
        else
            return false;
    });
}
