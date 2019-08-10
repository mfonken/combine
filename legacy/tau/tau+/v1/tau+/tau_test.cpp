//
//  tau_test.cpp
//  tau+
//
//  Created by Matthew Fonken on 8/27/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "tau_test.hpp"

using namespace cv;

Vec3b pos_color = { 255,   0, 255};
Vec3b vel_color = { 255, 255,   0};
Vec3b acc_color = {   0, 255,   0};
Vec3b jrk_color = {   0, 255, 255};
Vec3b jnc_color = {   0,   0, 255};

Vec3b ap = { 150, 255, 150};
Vec3b bp = { 150, 150, 255};
Vec3b white = { 255, 255, 255};
Vec3b red   = {   0,   0, 255};
Vec3b blue  = { 255,   0,   0};

void drawTau(Mat M, density_map_pair_t * m, peak_list_pair_t * p, probability_list_pair_t * r, prediction_pair_t * e)
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
        int l = p->x.locations[i];
        for(int k = 0; k < 300; k++)
        {
            int dir = p->x.dir[i];
            if( dir == 1) M.at<Vec3b>(l,k) = red;
            else if( dir == 2 ) M.at<Vec3b>(l,k) = blue;
            else M.at<Vec3b>(l,k) = white;
        }
    }
    
//    int x = 0.1*MAP_SCALE*4 + MAP_INSET;
//    for(int y=0;y<h;y++) M.at<Vec3b>(y,x) = white;
    
    for(int i=0; i<p->x.length; i++)
    {
        int l = p->x.locations[i];
        FLOAT v = r->x.primary[i];
        v -= 0.6;
        v *= 3;
        if(v > 1 || v < 0) break;
        Vec3b rp;
        rp[0] = 0;
        rp[1] = 255*(1-v);
        rp[2] = 255*v;
        for(int z = 0; z < height*v*x_scale; z++)
            M.at<Vec3b>(l,z) = rp;
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
        int l = p->y.locations[i];
        for(int k = 0; k < 300; k++)
        {
            int dir = p->y.dir[i];
            if( dir == 1) M.at<Vec3b>(k,l) = red;
            else if( dir == 2 ) M.at<Vec3b>(k,l) = blue;
            else M.at<Vec3b>(k,l) = white;
        }
    }
    
    for(int i=0; i<p->y.length; i++)
    {
        int l = p->y.locations[i];
        FLOAT v = r->y.primary[i];
        v -= 0.6;
        v *= 3;
        if(v > 1 || v < 0) break;
        Vec3b rp;
        rp[0] = 0;
        rp[1] = 255*(1-v);
        rp[2] = 255*v;
        for(int z = 0; z < width*v*y_scale; z++)
            M.at<Vec3b>(z, l) = rp;
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
    
    imshow("...", M);
}

void drawDensityMap(density_map_pair_t * m)
{
    int w = m->x.length;
    int h = m->y.length;
    
    int z = DRAW_HEIGHT;
    
    Mat Xd(z, w, CV_8UC3, Scalar(0,0,0));
    Mat Yd(h, z, CV_8UC3, Scalar(0,0,0));
    
    Vec3b p;
    p[0] = 255;
    p[1] = 0;
    p[2] = 255;
    int d = 0;
    
    for(int x=0; x<w; x++)
    {
        d = m->x.map[x];
        if(d >= z) d = z-1;
        Xd.at<Vec3b>(z-1-d,x) = p;
    }
    for(int y=0; y<h; y++)
    {
        d = m->y.map[y];
        if(d >= z) d = z-1;
        Yd.at<Vec3b>(y,z-1-d) = p;
    }
    imshow("X Density Map", Xd);
    imshow("Y Density Map", Yd);
}

void drawDensityMapWithPeaks(density_map_pair_t * m, peak_list_pair_t * p)
{
    int w = m->x.length;
    int h = m->y.length;
    int z = 300;
    
    Mat Xd(z, w, CV_8UC3, Scalar(0,0,0));
    Mat Yd(h, z, CV_8UC3, Scalar(0,0,0));
    
    Vec3b mp = {255,0,255};
    Vec3b pp = {0,0,255};
    
    int d = 0;
    
    for(int x=0; x<w; x++)
    {
        d = m->x.map[x];
        if(d >= z) d = z-1;
        Xd.at<Vec3b>(z-1-d,x) = mp;
    }
    for(int i=0; i<p->x.length; i++)
    {
        int l = p->x.locations[i];
        for(int k = 0; k < z; k++)
            Xd.at<Vec3b>(k,l) = pp;
    }
    for(int y=0; y<h; y++)
    {
        d = m->y.map[y];
        if(d >= z) d = z-1;
        Yd.at<Vec3b>(y,z-1-d) = mp;
    }
    for(int j=0; j<p->y.length; j++)
    {
        int l = p->y.locations[j];
        for(int k = 0; k < z; k++)
            Yd.at<Vec3b>(l,k) = pp;
    }
    imshow("X Density Map", Xd);
    imshow("Y Density Map", Yd);
}

void drawDensityMapWithProbabilities(density_map_pair_t * m, peak_list_pair_t * p, probability_list_pair_t * r)
{
    int h = m->x.length;
    int w = m->y.length;
    int z = DRAW_HEIGHT;
    
    printf("(%d, %d)\n",w,h);
    
    Mat Xd(h, z, CV_8UC3, Scalar(0,0,0));
    Mat Yd(z, w, CV_8UC3, Scalar(0,0,0));
    
    Vec3b mp;
    mp[0] = 255;
    mp[1] = 0;
    mp[2] = 255;
    
    Vec3b pp;
    pp[0] = 0;
    pp[1] = 0;
    pp[2] = 255;
    
    int d = 0;
    
    for(int x=0; x<h; x++)
    {
        d = m->x.map[x];
        if(d >= z) d = z-1;
        Xd.at<Vec3b>(x,z-1-d) = mp;
    }
    for(int i=0; i<p->x.length; i++)
    {
        int l = p->x.locations[i];
        FLOAT v = r->x.primary[i]*3;
        if(v > 1) break;
        Vec3b rp;
        rp[0] = 0;
        rp[1] = 255*(1-v);
        rp[2] = 255*v;
        for(int z = 0; z < z*v; z++)
            Xd.at<Vec3b>(l,z-1-z) = rp;
    }
    for(int y=0; y<w; y++)
    {
        d = m->y.map[y];
        if(d >= z) d = z-1;
        Yd.at<Vec3b>(d,y) = mp;
    }
    for(int i=0; i<p->y.length; i++)
    {
        int l = p->y.locations[i];
        FLOAT v = r->y.primary[i]*3;
        if(v > 1) break;
        Vec3b rp;
        rp[0] = 0;
        rp[1] = 255*(1-v);
        rp[2] = 255*v;
        for(int z = 0; z < z*v; z++)
            Yd.at<Vec3b>(z, l) = rp;
    }
    imshow("X Density Map", Xd);
    imshow("Y Density Map", Yd);
}

void drawDensityMapWithPredictions(density_map_pair_t * m, peak_list_pair_t * p, probability_list_pair_t * r, prediction_pair_t * e)
{
    int h = m->x.length;
    int w = m->y.length;
    int z = 300;
    
    //    printf("(%d, %d)\n",e->x.primary,e->x.primary);
    
    Mat Xd(h, z, CV_8UC3, Scalar(0,0,0));
    Mat Yd(z, w, CV_8UC3, Scalar(0,0,0));
    
    Vec3b mp;
    mp[0] = 255;
    mp[1] = 0;
    mp[2] = 255;
    
    Vec3b pp;
    pp[0] = 0;
    pp[1] = 0;
    pp[2] = 255;
    
    Vec3b ep;
    ep[0] = 255;
    ep[1] = 0;
    ep[2] = 0;
    
    int d = 0;
    
    int xp = e->x.primary;
    int xs = e->y.secondary;
    int yp = e->x.primary;
    int ys = e->y.secondary;
    
    for(int x=0; x<h; x++)
    {
        d = m->x.map[x];
        if(d >= z) d = z-1;
        Xd.at<Vec3b>(x,z-1-d) = mp;
    }
    for(int i=0; i<p->x.length; i++)
    {
        int l = p->x.locations[i];
        FLOAT v = r->x.primary[i]*3;
        if(v > 1) break;
        Vec3b rp;
        rp[0] = 0;
        rp[1] = 255*(1-v);
        rp[2] = 255*v;
        for(int z = 0; z < z*v; z++)
            Xd.at<Vec3b>(l,z-1-z) = rp;
    }
    
    for(int p=0; p<z; p++)
    {
        Xd.at<Vec3b>(xp,p) = ep;
        Xd.at<Vec3b>(xs,p) = ep;
    }
    for(int y=0; y<w; y++)
    {
        d = m->y.map[y];
        if(d >= z) d = z-1;
        Yd.at<Vec3b>(d,y) = mp;
    }
    for(int i=0; i<p->y.length; i++)
    {
        int l = p->y.locations[i];
        FLOAT v = r->y.primary[i]*3;
        if(v > 1) break;
        Vec3b rp;
        rp[0] = 0;
        rp[1] = 255*(1-v);
        rp[2] = 255*v;
        for(int z = 0; z < z*v; z++)
            Yd.at<Vec3b>(z, l) = rp;
    }
    for(int p = 0; p < z; p++)
    {
        Yd.at<Vec3b>(p,yp) = ep;
        Yd.at<Vec3b>(p,ys) = ep;
    }
    
    imshow("X Density Map", Xd);
    imshow("Y Density Map", Yd);
}



/*** UTILITY FUNCTIONS ***/
void initImg( pixel_base_t *** i, int w, int h )
{
    *i = (pixel_base_t**)malloc(sizeof(pixel_base_t*) * h);
    for(int y=0;y<h;y++) (*i)[y] = (pixel_base_t*)malloc(sizeof(pixel_base_t) * w);
}

void print2DArray( pixel_base_t ** i, int w, int h )
{
    for(int y=0; y<h; y++)
    {
        for(int x=0; x<w; x++)
        {
            pixel_base_t p = i[y][x];
            printf("(%d, %d) is %d\n",x,y,p);
        }
    }
}

bool thresh(uint8_t t, Vec3d p)
{
    /* Check first BGR channel only */
    return p[0] > t;
}

void MatToCimage( uint16_t width, uint16_t height, Mat mat, pixel_base_t ** img )
{
    for(uint16_t y = 0; y < height; y++)
    {
        for(uint16_t x = 0; x < width; x++)
        {
            Vec3b pixel = mat.at<Vec3b>(y, x);
            img[y][x] = (thresh(THRESHOLD, pixel)) ? 1:0;
        }
    }
}
void cimageToMat( uint16_t width, uint16_t height, pixel_base_t ** img, Mat mat )
{
    for(uint16_t y = 0; y < height; y++)
    {
        for(uint16_t x = 0; x < width; x++)
        {
            Vec3b pixel;
            pixel[0] = img[y][x];
            pixel[1] = img[y][x];
            pixel[2] = img[y][x];
            mat.at<Vec3b>(y, x) = pixel;
        }
    }
}

void cimageToBandW( uint16_t width, uint16_t height, pixel_base_t ** img, Mat mat )
{
    for(uint16_t y = 0; y < height; y++)
    {
        for(uint16_t x = 0; x < width; x++)
        {
            Vec3b pixel;
            pixel[0] = img[y][x]*BRIGHTNESS;
            pixel[1] = img[y][x]*BRIGHTNESS;
            pixel[2] = img[y][x]*BRIGHTNESS;
            mat.at<Vec3b>(y, x) = pixel;
        }
    }
}

void deriveDensityMap( density_map_t * m )
{
    int l = m->length;
    for(int i = 1; i < l; i++)
    {
        m->map[i-1] -= m->map[i];
    }
    m->map[l-1] = 0;
}

void gaussianBlur( density_map_t * m )
{
    int len = m->length;
    double gaussian[] = {0.009033,0.018476,0.033851,0.055555,0.08167,0.107545,0.126854,0.134032,0.126854,0.107545,0.08167,0.055555,0.033851,0.018476,0.009033};
    size_t n, k_len = 15;
    
    double res[len + k_len - 1];
    
    for (n = 0; n < len + k_len - 1; n++)
    {
        size_t kmin, kmax, k;
        
        res[n] = 0;
        
        kmin = (n >= k_len - 1) ? n - (k_len - 1) : 0;
        kmax = (n < len - 1) ? n : len - 1;
        
        for (k = kmin; k <= kmax; k++)
        {
            res[n] += (FLOAT)m->map[k] * gaussian[n - k];
        }
    }
    int offset = (int)(( k_len - 1 ) / 2);
    for (n = 0; n < len; n++)
    {
        m->map[n] = res[n + offset];
    }
}

void smooth1D( density_map_t * m )
{
    uint16_t a,b,c,d,e,f,g;
    a = m->map[0];
    b = m->map[1];
    c = m->map[2];
    d = m->map[3];
    e = m->map[4];
    f = m->map[5];
    for(uint16_t i = 6; i < m->length; i+=7)
    {
        g = m->map[i];
        m->map[i] = ( a + b + c + d + e + f + g ) / 7;
        a = b;
        b = c;
        c = d;
        d = e;
        e = f;
        f = g;
    }
}
