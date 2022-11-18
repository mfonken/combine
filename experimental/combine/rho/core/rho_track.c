//
//  rho_track.c
//  ots-proto
//
//  Created by Matthew Fonken on 8/4/22.
//

#include "rho_track.h"

//blob_collection_t blob_collection;

/// Trackers/quadrants > blobs_order
void RhoTrack_PairPredictions( rho_core_t * core )
{
    prediction_pair_t * predictions = &core->prediction_pair;
    
    // Find diagonal direction: (-)up, (+)down, (0)invalid centroid
    RhoTrack.RedistributeDensities( core );
    int8_t diagonal_check = ( core->quadrant_final[0] > core->quadrant_final[1] ) + ( core->quadrant_final[2] < core->quadrant_final[3] ) - 1;
    if( diagonal_check == 0 )
    {
        printf("!"); /// TODO: Make case for quadrant_check == 0
    }
    
    byte_t active_n = 0;
    
    // Match inactive trackers
    bool match_matrix[MAX_TRACKERS][MAX_TRACKERS] = { false };
    byte_t ambiguities[MAX_TRACKERS] = { 0 };
    for( byte_t xi = 0; xi < MAX_TRACKERS; xi++ )
    {
        tracker_t * xt = &predictions->y.trackers[xi];
        if( !xt->valid )
            continue;
        
        bool left = xt->kalman.x.p < core->centroid.x;
        for( byte_t yi = 0; yi < MAX_TRACKERS; yi++ )
        {
            tracker_t * yt = &predictions->x.trackers[yi];
            if( !yt->valid )
                continue;
            bool above = yt->kalman.x.p < core->centroid.y;
            bool match = left ^ ( ( diagonal_check > 0 ) ^ above );
//            printf("%d|%d %c%c - %c\n", xi, yi, left?'<':'>', above?'^':'v', match?'x':'o');
            match_matrix[xi][yi] = match;
            ambiguities[xi] += match;
        }
    }
    
    // Assign trackers to blobs
    index_pair_t max = { core->width, core->height };
    for( byte_t xi = 0; xi < MAX_TRACKERS; xi++ )
    {
        if(ambiguities[xi] > 1)
            printf("@");
        tracker_t * xt = &predictions->y.trackers[xi];
        if( xt == NULL ) continue;
        for( byte_t yi = 0; yi < MAX_TRACKERS; yi++ )
        {
            if( !match_matrix[xi][yi] )
                continue;
            tracker_t * yt = &predictions->x.trackers[yi];
            if( yt == NULL ) continue;
            blob_t * b = &predictions->blobs[xi];
            if( RhoTrack.UpdateBlob( b, xt, yt, &max ) )
                active_n++;
        }
    }
    core->prediction_pair.num_blobs = active_n;
    
    for(int i = 0; i < active_n; i++)
    {
        blob_t * b = &predictions->blobs[i];
        if( !b
           || !b->motion.x
           || !b->motion.y
//           || b->motion.x->kalman
//           || b->motion.y->kalman
           )
            printf("*%d*", i);
    }
    
    if(active_n > 2)
        printf("\n");
}

floating_t GetBlobAxis( tracker_t * t, index_t max, floating_t * w, index_t * x, floating_t * confidence )
{
    kalman_t * k = &t->kalman;
    double now = TIMESTAMP(TIME_SEC);
    if( now - k->t_origin < RHO_MIN_TRACKER_AGE_SEC )
        return 0.0;
    floating_t v = fabs(k->x.v);
    if( v > RHO_MAX_TRACKER_VELOCITY )
        return 0.0;
    floating_t test_x = Kalman.TestSelf( k );
    *w = MAX( t->region->width * ( 1 + RHO_BLOB_PADDING_FACTOR ), t->region->width + v );
    *x = (index_t)MIN( max, MAX( 0, (test_x - *w / 2) ) );
    *confidence = Kalman.Confidence( k );
    return true;
}

bool RhoTrack_UpdateBlob( blob_t * b, tracker_t * tx, tracker_t * ty, index_pair_t * max )
{
    index_t x, y;
    floating_t w, h, cx, cy;
    if( !GetBlobAxis( tx, max->x, &w, &x, &cx ) )
        return false;
    if( !GetBlobAxis( ty, max->y, &h, &y, &cy ) )
        return false;
    
    b->x = x;
    b->y = y;
    b->w = w;
    b->h = h;
    b->motion.x = tx;
    b->motion.y = ty;
    b->confidence = AVG2( cx, cy );
//    printf("-.-.-. (%d, %d) - %dx%d - <%.2f, %.2f>[%.2f] | x%p y%p | %d-%d\n", b->x, b->y, b->w, b->h, b->motion.x->kalman.x.v, b->motion.y->kalman.x.v, b->confidence, &b->motion.x->region, &b->motion.y->region, b->motion.x->region->tracking_id, b->motion.y->region->tracking_id);
    return true;
    /// TODO: figure out why w1xh1, w2xh2 swaps to w2xh1, w1h2
}


//    predictions->num_blobs = 0;
//
//    byte_t n = 0;
//    byte_t xo[MAX_TRACKERS];
//    byte_t yo[MAX_TRACKERS];
//
//    index_pair_t prev[MAX_TRACKERS] = { 0 };
//
//    for( byte_t i = 0; i < MAX_TRACKERS; i++ )
//    {
//        xo[i] = i;
//        yo[i] = i;
//        if( !predictions->x.trackers[i].valid || !predictions->y.trackers[i].valid )
//            break;
//        prev[i].x = predictions->x.trackers[xo[i]].kalman.x.p;
//        prev[i].y = predictions->y.trackers[yo[i]].kalman.x.p;
//        n++;
//    }

//    // If two, immediately pair using centroid and background -> verify on next frame
//    // If more, check on next frame -> For each y active, try each x element and fill thresh buffer. Keep best
//    byte_pair_t pts[2];
//    for( byte_t i = 0; i < n; i++ )
//    {
//        pts[0] = (byte_pair_t){ xo[i], yo[i] };
//        if( n >= 2 && i < 2 )
//        {
//            i++;
//            pts[1] = (byte_pair_t){ xo[i], yo[i] };
//            RhoTrack.DisambiguatePair( core, pts );
//        }
//        else
//            RhoTrack.PairXY( predictions, pts[0] );
//    }
//}
//
void RhoTrack_DisambiguatePair( rho_core_t * core, byte_pair_t pts[2] ) {}
//{
//    prediction_pair_t * predictions = &core->prediction_pair;
//
//    floating_t x0 = predictions->x.trackers[pts[0].x].kalman.x.p;
//    floating_t x1 = predictions->x.trackers[pts[1].x].kalman.x.p;
//
//    floating_t y0 = predictions->y.trackers[pts[0].y].kalman.x.p;
//    floating_t y1 = predictions->y.trackers[pts[1].y].kalman.x.p;
//
////    index_pair_t centroid = core->centroid;
//    /// TODO: Decide if centroid check is valid
////    else
//    if( ( x0 < x1 ) ^ ( ( quadrant_check > 0 ) ^ ( y0 > y1 ) ) )
//        SWAP(pts[0].x, pts[1].x);
//
//    core->prediction_pair.descending = quadrant_check > 0;
//
////    if(swap) SWAP(pts[0].y, pts[1].y);
//    RhoTrack.PairXY( &core->prediction_pair, pts[0] );
//    RhoTrack.PairXY( &core->prediction_pair, pts[1] );
//}
//
void RhoTrack_PairXY( prediction_pair_t * predictions, byte_pair_t pt ) {}
//{
//    byte_t n = predictions->num_blobs;
//    if( n >= MAX_REGIONS) return;
//
//    predictions->blobs_order[n].x = pt.x;
//    predictions->blobs_order[n].y = pt.y;
//    predictions->num_blobs = n + 1;
//    /// TODO: Add use of 2D kalman!
//}

/* Perform density redistribution from combining current frame and background */
void RhoTrack_RedistributeDensities( rho_core_t * core )
{
    LOG_RHO(RHO_DEBUG_2, "Redistributing densities.\n");
    redistribution_variables _ =
    {
        { core->secondary.x, abs(core->centroid.x-core->secondary.x), core->width - core->centroid.x  },
        { core->secondary.y, abs(core->centroid.y-core->secondary.y), core->height - core->centroid.y },
        { 0 }, 0
    };
    if( core->centroid.x < core->secondary.x )
    {
        _.xl[0] = core->centroid.x;
        _.xl[2] = core->width - core->secondary.x;
        _.c |= 0x01;
    }
    if( core->centroid.y < core->secondary.y )
    {
        _.yl[0] = core->centroid.y;
        _.yl[2] = core->width - core->secondary.y;
        _.c |= 0x02;
    }
    while( _.y < 3 )
        for( _.x = 0; _.x < 3; )
            _.area[_.p++] = _.xl[_.x++] * _.yl[_.y++];
    for( ; _.q < 4; _.q++ )
    {
        _.l  = rlookup.config[_.c].length[    _.q];
        _.l_ = rlookup.config[_.c].length[3 - _.q];
        for( _.x = 0, _.b = 0; _.x < _.l; _.x++ )
        {
            _.a = _.area[rlookup.config[_.c].current[_.q][_.x]];
            for( _.y = 0; _.y < _.l_; _.y++ )
                _.b += _.area[rlookup.config[_.c].background[rlookup.config[_.c].factor[_.q][_.x]][_.y]];
            _.d += ZDIV( _.a, _.b ) * core->quadrant_background[_.q];
        }
#ifndef ALLOW_NEGATIVE_REDISTRIBUTION
        if( _.d > core->quadrant[_.q] ) core->quadrant_final[_.q] = 0;
        else
#endif
            core->quadrant_final[_.q] = core->quadrant[_.q] - _.d;
    }
}

const rho_track_functions RhoTrack =
{
    .PairPredictions = RhoTrack_PairPredictions,
    .UpdateBlob = RhoTrack_UpdateBlob,
    .DisambiguatePair = RhoTrack_DisambiguatePair,
    .PairXY = RhoTrack_PairXY,
    .RedistributeDensities = RhoTrack_RedistributeDensities
};
