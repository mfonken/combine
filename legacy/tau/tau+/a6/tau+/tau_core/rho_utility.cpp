//
//  rho_utility.cpp
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_utility.hpp"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>

/* RHO PIXEL CHECK */
#define RPC(X) if(X&0xf8)
#define RPCB(X,Y) {RPC(X){Q##Y++;mapy[x]++;}}

//#define FOR(X,L) for( int X = L; X > 0; --X )
#define FOR(X,L) for( int X = 0; X < L; X++)
#define FORA(X,L,A) for( int X = L; X > 0; --X, A )
#define ZDIV(X,Y) (!Y?2<<10:X/Y)

using namespace cv;

Rho::Rho( int width, int height ) : gaussian(DEFAULT_GAUSS_LEN), density_map_pair(width, height), frame(height, width, CV_8UC3, Scalar(0,0,0))
{
    printf("Initializing Rho Utility: %dx%d & [%.3f, %.3f, %.3f]\n", width, height, RHO_K_TARGET, RHO_VARIANCE_NORMAL, RHO_VARIANCE_SCALE);
    this->width  = width;
    this->height = height;
    gaussian.generate( DEFAULT_GAUSS_LEN, DEFAULT_SIGMA);
    cimageInit(&image, width, height);
    pthread_mutex_init(&density_map_pair_mutex, NULL);
    pthread_mutex_init(&frame_mutex, NULL);
    pthread_mutex_init(&c_mutex, NULL);
}

void Rho::perform( cimage_t * img, PredictionPair * p )
{
    image = *img;
    generateCenterOfMass(p);
    pthread_mutex_lock(&density_map_pair_mutex);
    generateDensityMapFromCImageWithQuadrantMasses();
    getDensityMaxAndUpdateVarianceThenFilterAnalyzeAndSelectPeakPair(p);
    updatePredictions(p);
    pthread_mutex_unlock(&density_map_pair_mutex);
}

void Rho::generateCenterOfMass( PredictionPair * r )
{
    pthread_mutex_lock(&c_mutex);
    Cy = ((int)(r->x.primary.value + r->x.secondary.value)) / 2;
    Cx = ((int)(r->y.primary.value + r->y.secondary.value)) / 2;
    if(Cx < 0) Cx = 0;
    if(Cy < 0) Cy = 0;
    if(Cx > width ) Cx = width;
    if(Cy > height) Cy = height;
    pthread_mutex_unlock(&c_mutex);
}

/* Unrolled Density map generator */
void Rho::generateDensityMapFromCImageWithQuadrantMasses()
{
    int h = height, w = width;
    int * mapx = density_map_pair.x.map, * mapy = density_map_pair.y.map;
    density_map_pair.y.length = w;
    density_map_pair.x.length = h;
    
    int y, x, p;
    
    pthread_mutex_lock(&c_mutex);
    Q[0] = Q[1] = Q[2] = Q[3] = QT = p = 0;
    memset(mapy, 0, sizeof(int) * w);
    memset(mapx, 0, sizeof(int) * h);
    int Q0 = 0, Q1 = 0, Q2 = 0, Q3 = 0, QN = 0, QN_ = 0;
    for( y = 0, p = 0; y < Cy; y++ )
    {
        for( x = 0; x < Cx; x++, p++ ) RPCB(image.pixels[p], 0);
        for(      ; x <  w; x++, p++ ) RPCB(image.pixels[p], 1);
        QN = Q0 + Q1;
        mapx[y] = QN - QN_;
        QN_ = QN;
    }
    for( ; y < h; y++ )
    {
        for( x = 0; x < Cx; x++, p++ ) RPCB(image.pixels[p], 2);
        for(      ; x <  w; x++, p++ ) RPCB(image.pixels[p], 3);
        QN = Q2 + Q3;
        mapx[y] = QN - QN_;
        QN_ = QN;
    }
    pthread_mutex_unlock(&c_mutex);
    Q[0] = Q0; Q[1] = Q1; Q[2] = Q2; Q[3] = Q3;
    QT = Q[0] + Q[1] + Q[2] + Q[3];

#ifdef RHO_DEBUG
    printf("# Total coverage is %.3f%%\n", ((double)QT)/((double)w*h)*100);
#endif
}


void Rho::getDensityMaxAndUpdateVarianceThenFilterAnalyzeAndSelectPeakPair( PredictionPair * r )
{
    getDensityMaxAndUpdateVarianceThenFilterAnalyzeAndSelectPeak( &density_map_pair.x, &peak_list_pair.x, &r->x );
    getDensityMaxAndUpdateVarianceThenFilterAnalyzeAndSelectPeak( &density_map_pair.y, &peak_list_pair.y, &r->y );
}

void Rho::getDensityMaxAndUpdateVarianceThenFilterAnalyzeAndSelectPeak( DensityMap * d, PeakList * p, Prediction * r )
{
    QF = 0;
    int l = d->length,
        c = 0,
        m = 0,
        t = 0,
        v = 0,
        g = 0;
    
    int c1      = 0,
        c2      = 0,
        count   = 0,
        gapc    = 0,
        has     = 0,
        denc    = 0,
        maxc    = 0,
        den[]   = {0,0},
        loc[]   = {0,0},
        max[]   = {0,0,0};
    
    double cavg = 0,
           mavg = 0;
    
    /*** Find max and update kalman ***/
    FORA(i,l,c = d->map[i])
        if( c > m )
            m = c;
    d->kalman.update(m, 0.);
    d->max = m;
    d->variance = v;
    t = d->kalman.value;
    v = RHO_VARIANCE_NORMAL * ( 1 + RHO_VARIANCE_SCALE * ( RHO_K_TARGET - d->kalman.K[0] ) );
    if( v < t && v > 0) g = t - v;
    else return;
    /***/
    
    /* Find blob centers and sort out the top 2 */
    FORA(i,l,c1 = d->map[i])
    {
        if(c1 > (t+1)) c1 = t - RHO_PUNISH_FACTOR*(c1 - t);
        if(c1 > g)
        {
            c2 = c1 - g;
            cma_M0_M1((double)c2, i, &cavg, &mavg, &count);
            if(c2 > maxc)
                maxc = c2;
            has = 1;
            gapc = 0;
        }
        else if(has && gapc > RHO_GAP_MAX)
        {
            denc = (int)cavg;
            if( denc > den[0] )
            {
                loc[1] = loc[0]; loc[0] = (int)mavg/cavg;
                den[1] = den[0]; den[0] = denc;
                max[2] = max[1]; max[1] = max[0]; max[0] = maxc;
            }
            else if( denc > den[1] )
            {
                loc[1] = (int)mavg/cavg;
                den[1] = denc;
                max[2] = max[1]; max[1] = maxc;
            }
            QF += cavg;
            mavg = cavg = 0;
            has = count = gapc = 0;
        }
        else gapc++;
        
        /* Fill filter array for display only */
        d->fil[i] = (c1 < g)?0:c1;
    }
    FT = ((double)QF)/((double)QT);
#ifdef RHO_DEBUG
    printf("* Filtered coverage is %.3f%%\n", FT*100);
#endif
    
    if( !loc[0] || !loc[1] ) return;
    
    
    /* Update prediction with best peaks */
    r->primary_new   = loc[0];
    r->secondary_new = loc[1];
    
    double v_ = ZDIV(1.0,v);
    
    r->probabilities.primary   = ((double)max[0]) * v_;
    r->probabilities.secondary = ((double)max[1]) * v_;
    
    double comp = 1 - FT/FILTERED_CONVERAGE_TARGET;
    if(comp < 0)
        r->probabilities.alternate = ((double)max[2]) * v_;
    else
        r->probabilities.alternate = comp;
    
#ifdef RHO_DEBUG
    printf("Alternate probability is %.3f\n", r->probabilities.alternate);
#endif
}

void Rho::updatePredictions( PredictionPair * r )
{
    int Ax = r->x.primary_new, Ay = r->y.primary_new, Bx = r->x.secondary_new, By = r->y.secondary_new;
//    double Afx = Ax, Afy = Ay, Bfx = Bx, Bfy = By;
    
    bool nondiag = false;
    signed char qcheck = ( Q[0] > Q[1] ) + ( Q[2] < Q[3] ) - 1;
    
    pthread_mutex_lock(&c_mutex);
    if( ( Ax < Cx ) ^ ( Bx > Cx ) )
    {
        // X ambiguous
        double x1 = r->x.primary.value + r->x.primary.velocity;
        double p1 = fabs(x1-Ax), p2 = fabs(x1-Bx);
        if( p1 > p2 ) iswap(&Ax, &Bx);
        nondiag = true;
#ifdef RHO_DEBUG
        printf("X Ambiguity (%d,%d,%d)\n", Ax, Bx, Cx);
#endif
    }
    if( ( Ay < Cy ) ^ ( By > Cy ) )
    {
        // Y ambiguous
        double y1 = r->y.primary.value + r->y.primary.velocity;
        double p1 = fabs(y1-Ay), p2 = fabs(y1-By);
        if( p1 > p2 ) iswap(&Ay, &By);
        nondiag = true;
#ifdef RHO_DEBUG
        printf("Y Ambiguity (%d,%d,%d)\n", Ay, By, Cy);
#endif
    }
    pthread_mutex_unlock(&c_mutex);
    
    if ( !nondiag )
    {
        if( Ax > Bx ) iswap(&Ax, &Bx);
        if ( ( qcheck > 0 ) ^ ( Ay < By ) ) iswap(&Ay, &By);
        
#ifdef RHO_DEBUG
        if( qcheck < 0 ) printf("Secondary diagonal\n");
        if( qcheck > 0 ) printf("Primary diagonal\n");
#endif
    }
    
    r->x.primary.update(    Ax, 0.);
    r->x.secondary.update(  Bx, 0.);
    r->y.primary.update(    Ay, 0.);
    r->y.secondary.update(  By, 0.);
}

