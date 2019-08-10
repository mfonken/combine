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
    
    initGaussian(    &r->gaussian, r->width/2);
    generateGaussian(&r->gaussian, 15, DEFAULT_SIGMA);
    
    initDensityMapPair(      &r->density_map_pair,      width, height);
    initPeaksListPair(       &r->peak_list_pair,        width, height);
}

void performRho( rho_t * r, pixel_base_t ** img )
{
    generateDensityMap( r, img);
    generatePeakListPair( &r->density_map_pair,      &r->peak_list_pair );
}

void generateDensityMap( rho_t * r, pixel_base_t ** img )
{
    image_dimension_base_t p = 0;
    uint16_t row_sum;
    memset(r->density_map_pair.x.map, 0, sizeof(float) * r->density_map_pair.x.length);
    memset(r->density_map_pair.y.map, 0, sizeof(float) * r->density_map_pair.y.length);
    for( uint16_t i = 0; i < r->height; i++ )
    {
        row_sum = 0;
        for( uint16_t j = 0; j < r->width; j++ )
        {
            p = img[i][j];
            row_sum += p;
            r->density_map_pair.y.map[j] += p;
        }
        r->density_map_pair.x.map[i] += row_sum;
    }
    r->density_map_pair.x.length = r->height;
    r->density_map_pair.y.length = r->width;
    
    gaussianBlur(&r->gaussian, r->density_map_pair.x.map, r->density_map_pair.x.length);
    gaussianBlur(&r->gaussian, r->density_map_pair.y.map, r->density_map_pair.y.length);
    fillDensityMapPairData( &r->density_map_pair );
    gaussianBlur(&r->gaussian, r->density_map_pair.x.vel, r->density_map_pair.x.length);
    gaussianBlur(&r->gaussian, r->density_map_pair.y.vel, r->density_map_pair.y.length);
}

void generatePeakListPair( density_map_pair_t * density_pair, peak_list_pair_t * peaks )
{
    generatePeakList( &density_pair->x, &peaks->x );
    generatePeakList( &density_pair->y, &peaks->y );
}

void generatePeakList( density_map_t * density_map, peak_list_t * peaks )
{
//    float pos_thresh = 50, vel_thresh = 1, acc_thresh = 0.01, jrk_thresh = 0, jnc_thresh = 0.1;
    float pos_thresh = 80, vel_thresh = 0;// acc_thresh = 0.1, jrk_thresh = 0.13, jnc_thresh = 0.01;
    
    uint16_t l = density_map->length, peak_index = 0;
    
    float last_jnc = density_map->jnc[0], curr_jnc = 0;
    float last_jrk = density_map->jrk[0], curr_jrk = 0;
    float last_acc = density_map->acc[0], curr_acc = 0;
    float last_vel = density_map->vel[0], curr_vel = 0;
    float last_pos = density_map->map[0], curr_pos = 0;
    
    for( int i = 1; i < l; i++ )
    {
        curr_pos = density_map->map[i];
        curr_vel = density_map->vel[i];
        curr_acc = density_map->acc[i];
        curr_jrk = density_map->jrk[i];
        curr_jnc = density_map->jnc[i];

        if( curr_pos >= pos_thresh
              && fallingthresh(curr_vel, last_vel, vel_thresh) //&& curr_vel >= vel_thresh
//            && fallingthresh(curr_acc, last_acc, acc_thresh)
//            && curr_jrk <= -jrk_thresh
//            && risingthresh(curr_jnc, last_jnc, jnc_thresh)
           )
        {
            peaks->map[peak_index] = i;
            peaks->den[peak_index] = curr_pos;
            peaks->dir[peak_index] = 0;
            peak_index++;
        }
        
        last_jnc = curr_jnc;
        last_jrk = curr_jrk;
        last_acc = curr_acc;
        last_vel = curr_vel;
        last_pos = curr_pos;
    }
    
    peaks->length = peak_index;
}

void fillDensityMapPairData( density_map_pair_t * m )
{
    fillDensityMapData(&m->x);
    fillDensityMapData(&m->y);
}

void fillDensityMapData( density_map_t * m )
{
    int l = m->length, i;
    for( i = 1; i < l; i++ )
    {
        m->vel[i]   = m->map[i] - m->map[i-1];
//        m->acc[i]   = m->vel[i] - m->vel[i-1];
//        m->jrk[i]   = m->acc[i] - m->acc[i-1];
//        m->jnc[i]   = m->jrk[i] - m->jrk[i-1];
    }
    m->map[i] = 0;
    m->vel[i] = 0;
//    m->acc[i] = 0;
//    m->jrk[i] = 0;
//    m->jnc[i] = 0;
}

void initDensityMapPair( density_map_pair_t * m, int w, int h )
{
    (*m).x.map = (float*)malloc(sizeof(float)*h);
    (*m).x.vel = (float*)malloc(sizeof(float)*h);
    (*m).x.acc = (float*)malloc(sizeof(float)*h);
    (*m).x.jrk = (float*)malloc(sizeof(float)*h);
    (*m).x.jnc = (float*)malloc(sizeof(float)*h);
    (*m).x.length = 0;
    (*m).y.map = (float*)malloc(sizeof(float)*w);
    (*m).y.vel = (float*)malloc(sizeof(float)*w);
    (*m).y.acc = (float*)malloc(sizeof(float)*w);
    (*m).y.jrk = (float*)malloc(sizeof(float)*w);
    (*m).y.jnc = (float*)malloc(sizeof(float)*w);
    (*m).y.length = 0;
}
void initPeaksListPair( peak_list_pair_t * p, int w, int h )
{
    int max_peaks_w = w, max_peaks_h = h;
    (*p).x.map      = (uint16_t*)malloc(sizeof(uint16_t)*max_peaks_h);
    (*p).x.den      = (uint16_t*)malloc(sizeof(uint16_t)*max_peaks_h);
    (*p).x.dir      = (int*)malloc(sizeof(int)*max_peaks_h);
    (*p).x.length   = 0;
    (*p).y.map      = (uint16_t*)malloc(sizeof(uint16_t)*max_peaks_w);
    (*p).y.den      = (uint16_t*)malloc(sizeof(uint16_t)*max_peaks_w);
    (*p).y.dir      = (int*)malloc(sizeof(int)*max_peaks_w);
    (*p).y.length   = 0;
}
