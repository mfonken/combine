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

Vec3b pos_color = { 255,   0, 255};
Vec3b vel_color = { 255, 255,   0};
Vec3b acc_color = {   0, 255,   0};
Vec3b jrk_color = {   0, 255, 255};
Vec3b jnc_color = {   0,   0, 255};

Vec3b ap        = { 150, 255, 150};
Vec3b bp        = { 150, 150, 255};
Vec3b white     = { 255, 255, 255};
Vec3b red       = {   0,   0, 255};
Vec3b blue      = { 255,   0,   0};

void drawTau(Mat M, density_map_pair_t * m, peak_list_pair_t * p, prediction_pair_t * e)
{
    int h       = m->x.length;
    int w       = m->y.length;
    int width   = M.rows;
    int height  = M.cols;
    
    if( h > width )  h = width;
    if( w > height ) w = height;
    
    FLOAT x_scale = 1;
    FLOAT y_scale = 1;
    
    uint16_t pos = 0, vel = 0, acc = 0, jrk = 0, jnc = 0;
    
    int xp = e->x.primary;
    int xs = e->x.secondary;
    int yp = e->y.primary;
    int ys = e->y.secondary;
    
    for(int x=0; x<h; x++)
    {
        pos = m->x.map[x] * x_scale * 2;
        vel = m->x.vel[x] * x_scale * MAP_SCALE + MAP_INSET;
        acc = m->x.acc[x] * x_scale * MAP_SCALE*2 + MAP_INSET;
        jrk = m->x.jrk[x] * x_scale * MAP_SCALE*4 + MAP_INSET;
        jnc = m->x.jnc[x] * x_scale * MAP_SCALE*8 + MAP_INSET;
        if(pos >= height) pos = height-1;
        if(vel >= height) vel = height-1;
        if(acc >= height) acc = height-1;
        if(jrk >= height) jrk = height-1;
        if(jnc >= height) jnc = height-1;
//        for(int q=(MAP_INSET-10);q<=(MAP_INSET+10);q+=5) M.at<Vec3b>(x,q) = white;
        M.at<Vec3b>(x,pos) = pos_color;
//        M.at<Vec3b>(x,vel) = vel_color;
        M.at<Vec3b>(x,acc) = acc_color;
        
        M.at<Vec3b>(x,jnc) = jnc_color;
        
        if( fabs(m->x.jrk[x]) >= 0.10 ) M.at<Vec3b>(x,jrk) = white;
        else M.at<Vec3b>(x,jrk) = jrk_color;
        
        if( fabs(m->x.vel[x]) >= 2 ) M.at<Vec3b>(x,vel) = white;
        else M.at<Vec3b>(x,vel) = vel_color;
        
    }
    for(int i=0; i<p->x.length; i++)
    {
        int l = p->x.map[i];
        for(int k = 0; k < p->x.den[i]; k++)
        {
            int dir = p->x.dir[i];
            if( dir == 1) M.at<Vec3b>(l,k) = red;
            else if( dir == 2 ) M.at<Vec3b>(l,k) = blue;
            else M.at<Vec3b>(l,k) = white;
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
        vel = m->y.vel[y] * y_scale * MAP_SCALE + MAP_INSET;
        acc = m->y.acc[y] * y_scale * MAP_SCALE*4 + MAP_INSET;
        jrk = m->y.jrk[y] * y_scale * MAP_SCALE*8 + MAP_INSET;
        jnc = m->y.jnc[y] * y_scale * MAP_SCALE*16 + MAP_INSET;
        if(pos >= height) pos = width-1;
        if(vel >= height) vel = width-1;
        if(acc >= height) acc = width-1;
        if(jrk >= height) jrk = width-1;
        if(jnc >= height) jnc = width-1;
        M.at<Vec3b>(pos,y) = pos_color;
        M.at<Vec3b>(vel,y) = vel_color;
        M.at<Vec3b>(acc,y) = acc_color;
        M.at<Vec3b>(jrk,y) = jrk_color;
        M.at<Vec3b>(jnc,y) = jnc_color;
    }
    
    for(int i=0; i<p->y.length; i++)
    {
        int l = p->y.map[i];
        for(int k = 0; k < p->y.den[i]; k++)
        {
            int dir = p->y.dir[i];
            if( dir == 1) M.at<Vec3b>(k,l) = red;
            else if( dir == 2 ) M.at<Vec3b>(k,l) = blue;
            else M.at<Vec3b>(k,l) = white;
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

void drawKalmanData( prediction_kalman_pair_t * k )
{
    double a,b,c,d;
    string A("A:"),B("B:"),C("C:"),D("D:");
    /* GET VALUES */
    a = k->x.primary.value;
    b = k->x.secondary.value;
    c = k->y.primary.value;
    d = k->y.secondary.value;
    A.append(to_string(a));
    B.append(to_string(b));
    C.append(to_string(c));
    D.append(to_string(d));
    
    /* GET P VALUES */
    string E("xp-P00: "),F("xs-P00: "),G("yp-P00: "),H("ys-P00: ");
    a = k->x.primary.P_k[1][0];
    b = k->x.secondary.P_k[1][0];
    c = k->y.primary.P_k[1][0];
    d = k->y.secondary.P_k[1][0];
    E.append(to_string(a));
    F.append(to_string(b));
    G.append(to_string(c));
    H.append(to_string(d));
    
    int grid = 200, ox = 3, oy = 18;
    int thickness = 1;
    
    Mat O(2*grid, 2*grid, CV_8UC3, Scalar(245,245,245));
    line(O, Point(0,grid), Point(2*grid,grid), Scalar::all(0), thickness,8);
    line(O, Point(grid,0), Point(grid,2*grid), Scalar::all(0), thickness,8);
    
    Point AOrg(ox, oy);
    Point BOrg(ox + grid, oy);
    Point COrg(ox, oy + grid);
    Point DOrg(ox + grid, oy + grid);
    Point EOrg(ox, oy+oy);
    Point FOrg(ox + grid, oy+oy);
    Point GOrg(ox, oy+oy + grid);
    Point HOrg(ox + grid, oy+oy + grid);
    
    int fontFace = FONT_HERSHEY_SIMPLEX;
    double fontScale = 0.5;
    
    putText(O, A, AOrg, fontFace, fontScale, Scalar::all(0), thickness,8);
    putText(O, B, BOrg, fontFace, fontScale, Scalar::all(0), thickness,8);
    putText(O, C, COrg, fontFace, fontScale, Scalar::all(0), thickness,8);
    putText(O, D, DOrg, fontFace, fontScale, Scalar::all(0), thickness,8);
    
    putText(O, E, EOrg, fontFace, fontScale, Scalar::all(0), thickness,8);
    putText(O, F, FOrg, fontFace, fontScale, Scalar::all(0), thickness,8);
    putText(O, G, GOrg, fontFace, fontScale, Scalar::all(0), thickness,8);
    putText(O, H, HOrg, fontFace, fontScale, Scalar::all(0), thickness,8);
    
    imshow("Kalman Data", O);
}

void drawDensityMaps(density_map_pair_t * m)
{
    int MAX_DENSITY = 150;
    int GAP = 10;
    int xl = m->x.length, yl = m->y.length;
    int xoh = GAP+MAX_DENSITY, yoh = 2*(GAP+MAX_DENSITY);
    int width = xl > yl ? xl:yl, height = 2*MAX_DENSITY+3*GAP;
    Mat M(height, width, CV_8UC3, Scalar(245,245,245));
    Mat V(height, width, CV_8UC3, Scalar(245,245,245));
    Mat A(height, width, CV_8UC3, Scalar(245,245,245));
    
    double map_f = 0.5, vel_f = 8, acc_f = 40;
    
    Point prev(0,0), curr(0,0);
    line(M, Point(0,xoh), Point(width,xoh), Scalar::all(100), 1,8);
    for(int x = 0; x < xl; x++)
    {
        int h = m->x.map[x]*map_f;
        int v = m->x.vel[x]*vel_f;
        int a = m->x.acc[x]*acc_f;
        if(h > MAX_DENSITY) h = MAX_DENSITY;
        else if( h < 0 ) h = 0;
        curr.x = x;
        curr.y = xoh-h;
        line(M, Point(x,xoh), Point(x,xoh-h), Scalar::all(0), 1,8);
        line(V, Point(x,xoh-MAX_DENSITY/2), Point(x,xoh-v-MAX_DENSITY/2), Scalar::all(0), 1,8);
        line(A, Point(x,xoh-MAX_DENSITY/2), Point(x,xoh-a-MAX_DENSITY/2), Scalar::all(0), 1,8);
        line(M, prev, curr, Scalar::all(0), 1,8);
        prev = curr;
    }
    prev.x = 0;
    prev.y = 0;
    line(M, Point(0,yoh), Point(width,yoh), Scalar::all(100), 1,8);
    for(int y = 0; y < yl; y++)
    {
        int h = m->y.map[y]*map_f;
        int v = m->y.vel[y]*vel_f;
        int a = m->y.acc[y]*acc_f;
        if(h > MAX_DENSITY) h = MAX_DENSITY;
        else if( h < 0 ) h = 0;
        curr.x = y;
        curr.y = yoh-h;
        line(M, Point(y,yoh), Point(y,yoh-h), Scalar::all(0), 1,8);
        line(V, Point(y,yoh-MAX_DENSITY/2), Point(y,yoh-v-MAX_DENSITY/2), Scalar::all(0), 1,8);
        line(A, Point(y,yoh-MAX_DENSITY/2), Point(y,yoh-a-MAX_DENSITY/2), Scalar::all(0), 1,8);
        line(M, prev, curr, Scalar::all(0), 1,8);
        prev = curr;
    }
    
    imshow("Density Maps", M);
    imshow("Density Velocities", V);
    imshow("Density Accelerations", A);
}

void drawTiming( double *p, int n)
{
    const char *names[] = {"RHO", "SIGMA", "STATE"};
    
    int width = 1000, height = 36;
    Mat O(height, width, CV_8UC3, Scalar(245,245,245));
    
    Vec3b color[5] =  { { 255,   0, 255},
                        { 255, 255,   0},
                        {   0, 255,   0},
                        {   0, 255, 255},
                        {   0,   0, 255}
                      };
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
