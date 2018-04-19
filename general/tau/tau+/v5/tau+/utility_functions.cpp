//
//  utility_functions.cpp
//  tau+
//
//  Created by Matthew Fonken on 10/6/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "utility_functions.hpp"


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

bool thresh(uint8_t t, cv::Vec3d p)
{
    /* Check first BGR channel only */
    return p[0] > t;
}

void MatToCimage( uint16_t width, uint16_t height, cv::Mat mat, pixel_base_t ** img, int threshold )
{
    for(uint16_t y = 0; y < height; y++)
    {
        for(uint16_t x = 0; x < width; x++)
        {
            Vec3b pixel = mat.at<Vec3b>(y, x);
            img[y][x] = (thresh(threshold, pixel)) ? 1:0;
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
