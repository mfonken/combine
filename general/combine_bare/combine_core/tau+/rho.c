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
    printf("Initializing Rho: %dx%d\n", width, height);
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
    memset(r->density_map_pair.y.map, 0, sizeof(int) * w);
    memset(r->density_map_pair.x.map, 0, sizeof(int) * h);
    
    int row_sum, z = 0, y = h, x;
    
    for(; y > 0; --y )
    {
        for( x = w, row_sum = 0; x > 0; --x )
        {
            if(img[z++])
            {
                r->density_map_pair.y.map[x]++;
                row_sum++;
            }
        }
        r->density_map_pair.x.map[y] += row_sum;
    }
    
    gaussianBlurInt( &r->gaussian, r->density_map_pair.x.map, h, 4);
    gaussianBlurInt( &r->gaussian, r->density_map_pair.y.map, w, 4);
    r->density_map_pair.y.length = w;
    r->density_map_pair.x.length = h;
}

void generatePeakListPair( density_map_pair_t * density_pair, peak_list_pair_t * peaks )
{
    generatePeakList( &density_pair->x, &peaks->x );
    generatePeakList( &density_pair->y, &peaks->y );
}

void generatePeakList( density_map_t * density_map, peak_list_t * peaks )
{
    int map_thresh = 60, l = density_map->length, peak_index = 0;
    int curr_vel = 0, last_vel = 0;
    int curr_map = 0, last_map = 0;
    
    for( int i = 1; i < l; i += PEAK_LIST_SCAN_STEP )
    {
        curr_map = density_map->map[i];
        curr_vel = curr_map - last_map;

        if( curr_map >= map_thresh
            && fallingthresh(curr_vel, last_vel, 2)
           )
        {
            peaks->map[peak_index] = i;
            peaks->den[peak_index] = curr_map;
            peaks->dir[peak_index] = 0;
            if(peak_index < MAX_PEAKS_RHO) peak_index++;
        }
        last_map = curr_map;
        last_vel = curr_vel;
    }
    peaks->length = peak_index;
}

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
