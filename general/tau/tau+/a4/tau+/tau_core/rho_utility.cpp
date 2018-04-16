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
//#define FOR(X,L) for( int X = L; X > 0; --X )
#define FOR(X,L) for( int X = 0; X < L; X++)
#define FORA(X,L,A) for( int X = L; X > 0; --X, A )

using namespace cv;

Rho::Rho( int width, int height ) : gaussian(DEFAULT_GAUSS_LEN), density_map_pair(width, height), frame(height, width, CV_8UC3, Scalar(0,0,0))
{
    printf("Initializing Rho Utility: %dx%d\n", width, height);
    this->width  = width;
    this->height = height;
    gaussian.generate( DEFAULT_GAUSS_LEN, DEFAULT_SIGMA);
    cimageInit(&image, width, height);
    pthread_mutex_init(&density_map_pair_mutex, NULL);
    pthread_mutex_init(&frame_mutex, NULL);
}

void Rho::perform( cimage_t * img, PredictionPair * p )
{
    image = *img;
    generateCenterOfMass(p);
    
    pthread_mutex_lock(&density_map_pair_mutex);
    generateDensityMapFromCImageWithQuadrantMasses();
    getDensityMaxPair();
    updateDensityKalmanPair();
    filterAndAnalyzeDensityPair();
    selectPeakListPair(p);
    updatePredictions(p);
    pthread_mutex_unlock(&density_map_pair_mutex);
}

void Rho::generateCenterOfMass( PredictionPair * r )
{
    comY = ((int)(r->x.primary.value + r->x.secondary.value)) >> 1;
    comX = ((int)(r->y.primary.value + r->y.secondary.value)) >> 1;
    if(comX < 0) comX = 0;
    if(comY < 0) comY = 0;
    if(comX > width ) comX = width;
    if(comY > height) comY = height;
}

/* Unrolled Density map generator */
void Rho::generateDensityMapFromCImageWithQuadrantMasses()
{
    frame = {0.};
    int h = height, w = width, dR = 0, y, x, p = 0;
    int * mapx = density_map_pair.x.map, * mapy = density_map_pair.y.map;
    density_map_pair.y.length = w;
    density_map_pair.x.length = h;
    
    Q[0] = Q[1] = QT = 0;
    memset(mapy, 0, sizeof(int) * w);
    memset(mapx, 0, sizeof(int) * h);

    for( y = 0, p = 0; y < comY; y++ )
    {
        for( x = 0, dR = 0; x < w; x++)
        {
            RPC( image.pixels[p++] )
            {
                dR      ++;
                mapy[x] ++;
                Q[x<comX] ++;
            }
        }
        mapx[y] = dR;
    }
    QT = Q[0] + Q[1];
    for( y = comY; y < h; y++ )
    {
        for( x = 0, dR = 0; x < w; x++)
        {
            RPC( image.pixels[p++] )
            {
                dR      ++;
                mapy[x] ++;
            }
        }
        mapx[y] = dR;
        QT += dR;
    }
#ifdef RHO_DEBUG
    printf("# Total coverage is %.3f%%\n", ((double)QT)/((double)w*h)*100);
#endif
}

void Rho::getDensityMaxPair()
{
    getDensityMax(&density_map_pair.x);
    getDensityMax(&density_map_pair.y);
}

void Rho::getDensityMax( DensityMap * d )
{
    int l = d->length, c = 0, m = 0;
    FORA(i,l,c = d->map[i]) if( c > m ) m = c;
    d->max = m;
}

void Rho::updateDensityKalmanPair()
{
    updateDensityKalman( &density_map_pair.x );
    updateDensityKalman( &density_map_pair.y );
}

void Rho::updateDensityKalman( DensityMap * d )
{
    d->kalman.update(d->max, 0.);
    d->variance = RHO_VARIANCE_NORMAL * ( 1 + RHO_VARIANCE_SCALE*(RHO_K_TARGET - d->kalman.K[0]) );
}

void Rho::filterAndAnalyzeDensityPair()
{
    filterAndAnalyzeDensity( &density_map_pair.x, &peak_list_pair.x );
    filterAndAnalyzeDensity( &density_map_pair.y, &peak_list_pair.y );
}

void Rho::filterAndAnalyzeDensity( DensityMap * d, PeakList * p )
{
    int c1, c2, t = d->kalman.value, v = d->variance, g = t - v, l = d->length, max = 0, count = 0;
    bool has = false;
    double cavg = 0, mavg = 0;
    
    QF = 0;
    p->length = 0;
    if(g < 0) g = 0;
    memset(d->fil, 0, sizeof(int) * d->length);
    
    FOR(i,l)
    {
        c1 = d->map[i];
        if(c1 > (t+1)) c1 = t - RHO_PUNISH_FACTOR*(c1 - t);
        c2 = d->fil[i] = (c1 < g)?0:c1-g;
        if(c2)
        {
            has = true;
            cma((double)c2, &cavg, count);
            cma((double)(c2*i), &mavg, count);
            count++;
            if(c2 > max) max = c2;
        }
        else if(has)
        {
            if(p->length < MAX_PEAKS_RHO)
            {
                p->map[p->length] = (int)mavg/cavg;
                p->den[p->length] = (int)cavg;
                p->max[p->length] = max;
                p->length++;
                
                QF += cavg;
            }
            mavg = 0;
            cavg = 0;
            count = 0;
            has = false;
        }
    }
    FT = ((double)QF)/((double)QT);
#ifdef RHO_DEBUG
    printf("* Filtered coverage is %.3f%%\n", FT*100);
#endif
}

void Rho::selectPeakListPair( PredictionPair * r )
{
    selectPeakList( 2*density_map_pair.x.variance, &peak_list_pair.x, &r->x );
    selectPeakList( 2*density_map_pair.y.variance, &peak_list_pair.y, &r->y );
}

void Rho::selectPeakList( double v, PeakList * p, Prediction * r )
{
    int l = p->length, denc = 0, locc = 0, maxc = 0;
    int den[] = {0,0}, loc[] = {0,0}, max[] = {0,0,0};
    FOR(i,l)
    {
        denc = p->den[i];
        locc = p->map[i];
        maxc = p->max[i];
        if( denc > den[0] )
        {
            max[2] = max[1];
            den[1] = den[0];
            loc[1] = loc[0];
            max[1] = max[0];
            den[0] = denc;
            loc[0] = locc;
            max[0] = maxc;
        }
        else if( denc > den[1] )
        {
            max[2] = max[1];
            den[1] = denc;
            loc[1] = locc;
            max[1] = maxc;
        }
    }
    
    if( !loc[0] || !loc[1] ) return;
    
    r->primary_new   = loc[0];
    r->secondary_new = loc[1];
    
    r->probabilities.primary   = ((double)max[0])/v;
    r->probabilities.secondary = ((double)max[1])/v;
    
    
    double comp = 1 - FT/FILTERED_CONVERAGE_TARGET;
    if(comp < 0)
        r->probabilities.alternate = ((double)max[2])/v;
    else
        r->probabilities.alternate = comp;
#ifdef RHO_DEBUG
    printf("Alternate probability is %.3f\n", r->alternate_probability);
#endif
}

void Rho::updatePredictions( PredictionPair * r )
{
    double Ax = r->x.primary_new, Ay = r->y.primary_new, Bx = r->x.secondary_new, By = r->y.secondary_new;
    if(Ay > By) fswap(&Ay, &By);
    if( !( ( Q[0] > Q[1] ) ^ ( Ax > Bx ) ) ) fswap(&Ax, &Bx);
    
    if(matchKalmans(&r->x.primary, &r->x.secondary, Ax, Bx))
    {
        r->y.primary.update(Ay, 0.);
        r->y.secondary.update(By, 0.);
        r->selection_pair = SIMILAR;
    }
    else
    {
        r->y.primary.update(By, 0.);
        r->y.secondary.update(Ay, 0.);
        r->selection_pair = OPPOSITE;
    }
}

int matchKalmans(KalmanFilter * K1, KalmanFilter * K2, double i1, double i2)
{
    double x_1 = K1->value, v1 = K1->velocity, v2 = K2->velocity;
    double x1 = x_1 + v1;
    double p1 = fabs(x1-i1), p2 = fabs(x1-i2);
    
    if( p1 < p2 )
    {
        K1->update(i1, v1);
        K2->update(i2, v2);
        return 0;
    }
    else
    {
        K1->update(i2, v1);
        K2->update(i1, v2);
        return 1;
    }
}
