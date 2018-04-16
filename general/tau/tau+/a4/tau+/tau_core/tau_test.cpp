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

Vec3b pos_color(255,   0, 255);
Vec3b vel_color(255, 255,   0);
Vec3b acc_color(  0, 255,   0);
Vec3b jrk_color(  0, 255, 255);
Vec3b jnc_color(  0,   0, 255);

Vec3b ap       (150, 255, 150);
Vec3b bp       (150, 150, 255);
Vec3b white    (255, 255, 255);
Vec3b red      (  0,   0, 255);
Vec3b blue     (255,   0,   0);

TauDraw::TauDraw( Tau * tau, Mat inframe )
{
    printf("Initializing Tau Drawer.\n");
    this->tau = tau;
    w = tau->width; h = tau->height;
    W = w + SIDEBAR_WIDTH; H = h + SIDEBAR_WIDTH;
    Mat N(H, W, CV_8UC3, Scalar(0));
    inframe.copyTo(N(Rect(0,0,w,h)));
    frame = N;
}

/*
void TauDraw::drawTau(Mat M, DensityMapPair * m, PeakListPair * p, PredictionPair * e)
{
    int h       = m->x.length;
    int w       = m->y.length;
    int width   = M.rows;
    int height  = M.cols;
    
    if( h > width )  h = width;
    if( w > height ) w = height;
    
    double x_scale = 1;
    double y_scale = 1;
    
    int pos = 0;
    
    int xp = e->x.primary.value;
    int xs = e->x.secondary.value;
    int yp = e->y.primary.value;
    int ys = e->y.secondary.value;
    
    for(int x=0; x<h; x++)
    {
        pos = m->x.map[x] * x_scale * 2;
        if(pos >= height) pos = height-1;
        M.at<Vec3b>(x,pos) = pos_color;
        
    }
    for(int i=0; i<p->x.length; i++)
    {
        int l = p->x.map[i];
        for(int q = 0; q < p->x.den[i]; q++)
        {
//            int dir = p->x.dir[i];
//            if( dir == 1) M.at<Vec3b>(l,q) = red;
            else if( dir == 2 ) M.at<Vec3b>(l,q) = blue;
            else M.at<Vec3b>(l,q) = white;
        }
    }
    
    for(int j=0; j<height; j++)
    {
        M.at<Vec3b>(xp,j) = ap;
        M.at<Vec3b>(xs,j) = bp;
    }
    
    for(int y=0; y<w; y++)
    {
        pos = m->y.map[y] * y_scale * 2;
        if(pos >= height) pos = width-1;
        M.at<Vec3b>(pos,y) = pos_color;
    }
    
    for(int i=0; i<p->y.length; i++)
    {
        int l = p->y.map[i];
        for(int q = 0; q < p->y.den[i]; q++)
        {
//            int dir = p->y.dir[i];
//            if( dir == 1) M.at<Vec3b>(q,l) = red;
            else if( dir == 2 ) M.at<Vec3b>(q,l) = blue;
            else M.at<Vec3b>(q,l) = white;
        }
    }
    
    for(int j = 0; j < width; j++)
    {
        M.at<Vec3b>(j,yp) = ap;
        M.at<Vec3b>(j,ys) = bp;
    }
    
    Mat overlay;
    float alpha = 0.3;
    M.copyTo(overlay);
    rectangle(overlay,Point(yp,xp), Point(ys,xs), Scalar(0,255,255), -1);
    addWeighted(overlay, alpha, M, 1 - alpha, 0, M);
    
    imshow(TITLE_STRING, M);
}
 */

void TauDraw::drawDensityMaps(DensityMapPair * m)
{
    int MAX_DENSITY = 150;
    int GAP = 10;
    int xl = m->x.length, yl = m->y.length;
    int xoh = GAP+MAX_DENSITY, yoh = 2*(GAP+MAX_DENSITY);
    int width = xl > yl ? xl:yl, height = 2*MAX_DENSITY+3*GAP;
    Mat M(height, width, CV_8UC3, Scalar(245,245,245));
    Mat V(height, width, CV_8UC3, Scalar(245,245,245));
    Mat A(height, width, CV_8UC3, Scalar(245,245,245));
    
    double map_f = 0.5;
    
    Point prev(0,0), curr(0,0);
    line(M, Point(0,xoh), Point(width,xoh), Scalar::all(100), 1,8);
    for(int x = 0; x < xl; x++)
    {
        int h = m->x.map[x]*map_f;
        if(h > MAX_DENSITY) h = MAX_DENSITY;
        else if( h < 0 ) h = 0;
        curr.x = x;
        curr.y = xoh-h;
        line(M, Point(x,xoh), Point(x,xoh-h), Scalar::all(0), 1,8);
        line(M, prev, curr, Scalar::all(0), 1,8);
        prev = curr;
    }
    prev.x = 0;
    prev.y = 0;
    line(M, Point(0,yoh), Point(width,yoh), Scalar::all(100), 1,8);
    for(int y = 0; y < yl; y++)
    {
        int h = m->y.map[y]*map_f;
        if(h > MAX_DENSITY) h = MAX_DENSITY;
        else if( h < 0 ) h = 0;
        curr.x = y;
        curr.y = yoh-h;
        line(M, Point(y,yoh), Point(y,yoh-h), Scalar::all(0), 1,8);
        line(M, prev, curr, Scalar::all(0), 1,8);
        prev = curr;
    }
    
    imshow("Density Maps", M);
}

void TauDraw::drawTiming( double *p, int n)
{
    const char *names[] = {"RHO", "SIGMA", "STATE"};
    
    int width = 1000, height = 36;
    Mat O(height, width, CV_8UC3, Scalar(245,245,245));
    
    int colors[5][3] =  { { 255,   0, 255},
        { 255, 255,   0},
        {   0, 255,   0},
        {   0, 255, 255},
        {   0,   0, 255}
    };
    Vec3b color[5];
    for(int i = 0; i < 5; i++)
    {
        color[i][0] = colors[i][0];
        color[i][1] = colors[i][1];
        color[i][2] = colors[i][2];
    }
    double total = p[n-1] - 0;
    double scale = width/total;
    
    Mat overlay;
    float alpha = 0.3;
    O.copyTo(overlay);
    char buff[32];
    snprintf(buff, sizeof(buff), "TOTAL: %.4f(s)",total);
    putText(overlay, buff, Point(width-135,height-2), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(0), 1,8);
    for(int i = 1; i < n; i++)
    {
        int a = p[i-1]*scale;
        int b = p[i]*scale;
        rectangle(overlay,Point(a,2), Point(b,20), color[i%5], -1);
        snprintf(buff, sizeof(buff), "%i%% %s", (int)(((p[i]-p[i-1])/total)*100), names[(i-1)%3]);
        std::string text = buff;
        putText(overlay, text, Point(a,18), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(0), 1,8);
        
    }
    
    addWeighted(overlay, alpha, O, 1 - alpha, 0, O);
    imshow("Timing", O);
}

void TauDraw::drawDensitiesOnFrame(Mat M)
{
    image = M;
    M.copyTo(frame(Rect(0,0,w,h)));
    drawDensityGraph(frame);
    drawDensityMaps(frame);
}

void TauDraw::drawDensityGraph(Mat M)
{
    int c, /*p1,*/ p2, u, v, o, f, w = tau->width, h = tau->height;
    Vec3b greyish(25,25,25), bluish(255,255,100), greenish(100,255,100), redish(50,100,255), orangish(100,150,255), yellowish(100,255,255), white(255,255,255);
    
//    tau->updatePrediction();
    
    KalmanFilter xk = tau->rho.density_map_pair.x.kalman, yk = tau->rho.density_map_pair.y.kalman;
    int mXv = tau->rho.density_map_pair.x.variance, mYv = tau->rho.density_map_pair.y.variance;

    int min_thresh = 10;
    int n = h-(xk.value/DENSITY_SCALE), m = w-(yk.value/DENSITY_SCALE);
    int nt = h-min_thresh, mt = w-min_thresh;

    int nv1 = n-(mXv/DENSITY_SCALE), nv2 = n+(mXv/DENSITY_SCALE), mv1 = m-(mYv/DENSITY_SCALE), mv2 = m+(mYv/DENSITY_SCALE);

    
//    int *dX = tau->rho.density_map_pair.x.map, *dY = tau->rho.density_map_pair.y.map;
    pthread_mutex_lock(&tau->rho.density_map_pair_mutex);
    int dX[h], dY[w], fX[h], fY[w];
    for( int y = 0; y < h; y++ )
    {
        dX[y] = tau->rho.density_map_pair.x.map[y];
        fX[y] = tau->rho.density_map_pair.x.fil[y];
    }
    for( int x = 0; x < w; x++ )
    {
        dY[x] = tau->rho.density_map_pair.y.map[x];
        fY[x] = tau->rho.density_map_pair.y.fil[x];
    }
    pthread_mutex_unlock(&tau->rho.density_map_pair_mutex);
    
    line(M, Point(n,0), Point(n,W), orangish);
    line(M, Point(nv1,0), Point(nv1,W), yellowish);
    line(M, Point(nv2,0), Point(nv2,W), yellowish);
    for( int y = 0, p1 = p2 = 0; y < h; y++ )
    {
        u = INR(w-(fX[y]/DENSITY_SCALE),M.rows);
        v = INR(w-(dX[y]/DENSITY_SCALE),M.rows);
        o = abs(n - u);
        c = INR(n+o,M.rows);

        if(c>nv2)M.at<Vec3b>(y,c) = greyish;
        else M.at<Vec3b>(y,c) = bluish;
        if(u<n) M.at<Vec3b>(y,v) = redish;

        f = w-(dX[y]/DENSITY_SCALE);
        if(f < 0) f = 0; if(f >= w) f = w-1;
        M.at<Vec3b>(y,f) = white;
//        if(c < nt && p1 < c && p1 <= p2) line(M, Point(0,y), Point(w,y), greenish);
        p2 = p1;
        p1 = c;
    }

    line(M, Point(0,m), Point(H,m), orangish);
    line(M, Point(0,mv1), Point(H,mv1), yellowish);
    line(M, Point(0,mv2), Point(H,mv2), yellowish);
    for( int x = 0, p1 = p2 = 0; x < w; x++ )
    {
        u = INR(h-(fY[x]/DENSITY_SCALE),M.cols);
        v = INR(h-(dY[x]/DENSITY_SCALE),M.cols);
        o = abs(m - u);
        c = INR(m+o,M.cols);

        if(c>mv2) M.at<Vec3b>(c,x) = greyish;
        else M.at<Vec3b>(c,x) = bluish;
        if(u<m) M.at<Vec3b>(v,x) = redish;

        f = h-(dY[x]/DENSITY_SCALE);
        if(f < 0) f = 0; if(f >= h) f = w-1;
        M.at<Vec3b>(f,x) = white;
//        if(c < mt && p1 < c && p1 <= p2) line(M, Point(x,0), Point(x,h), greenish);
        p2 = p1;
        p1 = c;
    }

    int nbX = tau->rho.peak_list_pair.x.length, nbY = tau->rho.peak_list_pair.y.length;
    for( int i = 0; i < nbY; i++ ) line(M, Point(tau->rho.peak_list_pair.y.map[i],0), Point(tau->rho.peak_list_pair.y.map[i],h), greenish);
    for( int i = 0; i < nbX; i++ ) line(M, Point(0,tau->rho.peak_list_pair.x.map[i]), Point(W,tau->rho.peak_list_pair.x.map[i]), greenish);
    
    pthread_mutex_lock(&tau->predictions_mutex);
    Point2f a(tau->predictions.x.primary.value,   tau->predictions.y.primary.value),
            b(tau->predictions.x.secondary.value, tau->predictions.y.secondary.value);
    pthread_mutex_unlock(&tau->predictions_mutex);
    
//    line(M, Point(a.y, 0), Point(a.y, h), bluish);
//    line(M, Point(b.y, 0), Point(b.y, h), bluish);
//    line(M, Point(0, a.x), Point(w, a.x), bluish);
//    line(M, Point(0, b.x), Point(w, b.x), bluish);
    
    putText(M, "A", Point(a.y, a.x), FONT_HERSHEY_PLAIN, 2, Vec3b(0,55,255), 3);
    putText(M, "B", Point(b.y, b.x), FONT_HERSHEY_PLAIN, 2, Vec3b(0,255,55), 3);
}

void TauDraw::drawDensityMaps(Mat M)
{
    Vec3b c(0,0,0);
    int mX = tau->rho.density_map_pair.x.max, mY = tau->rho.density_map_pair.y.max;
    if(!mX) mX = 1;
    if(!mY) mY = 1;
    int *dX = tau->rho.density_map_pair.x.map, *dY = tau->rho.density_map_pair.y.map;
    
    double a,b;
    for( int y = 0; y < h; y++ )
    {
        a = (double)dX[y];
        b = (double)mX;
        int v = (a/b) * 255;
//        int v = (double)dX[y]/(double)mX * 255;
        c = densityColor(v);
        line(M, Point(w,y), Point(W,y), c);
    }
    
    for( int x = 0; x < w; x++ )
    {
        int v = (double)dY[x]/(double)mY * 255;
        c = densityColor(v);
        line(M, Point(x,h), Point(x,H), c);
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
