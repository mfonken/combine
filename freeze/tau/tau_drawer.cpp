//
//  tau_test.cpp
//  tau+
//
//  Created by Matthew Fonken on 8/27/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "tau_drawer.hpp"
#include <string>

#define RANGE(X,M) (X<0?0:(X>M?M-1:X))

using namespace cv;
using namespace std;

Vec3b ap       (150, 255, 150);
Vec3b bp       (150, 150, 255);
//Vec3b white    (255, 255, 255);
Vec3b red      (  0,   0, 255);
Vec3b blue     (255,   0,   0);

Vec3b blackish(25,25,25), greyish(100,90,90), bluish(255,255,100), greenish(100,255,100), redish(50,100,255), orangish(100,150,255), yellowish(100,255,255), white(255,255,255);

TauDrawer::TauDrawer( const char * name, int width, int height, std::string f, int num )
: Tau(name, width, height, f, num), frame(Size(width,  height), CV_8UC3, Scalar(0,0,0)), //probabilities(Size(800, 200), CV_8UC3, Scalar(245,245,245)),
rho_frame(Size(PROBABILITIES_FRAME_WIDTH, height), CV_8UC3, Scalar(0,0,0)),
rho_detection_x(Size(DETECTION_FRAME_WIDTH, height), CV_8UC3, Scalar(0,0,0)),
rho_detection_y(Size(DETECTION_FRAME_WIDTH, height), CV_8UC3, Scalar(0,0,0))
#ifdef DRAW_RHO_MAPS
,RMX(Size(TauDrawerwidth,  RHO_MAPS_HEIGHT), CV_8UC3, Scalar(255,255,255))
,RMY(Size(TauDrawerheight, RHO_MAPS_HEIGHT), CV_8UC3, Scalar(255,255,255))
#endif
{
    LOG_TAU("Initializing Tau Drawer.\n");
    w = frame.cols; h = frame.rows;
    W = w + SIDEBAR_WIDTH; H = h + SIDEBAR_WIDTH;
    Mat N(H, W, CV_8UC3, Scalar(0));
    utility.outframe.copyTo(N(Rect(0,0,w,h)));
    frame = N;
    pid_data_x = 0; x_match_data_x = 0; y_match_data_x = 0; x_peak_data_x = 0; y_peak_data_x = 0; frame_i = 0; x_detection_i = 0; y_detection_i = 0;
    pid_data = new floating_t[width];
    pid_data_target = new floating_t[width];
    x_match_data = new int[width];
    y_match_data = new int[width];
    x_peak_data_l = new floating_t[width];
    x_peak_data_u = new floating_t[width];
    x_peak_data_lv = new floating_t[width];
    x_peak_data_uv = new floating_t[width];
    x_peak_data_lt = new floating_t[width];
    x_peak_data_ut = new floating_t[width];
    
    y_peak_data_l = new floating_t[width];
    y_peak_data_u = new floating_t[width];
    y_peak_data_lv = new floating_t[width];
    y_peak_data_uv = new floating_t[width];
    y_peak_data_lt = new floating_t[width];
    y_peak_data_ut = new floating_t[width];
    
    ofstream file(FILENAME);
    file << "#,cvg%," << "flt%";
    for(int i = 0; i < NUM_STATE_GROUPS; i++) file << ",X" << to_string(i);
    for(int i = 0; i < NUM_STATE_GROUPS; i++) file << ",Y" << to_string(i);
    for(int i = 0; i < NUM_STATE_GROUPS; i++) file << ",C" << to_string(i);
    file << ",UN,S0,U0,S1,U1,S2,U2,SM,UM," << "tc%,pidv" << endl;
    file.close();
    
    file.open(X_DIM_FILENAME);
    // Header
    file << "#";
    for(int i = 0; i < /*MAX_BLOBS*/4; i++)
    {
        string blob_id = "B" + to_string(i);
        file << "," << "Den(" << blob_id << ")";
        file << "," << "Wth(" << blob_id << ")";
        file << "," << "Loc(" << blob_id << ")";
        file << "," << "Max(" << blob_id << ")";
        file << "," << "Scr(" << blob_id << ")";
        file << "," << "Flt(" << blob_id << ")";
    }
    for(int i = 0; i < MAX_TRACKING_FILTERS; i++)
    {
        string filter_id = "F" + to_string(i);
        file << "," << "Bias(" << filter_id << ")";
        file << "," << "K0(" << filter_id << ")";
        file << "," << "K1(" << filter_id << ")";
        file << "," << "Val(" << filter_id << ")";
        file << "," << "Vel(" << filter_id << ")";
        file << "," << "Scr(" << filter_id << ")";
    }
    file << ",Centroid,Peak(Lower),Target(Lower),Variance(Lower),Peak(Upper),Target(Upper),Variance(Upper)";
    file << endl;
    file.close();
    file.open(Y_DIM_FILENAME);
    // Header
    file << "#";
    for(int i = 0; i < /*MAX_BLOBS*/4; i++)
    {
        string blob_id = "B" + to_string(i);
        file << "," << "Den(" << blob_id << ")";
        file << "," << "Wth(" << blob_id << ")";
        file << "," << "Loc(" << blob_id << ")";
        file << "," << "Max(" << blob_id << ")";
        file << "," << "Scr(" << blob_id << ")";
        file << "," << "Flt(" << blob_id << ")";
    }
    for(int i = 0; i < MAX_TRACKING_FILTERS; i++)
    {
        string filter_id = "F" + to_string(i);
        file << "," << "Bias(" << filter_id << ")";
        file << "," << "K0(" << filter_id << ")";
        file << "," << "K1(" << filter_id << ")";
        file << "," << "Val(" << filter_id << ")";
        file << "," << "Vel(" << filter_id << ")";
        file << "," << "Scr(" << filter_id << ")";
    }
    file << ",Centroid,Peak(Lower),Target(Lower),Variance(Lower),Peak(Upper),Target(Upper),Variance(Upper)";
    file << endl;
    file.close();
    pthread_mutex_init( &drawer_mutex, NULL );
    
    timestamp = now();
    frame_rate_counter = frame_rate_display_delay;
}

TauDrawer::~TauDrawer()
{
    if(pid_data != NULL)
        delete pid_data;
    if(pid_data_target != NULL)
        delete pid_data_target;
    if(x_match_data != NULL)
        delete x_match_data;
    if(y_match_data != NULL)
        delete y_match_data;
    if(x_peak_data_l != NULL)
        delete x_peak_data_l;
    if(x_peak_data_u != NULL)
        delete x_peak_data_u;
    if(x_peak_data_lv != NULL)
        delete x_peak_data_lv;
    if(x_peak_data_uv != NULL)
        delete x_peak_data_uv;
    if(x_peak_data_lt != NULL)
        delete x_peak_data_lt;
    if(x_peak_data_ut != NULL)
        delete x_peak_data_ut;
    if(y_peak_data_l != NULL)
        delete y_peak_data_l;
    if(y_peak_data_u != NULL)
        delete y_peak_data_u;
    if(y_peak_data_lv != NULL)
        delete y_peak_data_lv;
    if(y_peak_data_uv != NULL)
        delete y_peak_data_uv;
    if(y_peak_data_lt != NULL)
        delete y_peak_data_lt;
    if(y_peak_data_ut != NULL)
        delete y_peak_data_ut;
}

Mat TauDrawer::DrawAll(void)
{
    Mat spare(Size(W + PROBABILITIES_FRAME_WIDTH + 2*DETECTION_FRAME_WIDTH, H ), CV_8UC3, Scalar(0,0,0));
    frame = spare;
    GetDensitiesFrame(frame);
    DrawRhoFrame(frame);
    DrawRhoDetection(X_DIMENSION, frame);
    DrawRhoDetection(Y_DIMENSION, frame);
    
    return frame;
}

Mat& TauDrawer::GetDensitiesFrame(Mat& M)
{
    pthread_mutex_lock(&drawer_mutex);
    Mat temp(utility.outframe);
    temp.copyTo(M(Rect(0,0,w,h)));
    DrawDensityGraph(M);
    DrawDensityMaps(M);
    
    density_2d_t    Qv[] = { rho.core.Q[0],  rho.core.Q[1],  rho.core.Q[2],  rho.core.Q[3]  },
                    Qb[] = { rho.core.Qb[0], rho.core.Qb[1], rho.core.Qb[2], rho.core.Qb[3] },
                    Qf[] = { rho.core.Qf[0], rho.core.Qf[1], rho.core.Qf[2], rho.core.Qf[3] };

    putText(M, to_string(Qv[0]), Point(inseta_, insetb_),                       FONT_HERSHEY_PLAIN, fontsize_, fontcolora_, 4);
    putText(M, to_string(Qv[1]), Point(w-insetb_-RHO_MAPS_INSET, insetb_),      FONT_HERSHEY_PLAIN, fontsize_, fontcolora_, 4);
    putText(M, to_string(Qv[2]), Point(inseta_, h-inseta_),                     FONT_HERSHEY_PLAIN, fontsize_, fontcolora_, 4);
    putText(M, to_string(Qv[3]), Point(w-insetb_-RHO_MAPS_INSET, h-inseta_),    FONT_HERSHEY_PLAIN, fontsize_, fontcolora_, 4);

    putText(M, to_string(Qb[0]), Point(inseta_, insetb_+18),                    FONT_HERSHEY_PLAIN, fontsize_, fontcolorb_, 4);
    putText(M, to_string(Qb[1]), Point(w-insetb_-RHO_MAPS_INSET, insetb_+18),   FONT_HERSHEY_PLAIN, fontsize_, fontcolorb_, 4);
    putText(M, to_string(Qb[2]), Point(inseta_, h-inseta_+18),                  FONT_HERSHEY_PLAIN, fontsize_, fontcolorb_, 4);
    putText(M, to_string(Qb[3]), Point(w-insetb_-RHO_MAPS_INSET, h-inseta_+18), FONT_HERSHEY_PLAIN, fontsize_, fontcolorb_, 4);

    putText(M, to_string(Qf[0]), Point(inseta_, insetb_+36),                    FONT_HERSHEY_PLAIN, fontsize_, fontcolorc_, 4);
    putText(M, to_string(Qf[1]), Point(w-insetb_-RHO_MAPS_INSET, insetb_+36),   FONT_HERSHEY_PLAIN, fontsize_, fontcolorc_, 4);
    putText(M, to_string(Qf[2]), Point(inseta_, h-inseta_+36),                  FONT_HERSHEY_PLAIN, fontsize_, fontcolorc_, 4);
    putText(M, to_string(Qf[3]), Point(w-insetb_-RHO_MAPS_INSET, h-inseta_+36), FONT_HERSHEY_PLAIN, fontsize_, fontcolorc_, 4);
    
    putText(M, "Thresh: " + to_string(utility.thresh), Point(0, 14), FONT_HERSHEY_PLAIN, 1, Vec3b(255,0,255), 2);
    putText(M, "State: " + string(stateString(rho.core.BayeSys.state)), Point(0, 28), FONT_HERSHEY_PLAIN, 1, Vec3b(255,0,155), 2);
    
    putText(M, "X", Point(utility.pCx-9, utility.pCy+10), FONT_HERSHEY_PLAIN, 2, greyish, 4);
    line(M, Point(rho.core.Cx, rho.core.Cy), Point(utility.pCx, utility.pCy), Scalar(0,255,255));
    putText(M, "X (" + to_string(rho.core.Cx) + ", " + to_string(rho.core.Cy) + ")", Point(rho.core.Cx-9, rho.core.Cy+10), FONT_HERSHEY_PLAIN, 2, yellowish, 4);
    putText(M, "o", Point(rho.core.Px-9, rho.core.Py+12), FONT_HERSHEY_PLAIN, 2, orangish, 4);
    putText(M, "o", Point(rho.core.Sx-9, rho.core.Sy+12), FONT_HERSHEY_PLAIN, 2, orangish, 4);
    
    double dnow = now();
    rectangle(M, Point(W-SIDEBAR_WIDTH,H-SIDEBAR_WIDTH*2), Point(W,H), Scalar(0,0,0), FILLED);
    if(--frame_rate_counter == 0)
    {
        frame_rate_counter = frame_rate_display_delay;
        frame_rate = 1. / (dnow - timestamp);
    }
    timestamp = dnow;
    
    putText(M, to_string((int)accuracy), Point(W-SIDEBAR_WIDTH+2, H-SIDEBAR_WIDTH*3/2+4), FONT_HERSHEY_PLAIN, 1, Scalar(100,150,0));
    putText(M, to_string(frame_rate), Point(W-SIDEBAR_WIDTH+2, H-SIDEBAR_WIDTH/2+4), FONT_HERSHEY_PLAIN, 1, Scalar(0,100,150));
    pthread_mutex_unlock(&drawer_mutex);
    
    return frame;
}

void TauDrawer::DrawDensityGraph(Mat &M)
{
    int u, v, w = width, h = height;
    
    int x1 = w, x2 = w, y1 = h, y2 = h,
    rangex[3] = { w, (int)rho.core.Cx, 0 },
    rangey[3] = { h, (int)rho.core.Cy, 0 },
    Bx = rho.core.Bx,
    By = rho.core.By;
    line(M, Point(rangex[1],0),   Point(rangex[1],H), redish);
    line(M, Point(0,rangey[1]),   Point(W,rangey[1]), redish);
    
    Point pA[2], pB[2], pC[2], pD[2];
    
    for( int i = 0, j = 1; i < 2; i++, j++ )
    {
        bool pT = 1;
//        bool first = 1;
        /* Kalman Values */
        rho_kalman_t yk = rho.core.DensityMapPair.x.kalmans[i], xk = rho.core.DensityMapPair.y.kalmans[i];
        int mYv = rho.core.DensityMapPair.x.kalmans[i].variance, mXv = rho.core.DensityMapPair.y.kalmans[i].variance;
       
        int m = OP_ALIGN((xk.value/DENSITY_SCALE),h), n = OP_ALIGN((yk.value/DENSITY_SCALE),w);
        int mv1 = OP_ALIGN((mXv/DENSITY_SCALE),m), mv2 = OP_ALIGN(-(mXv/DENSITY_SCALE),m), nv1 = OP_ALIGN((mYv/DENSITY_SCALE),n), nv2 = OP_ALIGN(-(mYv/DENSITY_SCALE),n);
        
        double ndm = INR(OP_ALIGN(yk.value/DENSITY_SCALE,w),w), mdm = INR(OP_ALIGN(xk.value/DENSITY_SCALE,h),h);
        
        int k = i, l = j;
        line(M, Point(n,rangey[k]),   Point(n,rangey[l]), orangish);
        line(M, Point(nv1,rangey[k]), Point(nv1,rangey[l]), yellowish);
        line(M, Point(nv2,rangey[k]), Point(nv2,rangey[l]), yellowish);
        
        line(M, Point(rangex[k],m),   Point(rangex[l],m), orangish);
        line(M, Point(rangex[k],mv1), Point(rangex[l],mv1), yellowish);
        line(M, Point(rangex[k],mv2), Point(rangex[l],mv2), yellowish);
        
#ifdef DRAW_RHO_MAPS
        RMX = Scalar(255,255,255);
        RMY = Scalar(255,255,255);
        int md = OP_ALIGN((xk.value/DENSITY_SCALE),RHO_MAPS_HEIGHT), nd = OP_ALIGN((yk.value/DENSITY_SCALE),RHO_MAPS_HEIGHT);
        int mv1d = OP_ALIGN((mXv/DENSITY_SCALE),m), mv2d = OP_ALIGN(-(mXv/DENSITY_SCALE),md), nv1d = OP_ALIGN((mYv/DENSITY_SCALE),nd), nv2d = OP_ALIGN(-(mYv/DENSITY_SCALE),nd);
#endif
        
        /* Density Maps */
        pthread_mutex_lock(&rho.density_map_pair_mutex);
        int dX[h], dY[w], fX[h], fY[w];
        for( y1 = rangey[i]; y1 > rangey[j];  )
        {
            --y1;
            dX[y1] = INR(rho.core.DensityMapPair.x.map[y1], h);
            fX[y1] = INR(rho.core.DensityMapPair.x.background[y1], h);
        }
        for( x1 = rangex[i]; x1 > rangex[j];  )
        {
            --x1;
            dY[x1] = INR(rho.core.DensityMapPair.y.map[x1], w);
            fY[x1] = INR(rho.core.DensityMapPair.y.background[x1], w);
        }
        pthread_mutex_unlock(&rho.density_map_pair_mutex);
        
//        line(RMX, Point(By,RHO_MAPS_HEIGHT), Point(By,0), orangish);
//        line(RMY, Point(Bx,RHO_MAPS_HEIGHT), Point(Bx,0), orangish);
        
#ifdef DRAW_RHO_MAPS
//        line(RMY, Point(0,nd), Point(H,nd), orangish);
//        line(RMY, Point(0,nv1d), Point(H,nv1d), yellowish);
//        line(RMY, Point(0,nv2d), Point(H,nv2d), yellowish);
#endif
        int vprev = 0, uprev = 0, underprev = 0, overprev = 0;
        for( y2 = rangey[i]; y2 > rangey[j];  )
        {
            --y2;
            u = INR(OP_ALIGN((fX[y2]/DENSITY_SCALE),w),w);
            v = INR(OP_ALIGN((dX[y2]/DENSITY_SCALE),w),w);
            
            if(v>ndm)
            {
//                M.at<Vec3b>(y2,v) = white;
                line(M, Point(vprev,y2), Point(v,y2-1), white);
                underprev = 0;
                overprev = 0;
            }
            else
            {
                int under = RANGE(ndm + abs(ndm-v),w);
                int over  = RANGE(ndm - abs(ndm-v),w);
                
//                M.at<Vec3b>(y2,under)   = bluish;
//                M.at<Vec3b>(y2,over)   = redish;
//                M.at<Vec3b>(y2, u) = greenish;
                line(M, Point(underprev,y2), Point(under,y2-1), bluish);
                line(M, Point(overprev,y2), Point(over,y2-1), redish);
                line(M, Point(uprev,y2), Point(u,y2-1), greenish);
                
                underprev = under;
                overprev = over;
            }
            vprev = v;
            uprev = u;
            pT = !pT;
//            first = 0;
#ifdef DRAW_RHO_MAPS
            u = INR(OP_ALIGN((fX[y2]/DENSITY_SCALE),RHO_MAPS_HEIGHT),RHO_MAPS_HEIGHT);
            line(RMY, Point(y2,RHO_MAPS_HEIGHT), Point(y2,u), greenish);
#endif
        }
        
#ifdef DRAW_RHO_MAPS
        line(RMX, Point(0,md), Point(W,md), orangish);
        line(RMX, Point(0,mv1d), Point(W,mv1d), yellowish);
        line(RMX, Point(0,mv2d), Point(W,mv2d), yellowish);
#endif
        
        vprev = 0; uprev = 0; underprev = 0; overprev = 0;
        for( x2 = rangex[i]; x2 > rangex[j]; --x2 )
        {
            u = INR(OP_ALIGN((fY[x2]/DENSITY_SCALE),h),h);
            v = INR(OP_ALIGN((dY[x2]/DENSITY_SCALE),h),h);
            
            if(v>mdm)
            {
//                M.at<Vec3b>(v,x2) = white;
                line(M, Point(x2,vprev), Point(x2-1,v), white);
//                line(M, Point(x2,v), Point(x2,h), white);
                underprev = 0;
                overprev = 0;
            }
            else
            {
                int under = RANGE(mdm + abs(mdm-v),h);
                int over = RANGE(mdm - abs(mdm-v),h);
                
//                M.at<Vec3b>(under,x2) = bluish;
//                M.at<Vec3b>(over,x2) = redish;
//                M.at<Vec3b>(u, x2) = greenish;

                line(M, Point(x2,underprev), Point(x2-1,under), bluish);
                line(M, Point(x2,overprev), Point(x2-1,over), redish);
                line(M, Point(x2,uprev), Point(x2-1,u), greenish);
                
//                line(M, Point(x2,under), Point(x2,h), white);
//                line(M, Point(x2,under), Point(x2,over), bluish);
                
                
                underprev = under;
                overprev = over;
            }
            
            vprev = v;
            uprev = u;
#ifdef DRAW_RHO_MAPS
            u = INR(OP_ALIGN((fY[x2]/DENSITY_SCALE),RHO_MAPS_HEIGHT),RHO_MAPS_HEIGHT);
            line(RMX, Point(x2,RHO_MAPS_HEIGHT), Point(x2,u), greenish);
#endif
        }
    }
    
    pthread_mutex_lock(&predictions_mutex);
    Point2f a(packet.px, packet.py),
    b(packet.sx, packet.sy);
    pthread_mutex_unlock(&predictions_mutex);
    
    line(M, Point(0,Bx),Point(w,Bx), greenish);
    line(M, Point(By,0),Point(By,h), greenish);
    //    putText(M, "A", Point(A.x, A.y), FONT_HERSHEY_PLAIN, 2, Vec3b(0,55,150), 3);
    //    putText(M, "B", Point(B.x, B.y), FONT_HERSHEY_PLAIN, 2, Vec3b(0,150,55), 3);
    
    
    // Draw blobs
    for( int i = 0; i < rho.core.PredictionPair.y.NumBlobs; i++ )
    {
        int o = rho.core.PredictionPair.y.BlobsOrder[i];
        int v = rho.core.PredictionPair.y.Blobs[o].loc;
        line(M, Point(v,0),Point(v,h), bluish);
    }
    for( int i = 0; i < rho.core.PredictionPair.x.NumBlobs; i++ )
    {
        int o = rho.core.PredictionPair.x.BlobsOrder[i];
        int v = rho.core.PredictionPair.x.Blobs[o].loc;
        line(M, Point(0,v),Point(w,v), bluish);
    }
    
    // Draw tracking predictions
    for( int i = 0; i < 2; i++ )
    {
        int o = rho.core.PredictionPair.y.TrackingFiltersOrder[i];
        int v = rho.core.PredictionPair.y.TrackingFilters[o].value;
        line(M, Point(v,0),Point(v,h), greyish);
    }
    for( int i = 0; i < 2; i++ )
    {
        int o = rho.core.PredictionPair.x.TrackingFiltersOrder[i];
        int v = rho.core.PredictionPair.x.TrackingFilters[o].value;
        line(M, Point(0,v),Point(w,v), greyish);
    }
}

void TauDrawer::DrawDensityMaps(Mat& M)
{
    Vec3b c(0,0,0);
    int x = 0, y = 0, rangex[2] = { (int)rho.core.Cx, w }, rangey[2] = { (int)rho.core.Cy, h };
    for( int i = 0; i < 2; i++ )
    {
        int mX = rho.core.DensityMapPair.x.max[1-i], mY = rho.core.DensityMapPair.y.max[1-i];
        if(!mX) mX = 1;
        if(!mY) mY = 1;
        density_t *dX = rho.core.DensityMapPair.x.map, *dY = rho.core.DensityMapPair.y.map;
        
        for(; y < rangey[i]; y++ )
        {
            int v = ( dX[y] * 255 ) / mX;
            c = densityColor(v);
            line(M, Point(w,y), Point(W,y), c);
        }
        
        for( ; x < rangex[i]; x++ )
        {
            int v = ( dY[x] * 255 ) / mY;
            c = densityColor(v);
            line(M, Point(x,h), Point(x,H), c);
        }
    }
}

//void TauDrawer::DrawRhoProbabilities(Mat &M)
//{
//    int w = M.cols, h = M.rows, h_ = h-RHO_MAPS_INSET;
//    M = {Scalar(245,245,245)};
//    double prob[8] =
//    {
//        rho.core.PredictionPair.x.Probabilities.P[1],
//        rho.core.PredictionPair.x.Probabilities.P[2],
//        rho.core.PredictionPair.x.Probabilities.P[3],
//        rho.core.PredictionPair.x.Probabilities.P[0],
//        rho.core.PredictionPair.y.Probabilities.P[1],
//        rho.core.PredictionPair.y.Probabilities.P[2],
//        rho.core.PredictionPair.y.Probabilities.P[3],
//        rho.core.PredictionPair.y.Probabilities.P[0]
//    };
//
//    line(M, Point(RHO_MAPS_INSET, h_+1), Point(w-RHO_MAPS_INSET, h_+1), Scalar(15,15,15));
//    line(M, Point(w/2, h_+10), Point(w/2, RHO_MAPS_INSET-10), Scalar(15,15,15));
//
//    int col_space = 10, col_width = (w - ((8*col_space)+2*RHO_MAPS_INSET))/8, col_scale = h-RHO_MAPS_INSET*2;
//
//    line(M, Point(RHO_MAPS_INSET, h_-col_scale), Point(w-RHO_MAPS_INSET, h_-col_scale), Scalar(115,115,145));
//
//    int x = RHO_MAPS_INSET, c = 0;
//    for( int i = 0; i < 8; i++)
//    {
//        int y = h_- prob[i]*col_scale;
//        rectangle(M, Point(x,h_), Point(x+col_width, y), densityColor(c), -1);
//        x += col_space + col_width;
//        c += 255/4;
//        if(i == 3)
//        {
//            x += col_space;
//            c = 0;
//        }
//    }
//}

Vec3b TauDrawer::densityColor( int v )
{
    if(v > 255) v = 255;
    if(v < 0 ) v = 0;
    int h, s;
    h = v * 120 / 255;
    s = abs(v - (255/2));
    Vec3b hsv(h,s,220), ret(0,0,0);
    ret = hsv2bgr(hsv);
    return ret;
}

Vec3b TauDrawer::hsv2bgr(Vec3b hsv)
{
    float h = hsv[0] *   2.0f; // 0-360
    float s = hsv[1] / 255.0f; // 0.0-1.0
    float v = hsv[2] / 255.0f; // 0.0-1.0
    
    float r = 0, g = 0, b = 0; // 0.0-1.0
    
    int   hi = (int)(h / 60.0f) % 6;
    float f  = (h / 60.0f) - hi;
    float p  = v * (1.0f - s);
    float q  = v * (1.0f - s * f);
    float t  = v * (1.0f - s * (1.0f - f));
    
    switch(hi) {
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    
    return Vec3b(r*255,g*255,b*255);
}

#define RHO_DRAWERS_BACKGROUND_COLOR blackish

Mat& TauDrawer::DrawRhoFrame(Mat&M)
{
    pthread_mutex_lock(&drawer_mutex);
    int global_offset = 0;
    if(M.cols > 0) global_offset = W;
    Mat &mat = global_offset>0?M:rho_frame;
    
    /* Gather data */
    pthread_mutex_lock(&rho.c_mutex);
    floating_t total_cvg_percent = (floating_t)rho.core.TotalCoverage/(rho.core.Width*rho.core.Height),
    filtered_cvg_percent = rho.core.FilteredPercentage;
    floating_t X_kumaraswamy[NUM_STATE_GROUPS] = { 0. },
    Y_kumaraswamy[NUM_STATE_GROUPS] = { 0. },
    C_kumaraswamy[NUM_STATE_GROUPS] = { 0. };
    memcpy(X_kumaraswamy, rho.core.PredictionPair.x.Probabilities.P, sizeof(floating_t)*NUM_STATE_GROUPS);
    memcpy(Y_kumaraswamy, rho.core.PredictionPair.y.Probabilities.P, sizeof(floating_t)*NUM_STATE_GROUPS);
    memcpy(C_kumaraswamy, rho.core.PredictionPair.Probabilities.P, sizeof(floating_t)*NUM_STATE_GROUPS);
    floating_t
    state_P[NUM_STATES] = { 0. },
    x_nu = rho.core.PredictionPair.x.NuBlobs,
    y_nu = rho.core.PredictionPair.y.NuBlobs;
    state_t state = rho.core.BayeSys.state;
    for(int i = 0; i < NUM_STATES; i++)
        state_P[i] = rho.core.BayeSys.probabilities.map[i][state];
    
    floating_t target_cvg_percent = rho.core.TargetCoverageFactor;
    pid_data[pid_data_x] = rho.core.TargetFilter.value;
    pid_data_target[pid_data_x] = target_cvg_percent;
    pthread_mutex_unlock(&rho.c_mutex);
    
    /* Write to file */
    ofstream file(FILENAME, ofstream::out | ofstream::app);
    file << frame_i++ << "," << total_cvg_percent << "," << filtered_cvg_percent;
    for(int i = 0; i < NUM_STATE_GROUPS; i++) file << "," << X_kumaraswamy[i];
    for(int i = 0; i < NUM_STATE_GROUPS; i++) file << "," << Y_kumaraswamy[i];
    for(int i = 0; i < NUM_STATE_GROUPS; i++) file << "," << C_kumaraswamy[i];
    for(int i = 0; i < NUM_STATES; i++) file << "," << state_P[i];
    file << "," << target_cvg_percent << "," << pid_data[pid_data_x];
    file << endl;
    file.close();
    
    /* Clear */
    rectangle(mat, Point(global_offset,0), Point(global_offset+rho_frame.cols,H), RHO_DRAWERS_BACKGROUND_COLOR, FILLED);
    
    /* Draw */
#define RF_SPACE 10
#define RF_WIDTH rho_frame.cols
#define RF_HEIGHT rho_frame.rows
#define RF_FONT FONT_HERSHEY_PLAIN
#define RF_TEXT_SM 1
#define RF_TEXT_SM_OFFSET 16
#define RF_TEXT_MD 2
#define RF_TEXT_LG 4
#define RF_TEXT_COLOR Scalar{245,245,245}
#define RF_TEXT_PRECISION 2
#define RF_LINE_WIDTH 1
#define RF_LINE_COLOR white
#define RF_ORIGIN Point(global_offset,0)
    
    Scalar RF_COLOR_A(0,50,150),
    RF_COLOR_B(150,100,0),
    RF_COLOR_C(50,150,50);
    
#define CVG_SPLIT 0.05
#define ST_SPLIT 0.5
#define PID_SPLIT 0.45
    
#define RF_SCALED_HEIGHT(S) (RF_HEIGHT*S-RF_SPACE)
    
    /* Coverage */
#define CVG_ORIGIN Point(RF_ORIGIN.x+RF_SPACE, RF_ORIGIN.y+RF_SPACE)
#define CVG_WIDTH (RF_WIDTH-2*RF_SPACE)
#define CVG_HEIGHT RF_SCALED_HEIGHT(CVG_SPLIT)
#define CVG_TEXT_X (CVG_ORIGIN.x)
#define CVG_TEXT_HEIGHT (CVG_ORIGIN.y+RF_TEXT_SM_OFFSET)
#define CVG_END Point(CVG_ORIGIN.x+CVG_WIDTH, CVG_ORIGIN.y+CVG_HEIGHT)
#define CVG_COLOR white
#define CVG_TOP_BOUND 1.
#define TOTAL_CVG_TEXT_X (TOTAL_CVG_ORIGIN.x+CVG_TEXT_SM_OFFSET)
    
    // Bound
    rectangle(mat, CVG_ORIGIN, CVG_END, CVG_COLOR, RF_LINE_WIDTH);
   // Total
    int total_offset = (1-(total_cvg_percent*100.)/CVG_TOP_BOUND)*CVG_WIDTH;
    if(total_offset > CVG_ORIGIN.x)
    {
        putText(mat, to_stringn(CVG_TOP_BOUND, RF_TEXT_PRECISION)+"%", Point(CVG_TEXT_X, CVG_TEXT_HEIGHT+2), RF_FONT, RF_TEXT_SM, RF_TEXT_COLOR);
        rectangle(mat, Point(CVG_ORIGIN.x+total_offset+1,CVG_ORIGIN.y+1), Point(CVG_END.x-1,CVG_END.y-1), RF_COLOR_B, FILLED);
        putText(mat, to_stringn(total_cvg_percent*100, RF_TEXT_PRECISION)+"%", Point(CVG_ORIGIN.x+total_offset, CVG_TEXT_HEIGHT+2), RF_FONT, RF_TEXT_SM, RF_TEXT_COLOR);
        //Filtered
        int filtered_offset = (1-(total_cvg_percent*filtered_cvg_percent*100.)/CVG_TOP_BOUND)*CVG_WIDTH;
        if(filtered_offset > CVG_ORIGIN.x)
        {
            rectangle(mat, Point(CVG_ORIGIN.x+filtered_offset+1,CVG_ORIGIN.y+1), Point(CVG_END.x-1,CVG_END.y-1), RF_COLOR_C, FILLED);
            if(CVG_ORIGIN.x+filtered_offset < RF_WIDTH - 100)
                putText(mat, to_stringn(filtered_cvg_percent*100, RF_TEXT_PRECISION)+"%", Point(CVG_ORIGIN.x+filtered_offset, CVG_TEXT_HEIGHT+2), RF_FONT, RF_TEXT_SM, RF_TEXT_COLOR);
        }
    }
    else
    {
        putText(mat, to_stringn(total_cvg_percent*100, RF_TEXT_PRECISION)+"%", Point(CVG_TEXT_X, CVG_TEXT_HEIGHT+2), RF_FONT, RF_TEXT_SM, RF_TEXT_COLOR);
    }
    
    /* States */
#define ST_ORIGIN Point(RF_ORIGIN.x+RF_SPACE, CVG_ORIGIN.y+CVG_HEIGHT+RF_SPACE)
#define ST_WIDTH (RF_WIDTH-2*RF_SPACE)
#define ST_HEIGHT RF_SCALED_HEIGHT(ST_SPLIT)
#define ST_BAR_MAX (ST_HEIGHT/2)
#define ST_BAR_Y (ST_ORIGIN.y+ST_HEIGHT/2-1)
#define ST_RADIUS (ST_BAR_MAX/3)
#define ST_TEXT_X (RF_SPACE+5)
#define ST_TEXT_HEIGHT (ST_ORIGIN.y+RF_TEXT_SM_OFFSET)
#define ST_END Point(ST_ORIGIN.x+ST_WIDTH, ST_ORIGIN.y+ST_HEIGHT)
#define ST_COLOR white
    // Bound
    rectangle(mat, ST_ORIGIN, ST_END, ST_COLOR, RF_LINE_WIDTH);

    // Split Lines
    line(mat, Point(ST_ORIGIN.x,ST_ORIGIN.y+ST_HEIGHT/2), Point(ST_END.x,ST_ORIGIN.y+ST_HEIGHT/2), RF_LINE_COLOR, RF_LINE_WIDTH);
    int width = ST_WIDTH/(NUM_STATE_GROUPS);
    for(int i = 0, x = width+ST_ORIGIN.x; i < NUM_STATE_GROUPS; i++, x += width)
        line(mat, Point(x,ST_ORIGIN.y+RF_SPACE), Point(x,ST_ORIGIN.y+ST_HEIGHT-RF_SPACE), RF_LINE_COLOR, RF_LINE_WIDTH);
    
    //State Data
    int bar_width = (width-2*RF_SPACE)/3;
    for(int i = 0, x = ST_ORIGIN.x+RF_SPACE; i < NUM_STATE_GROUPS; i++, x += width)
    {
        rectangle(mat, Point(x,ST_BAR_Y), Point(x+bar_width, ST_BAR_Y+2-X_kumaraswamy[i]*ST_BAR_MAX), RF_COLOR_A, FILLED);
        rectangle(mat, Point(x+bar_width,ST_BAR_Y), Point(x+2*bar_width, ST_BAR_Y+2-Y_kumaraswamy[i]*ST_BAR_MAX), RF_COLOR_B, FILLED);
        rectangle(mat, Point(x+2*bar_width,ST_BAR_Y), Point(x+3*bar_width, ST_BAR_Y+2-C_kumaraswamy[i]*ST_BAR_MAX), RF_COLOR_C, FILLED);
        
        int state_i = 2 + i*2;
        string state_n = "U" + to_string(i);
        Scalar c(RF_COLOR_B);
        if(state_i-1 == state)
        {
            state_i--;
            state_n = "S" + to_string(i);
            c = RF_COLOR_A;
        }
        int r = ST_RADIUS*state_P[state_i];
        putText(mat, state_n, Point(x, ST_BAR_Y+2*RF_SPACE), RF_FONT, RF_TEXT_SM, c);
        circle(mat, Point(x+width/2-RF_SPACE,ST_BAR_Y+ST_BAR_MAX/2), r, c, FILLED);
        putText(mat, to_string((int)(state_P[state_i]*100))+"%", Point(x+width/2-2.8*RF_SPACE,ST_BAR_Y+ST_BAR_MAX/2+5), RF_FONT, RF_TEXT_SM, RF_LINE_COLOR);
    }
    
    // Legend
    rectangle(mat, Point(ST_ORIGIN.x+ST_TEXT_X*0.5,ST_ORIGIN.y+ST_TEXT_X*0.5), Point(ST_ORIGIN.x+ST_TEXT_X*4.,ST_ORIGIN.y+ST_TEXT_X*4.5), RHO_DRAWERS_BACKGROUND_COLOR, FILLED);
    rectangle(mat, Point(ST_ORIGIN.x+ST_TEXT_X*0.5,ST_ORIGIN.y+ST_TEXT_X*0.5), Point(ST_ORIGIN.x+ST_TEXT_X*4.,ST_ORIGIN.y+ST_TEXT_X*4.5), RF_LINE_COLOR);
    rectangle(mat, Point(ST_ORIGIN.x+ST_TEXT_X,ST_ORIGIN.y+ST_TEXT_X), Point(ST_ORIGIN.x+2*ST_TEXT_X,ST_ORIGIN.y+2*ST_TEXT_X), RF_COLOR_A, FILLED);
    rectangle(mat, Point(ST_ORIGIN.x+ST_TEXT_X,ST_ORIGIN.y+2*ST_TEXT_X), Point(ST_ORIGIN.x+2*ST_TEXT_X,ST_ORIGIN.y+3*ST_TEXT_X), RF_COLOR_B, FILLED);
    rectangle(mat, Point(ST_ORIGIN.x+ST_TEXT_X,ST_ORIGIN.y+3*ST_TEXT_X), Point(ST_ORIGIN.x+2*ST_TEXT_X,ST_ORIGIN.y+4*ST_TEXT_X), RF_COLOR_C, FILLED);
    putText(mat, "X", Point(ST_ORIGIN.x+2.75*ST_TEXT_X, ST_TEXT_HEIGHT+ST_TEXT_X*0.75), RF_FONT, RF_TEXT_SM, RF_COLOR_A);
    putText(mat, "Y", Point(ST_ORIGIN.x+2.75*ST_TEXT_X, ST_TEXT_HEIGHT+ST_TEXT_X*1.75), RF_FONT, RF_TEXT_SM, RF_COLOR_B);
    putText(mat, "A", Point(ST_ORIGIN.x+2.75*ST_TEXT_X, ST_TEXT_HEIGHT+ST_TEXT_X*2.75), RF_FONT, RF_TEXT_SM, RF_COLOR_C);
    
    // Nu
    rectangle(mat, Point(ST_END.x-ST_TEXT_X*4,ST_ORIGIN.y+ST_TEXT_X*0.5), Point(ST_END.x-ST_TEXT_X*0.5,ST_ORIGIN.y+ST_TEXT_X*4.5), RHO_DRAWERS_BACKGROUND_COLOR, FILLED);
    rectangle(mat, Point(ST_END.x-ST_TEXT_X*4,ST_ORIGIN.y+ST_TEXT_X*0.5), Point(ST_END.x-ST_TEXT_X*0.5,ST_ORIGIN.y+ST_TEXT_X*4.5), RF_LINE_COLOR);
    putText(mat, to_stringn(x_nu,2), Point(ST_END.x-ST_TEXT_X*3.5,ST_TEXT_HEIGHT+ST_TEXT_X*0.75), RF_FONT, RF_TEXT_SM, RF_COLOR_A);
    putText(mat, to_stringn(y_nu,2), Point(ST_END.x-ST_TEXT_X*3.5,ST_TEXT_HEIGHT+ST_TEXT_X*1.75), RF_FONT, RF_TEXT_SM, RF_COLOR_B);
    putText(mat, to_stringn((x_nu+y_nu)/2,2), Point(ST_END.x-ST_TEXT_X*3.5,ST_TEXT_HEIGHT+ST_TEXT_X*2.75), RF_FONT, RF_TEXT_SM, RF_COLOR_C);
    
    /* PID */
#define PID_ORIGIN Point(RF_ORIGIN.x+RF_SPACE, ST_ORIGIN.y+ST_HEIGHT+RF_SPACE)
#define PID_WIDTH (RF_WIDTH-2*RF_SPACE)
#define PID_HEIGHT (RF_SCALED_HEIGHT(PID_SPLIT)-RF_SPACE)
#define PID_TICKS 5
#define PID_TICK_SPACING (PID_HEIGHT/(PID_TICKS+1))
#define PID_HAS_SUBTICKS true
#define PID_TICK_WIDTH 7
#define PID_TICK_FRAME_INSET (PID_ORIGIN.x+2*RF_SPACE)
#define PID_SUBTICK_INSET 2
#define PID_GRAPH_START PID_ORIGIN.x+RF_SPACE
#define PID_GRAPH_WIDTH (PID_WIDTH-2*RF_SPACE)
//#define PID_MAX_BOUND 2. // percent
#define PID_TEXT_X (RF_SPACE)
#define PID_TEXT_HEIGHT (PID_ORIGIN.y+RF_TEXT_SM_OFFSET)
#define PID_END Point(PID_ORIGIN.x+PID_WIDTH, PID_ORIGIN.y+PID_HEIGHT)
#define PID_COLOR white
#define PID_INCREMENT 2
    
    // Bound
    rectangle(mat, PID_ORIGIN, PID_END, PID_COLOR, RF_LINE_WIDTH);

    int interval = 50;
    if( pid_data_x < interval)
        scale_x = 0;
    else if( pid_data_x % interval == 0 )
        scale_x = pid_data_x - interval;

    floating_t PID_MAX_BOUND = 0.70, PID_MIN_BOUND = 0.67, PID_DIFF = PID_MAX_BOUND - PID_MIN_BOUND;
    for(int i = scale_x; i < pid_data_x; i++)
    {
        floating_t curr = pid_data[i] * 100., curr_target = pid_data_target[i] * 100.;
        if(curr > PID_MAX_BOUND) PID_MAX_BOUND = curr;
        if(curr < PID_MIN_BOUND) PID_MIN_BOUND = curr;
        if(curr_target > PID_MAX_BOUND) PID_MAX_BOUND = curr_target;
        if(curr_target < PID_MIN_BOUND) PID_MIN_BOUND = curr_target;
    }
//    PID_MAX_BOUND += 0.2;
    PID_DIFF = PID_MAX_BOUND - PID_MIN_BOUND;

#define SCALE(X) BOUND((X-PID_MIN_BOUND/100.)*100./PID_DIFF*(PID_HEIGHT-PID_TICK_SPACING)+1, 1, PID_HEIGHT)
    // Graph
    int pid_prev = 0, pid_prev_target = 0;
    for(int i = 0, x = PID_GRAPH_START; i < pid_data_x; i++, x++)
    {
        floating_t curr = pid_data[i], curr_target = pid_data_target[i];
        if(curr > PID_DIFF) continue;
        int scaled_pid_value = SCALE(curr),
        curr_scaled = PID_END.y - scaled_pid_value,
        scaled_pid_target_value = SCALE(curr_target),// BOUND((curr_target*(100./PID_DIFF-PID_MIN_BOUND))*(PID_HEIGHT-PID_TICK_SPACING), 1, PID_HEIGHT),
        curr_scaled_target = PID_END.y - scaled_pid_target_value;
        int xs = i * PID_INCREMENT + PID_ORIGIN.x;
        if(pid_prev == 0)
        {
            mat.at<Scalar>(curr_scaled,xs) = RF_COLOR_A;
            mat.at<Scalar>(curr_scaled_target, xs) = RF_COLOR_B;
        }
        else
        {
            line(mat, Point(xs-1,pid_prev), Point(xs, curr_scaled), RF_COLOR_A);
            line(mat, Point(xs-1,pid_prev_target), Point(xs, curr_scaled_target), RF_COLOR_B);
        }
        pid_prev = curr_scaled;
        pid_prev_target = curr_scaled_target;
    }
    
    // Ticks
    if(PID_HAS_SUBTICKS) line(mat, Point(PID_TICK_FRAME_INSET+PID_SUBTICK_INSET,PID_END.y-PID_TICK_SPACING/2), Point(PID_TICK_FRAME_INSET+PID_TICK_WIDTH-PID_SUBTICK_INSET,PID_END.y-PID_TICK_SPACING/2), RF_LINE_COLOR);
    for(int i = 1, y = PID_END.y - PID_TICK_SPACING; i <= PID_TICKS; i++, y -= PID_TICK_SPACING)
    {
        line(mat, Point(PID_TICK_FRAME_INSET,y), Point(PID_TICK_FRAME_INSET+PID_TICK_WIDTH,y), RF_LINE_COLOR, RF_LINE_WIDTH);
        floating_t tick_value = PID_DIFF*((floating_t)i)/PID_TICKS + PID_MIN_BOUND;
        putText((mat), to_stringn(tick_value, 2) + "%", Point(PID_TICK_FRAME_INSET+PID_TICK_WIDTH+RF_SPACE,y+RF_TEXT_SM_OFFSET/2.5), RF_FONT, RF_TEXT_SM, RF_TEXT_COLOR);
        if(PID_HAS_SUBTICKS) line(mat, Point(PID_TICK_FRAME_INSET+PID_SUBTICK_INSET,y-PID_TICK_SPACING/2), Point(PID_TICK_FRAME_INSET+PID_TICK_WIDTH-PID_SUBTICK_INSET,y-PID_TICK_SPACING/2), RF_LINE_COLOR);
    }
    
    if(pid_data_x < 0 || pid_data_x > PID_GRAPH_WIDTH / PID_INCREMENT)
    {
        pid_data_x = 0;
        memset(pid_data, 0, sizeof(floating_t)*width);
        memset(pid_data_target, 0, sizeof(floating_t)*width);
    }
    pid_data_x ++;
    
    pthread_mutex_unlock(&drawer_mutex);
    return mat;
}

Mat& TauDrawer::DrawRhoDetection(int dimension, Mat&M)
{
    pthread_mutex_lock(&drawer_mutex);
    int global_offset = 0;
    if(M.cols > 0) global_offset = W + PROBABILITIES_FRAME_WIDTH + (dimension==Y_DIMENSION?DETECTION_FRAME_WIDTH:0);
    Mat &mat = global_offset > 0?M:((dimension==X_DIMENSION)?rho_detection_x:rho_detection_y);
    int &mX = (dimension==X_DIMENSION)?x_match_data_x:y_match_data_x,
    &pX = (dimension==X_DIMENSION)?x_peak_data_x:y_peak_data_x;
    int *mD = (dimension==X_DIMENSION)?x_match_data:y_match_data;
    floating_t *pDu = (dimension==X_DIMENSION)?x_peak_data_u:y_peak_data_u;
    floating_t *pDuv = (dimension==X_DIMENSION)?x_peak_data_uv:y_peak_data_uv;
    floating_t *pDut = (dimension==X_DIMENSION)?x_peak_data_ut:y_peak_data_ut;
    floating_t *pDl = (dimension==X_DIMENSION)?x_peak_data_l:y_peak_data_l;
    floating_t *pDlv = (dimension==X_DIMENSION)?x_peak_data_lv:y_peak_data_lv;
    floating_t *pDlt = (dimension==X_DIMENSION)?x_peak_data_lt:y_peak_data_lt;
    
    density_map_t &map = (dimension==X_DIMENSION)?rho.core.DensityMapPair.x:rho.core.DensityMapPair.y;
    prediction_t &prediction = (dimension==X_DIMENSION)?rho.core.PredictionPair.x:rho.core.PredictionPair.y;
    string filename =(dimension==X_DIMENSION)?X_DIM_FILENAME:Y_DIM_FILENAME;
    int &iteration = (dimension==X_DIMENSION)?x_detection_i:y_detection_i;
    
    floating_t c_percent = (floating_t)(map.centroid)/(floating_t)(map.length);
    
    pDu[pX] = prediction.PreviousPeak[0];
    pDl[pX] = prediction.PreviousPeak[1];
    pDuv[pX] = map.kalmans[0].variance;
    pDlv[pX] = map.kalmans[1].variance;
    pDut[pX] = map.kalmans[0].value;
    pDlt[pX] = map.kalmans[1].value;
    
    blob_t blobs[MAX_BLOBS];
    for(int i = 0; i < MAX_BLOBS; i++)
        memcpy(&blobs[i], &prediction.Blobs[prediction.BlobsOrder[i]], sizeof(blob_t));
    
    
#define MATCH_DATA_WIDTH 3
#define MATCH_ENCODE(X,i,S) X |= S << (i*MATCH_DATA_WIDTH)
#define MATCH_DECODE(X,i) ((1 << MATCH_DATA_WIDTH)-1) & X >> (i*MATCH_DATA_WIDTH)
    
    int num_tracks = 0;
    for(int i = 0; i < MAX_BLOBS; i++)
    {
        blob_t curr = blobs[i];
        if(curr.den > 0 && curr.den < 2000)
            num_tracks++;
    }
    
    /* Gather data */
    int match_value = 0;
    rho_kalman_t tracking_filters[MAX_TRACKING_FILTERS];
    int tracking_filters_order[MAX_TRACKING_FILTERS] = {0};
    for(int i = 0; i < MAX_TRACKING_FILTERS; i++)
    {
        if(i < num_tracks)
        {
            int c = prediction.TrackingFiltersOrder[i];
            MATCH_ENCODE(match_value, i, c);
            memcpy(&tracking_filters[i], &prediction.TrackingFilters[c], sizeof(rho_kalman_t));
            tracking_filters_order[i] = c;
        }
        else
        {
            MATCH_ENCODE(match_value, i, MAX_TRACKING_FILTERS);
        }
        //        printf("%d %d %x\n", i, c, match_value);
    }
    mD[mX] = match_value;
    //    printf("\n");
    //    for(int i = 0; i < MAX_TRACKING_FILTERS; i++)
    //    {
    //        printf("%d %d\n", i, MATCH_DECODE(match_value, i));
    //    }
    
    pthread_mutex_unlock(&rho.c_mutex);
    
    ofstream file(filename, ofstream::out | ofstream::app);
    /* Write to file */
    file << iteration++;
    for(int i = 0; i < /*MAX_BLOBS*/4; i++)
    {
        blob_t curr = blobs[i];
        file << "," << curr.den << "," << curr.wth << "," << curr.loc << "," << curr.max << "," << curr.scr << "," << tracking_filters_order[i];
    }
    for(int i = 0; i < MAX_TRACKING_FILTERS; i++)
    {
        rho_kalman_t curr = tracking_filters[i];
        file << "," << curr.bias << "," << curr.K[0] << "," << curr.K[1] << "," << curr.value << "," << curr.velocity << "," << curr.score;
    }
    file << "," << c_percent << ","  << pDl[pX] << "," << pDlt[pX] << "," << pDlv[pX] << "," << pDu[pX] << "," << pDut[pX] << "," << pDuv[pX];
    file << endl;
    file.close();
    
    /* Clear */
    //rectangle(M, Point(global_offset,0), Point(global_offset+DETECTION_FRAME_WIDTH,H), RHO_DRAWERS_BACKGROUND_COLOR);
    
    /* Draw */
#define RD_SPACE 10
#define RD_WIDTH rho_detection_x.cols
#define RD_HEIGHT rho_detection_x.rows
#define RD_FONT FONT_HERSHEY_PLAIN
#define RD_TEXT_SM 1
#define RD_TEXT_SM_OFFSET 16
#define RD_TEXT_MD 2
#define RD_TEXT_LG 4
#define RD_TEXT_COLOR Scalar{245,245,245}
#define RD_TEXT_PRECISION 2
#define RD_LINE_WIDTH 1
#define RD_INNER_WIDTH (RD_WIDTH-2*RD_SPACE)
#define RD_LINE_COLOR white
#define RD_ORIGIN Point(global_offset,0)
    
#define RD_SCALED_HEIGHT(S) (RD_HEIGHT*S-RF_SPACE)
    
#define BL_SPLIT 0.5
#define MCH_SPLIT 0.1
#define PK_SPLIT 0.4
    
    /* Blobs */
#define BL_WIDTH RD_INNER_WIDTH
#define BL_HEIGHT (RD_SCALED_HEIGHT(BL_SPLIT))
#define BL_MAX_NUM 4
#define BL_IND_HEIGHT ((BL_HEIGHT-RD_SPACE)/BL_MAX_NUM-RD_SPACE)
#define BL_ORIGIN Point(RD_SPACE+RD_ORIGIN.x,RD_SPACE+RD_ORIGIN.y)
#define BL_END Point(BL_ORIGIN.x+BL_WIDTH, BL_ORIGIN.y+BL_HEIGHT)
#define BL_MAX_SIZE 90
#define BL_MAX_RADIUS (BL_IND_HEIGHT*0.5)
#define BL_RADIUS_SCALE BL_MAX_RADIUS/(BL_MAX_SIZE/(2*M_PI))

    // Bounds
    rectangle(mat, BL_ORIGIN, BL_END, RD_LINE_COLOR);
    
    // Blobs
    for(int i = 0, y = BL_ORIGIN.y+RD_SPACE; i < BL_MAX_NUM; i++, y += (BL_IND_HEIGHT+RD_SPACE))
    {
        blob_t curr = blobs[i];
        
        Scalar c = RD_LINE_COLOR;
        if(curr.den > 2000)
            c = blackish;
        rectangle(mat, Point(BL_ORIGIN.x+RD_SPACE, y), Point(BL_ORIGIN.x+BL_WIDTH-RD_SPACE, y+BL_IND_HEIGHT), c);
        
        if(curr.den > 2000) continue;
        
        putText(mat, to_string(i)+":"+to_string(tracking_filters_order[i]), Point(BL_ORIGIN.x+RD_TEXT_SM_OFFSET/2+3,y+RD_TEXT_SM_OFFSET-3), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
        
        if(curr.den > 0)
        {
            int r = BOUNDU(BL_RADIUS_SCALE*curr.den/(2*M_PI),BL_MAX_RADIUS);
            circle(mat, Point(BL_ORIGIN.x+BL_MAX_RADIUS, y+BL_IND_HEIGHT/2), r, redish);
        }
        Point textOrigin(BL_ORIGIN.x+BL_MAX_RADIUS*2-RD_SPACE, y+RD_SPACE);
        line(mat, textOrigin, Point(textOrigin.x, y+BL_IND_HEIGHT-RD_SPACE), RD_LINE_COLOR);
        
        int x_offset[2] = {RD_SPACE, RD_SPACE+80};
        int y_offset[3] = {RD_SPACE, RD_SPACE+18, RD_SPACE+36};
        
        // Blob text
        putText(mat, "Den: " + to_string(curr.den), Point(textOrigin.x+x_offset[0], textOrigin.y+y_offset[0]), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
        putText(mat, "Wth: " + to_string(curr.wth), Point(textOrigin.x+x_offset[1], textOrigin.y+y_offset[0]), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
        putText(mat, "Loc: " + to_string(curr.loc), Point(textOrigin.x+x_offset[0], textOrigin.y+y_offset[1]), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
        putText(mat, "Max: " + to_string(curr.max), Point(textOrigin.x+x_offset[1], textOrigin.y+y_offset[1]), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
        
        putText(mat, "Score: " + to_stringn(curr.scr,3), Point(textOrigin.x+x_offset[0], textOrigin.y+y_offset[2]), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
        
        
        // Matching text & tracking filters
        Point matchOrigin(textOrigin.x+180, textOrigin.y);
        line(mat, matchOrigin, Point(matchOrigin.x, y+BL_IND_HEIGHT-RD_SPACE), RD_LINE_COLOR);
        
        rho_kalman_t kalman = tracking_filters[tracking_filters_order[i]];
        putText(mat, "Bi: " + to_stringn(kalman.bias,1), Point(matchOrigin.x+x_offset[0], matchOrigin.y+y_offset[0]), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
        putText(mat, "K0: " + to_stringn(kalman.K[0],2), Point(matchOrigin.x+x_offset[1], matchOrigin.y+y_offset[0]), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
        putText(mat, "Vl: " + to_stringn(kalman.value,1), Point(matchOrigin.x+x_offset[0], matchOrigin.y+y_offset[1]), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
        putText(mat, "K1: " + to_stringn(kalman.K[1],2), Point(matchOrigin.x+x_offset[1], matchOrigin.y+y_offset[1]), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
        putText(mat, "Sc: " + to_stringn(kalman.score,2), Point(matchOrigin.x+x_offset[0], matchOrigin.y+y_offset[2]), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
        putText(mat, "Ve: " + to_stringn(kalman.velocity,2), Point(matchOrigin.x+x_offset[1], matchOrigin.y+y_offset[2]), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
    }
    
    /* Matching */
#define MCH_WIDTH RD_INNER_WIDTH
#define MCH_HEIGHT (RD_SCALED_HEIGHT(MCH_SPLIT))
#define MCH_MAX_NUM BL_MAX_NUM
#define MCH_IND_HEIGHT ((MCH_HEIGHT)/MCH_MAX_NUM)
#define MCH_ORIGIN Point(RD_SPACE+RD_ORIGIN.x,BL_END.y+RD_SPACE+RD_ORIGIN.y)
#define MCH_END Point(MCH_ORIGIN.x+MCH_WIDTH, MCH_ORIGIN.y+MCH_HEIGHT)
#define MCH_INSET_X (MCH_ORIGIN.x+3*RD_SPACE)
#define MCH_INTERVAL_WIDTH 3 // pixels
#define MCH_INTERVAL_SPACE 5 // pixels
#define MCH_INTERVAL_TOTAL (MCH_INTERVAL_WIDTH+MCH_INTERVAL_SPACE)
#define MCH_NUM_INTERVALS ((MCH_END.x-MCH_INSET_X)/MCH_INTERVAL_TOTAL)
#define MCH_LINE_WIDTH 1
    
    //Bounds
    rectangle(mat, MCH_ORIGIN, MCH_END, RD_LINE_COLOR);
    
    //Label
    for(int i = 1, y = MCH_ORIGIN.y+MCH_IND_HEIGHT; i <= MCH_MAX_NUM; i++, y += MCH_IND_HEIGHT)
    {
        if(i < MCH_MAX_NUM)
            line(mat, Point(BL_ORIGIN.x+1, y), Point(BL_ORIGIN.x+BL_WIDTH-1, y), blackish);
        putText(mat, to_string(i), Point(BL_ORIGIN.x+RD_SPACE, y-3), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
    }
    
    // Match Data
    int prevD = mD[0], prevV, currD, currV, y = MCH_ORIGIN.y + MCH_IND_HEIGHT/2;
    for(int i = 1, x = MCH_INSET_X; i < mX; i++, x+=MCH_INTERVAL_TOTAL)
    {
        currD = mD[i];
        for(int j = 0; j < MCH_MAX_NUM; j++)
        {
            currV = MATCH_DECODE(currD,j);
            prevV = MATCH_DECODE(prevD,j);
            if(currV < MCH_MAX_NUM && prevV < MCH_MAX_NUM)
            {
                line(mat, Point(x,y+MCH_IND_HEIGHT*currV), Point(x+MCH_INTERVAL_WIDTH,y+MCH_IND_HEIGHT*currV), red, MCH_LINE_WIDTH);
                line(mat, Point(x+MCH_INTERVAL_WIDTH,y+MCH_IND_HEIGHT*prevV), Point(x+MCH_INTERVAL_WIDTH+MCH_INTERVAL_SPACE,y+MCH_IND_HEIGHT*currV), bluish, MCH_LINE_WIDTH);
            }
//            if(i == mX-1 && prevV < MCH_MAX_NUM && currV < MCH_MAX_NUM && prevV != currV)
//                printf(" ");
        }
        prevD = currD;
    }
    // Match index handler
    mX++;
    if(mX < 0 || mX > MCH_NUM_INTERVALS)
    {
        mX = 0;
        memset(mD, 0, sizeof(floating_t)*width);
    }
    
    /* Peak */
#define PK_WIDTH RD_INNER_WIDTH
#define PK_HEIGHT (RD_SCALED_HEIGHT(PK_SPLIT)-RD_SPACE)
#define PK_CENTROID_DISPLAY_PERCENT 10.
#define PK_CENTROID_DISPLAY_HEIGHT (PK_HEIGHT*PK_CENTROID_DISPLAY_PERCENT/100)
#define PK_GRAPH_PERCENT (100.-PK_CENTROID_DISPLAY_PERCENT)
#define PK_GRAPH_HEIGHT (PK_HEIGHT*PK_GRAPH_PERCENT/100.)
#define PK_ORIGIN Point(RD_SPACE+RD_ORIGIN.x,MCH_END.y+RD_SPACE+RD_ORIGIN.y)
#define PK_END Point(PK_ORIGIN.x+PK_WIDTH, PK_ORIGIN.y+PK_HEIGHT)
#define PK_GRAPH_ORIGIN Point(PK_ORIGIN.x, PK_ORIGIN.y+PK_GRAPH_HEIGHT)
    
#define PK_INSET_X (PK_ORIGIN.x+3*RD_SPACE)
#define PK_INTERVAL_WIDTH 5 // pixels
#define PK_INTERVAL_SPACE 3 // pixels
#define PK_INTERVAL_TOTAL (PK_INTERVAL_WIDTH+PK_INTERVAL_SPACE)
#define PK_NUM_INTERVALS ((PK_END.x-PK_INSET_X)/PK_INTERVAL_TOTAL)
#define PK_LINE_WIDTH 3
#define PK_GRAPH_MAX_VALUE 100
    
#define PK_TICKS 5
#define PK_TICK_SPACING (PK_GRAPH_HEIGHT/(PK_TICKS))
#define PK_HAS_SUBTICKS true
#define PK_TICK_WIDTH 7
#define PK_TICK_FRAME_INSET PK_ORIGIN.x+RD_SPACE
#define PK_SUBTICK_INSET 2
    
    //Bounds
    rectangle(mat, PK_ORIGIN, Point(PK_END.x, PK_END.y+1), RD_LINE_COLOR);
    line(mat, Point(PK_ORIGIN.x,PK_ORIGIN.y+PK_CENTROID_DISPLAY_HEIGHT), Point(PK_ORIGIN.x+PK_WIDTH,PK_ORIGIN.y+PK_CENTROID_DISPLAY_HEIGHT), RD_LINE_COLOR);
    
    //Centroid
    int centroid_x = PK_WIDTH*c_percent;
    line(mat, Point(centroid_x+RD_ORIGIN.x, PK_ORIGIN.y), Point(centroid_x+RD_ORIGIN.x, PK_ORIGIN.y+PK_CENTROID_DISPLAY_HEIGHT), RD_LINE_COLOR);
    putText(mat, to_stringn(c_percent,2)+"%", Point(PK_ORIGIN.x+RD_TEXT_SM_OFFSET,PK_ORIGIN.y+RD_TEXT_SM_OFFSET), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
    putText(mat, to_stringn(1-c_percent,2)+"%", Point(PK_END.x-60,PK_ORIGIN.y+RD_TEXT_SM_OFFSET), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
    
    // Ticks
    // Upper
    if(PK_HAS_SUBTICKS) line(mat, Point(PK_TICK_FRAME_INSET+PK_SUBTICK_INSET,PK_END.y-PK_TICK_SPACING/2), Point(PK_TICK_FRAME_INSET+PK_TICK_WIDTH-PK_SUBTICK_INSET,PK_END.y-PK_TICK_SPACING/2), RD_LINE_COLOR);
    for(int i = 1, y = PK_END.y - PK_TICK_SPACING; i < PK_TICKS; i++, y -= PK_TICK_SPACING)
    {
        line(mat, Point(PK_TICK_FRAME_INSET,y), Point(PK_TICK_FRAME_INSET+PK_TICK_WIDTH,y), RD_LINE_COLOR, RD_LINE_WIDTH);
        floating_t tick_value = PK_GRAPH_MAX_VALUE*((floating_t)i)/PK_TICKS;
        putText(mat, to_stringn(tick_value, 1) + "%", Point(PK_TICK_FRAME_INSET+PK_TICK_WIDTH+RD_SPACE,y+RD_TEXT_SM_OFFSET/2.5), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
        if(PK_HAS_SUBTICKS) line(mat, Point(PK_TICK_FRAME_INSET+PK_SUBTICK_INSET,y-PK_TICK_SPACING/2), Point(PK_TICK_FRAME_INSET+PK_TICK_WIDTH-PK_SUBTICK_INSET,y-PK_TICK_SPACING/2), RD_LINE_COLOR);
    }
    line(mat, Point(PK_ORIGIN.x+PK_WIDTH/2, PK_ORIGIN.y+PK_CENTROID_DISPLAY_HEIGHT), Point(PK_ORIGIN.x+PK_WIDTH/2, PK_ORIGIN.y+PK_HEIGHT), greyish);
    // Lower
    if(PK_HAS_SUBTICKS) line(mat, Point(PK_TICK_FRAME_INSET+PK_SUBTICK_INSET,PK_END.y-PK_TICK_SPACING/2), Point(PK_TICK_FRAME_INSET+PK_TICK_WIDTH-PK_SUBTICK_INSET,PK_END.y-PK_TICK_SPACING/2), RD_LINE_COLOR);
    for(int i = 1, y = PK_END.y - PK_TICK_SPACING; i < PK_TICKS; i++, y -= PK_TICK_SPACING)
    {
        line(mat, Point(PK_TICK_FRAME_INSET+PK_WIDTH/2,y), Point(PK_TICK_FRAME_INSET+PK_TICK_WIDTH+PK_WIDTH/2,y), RD_LINE_COLOR, RD_LINE_WIDTH);
//        floating_t tick_value = PK_GRAPH_MAX_VALUE*((floating_t)i)/PK_TICKS;
//        putText(mat, to_stringn(tick_value, 1), Point(PK_TICK_FRAME_INSET+PK_TICK_WIDTH+RD_SPACE+PK_WIDTH/2,y+RD_TEXT_SM_OFFSET/2.5), RD_FONT, RD_TEXT_SM, RD_TEXT_COLOR);
        if(PK_HAS_SUBTICKS) line(mat, Point(PK_TICK_FRAME_INSET+PK_SUBTICK_INSET+PK_WIDTH/2,y-PK_TICK_SPACING/2), Point(PK_TICK_FRAME_INSET+PK_TICK_WIDTH-PK_SUBTICK_INSET+PK_WIDTH/2,y-PK_TICK_SPACING/2), RD_LINE_COLOR);
    }
    
    // Graph data
    ///
    floating_t f = PK_GRAPH_HEIGHT/PK_GRAPH_MAX_VALUE-1, o = PK_WIDTH/2;
    int prev_u = PK_END.y-BOUNDU(pDu[0]*f, PK_GRAPH_HEIGHT),
    prev_l = PK_END.y-BOUNDU(pDl[0]*f, PK_GRAPH_HEIGHT),
    prev_lv = PK_END.y-BOUNDU(pDlv[0]*f, PK_GRAPH_HEIGHT),
    prev_lt = PK_END.y-BOUNDU(pDlt[0]*f, PK_GRAPH_HEIGHT),
    prev_uv = PK_END.y-BOUNDU(pDuv[0]*f, PK_GRAPH_HEIGHT),
    prev_ut = PK_END.y-BOUNDU(pDut[0]*f, PK_GRAPH_HEIGHT),
    curr_l, curr_lv, curr_lt, curr_u, curr_uv, curr_ut;
    for(int i = 1, x = PK_TICK_FRAME_INSET+2*RD_SPACE; i < pX && x < PK_END.x/2+RD_ORIGIN.x; i++, x++)
    {
        curr_u = PK_END.y-BOUNDU(pDu[i]*f, PK_GRAPH_HEIGHT);
        curr_l = PK_END.y-BOUNDU(pDl[i]*f, PK_GRAPH_HEIGHT);
        curr_uv = PK_END.y-BOUNDU(pDuv[i]*f, PK_GRAPH_HEIGHT);
        curr_lv = PK_END.y-BOUNDU(pDlv[i]*f, PK_GRAPH_HEIGHT);
        curr_ut = PK_END.y-BOUNDU(pDut[i]*f, PK_GRAPH_HEIGHT);
        curr_lt = PK_END.y-BOUNDU(pDlt[i]*f, PK_GRAPH_HEIGHT);
        
        line(mat, Point(x-1, prev_u), Point(x, curr_u), red);
        line(mat, Point(x-1, prev_uv), Point(x, curr_uv), redish);
        line(mat, Point(x-1, prev_ut), Point(x, curr_ut), orangish);
        
        line(mat, Point(x-1+o, prev_l), Point(x+o, curr_l), blue);
        line(mat, Point(x-1+o, prev_lv), Point(x+o, curr_lv), bluish);
        line(mat, Point(x-1+o, prev_lt), Point(x+o, curr_lt), greenish);
        
        prev_u = curr_u;
        prev_l = curr_l;
        prev_uv = curr_uv;
        prev_lv = curr_lv;
        prev_ut = curr_ut;
        prev_lt = curr_lt;
    }
    pX++;
    if(pX < 0 || pX > PK_WIDTH/2-3*RD_SPACE)
    {
        pX = 0;
        memset(pDu, 0, sizeof(floating_t)*width);
        memset(pDl, 0, sizeof(floating_t)*width);
    }
    pthread_mutex_unlock(&drawer_mutex);
    return mat;
}
