//
//  rho.c
//  tau+
//
//  Created by Matthew Fonken on 8/22/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

/* Own Include */
#include "rho.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define opsign(X,Y)     (X<0)^(Y<0)
#define rising(X,Y)    (X>=0)&(Y<0)
#define falling(X,Y)    (X<0)&(Y>=0)

#define opsignthresh(X,Y,T)    ((X*Y)<T)
#define risingthresh(X,Y,T)    ((X*Y)<T)&(Y<X)
#define fallingthresh(X,Y,T)   ((X*Y)<T)&(Y>X)

void initRho( rho_t * r, uint16_t width, uint16_t height )
{
    printf("Init-ing rho: %dx%d\n", width, height);
    r->width  = width;
    r->height = height;
    
    initGaussian(    &r->gaussian, DEFAULT_GAUSS_LEN);
    generateGaussian(&r->gaussian, DEFAULT_GAUSS_LEN, DEFAULT_SIGMA);
    
    initDensityMapPair( &r->density_map_pair, width, height);
    initPeaksListPair(  &r->peak_list_pair,   width, height);
}

void performRho( rho_t * r, cimage_t img )
{
    generateDensityMap( r, img );
    generatePeakListPair( &r->density_map_pair, &r->peak_list_pair );
}

void generateDensityMap( rho_t * r, cimage_t img )
{
    int h = r->height, w = r->width;
    int mapx[h], mapy[w], velx[h], vely[w];
    memset(mapy, 0, sizeof(int) * w);
    memset(mapx, 0, sizeof(int) * h);
    memset(vely, 0, sizeof(int) * w);
    memset(velx, 0, sizeof(int) * h);
    
    int row_sum, z = 0, y = h, x;
    
    for(; y > 0; --y )
    {
        for( x = w, row_sum = 0; x > 0; --x )
        {
            if(img[z++])
            {
                mapy[x]++;
                row_sum++;
            }
        }
        mapx[y] += row_sum;
    }
    
    gaussianBlurInt( &r->gaussian, mapx, h, 1);
    gaussianBlurInt( &r->gaussian, mapy, w, 1);
    r->density_map_pair.y.length = w;
    r->density_map_pair.x.length = h;
    r->density_map_pair.y.map = mapy;
    r->density_map_pair.x.map = mapx;
}

void generatePeakListPair( density_map_pair_t * density_pair, peak_list_pair_t * peaks )
{
    generatePeakList( &density_pair->x, &peaks->x );
    generatePeakList( &density_pair->y, &peaks->y );
}

void generatePeakList( density_map_t * density_map, peak_list_t * peaks )
{
    int pos_thresh = 80, l = density_map->length, peak_index = 0;
    int curr_vel = 0, last_vel = 0;
    int curr_pos = 0, last_pos = 0;
    
    for( int i = 1; i < l; i += PEAK_LIST_SCAN_STEP )
    {
        curr_pos = density_map->map[i];
        curr_vel = curr_pos - last_pos;

        if( curr_pos >= pos_thresh
            && falling(curr_vel, last_vel)
           )
        {
            peaks->map[peak_index] = i;
            peaks->den[peak_index] = curr_pos;
            peaks->dir[peak_index] = 0;
            if(peak_index < MAX_PEAKS_RHO) peak_index++;
        }
        last_pos = curr_pos;
        last_vel = curr_vel;
    }
    peaks->length = peak_index;
}

//void fillDensityMapPairData( density_map_pair_t * m )
//{
//    fillDensityMapData(&m->x);
//    fillDensityMapData(&m->y);
//}

void fillDensityMapDataInt( int * m, int * v, int l )
{
    uint16_t i, c, p = m[0];
    for( i = 1; i < l; i++ )
    {
        c = m[i];
        v[i] = c - p;
        p = c;
    }
    m[0] = 0;
    v[0] = 0;
}

//void fillDensityMapData( density_map_t * m )
//{
//    int l = m->length, i;
//    for( i = 1; i < l; i++ )
//    {
//        m->vel[i]   = m->map[i] - m->map[i-1];
//    }
//    m->map[i] = 0;
//    m->vel[i] = 0;
//}

void initDensityMapPair( density_map_pair_t * m, int w, int h )
{
    m->x.map = (int*)malloc(sizeof(int)*h);
    m->x.length = h;
    m->y.map = (int*)malloc(sizeof(int)*w);
    m->y.length = w;
}
void initPeaksListPair( peak_list_pair_t * p, int w, int h )
{
    p->x.map      = (uint16_t*)malloc(sizeof(uint16_t)*MAX_PEAKS_RHO);
    p->x.den      = (uint16_t*)malloc(sizeof(uint16_t)*MAX_PEAKS_RHO);
    p->x.dir      = (int8_t*)malloc(sizeof(int8_t)*MAX_PEAKS_RHO);
    p->x.length   = 0;
    p->y.map      = (uint16_t*)malloc(sizeof(uint16_t)*MAX_PEAKS_RHO);
    p->y.den      = (uint16_t*)malloc(sizeof(uint16_t)*MAX_PEAKS_RHO);
    p->y.dir      = (int8_t*)malloc(sizeof(int8_t)*MAX_PEAKS_RHO);
    p->y.length   = 0;
}
