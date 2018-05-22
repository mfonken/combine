//
//  tau_test.cpp
//  tau+
//
//  Created by Matthew Fonken on 8/27/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "tau_test.hpp"
#include <string.h>

using namespace cv;
using namespace std;

Vec3b ap       (150, 255, 150);
Vec3b bp       (150, 150, 255);
Vec3b white    (255, 255, 255);
Vec3b red      (  0,   0, 255);
Vec3b blue     (255,   0,   0);

TauDraw::TauDraw( Tau * tau, Mat inframe )
#ifdef DRAW_RHO_MAPS
:
RMX(Size(tau->width,  RHO_MAPS_HEIGHT), CV_8UC3, Scalar(255,255,255)),
RMY(Size(tau->height, RHO_MAPS_HEIGHT), CV_8UC3, Scalar(255,255,255))
#endif
{
    printf("Initializing Tau Drawer.\n");
    this->tau = tau;
    w = tau->width; h = tau->height;
    W = w + SIDEBAR_WIDTH; H = h + SIDEBAR_WIDTH;
    Mat N(H, W, CV_8UC3, Scalar(0));
    inframe.copyTo(N(Rect(0,0,w,h)));
    frame = N;
    
    pthread_mutex_init( &drawer_mutex, NULL );
}

int compare (const void * a, const void * b)
{
    return ( *(int*)a - *(int*)b );
}

void TauDraw::drawDensitiesOnFrame(Mat M)
{
    image = M;
    pthread_mutex_lock(&drawer_mutex);
    M.copyTo(frame(Rect(0,0,w,h)));
    drawDensityGraph(frame);
    //    drawDensityMaps(frame);
    
    pthread_mutex_lock(&tau->rho.c_mutex);
    
    int Qv[] = { tau->rho.utility.Q[0], tau->rho.utility.Q[1], tau->rho.utility.Q[2], tau->rho.utility.Q[3] };
    //    qsort(Qv, 4, sizeof(int), compare);
#define inseta_ 10
#define insetb_ 40
#define fontsize_ 1.4
#define fontcolor_ Vec3b(100,100,0)
    //    putText(frame, to_string(Qv[0]), Point(inseta_, insetb_), FONT_HERSHEY_PLAIN, fontsize_, fontcolor_, 4);
    //    putText(frame, to_string(Qv[1]), Point(w-insetb_-RHO_MAPS_INSET, insetb_), FONT_HERSHEY_PLAIN, fontsize_, fontcolor_, 4);
    //    putText(frame, to_string(Qv[2]), Point(inseta_, h-inseta_), FONT_HERSHEY_PLAIN, fontsize_,fontcolor_, 4);
    //    putText(frame, to_string(Qv[3]), Point(w-insetb_-RHO_MAPS_INSET, h-inseta_), FONT_HERSHEY_PLAIN, fontsize_, fontcolor_, 4);
    //
    //    putText(frame, "Thresh: " + to_string(tau->utility->thresh), Point(0, 18), FONT_HERSHEY_PLAIN, 1, Vec3b(255,0,255), 2);
    //
    //    putText(frame, "X (" + to_string(tau->rho.utility.Cx) + ", " + to_string(tau->rho.utility.Cy) + ")", Point(tau->rho.utility.Cx, tau->rho.utility.Cy), FONT_HERSHEY_PLAIN, 2, Vec3b(0,255,255), 4);
    pthread_mutex_unlock(&tau->rho.c_mutex);
    pthread_mutex_unlock(&drawer_mutex);
}

void TauDraw::drawDensityGraph(Mat M)
{
    int u, v, w = tau->width, h = tau->height;
    Vec3b blackish(25,25,25), greyish(100,90,90), bluish(255,255,100), greenish(100,255,100), redish(50,100,255), orangish(100,150,255), yellowish(100,255,255), white(255,255,255);
    
    int x1 = 0, x2 = 0, y1 = 0, y2 = 0, rangex[2] = { tau->rho.utility.Cx, w }, rangey[2] = { tau->rho.utility.Cy, h };
    for( int i = 0; i < 2; i++ )
    {
        /* Kalman Values */
        rho_kalman_t yk = tau->rho.utility.density_map_pair.x.kalmans[i], xk = tau->rho.utility.density_map_pair.y.kalmans[i];
        int mYv = tau->rho.utility.density_map_pair.x.variance[i], mXv = tau->rho.utility.density_map_pair.y.variance[i];
        
        int m = OP_ALIGN((xk.value/DENSITY_SCALE),h), n = OP_ALIGN((yk.value/DENSITY_SCALE),w);
        int mv1 = OP_ALIGN((mXv/DENSITY_SCALE),m), mv2 = OP_ALIGN(-(mXv/DENSITY_SCALE),m), nv1 = OP_ALIGN((mYv/DENSITY_SCALE),n), nv2 = OP_ALIGN(-(mYv/DENSITY_SCALE),n);
        
        double ndm = INR(OP_ALIGN(yk.value/DENSITY_SCALE,w),w), mdm = INR(OP_ALIGN(xk.value/DENSITY_SCALE,h),h);
        
        line(M, Point(n,0),   Point(n,H), orangish);
        line(M, Point(nv1,0), Point(nv1,H), yellowish);
        line(M, Point(nv2,0), Point(nv2,H), yellowish);
        
        line(M, Point(0,m),   Point(W,m), orangish);
        line(M, Point(0,mv1), Point(W,mv1), yellowish);
        line(M, Point(0,mv2), Point(W,mv2), yellowish);
        
#ifdef DRAW_RHO_MAPS
        RMX = Scalar(255,255,255);
        RMY = Scalar(255,255,255);
        int md = OP_ALIGN((xk.value/DENSITY_SCALE),RHO_MAPS_HEIGHT), nd = OP_ALIGN((yk.value/DENSITY_SCALE),RHO_MAPS_HEIGHT);
        int mv1d = OP_ALIGN((mXv/DENSITY_SCALE),m), mv2d = OP_ALIGN(-(mXv/DENSITY_SCALE),md), nv1d = OP_ALIGN((mYv/DENSITY_SCALE),nd), nv2d = OP_ALIGN(-(mYv/DENSITY_SCALE),nd);
#endif
        
        /* Density Maps */
        pthread_mutex_lock(&tau->rho.density_map_pair_mutex);
        int dX[h], dY[w], fX[h], fY[w];
        for(; y1 < rangey[i]; y1++ )
        {
            dX[y1] = tau->rho.utility.density_map_pair.x.map[y1];
            fX[y1] = tau->rho.utility.background_map_pair.x.map[y1];
        }
        for(; x1 < rangex[i]; x1++ )
        {
            dY[x1] = tau->rho.utility.density_map_pair.y.map[x1];
            fY[x1] = tau->rho.utility.background_map_pair.y.map[x1];
        }
        pthread_mutex_unlock(&tau->rho.density_map_pair_mutex);
        
#ifdef DRAW_RHO_MAPS
        line(RMY, Point(0,nd), Point(H,nd), orangish);
        line(RMY, Point(0,nv1d), Point(H,nv1d), yellowish);
        line(RMY, Point(0,nv2d), Point(H,nv2d), yellowish);
#endif
        for( ; y2 < rangey[i]; y2++ )
        {
            v = INR(OP_ALIGN((dX[y2]/DENSITY_SCALE),w),w);
            
            if(v>ndm)
            {
                M.at<Vec3b>(y2,v) = white;
                line(M, Point(v,y2), Point(w,y2), white);
            }
            else
            {
                int under = ndm + abs(ndm-v);
                int over = ndm - abs(ndm-v);
                
                //            M.at<Vec3b>(y,under) = bluish;
                //            M.at<Vec3b>(y,over) = redish;
                
                line(M, Point(under,y2), Point(w,y2), white);
                line(M, Point(under,y2), Point(over,y2), bluish);
            }
            
            
            
#ifdef DRAW_RHO_MAPS
            //        u = INR(OP_ALIGN((fX[y]/DENSITY_SCALE),RHO_MAPS_HEIGHT),RHO_MAPS_HEIGHT);
            v = INR(OP_ALIGN((dX[y2]/DENSITY_SCALE),RHO_MAPS_HEIGHT),RHO_MAPS_HEIGHT);
            
            if(v>nd)  line(RMY, Point(y2,RHO_MAPS_HEIGHT), Point(y2,v), redish);
            else
            {
                int under = nd + abs(nd-v);
                int over = nd - abs(nd-v);
                line(RMY, Point(y2,RHO_MAPS_HEIGHT), Point(y2,under), redish);
                line(RMY, Point(y2,under), Point(y2,over), greyish);
            }
#endif
        }
        
#ifdef DRAW_RHO_MAPS
        line(RMX, Point(0,md), Point(W,md), orangish);
        line(RMX, Point(0,mv1d), Point(W,mv1d), yellowish);
        line(RMX, Point(0,mv2d), Point(W,mv2d), yellowish);
#endif
        for( ; x2 < rangex[i]; x2++ )
        {
            v = INR(OP_ALIGN((dY[x2]/DENSITY_SCALE),h),h);
            
            if(v>mdm)
            {
                M.at<Vec3b>(v,x2) = white;
                line(M, Point(x2,v), Point(x2,h), white);
            }
            else
            {
                int under = mdm + abs(mdm-v);
                int over = mdm - abs(mdm-v);
                
                //            M.at<Vec3b>(y,under) = bluish;
                //            M.at<Vec3b>(y,over) = redish;
                
                line(M, Point(x2,under), Point(x2,h), white);
                line(M, Point(x2,under), Point(x2,over), bluish);
            }
#ifdef DRAW_RHO_MAPS
            u = INR(OP_ALIGN((fY[x2]/DENSITY_SCALE),RHO_MAPS_HEIGHT),RHO_MAPS_HEIGHT);
            v = INR(OP_ALIGN((dY[x2]/DENSITY_SCALE),RHO_MAPS_HEIGHT),RHO_MAPS_HEIGHT);
            
            RMX.at<Vec3b>(v,x2) = blackish;
            RMX.at<Vec3b>(u,x2) = orangish;
            line(RMX, Point(x2,RHO_MAPS_HEIGHT), Point(x2,v), greyish);
            
            if(v>md)  line(RMX, Point(x2,RHO_MAPS_HEIGHT), Point(x2,v), redish);
            else
            {
                int under = nd + abs(md-v);
                int over = nd - abs(md-v);
                
                line(RMX, Point(x2,RHO_MAPS_HEIGHT), Point(x2,under), redish);
                line(RMX, Point(x2,under), Point(x2,over), bluish);
            }
#endif
        }
    }
    
    /*
     int nbX = tau->rho.utility.peak_list_pair.x.length, nbY = tau->rho.utility.peak_list_pair.y.length;
     for( int i = 0; i < nbY; i++ ) line(M, Point(tau->rho.utility.peak_list_pair.y.map[i],0), Point(tau->rho.utility.peak_list_pair.y.map[i],h), greenish);
     for( int i = 0; i < nbX; i++ ) line(M, Point(0,tau->rho.utility.peak_list_pair.x.map[i]), Point(W,tau->rho.utility.peak_list_pair.x.map[i]), greenish);
     */
    
    pthread_mutex_lock(&tau->predictions_mutex);
    Point2f a(tau->predictions.x.primary.value,   tau->predictions.y.primary.value),
    b(tau->predictions.x.secondary.value, tau->predictions.y.secondary.value);
    pthread_mutex_unlock(&tau->predictions_mutex);
    
    line(M, Point(a.y, 0), Point(a.y, h), bluish);
    line(M, Point(b.y, 0), Point(b.y, h), bluish);
    line(M, Point(0, a.x), Point(w, a.x), bluish);
    line(M, Point(0, b.x), Point(w, b.x), bluish);
    putText(M, "A", Point(tau->A.x, tau->A.y), FONT_HERSHEY_PLAIN, 2, Vec3b(0,55,150), 3);
    putText(M, "B", Point(tau->B.x, tau->B.y), FONT_HERSHEY_PLAIN, 2, Vec3b(0,150,55), 3);
}

void TauDraw::drawDensityMaps(Mat M)
{
    Vec3b c(0,0,0);
    int x = 0, y = 0, rangex[2] = { tau->rho.utility.Cx, w }, rangey[2] = { tau->rho.utility.Cy, h };
    for( int i = 0; i < 2; i++ )
    {
        int mX = tau->rho.utility.density_map_pair.x.max[i], mY = tau->rho.utility.density_map_pair.y.max[i];
        if(!mX) mX = 1;
        if(!mY) mY = 1;
        int *dX = tau->rho.utility.density_map_pair.x.map, *dY = tau->rho.utility.density_map_pair.y.map;
        
        double a,b;
        for(; y < rangey[i]; y++ )
        {
            a = (double)dX[y];
            b = (double)mX;
            int v = (a/b) * 255;
            if(v > 255) v = 255;
            if(v < 0 ) v = 0;
            //v++;
            //        int v = (double)dX[y]/(double)mX * 255;
            c = densityColor(v);
            line(M, Point(w,y), Point(W,y), c);
        }
        
        for( ; x < rangex[i]; x++ )
        {
            int v = (double)dY[x]/(double)mY * 255;
            c = densityColor(v);
            line(M, Point(x,h), Point(x,H), c);
        }
    }
}

Vec3b TauDraw::densityColor( int v )
{
    int h, s;
    h = v * 120 / 255;
    s = abs(v - (255/2));
    Vec3b hsv(h,s,200), ret(0,0,0);
    ret = hsv2bgr(hsv);
    return ret;
}

Vec3b TauDraw::hsv2bgr(Vec3b hsv)
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

//void TauDraw::drawKalmans()
//{
//    char buff[120];
//    rho_kalman_t * xk = &tau->rho.utility.density_map_pair.x.kalman, * yk = &tau->rho.utility.density_map_pair.y.kalman;
//    snprintf(buff, sizeof(buff), "P[%.3f][%.3f][%.3f][%.3f] K[%.3f][%.3f] R[%.3f] B[%.3f] V[%.3f] E[%.3f]", xk->P[0][0], xk->P[0][1], xk->P[1][0], xk->P[1][1], xk->K[0], xk->K[1], xk->rate, xk->bias, xk->value, xk->velocity);
//    string xks = buff;
//    snprintf(buff, sizeof(buff), "P[%.3f][%.3f][%.3f][%.3f] K[%.3f][%.3f] R[%.3f] B[%.3f] V[%.3f] E[%.3f]", yk->P[0][0], yk->P[0][1], yk->P[1][0], yk->P[1][1], yk->K[0], yk->K[1], yk->rate, yk->bias, yk->value, yk->velocity);
//    string yks = buff;
//    Mat dataframe(34, 960, CV_8UC3, Scalar(245,245,245));
//    putText(dataframe, xks, Point(0,12), FONT_HERSHEY_PLAIN, 1, Scalar(15,15,15));
//    putText(dataframe, yks, Point(0,28), FONT_HERSHEY_PLAIN, 1, Scalar(15,15,15));
//    imshow("Kalman Data", dataframe);
//}

