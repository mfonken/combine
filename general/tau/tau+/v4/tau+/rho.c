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

ring_buffer_t peak_buffer;
uint16_t image_width, image_height;
density_map_pair_t      density_map_pair;
peak_list_pair_t        peak_list_pair;
probability_list_pair_t probability_list_pair;
gaussian_t              gaussian;

void initRho( uint16_t img_width, uint16_t img_height )
{
    printf("Init-ing rho: %dx%d\n", img_width, img_height);
    image_width  = img_width;
    image_height = img_height;
    
    initGaussian(           &gaussian, image_width/2);
    generateGaussian(&gaussian, 15, DEFAULT_SIGMA);
    
    initDensityMapPair(     &density_map_pair, image_width, image_height);
    initPeaksListPair(      &peak_list_pair, image_width, image_height);
    initProbabilityListPair(&probability_list_pair, image_width, image_height);
}

void performRho( pixel_base_t ** img,  peak_list_pair_t * p )
{
    generateDensityMap(      img,     &density_map_pair );
    generatePeakListPair(             &density_map_pair,      &peak_list_pair );
    (*p) = peak_list_pair;
}

void getRhoData( density_map_pair_t * d, peak_list_pair_t * p, probability_list_pair_t * r )
{
    (*d) = density_map_pair;
    (*p) = peak_list_pair;
    (*r) = probability_list_pair;
}

void generateDensityMap( pixel_base_t ** img, density_map_pair_t * density_maps )
{
    image_dimension_base_t p = 0;
    uint16_t row_sum;
    memset(density_maps->x.map, 0, sizeof(float) * density_maps->x.length);
    memset(density_maps->y.map, 0, sizeof(float) * density_maps->y.length);
    for( uint16_t i = 0; i < image_height; i++ )
    {
        row_sum = 0;
        for( uint16_t j = 0; j < image_width; j++ )
        {
            p = img[i][j];
            row_sum += p;
            density_maps->y.map[j] += p;
        }
        density_maps->x.map[i] += row_sum;
    }
    density_maps->x.length = image_height;
    density_maps->y.length = image_width;
    
    gaussianBlur(density_maps->x.map, density_maps->x.length);
    gaussianBlur(density_maps->y.map, density_maps->y.length);
    
//    smooth1D(density_maps->x.map, density_maps->x.length);
//    smooth1D(density_maps->y.map, density_maps->y.length);
    
    fillDensityMapPairData( density_maps );
    
//    gaussianBlur(density_maps->x.map, density_maps->x.length);
    gaussianBlur(density_maps->x.vel, density_maps->x.length);
    gaussianBlur(density_maps->x.acc, density_maps->x.length);
    gaussianBlur(density_maps->x.jrk, density_maps->x.length);
    gaussianBlur(density_maps->x.jnc, density_maps->x.length);
    
//    gaussianBlur(density_maps->y.map, density_maps->y.length);
    gaussianBlur(density_maps->y.vel, density_maps->y.length);
    gaussianBlur(density_maps->y.acc, density_maps->y.length);
    gaussianBlur(density_maps->y.jrk, density_maps->y.length);
    gaussianBlur(density_maps->y.jnc, density_maps->y.length);
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
        m->acc[i]   = m->vel[i] - m->vel[i-1];
        m->jrk[i]   = m->acc[i] - m->acc[i-1];
        m->jnc[i]   = m->jrk[i] - m->jrk[i-1];
    }
    m->map[i] = 0;
    m->vel[i] = 0;
    m->acc[i] = 0;
    m->jrk[i] = 0;
    m->jnc[i] = 0;
}


void gaussianBlur( float * arr, size_t len )
{
    size_t n, k_len = gaussian.length;
    
    double res[len + k_len - 1];
    
    for (n = 0; n < len + k_len - 1; n++)
    {
        size_t kmin, kmax, k;
        
        res[n] = 0;
        
        kmin = (n >= k_len - 1) ? n - (k_len - 1) : 0;
        kmax = (n < len - 1) ? n : len - 1;
        
        for (k = kmin; k <= kmax; k++)
        {
            res[n] += (FLOAT)arr[k] * gaussian.array[n - k];
        }
    }
    int offset = (int)(( k_len - 1 ) / 2);
    for (n = 0; n < len; n++)
    {
        arr[n] = res[n + offset];
    }
}

void smooth1D( float * arr, uint16_t len )
{
    uint16_t a,b,c,d,e,f,g;
    a = arr[0];
    b = arr[1];
    c = arr[2];
    d = arr[3];
    e = arr[4];
    f = arr[5];
    for(uint16_t i = 6; i < len; i++)
    {
        g = arr[i];
        arr[i] = ( a + b + c + d + e + f + g ) / 7;
        a = b;
        b = c;
        c = d;
        d = e;
        e = f;
        f = g;
    }
}

FLOAT gaussianDistribution( int x, int mu, FLOAT sigma)
{
    int d = x - mu;
    FLOAT n = 1.0 / (sqrt(2 * M_PI) * sigma);
    return exp(-d*d/(2 * sigma * sigma)) * n;
};

void generateGaussian( gaussian_t * g, int l, FLOAT s )
{
    if( l == g->length && s == g->sigma) return;
    int m = floor(l/2);
    for(int i = 0, d = l; i <= m; i++, d--)
    {
        g->array[i] = gaussianDistribution(i,m,s);
        if(i != d) g->array[d] = g->array[i];
    }
    g->length = l;
    g->sigma = s;
}

void initGaussian( gaussian_t * g, int l )
{
    (*g).array = (FLOAT*)malloc(sizeof(FLOAT)*l);
    (*g).length = 0;
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
void initProbabilityListPair( probability_list_pair_t * r, int w, int h )
{
    int max_peaks_w = w, max_peaks_h = h;
    (*r).x.primary = (FLOAT*)malloc(sizeof(FLOAT)*max_peaks_h);
    (*r).x.secondary = (FLOAT*)malloc(sizeof(FLOAT)*max_peaks_h);
    (*r).y.primary = (FLOAT*)malloc(sizeof(FLOAT)*max_peaks_w);
    (*r).y.secondary = (FLOAT*)malloc(sizeof(FLOAT)*max_peaks_w);
}
