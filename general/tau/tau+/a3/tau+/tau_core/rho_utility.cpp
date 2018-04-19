//
//  rho_utility.cpp
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_utility.hpp"
//#include "rho.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>

using namespace cv;

#define opsign(X,Y)     (X<0)^(Y<0)
#define rising(X,Y)    (X>=0)&(Y<0)
#define falling(X,Y)    (X<0)&(Y>=0)

#define opsignthresh(X,Y,T)    ((X*Y)<T)
#define risingthresh(X,Y,T)    ((X*Y)<T)&(Y<X)
#define fallingthresh(X,Y,T)   ((X*Y)<T)&(Y>X)

Rho::Rho( int width, int height ) : gaussian(DEFAULT_GAUSS_LEN), density_map_pair(width, height), frame(height, width, CV_8UC3, Scalar(0,0,0))
{
    printf("Initializing Rho Utility: %dx%d\n", width, height);
    this->width  = width;
    this->height = height;
    gaussian.generate( DEFAULT_GAUSS_LEN, DEFAULT_SIGMA);
    cimageInit(&image, width, height);
    pthread_mutex_init(&density_map_pair_mutex, NULL);
}

void Rho::perform( Mat M, PredictionPair * p )
{
//    frame = M;
//    generateDensityMap();
//    getDensityMaxPair();
//    updateDensityKalmanPair();
//    filterDensityPair();
//    analyzeDensityPair();
//    selectPeakListPair(p);
}

void Rho::perform( cimage_t * img, PredictionPair * p )
{
    image = *img;
    generateCenterOfMass(p);
    
    pthread_mutex_lock(&density_map_pair_mutex);
    generateDensityMapFromCImageWithQuadrantMasses();
    getDensityMaxPair();
    updateDensityKalmanPair();
    filterDensityPair();
    analyzeDensityPair();
    selectPeakListPair(&rho_predictions);
    updatePredictions(&rho_predictions, p);
    pthread_mutex_unlock(&density_map_pair_mutex);
}

void Rho::generateCenterOfMass( PredictionPair * r )
{
    comX = ((int)(r->x.primary + r->x.secondary)) >> 1;
    comY = ((int)(r->y.primary + r->y.secondary)) >> 1;
    if(comX < 0) comX = 0;
    if(comY < 0) comY = 0;
}

// Deprecated
void Rho::generateDensityMap()
{
    int h = height, w = width;
    int * mapx = density_map_pair.x.map, * mapy = density_map_pair.y.map;
    density_map_pair.y.length = w;
    density_map_pair.x.length = h;
    
    memset(mapy, 0, sizeof(int) * w);
    memset(mapx, 0, sizeof(int) * h);

    Vec3b pixel = {0,0,0};
//    double mX = 0, mY = 0;
    for( int y = 0; y < h; y++ )
    {
        int dR = 0;
        for( int x = 0; x < w; x++ )
        {
            int p = (int)frame.at<Vec3b>(y,x)[2];
            if(p > 100)
            {
                dR      ++;
                mapy[x] ++;
            }
        }
        mapx[y] = dR;
        
//        if(dR > mX) mX = dR;
    }
}

// Deprecated
void Rho::generateDensityMapFromCImage()
{
    int h = height, w = width, d = 0, dR = 0;
    int * mapx = density_map_pair.x.map, * mapy = density_map_pair.y.map;
    density_map_pair.y.length = w;
    density_map_pair.x.length = h;
    
    memset(mapy, 0, sizeof(int) * w);
    memset(mapx, 0, sizeof(int) * h);
    
//    double mX = 0;//, mY = 0;
    for( int y = 0; y < h; y++ )
    {
        dR = 0;
        for( int x = 0; x < w; x++ )
        {
            d = x + y*h;
            int p = (unsigned char)image.pixels[d];
            if(p > 10)
            {
                dR      ++;
                mapy[x] ++;
            }
        }
        mapx[y] = dR;
        
//        if(dR > mX) mX = dR;
    }
//    density_map_pair.x.max = mX;
}

void Rho::generateDensityMapFromCImageWithQuadrantMasses()
{
    int h = height, w = width, d = 0, dR = 0, y, x;
    int * mapx = density_map_pair.x.map, * mapy = density_map_pair.y.map;
    density_map_pair.y.length = w;
    density_map_pair.x.length = h;
    
    Q[0] = Q[1] = QT = 0;
    memset(mapy, 0, sizeof(int) * w);
    memset(mapx, 0, sizeof(int) * h);
    
    unsigned char p = 0;
    for( y = 0; y < comY; y++ )
    {
        for( x = 0, dR = 0; x < w; x++)
        {
            d = x + y*h;
            p = (unsigned char)image.pixels[d];
            if(p > 10)
            {
                dR      ++;
                mapy[x] ++;
                Q[x<comX]++;
            }
        }
        mapx[y] = dR;
        QT += dR;
    }
    for( ; y < h; y++ )
    {
        for( x = 0, dR = 0; x < w; x++)
        {
            d = x + y*h;
            p = (unsigned char)image.pixels[d];
            if(p > 10)
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
    for( int i = 1; i < l; i++, c = d->map[i] ) if(c > m ) m = c;
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

void Rho::filterDensityPair()
{
    filterDensity( &density_map_pair.x );
    filterDensity( &density_map_pair.y );
}

void Rho::filterDensity( DensityMap * d )
{
    int c, t = d->kalman.value, v = d->variance, g = t - v, l = d->length, punishf = 5;
    if(g < 0) g = 0;
    
    for( int i = 0; i < l; i++ )
    {
        c = d->map[i];
        if(c > (t+1)) c = t - punishf*(c - t);
        d->fil[i] = (c < g)?0:c-g;
    }
}

void Rho::analyzeDensityPair()
{
    analyzeDensity( &density_map_pair.x, &peak_list_pair.x );
    analyzeDensity( &density_map_pair.y, &peak_list_pair.y );
}

void Rho::analyzeDensity( DensityMap * d, PeakList * p )
{
    int c, max = 0, l = d->length;
    p->length = 0;
    bool has = false;
    double cavg = 0, mavg = 0;
    int count = 0;
    QF = 0;
    for( int i = 0; i < l; i++ )
    {
///TODO: FIX THIS!! 
        c = d->fil[i];
        if(c)
        {
            count++;
            if(!has) has = true;
            cma((double)c, &cavg, count);
            cma((double)(c*i), &mavg, count);
            if(c > max) max = c;
        }
        else if(has)
        {
            if(p->length < MAX_PEAKS_RHO)
            {
                p->map[p->length] = (int)mavg/cavg;
                p->den[p->length] = (int)cavg;
                p->max[p->length] = max;
                
                
                if(cavg < 0)
                    cavg = 0;
//                if(p->den[p->length] < 0) p->den[p->length] = 0;
                p->length++;
                
                QF += (int)cavg;
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
    int den[] = {0,0,0}, loc[] = {0,0,0}, max[] = {0,0,0};
    for(int i = 0; i < l; i++)
    {
        denc = p->den[i];
        locc = p->map[i];
        maxc = p->max[i];
        if( denc > den[0] )
        {
            den[2] = den[1];
            loc[2] = loc[1];
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
            den[2] = den[1];
            loc[2] = loc[1];
            max[2] = max[1];
            den[1] = denc;
            loc[1] = locc;
            max[1] = maxc;
        }
    }
    
    r->primary   = loc[0];
    r->secondary = loc[1];
    
    r->primary_probability   = ((double)max[0])/v;
    r->secondary_probability = ((double)max[1])/v;
    
    
    double comp = 1 - FT/FILTERED_CONVERAGE_TARGET;
    if(comp < 0)
        r->alternate_probability = ((double)max[2])/v;
    else
        r->alternate_probability = comp;
#ifdef RHO_DEBUG
    printf("Alternate probability is %.3f\n", r->alternate_probability);
#endif
    
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

void Rho::updatePredictions( PredictionPair * i, PredictionPair * r )
{
    double Ax = i->x.primary, Ay = i->y.primary, Bx = i->x.secondary, By = i->y.secondary;
    double Afx = Ax, Afy = Ay, Bfx = Bx, Bfy = By;
    double  Axp = i->x.primary_probability, Bxp = i->x.secondary_probability,
            Ayp = i->y.primary_probability, Byp = i->y.secondary_probability;
    double Afxp = Axp, Afyp = Ayp, Bfxp = Bxp, Bfyp = Byp;
    if( !Ax || !Ay || !Bx || !By ) return;

    if( Q[0] > Q[1])
    {
        if( ( Ax < Bx ) )
        {
            Afx = Ax;
            Bfx = Bx;
            Afxp = Bxp;
            Bfxp = Axp;
        }
        else
        {
            Afx = Bx;
            Bfx = Ax;
            Afxp = Axp;
            Bfxp = Bxp;
        }
        if(Ay < By)
        {
            Afy = Ay;
            Bfy = By;
            Afyp = Ayp;
            Bfyp = Byp;
        }
        else
        {
            Afy = By;
            Bfy = Ay;
            Afyp = Byp;
            Bfyp = Ayp;
        }
    }
    else
    {
        if( ( Ax < Bx ) )
        {
            Afx = Bx;
            Bfx = Ax;
            Afxp = Bxp;
            Bfxp = Axp;
        }
        else
        {
            Afx = Ax;
            Bfx = Bx;
            Afxp = Axp;
            Bfxp = Bxp;
        }
        if(Ay < By)
        {
            Afy = Ay;
            Bfy = By;
            Afyp = Ayp;
            Bfyp = Byp;
        }
        else
        {
            Afy = By;
            Bfy = Ay;
            Afyp = Byp;
            Bfyp = Ayp;
        }
    }
    
    if(matchKalmans(&r->x.a, &r->x.b, Afx, Bfx))
    {
        r->y.a.update(Afy, 0.);
        r->y.b.update(Bfy, 0.);
        Afyp = Ayp;
        Bfyp = Byp;
    }
    else
    {
        r->y.a.update(Bfy, 0.);
        r->y.b.update(Afy, 0.);
        Afyp = Byp;
        Bfyp = Ayp;
    }

    r->x.primary    = r->x.a.value;
    r->y.primary    = r->y.a.value;
    r->x.secondary  = r->x.b.value;
    r->y.secondary  = r->y.b.value;
    
    r->x.primary_probability    = Axp;
    r->y.primary_probability    = Ayp;
    r->x.secondary_probability  = Bxp;
    r->y.secondary_probability  = Byp;
    r->x.alternate_probability  = i->x.alternate_probability;
    r->y.alternate_probability  = i->y.alternate_probability;
}
