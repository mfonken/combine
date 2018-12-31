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
: Tau(name, width, height, f, num), frame(Size(width,  height), CV_8UC3, Scalar(0,0,0)), probabilities(Size(800, 200), CV_8UC3, Scalar(245,245,245)),
rho_frame(Size(width, height), CV_8UC3, Scalar(0,0,0)),
rho_detection_x(Size(width, height), CV_8UC3, Scalar(0,0,0)),
rho_detection_y(Size(width, height), CV_8UC3, Scalar(0,0,0))
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
}

TauDrawer::~TauDrawer()
{
    if(pid_data != NULL)
        delete pid_data;
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

Mat& TauDrawer::GetDensitiesFrame(void)
{
    pthread_mutex_lock(&drawer_mutex);
    utility.outframe.copyTo(frame(Rect(0,0,w,h)));
    DrawDensityGraph(frame);
    DrawDensityMaps(frame);
    
    density_2d_t    Qv[] = { rho.utility.Q[0],  rho.utility.Q[1],  rho.utility.Q[2],  rho.utility.Q[3]  },
                    Qb[] = { rho.utility.Qb[0], rho.utility.Qb[1], rho.utility.Qb[2], rho.utility.Qb[3] },
                    Qf[] = { rho.utility.Qf[0], rho.utility.Qf[1], rho.utility.Qf[2], rho.utility.Qf[3] };

    putText(frame, to_string(Qv[0]), Point(inseta_, insetb_),                       FONT_HERSHEY_PLAIN, fontsize_, fontcolora_, 4);
    putText(frame, to_string(Qv[1]), Point(w-insetb_-RHO_MAPS_INSET, insetb_),      FONT_HERSHEY_PLAIN, fontsize_, fontcolora_, 4);
    putText(frame, to_string(Qv[2]), Point(inseta_, h-inseta_),                     FONT_HERSHEY_PLAIN, fontsize_, fontcolora_, 4);
    putText(frame, to_string(Qv[3]), Point(w-insetb_-RHO_MAPS_INSET, h-inseta_),    FONT_HERSHEY_PLAIN, fontsize_, fontcolora_, 4);

    putText(frame, to_string(Qb[0]), Point(inseta_, insetb_+18),                    FONT_HERSHEY_PLAIN, fontsize_, fontcolorb_, 4);
    putText(frame, to_string(Qb[1]), Point(w-insetb_-RHO_MAPS_INSET, insetb_+18),   FONT_HERSHEY_PLAIN, fontsize_, fontcolorb_, 4);
    putText(frame, to_string(Qb[2]), Point(inseta_, h-inseta_+18),                  FONT_HERSHEY_PLAIN, fontsize_, fontcolorb_, 4);
    putText(frame, to_string(Qb[3]), Point(w-insetb_-RHO_MAPS_INSET, h-inseta_+18), FONT_HERSHEY_PLAIN, fontsize_, fontcolorb_, 4);

    putText(frame, to_string(Qf[0]), Point(inseta_, insetb_+36),                    FONT_HERSHEY_PLAIN, fontsize_, fontcolorc_, 4);
    putText(frame, to_string(Qf[1]), Point(w-insetb_-RHO_MAPS_INSET, insetb_+36),   FONT_HERSHEY_PLAIN, fontsize_, fontcolorc_, 4);
    putText(frame, to_string(Qf[2]), Point(inseta_, h-inseta_+36),                  FONT_HERSHEY_PLAIN, fontsize_, fontcolorc_, 4);
    putText(frame, to_string(Qf[3]), Point(w-insetb_-RHO_MAPS_INSET, h-inseta_+36), FONT_HERSHEY_PLAIN, fontsize_, fontcolorc_, 4);
    
    putText(frame, "Thresh: " + to_string(utility.thresh), Point(0, 14), FONT_HERSHEY_PLAIN, 1, Vec3b(255,0,255), 2);
    putText(frame, "State: " + string(stateString(rho.utility.BayeSys.state)), Point(0, 28), FONT_HERSHEY_PLAIN, 1, Vec3b(255,0,155), 2);
    
    putText(frame, "X (" + to_string(rho.utility.Cx) + ", " + to_string(rho.utility.Cy) + ")", Point(rho.utility.Cx-9, rho.utility.Cy+10), FONT_HERSHEY_PLAIN, 2, yellowish, 4);
    
    rectangle(frame, Point(frame.cols-RHO_MAPS_INSET,frame.rows-RHO_MAPS_INSET), Point(frame.cols-RHO_MAPS_INSET,frame.rows-RHO_MAPS_INSET), Scalar(0,0,0));
    
    pthread_mutex_unlock(&drawer_mutex);
    
    return frame;
}

void TauDrawer::DrawDensityGraph(Mat M)
{
    int u, v, w = width, h = height;
    
    int x1 = w, x2 = w, y1 = h, y2 = h,
    rangex[3] = { w, (int)rho.utility.Cx, 0 },
    rangey[3] = { h, (int)rho.utility.Cy, 0 },
    Bx = rho.utility.Bx,
    By = rho.utility.By;
    line(M, Point(rangex[1],0),   Point(rangex[1],H), redish);
    line(M, Point(0,rangey[1]),   Point(W,rangey[1]), redish);
    
    Point pA[2], pB[2], pC[2], pD[2];
    
    for( int i = 0, j = 1; i < 2; i++, j++ )
    {
        bool pT = 1;
//        bool first = 1;
        /* Kalman Values */
        rho_kalman_t yk = rho.utility.DensityMapPair.x.kalmans[i], xk = rho.utility.DensityMapPair.y.kalmans[i];
        int mYv = rho.utility.DensityMapPair.x.kalmans[i].variance, mXv = rho.utility.DensityMapPair.y.kalmans[i].variance;
       
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
            dX[y1] = INR(rho.utility.DensityMapPair.x.map[y1], h);
            fX[y1] = INR(rho.utility.DensityMapPair.x.background[y1], h);
        }
        for( x1 = rangex[i]; x1 > rangex[j];  )
        {
            --x1;
            dY[x1] = INR(rho.utility.DensityMapPair.y.map[x1], w);
            fY[x1] = INR(rho.utility.DensityMapPair.y.background[x1], w);
        }
        pthread_mutex_unlock(&rho.density_map_pair_mutex);
        
        line(RMX, Point(By,RHO_MAPS_HEIGHT), Point(By,0), orangish);
        line(RMY, Point(Bx,RHO_MAPS_HEIGHT), Point(Bx,0), orangish);
        
#ifdef DRAW_RHO_MAPS
        line(RMY, Point(0,nd), Point(H,nd), orangish);
        line(RMY, Point(0,nv1d), Point(H,nv1d), yellowish);
        line(RMY, Point(0,nv2d), Point(H,nv2d), yellowish);
#endif
        for( y2 = rangey[i]; y2 > rangey[j];  )
        {
            --y2;
            u = INR(OP_ALIGN((fX[y2]/DENSITY_SCALE),w),w);
            v = INR(OP_ALIGN((dX[y2]/DENSITY_SCALE),w),w);
            
            if(v>ndm)
            {
                M.at<Vec3b>(y2,v) = white;
            }
            else
            {
                int under = RANGE(ndm + abs(ndm-v),w);
                int over  = RANGE(ndm - abs(ndm-v),w);
                
                M.at<Vec3b>(y2,under)   = bluish;
                M.at<Vec3b>(y2,over)   = redish;
                M.at<Vec3b>(y2, u) = greenish;
            }
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
        for( x2 = rangex[i]; x2 > rangex[j]; --x2 )
        {
            u = INR(OP_ALIGN((fY[x2]/DENSITY_SCALE),h),h);
            v = INR(OP_ALIGN((dY[x2]/DENSITY_SCALE),h),h);
            
            if(v>mdm)
            {
                M.at<Vec3b>(v,x2) = white;
//                line(M, Point(x2,v), Point(x2,h), white);
            }
            else
            {
                int under = RANGE(mdm + abs(mdm-v),h);
                int over = RANGE(mdm - abs(mdm-v),h);
                
                M.at<Vec3b>(under,x2) = bluish;
                M.at<Vec3b>(over,x2) = redish;
                M.at<Vec3b>(u, x2) = greenish;
                
//                line(M, Point(x2,under), Point(x2,h), white);
//                line(M, Point(x2,under), Point(x2,over), bluish);
            }
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
    for( int i = 0; i < rho.utility.PredictionPair.y.NumBlobs; i++ )
    {
        int o = rho.utility.PredictionPair.y.BlobsOrder[i];
        int v = rho.utility.PredictionPair.y.Blobs[o].loc;
        line(M, Point(v,0),Point(v,h), bluish);
    }
    for( int i = 0; i < rho.utility.PredictionPair.x.NumBlobs; i++ )
    {
        int o = rho.utility.PredictionPair.x.BlobsOrder[i];
        int v = rho.utility.PredictionPair.x.Blobs[o].loc;
        line(M, Point(0,v),Point(w,v), bluish);
    }
    
    // Draw tracking predictions
    for( int i = 0; i < 2; i++ )
    {
        int o = rho.utility.PredictionPair.y.TrackingFiltersOrder[i];
        int v = rho.utility.PredictionPair.y.TrackingFilters[o].value;
        line(M, Point(v,0),Point(v,h), greyish);
    }
    for( int i = 0; i < 2; i++ )
    {
        int o = rho.utility.PredictionPair.x.TrackingFiltersOrder[i];
        int v = rho.utility.PredictionPair.x.TrackingFilters[o].value;
        line(M, Point(0,v),Point(w,v), greyish);
    }
}

void TauDrawer::DrawDensityMaps(Mat M)
{
    Vec3b c(0,0,0);
    int x = 0, y = 0, rangex[2] = { (int)rho.utility.Cx, w }, rangey[2] = { (int)rho.utility.Cy, h };
    for( int i = 0; i < 2; i++ )
    {
        int mX = rho.utility.DensityMapPair.x.max[1-i], mY = rho.utility.DensityMapPair.y.max[1-i];
        if(!mX) mX = 1;
        if(!mY) mY = 1;
        density_t *dX = rho.utility.DensityMapPair.x.map, *dY = rho.utility.DensityMapPair.y.map;
        
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

void TauDrawer::DrawRhoProbabilities(Mat M)
{
    int w = M.cols, h = M.rows, h_ = h-RHO_MAPS_INSET;
    M = {Scalar(245,245,245)};
    double prob[8] =
    {
        rho.utility.PredictionPair.x.Probabilities.P[1],
        rho.utility.PredictionPair.x.Probabilities.P[2],
        rho.utility.PredictionPair.x.Probabilities.P[3],
        rho.utility.PredictionPair.x.Probabilities.P[0],
        rho.utility.PredictionPair.y.Probabilities.P[1],
        rho.utility.PredictionPair.y.Probabilities.P[2],
        rho.utility.PredictionPair.y.Probabilities.P[3],
        rho.utility.PredictionPair.y.Probabilities.P[0]
    };
    
    line(M, Point(RHO_MAPS_INSET, h_+1), Point(w-RHO_MAPS_INSET, h_+1), Scalar(15,15,15));
    line(M, Point(w/2, h_+10), Point(w/2, RHO_MAPS_INSET-10), Scalar(15,15,15));
    
    int col_space = 10, col_width = (w - ((8*col_space)+2*RHO_MAPS_INSET))/8, col_scale = h-RHO_MAPS_INSET*2;
    
    line(M, Point(RHO_MAPS_INSET, h_-col_scale), Point(w-RHO_MAPS_INSET, h_-col_scale), Scalar(115,115,145));
    
    int x = RHO_MAPS_INSET, c = 0;
    for( int i = 0; i < 8; i++)
    {
        int y = h_- prob[i]*col_scale;
        rectangle(M, Point(x,h_), Point(x+col_width, y), densityColor(c), -1);
        x += col_space + col_width;
        c += 255/4;
        if(i == 3)
        {
            x += col_space;
            c = 0;
        }
    }
}

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

Mat& TauDrawer::DrawRhoFrame()
{
    /* Gather data */
    pthread_mutex_lock(&rho.c_mutex);
    floating_t total_cvg_percent = (floating_t)rho.utility.TotalCoverage/(rho.utility.Width*rho.utility.Height),
    filtered_cvg_percent = rho.utility.FilteredPercentage;
    floating_t X_kumaraswamy[NUM_STATE_GROUPS] = { 0. },
    Y_kumaraswamy[NUM_STATE_GROUPS] = { 0. },
    C_kumaraswamy[NUM_STATE_GROUPS] = { 0. };
    memcpy(X_kumaraswamy, rho.utility.PredictionPair.x.Probabilities.P, sizeof(floating_t)*NUM_STATE_GROUPS);
    memcpy(Y_kumaraswamy, rho.utility.PredictionPair.y.Probabilities.P, sizeof(floating_t)*NUM_STATE_GROUPS);
    memcpy(C_kumaraswamy, rho.utility.PredictionPair.Probabilities.P, sizeof(floating_t)*NUM_STATE_GROUPS);
    floating_t
    state_P[NUM_STATES] = { 0. };
    state_t state = rho.utility.BayeSys.state;
    for(int i = 0; i < NUM_STATES; i++)
        state_P[i] = rho.utility.BayeSys.probabilities.map[i][state];
    
    floating_t target_cvg_percent = rho.utility.TargetCoverageFactor;
    pid_data[pid_data_x++] = target_cvg_percent - rho.utility.ThreshFilter.Value;
    pthread_mutex_unlock(&rho.c_mutex);
    
    /* Write to file */
    ofstream file(FILENAME, ofstream::out | ofstream::app);
    file << frame_i++ << "," << total_cvg_percent << "," << filtered_cvg_percent;
    for(int i = 0; i < NUM_STATE_GROUPS; i++) file << "," << X_kumaraswamy[i];
    for(int i = 0; i < NUM_STATE_GROUPS; i++) file << "," << Y_kumaraswamy[i];
    for(int i = 0; i < NUM_STATE_GROUPS; i++) file << "," << C_kumaraswamy[i];
    for(int i = 0; i < NUM_STATES; i++) file << "," << state_P[i];
    file << "," << target_cvg_percent << "," << pid_data[pid_data_x-1];
    file << endl;
    file.close();

    /* Clear */
    rho_frame = RHO_DRAWERS_BACKGROUND_COLOR;
    
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
    
    Scalar RF_COLOR_A(0,50,150),
    RF_COLOR_B(150,100,0),
    RF_COLOR_C(50,150,50);
    
#define CVG_SPLIT 0.05
#define ST_SPLIT 0.5
#define PID_SPLIT 0.45
    
#define RF_SCALED_HEIGHT(S) (RF_HEIGHT*S-RF_SPACE)
    
    /* Coverage */
#define CVG_ORIGIN Point(RF_SPACE, RF_SPACE)
#define CVG_WIDTH (RF_WIDTH-2*RF_SPACE)
#define CVG_HEIGHT RF_SCALED_HEIGHT(CVG_SPLIT)
#define CVG_TEXT_X (CVG_ORIGIN.x)
#define CVG_TEXT_HEIGHT (CVG_ORIGIN.y+RF_TEXT_SM_OFFSET)
#define CVG_END Point(CVG_ORIGIN.x+CVG_WIDTH, CVG_ORIGIN.y+CVG_HEIGHT)
#define CVG_COLOR white
#define CVG_TOP_BOUND 1.
#define TOTAL_CVG_ORIGIN
#define TOTAL_CVG_TEXT_X (TOTAL_CVG_ORIGIN.x+CVG_TEXT_SM_OFFSET)
    
    // Bound
    rectangle(rho_frame, CVG_ORIGIN, CVG_END, CVG_COLOR, RF_LINE_WIDTH);
    putText(rho_frame, to_stringn(CVG_TOP_BOUND, RF_TEXT_PRECISION)+"%", Point(CVG_TEXT_X, CVG_TEXT_HEIGHT+2), RF_FONT, RF_TEXT_SM, RF_TEXT_COLOR);
    // Total
    int total_offset = (1-(total_cvg_percent*100.)/CVG_TOP_BOUND)*CVG_WIDTH;
    if(total_offset > CVG_ORIGIN.x)
    {
        rectangle(rho_frame, Point(CVG_ORIGIN.x+total_offset+1,CVG_ORIGIN.y+1), Point(CVG_END.x-1,CVG_END.y-1), RF_COLOR_B, CV_FILLED);
        putText(rho_frame, to_stringn(total_cvg_percent*100, RF_TEXT_PRECISION)+"%", Point(CVG_ORIGIN.x+total_offset, CVG_TEXT_HEIGHT+2), RF_FONT, RF_TEXT_SM, RF_TEXT_COLOR);
        //Filtered
        int filtered_offset = (1-(total_cvg_percent*filtered_cvg_percent*100.)/CVG_TOP_BOUND)*CVG_WIDTH;
        if(filtered_offset > CVG_ORIGIN.x)
        {
            rectangle(rho_frame, Point(CVG_ORIGIN.x+filtered_offset+1,CVG_ORIGIN.y+1), Point(CVG_END.x-1,CVG_END.y-1), RF_COLOR_C, CV_FILLED);
            if(CVG_ORIGIN.x+filtered_offset < RF_WIDTH - 100)
                putText(rho_frame, to_stringn(filtered_cvg_percent*100, RF_TEXT_PRECISION)+"%", Point(CVG_ORIGIN.x+filtered_offset, CVG_TEXT_HEIGHT+2), RF_FONT, RF_TEXT_SM, RF_TEXT_COLOR);
        }
    }
    
    /* States */
#define ST_ORIGIN Point(RF_SPACE, CVG_ORIGIN.y+CVG_HEIGHT+RF_SPACE)
#define ST_WIDTH (RF_WIDTH-2*RF_SPACE)
#define ST_HEIGHT RF_SCALED_HEIGHT(ST_SPLIT)
#define ST_BAR_MAX (ST_HEIGHT/2)
#define ST_BAR_Y (ST_ORIGIN.y+ST_HEIGHT/2-1)
#define ST_RADIUS (ST_BAR_MAX/3)
#define ST_TEXT_X (ST_ORIGIN.x)
#define ST_TEXT_HEIGHT (ST_ORIGIN.y+RF_TEXT_SM_OFFSET)
#define ST_END Point(ST_ORIGIN.x+ST_WIDTH, ST_ORIGIN.y+ST_HEIGHT)
#define ST_COLOR white
    // Bound
    rectangle(rho_frame, ST_ORIGIN, ST_END, ST_COLOR, RF_LINE_WIDTH);
    
    // Legend
    rectangle(rho_frame, Point(ST_ORIGIN.x+ST_TEXT_X*0.5,ST_ORIGIN.y+ST_TEXT_X*0.5), Point(ST_ORIGIN.x+ST_TEXT_X*4.,ST_ORIGIN.y+ST_TEXT_X*4.5), RF_LINE_COLOR);
    rectangle(rho_frame, Point(ST_ORIGIN.x+ST_TEXT_X,ST_ORIGIN.y+ST_TEXT_X), Point(ST_ORIGIN.x+2*ST_TEXT_X,ST_ORIGIN.y+2*ST_TEXT_X), RF_COLOR_A, CV_FILLED);
    rectangle(rho_frame, Point(ST_ORIGIN.x+ST_TEXT_X,ST_ORIGIN.y+2*ST_TEXT_X), Point(ST_ORIGIN.x+2*ST_TEXT_X,ST_ORIGIN.y+3*ST_TEXT_X), RF_COLOR_B, CV_FILLED);
    rectangle(rho_frame, Point(ST_ORIGIN.x+ST_TEXT_X,ST_ORIGIN.y+3*ST_TEXT_X), Point(ST_ORIGIN.x+2*ST_TEXT_X,ST_ORIGIN.y+4*ST_TEXT_X), RF_COLOR_C, CV_FILLED);
    putText(rho_frame, "X", Point(3.5*ST_TEXT_X, ST_TEXT_HEIGHT+ST_TEXT_X*0.5), RF_FONT, RF_TEXT_SM, RF_COLOR_A);
    putText(rho_frame, "Y", Point(3.5*ST_TEXT_X, ST_TEXT_HEIGHT+ST_TEXT_X*1.5), RF_FONT, RF_TEXT_SM, RF_COLOR_B);
    putText(rho_frame, "A", Point(3.5*ST_TEXT_X, ST_TEXT_HEIGHT+ST_TEXT_X*2.5), RF_FONT, RF_TEXT_SM, RF_COLOR_C);
    
    // Split Lines
    line(rho_frame, Point(ST_ORIGIN.x,ST_ORIGIN.y+ST_HEIGHT/2), Point(ST_END.x,ST_ORIGIN.y+ST_HEIGHT/2), RF_LINE_COLOR, RF_LINE_WIDTH);
    int width = ST_WIDTH/(NUM_STATE_GROUPS);
    for(int i = 0, x = width+ST_ORIGIN.x; i < NUM_STATE_GROUPS; i++, x += width)
        line(rho_frame, Point(x,ST_ORIGIN.y+RF_SPACE), Point(x,ST_ORIGIN.y+ST_HEIGHT-RF_SPACE), RF_LINE_COLOR, RF_LINE_WIDTH);
    
    //State Data
    int bar_width = (width-2*RF_SPACE)/3;
    for(int i = 0, x = ST_ORIGIN.x+RF_SPACE; i < NUM_STATE_GROUPS; i++, x += width)
    {
        rectangle(rho_frame, Point(x,ST_BAR_Y), Point(x+bar_width, ST_BAR_Y-X_kumaraswamy[i]*ST_BAR_MAX), RF_COLOR_A, CV_FILLED);
        rectangle(rho_frame, Point(x+bar_width,ST_BAR_Y), Point(x+2*bar_width, ST_BAR_Y-Y_kumaraswamy[i]*ST_BAR_MAX), RF_COLOR_B, CV_FILLED);
        rectangle(rho_frame, Point(x+2*bar_width,ST_BAR_Y), Point(x+3*bar_width, ST_BAR_Y-C_kumaraswamy[i]*ST_BAR_MAX), RF_COLOR_C, CV_FILLED);
        
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
        putText(rho_frame, state_n, Point(x, ST_BAR_Y+2*RF_SPACE), RF_FONT, RF_TEXT_SM, c);
        circle(rho_frame, Point(x+width/2-RF_SPACE,ST_BAR_Y+ST_BAR_MAX/2), r, c, CV_FILLED);
        putText(rho_frame, to_string((int)(state_P[state_i]*100))+"%", Point(x+width/2-2.8*RF_SPACE,ST_BAR_Y+ST_BAR_MAX/2+5), RF_FONT, RF_TEXT_SM, RF_LINE_COLOR);
    }
    
    /* PID */
#define PID_ORIGIN Point(RF_SPACE, ST_ORIGIN.y+ST_HEIGHT+RF_SPACE)
#define PID_WIDTH (RF_WIDTH-2*RF_SPACE)
#define PID_HEIGHT (RF_SCALED_HEIGHT(PID_SPLIT)-RF_SPACE)
#define PID_TICKS 5
#define PID_TICK_SPACING (PID_HEIGHT/(PID_TICKS+1))
#define PID_HAS_SUBTICKS true
#define PID_TICK_WIDTH 7
#define PID_TICK_FRAME_INSET 2*RF_SPACE
#define PID_SUBTICK_INSET 2
#define PID_GRAPH_START PID_ORIGIN.x+RF_SPACE
#define PID_GRAPH_WIDTH (PID_WIDTH-2*RF_SPACE)
#define PID_MAX_BOUND 2. // percent
#define PID_TEXT_X (PID_ORIGIN.x)
#define PID_TEXT_HEIGHT (PID_ORIGIN.y+RF_TEXT_SM_OFFSET)
#define PID_END Point(PID_ORIGIN.x+PID_WIDTH, PID_ORIGIN.y+PID_HEIGHT)
#define PID_COLOR white
    
    // Bound
    rectangle(rho_frame, PID_ORIGIN, PID_END, PID_COLOR, RF_LINE_WIDTH);
    
    // Ticks
    if(PID_HAS_SUBTICKS) line(rho_frame, Point(PID_TICK_FRAME_INSET+PID_SUBTICK_INSET,PID_END.y-PID_TICK_SPACING/2), Point(PID_TICK_FRAME_INSET+PID_TICK_WIDTH-PID_SUBTICK_INSET,PID_END.y-PID_TICK_SPACING/2), RF_LINE_COLOR);
    for(int i = 1, y = PID_END.y - PID_TICK_SPACING; i <= PID_TICKS; i++, y -= PID_TICK_SPACING)
    {
        line(rho_frame, Point(PID_TICK_FRAME_INSET,y), Point(PID_TICK_FRAME_INSET+PID_TICK_WIDTH,y), RF_LINE_COLOR, RF_LINE_WIDTH);
        floating_t tick_value = PID_MAX_BOUND*((floating_t)i)/PID_TICKS;
        putText((rho_frame), to_stringn(tick_value, 1) + "%", Point(PID_TICK_FRAME_INSET+PID_TICK_WIDTH+RF_SPACE,y+RF_TEXT_SM_OFFSET/2.5), RF_FONT, RF_TEXT_SM, RF_TEXT_COLOR);
        if(PID_HAS_SUBTICKS) line(rho_frame, Point(PID_TICK_FRAME_INSET+PID_SUBTICK_INSET,y-PID_TICK_SPACING/2), Point(PID_TICK_FRAME_INSET+PID_TICK_WIDTH-PID_SUBTICK_INSET,y-PID_TICK_SPACING/2), RF_LINE_COLOR);
    }
    
    if(pid_data_x <= 0 || pid_data_x > PID_GRAPH_WIDTH)
    {
        pid_data_x = 0;
        memset(pid_data, 0, sizeof(floating_t)*width);
    }
    
    // Graph
    int scaled_target_value = BOUND(target_cvg_percent*100/PID_MAX_BOUND*(PID_HEIGHT-PID_TICK_SPACING), 0, PID_HEIGHT);
    line(rho_frame, Point(PID_GRAPH_START,PID_END.y - scaled_target_value), Point(PID_GRAPH_START+PID_GRAPH_WIDTH, PID_END.y - scaled_target_value), RF_COLOR_B);
    int pid_prev = 0;
    for(int i = 0, x = PID_GRAPH_START; i < pid_data_x; i++, x++)
    {
        floating_t curr = pid_data[i];
        if(curr > PID_MAX_BOUND) continue;
        int scaled_pid_value = BOUND(curr*100/PID_MAX_BOUND*(PID_HEIGHT-PID_TICK_SPACING), 0, PID_HEIGHT),
        curr_scaled = PID_END.y - scaled_pid_value-1;
        if(pid_prev == 0)
            rho_frame.at<Scalar>(curr_scaled,x) = RF_COLOR_A;
        else
            line(rho_frame, Point(x-1,pid_prev), Point(x, curr_scaled), RF_COLOR_A);
        pid_prev = curr_scaled;
    }
    
    return rho_frame;
}

Mat& TauDrawer::DrawRhoDetection(int dimension)
{
    Mat &mat = (dimension==X_DIMENSION)?rho_detection_x:rho_detection_y;
    int &mX = (dimension==X_DIMENSION)?x_match_data_x:y_match_data_x,
    &pX = (dimension==X_DIMENSION)?x_peak_data_x:y_peak_data_x;
    int *mD = (dimension==X_DIMENSION)?x_match_data:y_match_data;
    floating_t *pDu = (dimension==X_DIMENSION)?x_peak_data_u:y_peak_data_u;
    floating_t *pDuv = (dimension==X_DIMENSION)?x_peak_data_uv:y_peak_data_uv;
    floating_t *pDut = (dimension==X_DIMENSION)?x_peak_data_ut:y_peak_data_ut;
    floating_t *pDl = (dimension==X_DIMENSION)?x_peak_data_l:y_peak_data_l;
    floating_t *pDlv = (dimension==X_DIMENSION)?x_peak_data_lv:y_peak_data_lv;
    floating_t *pDlt = (dimension==X_DIMENSION)?x_peak_data_lt:y_peak_data_lt;
    
    density_map_t &map = (dimension==X_DIMENSION)?rho.utility.DensityMapPair.x:rho.utility.DensityMapPair.y;
    prediction_t &prediction = (dimension==X_DIMENSION)?rho.utility.PredictionPair.x:rho.utility.PredictionPair.y;
    string filename =(dimension==X_DIMENSION)?X_DIM_FILENAME:Y_DIM_FILENAME;
    int &iteration = (dimension==X_DIMENSION)?x_detection_i:y_detection_i;
    
#define MATCH_DATA_WIDTH 3
#define MATCH_ENCODE(X,i,S) X |= S << (i*MATCH_DATA_WIDTH-1)
#define MATCH_DECODE(X,i) ((1 << MATCH_DATA_WIDTH)-1) & X >> (i*MATCH_DATA_WIDTH-1)
    
    /* Gather data */
    pthread_mutex_lock(&rho.c_mutex);
    int match_value = 0;
    rho_kalman_t tracking_filters[MAX_TRACKING_FILTERS];
    int tracking_filters_order[MAX_TRACKING_FILTERS] = {0};
    for(int i = 0; i < MAX_TRACKING_FILTERS; i++)
    {
        int c = prediction.TrackingFiltersOrder[i];
        MATCH_ENCODE(match_value, i, c);
        memcpy(&tracking_filters[i], &prediction.TrackingFilters[c], sizeof(rho_kalman_t));
        tracking_filters_order[i] = c;
//        printf("%d %d %x\n", i, c, match_value);
    }
//    printf("\n");
//    for(int i = 0; i < MAX_TRACKING_FILTERS; i++)
//    {
//        printf("%d %d\n", i, MATCH_DECODE(match_value, i));
//    }
    
    mD[mX] = match_value;
    
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
    mat = RHO_DRAWERS_BACKGROUND_COLOR;
    
    /* Draw */
#define RD_SPACE 10
#define RD_WIDTH mat.cols
#define RD_HEIGHT mat.rows
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
    
#define RD_SCALED_HEIGHT(S) (RD_HEIGHT*S-RF_SPACE)
    
#define BL_SPLIT 0.5
#define MCH_SPLIT 0.1
#define PK_SPLIT 0.4
    
    /* Blobs */
#define BL_WIDTH RD_INNER_WIDTH
#define BL_HEIGHT (RD_SCALED_HEIGHT(BL_SPLIT))
#define BL_MAX_NUM 4
#define BL_IND_HEIGHT ((BL_HEIGHT-RD_SPACE)/BL_MAX_NUM-RD_SPACE)
#define BL_ORIGIN Point(RD_SPACE,RD_SPACE)
#define BL_END Point(BL_ORIGIN.x+BL_WIDTH, BL_ORIGIN.y+BL_HEIGHT)
#define BL_MAX_SIZE 100
#define BL_MAX_RADIUS (BL_IND_HEIGHT*0.9)
#define BL_RADIUS_SCALE BL_MAX_RADIUS/(BL_MAX_SIZE/(2*M_PI))

    // Bounds
    rectangle(mat, BL_ORIGIN, BL_END, RD_LINE_COLOR);
    
    // Blobs
    int num_tracks = 0;
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
            num_tracks++;
            int r = BL_RADIUS_SCALE*curr.den/(2*M_PI);
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
        Point matchOrigin(textOrigin.x+160, textOrigin.y);
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
#define MCH_ORIGIN Point(RD_SPACE,BL_END.y+RD_SPACE)
#define MCH_END Point(MCH_ORIGIN.x+MCH_WIDTH, MCH_ORIGIN.y+MCH_HEIGHT)
#define MCH_INSET_X (MCH_ORIGIN.x+3*RD_SPACE)
#define MCH_INTERVAL_WIDTH 5 // pixels
#define MCH_INTERVAL_SPACE 3 // pixels
#define MCH_INTERVAL_TOTAL (MCH_INTERVAL_WIDTH+MCH_INTERVAL_SPACE)
#define MCH_NUM_INTERVALS ((MCH_END.x-MCH_INSET_X)/MCH_INTERVAL_TOTAL)
#define MCH_LINE_WIDTH 2
    
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
        for(int j = 0; j < num_tracks; j++)
        {
            currV = MATCH_DECODE(currD,j);
            prevV = MATCH_DECODE(prevD,j);
            line(mat, Point(x,y+MCH_IND_HEIGHT*prevV), Point(x+MCH_INTERVAL_WIDTH,y+MCH_IND_HEIGHT*currV), red, MCH_LINE_WIDTH);
            line(mat, Point(x+MCH_INTERVAL_WIDTH,y+MCH_IND_HEIGHT*currV), Point(x+MCH_INTERVAL_WIDTH+MCH_INTERVAL_SPACE,y+MCH_IND_HEIGHT*currV), red, MCH_LINE_WIDTH);
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
#define PK_ORIGIN Point(RD_SPACE,MCH_END.y+RD_SPACE)
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
    rectangle(mat, PK_ORIGIN, PK_END, RD_LINE_COLOR);
    line(mat, Point(PK_ORIGIN.x,PK_ORIGIN.y+PK_CENTROID_DISPLAY_HEIGHT), Point(PK_ORIGIN.x+PK_WIDTH,PK_ORIGIN.y+PK_CENTROID_DISPLAY_HEIGHT), RD_LINE_COLOR);
    
    //Centroid
    int centroid_x = PK_WIDTH*c_percent;
    line(mat, Point(centroid_x, PK_ORIGIN.y), Point(centroid_x, PK_ORIGIN.y+PK_CENTROID_DISPLAY_HEIGHT), RD_LINE_COLOR);
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
    line(mat, Point(PK_ORIGIN.x+PK_WIDTH/2, PK_ORIGIN.y+PK_CENTROID_DISPLAY_HEIGHT), Point(PK_ORIGIN.x+PK_WIDTH/2, PK_ORIGIN.y+PK_HEIGHT), RD_LINE_COLOR);
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
    int prev_u = PK_END.y-BOUNDU(pDu[0]*f, PK_HEIGHT),
    prev_l = PK_END.y-BOUNDU(pDl[0]*f, PK_HEIGHT),
    prev_lv = PK_END.y-BOUNDU(pDlv[0]*f, PK_HEIGHT),
    prev_lt = PK_END.y-BOUNDU(pDlt[0]*f, PK_HEIGHT),
    prev_uv = PK_END.y-BOUNDU(pDuv[0]*f, PK_HEIGHT),
    prev_ut = PK_END.y-BOUNDU(pDut[0]*f, PK_HEIGHT),
    curr_l, curr_lv, curr_lt, curr_u, curr_uv, curr_ut;
    for(int i = 1, x = PK_TICK_FRAME_INSET+2*RD_SPACE; i < pX && x < PK_END.x/2; i++, x++)
    {
        curr_u = PK_END.y-BOUNDU(pDu[i]*f, PK_HEIGHT);
        curr_l = PK_END.y-BOUNDU(pDl[i]*f, PK_HEIGHT);
        curr_uv = PK_END.y-BOUNDU(pDuv[i]*f, PK_HEIGHT);
        curr_lv = PK_END.y-BOUNDU(pDlv[i]*f, PK_HEIGHT);
        curr_ut = PK_END.y-BOUNDU(pDut[i]*f, PK_HEIGHT);
        curr_lt = PK_END.y-BOUNDU(pDlt[i]*f, PK_HEIGHT);
        
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
    if(pX < 0 || pX > PK_WIDTH/2)
    {
        pX = 0;
        memset(pDu, 0, sizeof(floating_t)*width);
        memset(pDl, 0, sizeof(floating_t)*width);
    }
    
    return mat;
}


//void TauDrawer::drawKalmans()
//{
//    char buff[120];
//    rho_kalman_t * xk = &rho.utility.density_map_pair.x.kalman, * yk = &rho.utility.density_map_pair.y.kalman;
//    snprintf(buff, sizeof(buff), "P[%.3f][%.3f][%.3f][%.3f] K[%.3f][%.3f] R[%.3f] B[%.3f] V[%.3f] E[%.3f]", xk->P[0][0], xk->P[0][1], xk->P[1][0], xk->P[1][1], xk->K[0], xk->K[1], xk->rate, xk->bias, xk->value, xk->velocity);
//    string xks = buff;
//    snprintf(buff, sizeof(buff), "P[%.3f][%.3f][%.3f][%.3f] K[%.3f][%.3f] R[%.3f] B[%.3f] V[%.3f] E[%.3f]", yk->P[0][0], yk->P[0][1], yk->P[1][0], yk->P[1][1], yk->K[0], yk->K[1], yk->rate, yk->bias, yk->value, yk->velocity);
//    string yks = buff;
//    Mat dataframe(34, 960, CV_8UC3, Scalar(245,245,245));
//    putText(dataframe, xks, Point(0,12), FONT_HERSHEY_PLAIN, 1, Scalar(15,15,15));
//    putText(dataframe, yks, Point(0,28), FONT_HERSHEY_PLAIN, 1, Scalar(15,15,15));
//    imshow("Kalman Data", dataframe);
//}

