//
//  rho_utility.c
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_utility.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <unistd.h>

#define INR(X,H)     (X<0?0:((X>H)?(H-1):X))
#define MAX(A,B)     ((A>B)?A:B)
#define XRNG(A,B,C)  (A<(B-C)?-1:(A>(B+C)?1:0)) //Check if exceeds range

#define ALLOW_NEGATIVE_REDISTRIBUTION

#define FOR(L)       for( int i = L; i > 0; --i)
#define FORA(L,A)    for( int i = L; i > 0; --i, A[i] )
#define FORA2(L,A,B) for( int i = L; i > 0; --i, A[i], B[i] )
#define ZDIV(X,Y)    (!X?0:(!Y?2<<10:X/Y))

#define BOUND(X,MIN,MAX) ((X<MIN)?MIN:((X>MAX)?MAX:X))

/* Universal Port for interrupt model */
pixel_base_t test_port = 0;

/* Quadrant density redistribution lookup table */
const density_redistribution_lookup_t rlookup =
{
    {
        {
            { { 0, 1, 3, 4 }, { 2, 5 }, { 6, 7 }, { 8 } },
            { { 0 }, { 1, 2 }, { 3, 6 }, { 4, 5, 7, 8 } },
            { { 0, 1, 2, 3 }, { 1, 3 }, { 2, 3 }, { 3 } },
            { 4, 2, 2, 1 }
        },
        {
            { { 0, 3 }, { 1, 2, 4, 5 }, { 6 }, { 7, 8 } },
            { { 0, 1 }, { 2 }, { 3, 4, 6, 7 }, { 5, 8 } },
            { { 0, 2 }, { 0, 1, 2, 3 }, { 2 }, { 2, 3 } },
            { 2, 4, 1, 2 }
        },
        {
            { { 0, 1 }, { 2 }, { 3, 4, 6, 7 }, { 5, 8 } },
            { { 0, 3 }, { 1, 2, 4, 5 }, { 6 }, { 7, 8 } },
            { { 0, 1 }, { 1 }, { 0, 1, 2, 3 }, { 1, 3 } },
            { 2, 1, 4, 2 }
        },
        {
            { { 0 }, { 1, 2 }, { 3, 6 }, { 4, 5, 7, 8 } },
            { { 0, 1, 3, 4 }, { 2, 5 }, { 6, 7 }, { 8 } },
            { { 0 }, { 0, 1 }, { 0, 2 }, { 0, 1, 2, 3 } },
            { 1, 2, 2, 4 }
        }
    }
};

/* Generic centroid and mass calculator */
static int calculateCentroid( DensityMapC * dmap, int * C, register int thresh )
{
    double avg = 0, mavg = 0;
    int cnt = 0, tot = 0;
    for( int i = 0; i < dmap->length; i++ )
    {
        int c = dmap->map[i];
        if( c > thresh )
        {
            cma_M0_M1(c, i, &avg, &mavg, &cnt);
            tot += c;
        }
    }
    *C = (int)(mavg/avg);
    return tot;
}

static void Redistribute_Densities( rho_c_utility * utility )
{
    int Bx = utility->Bx,
        By = utility->By,
        Cx = utility->Cx,
        Cy = utility->Cy,
        x0 = Bx, x1 = Cx, y0 = By, y1 = Cy,
        area[9];
    if( Cx < Bx ){x0 = Cx; x1 = Bx;}
    if( Cy < By ){y0 = Cy; y1 = By;}
    
    int xl[3] = {x0, x1-x0, utility->width-x1},
        yl[3] = {y0, y1-y0, utility->height-y1};
    for( int y = 0, p = 0; y < 3; y++ )
        for( int x = 0; x < 3; x++ )
            area[p++] = xl[x]* yl[y];
    
    uint8_t c = ( (uint8_t)( utility->Cy < By ) << 1 ) + (uint8_t)( utility->Cx < Bx );
    for( int q = 0; q < 4; q++ )
    {
        int bd = 0, l = rlookup.config[c].length[q], l_ = rlookup.config[c].length[3-q];
        for( int i = 0; i < l; i++ )
        {
            int a = 0, b = 0;
            a = area[rlookup.config[c].current[q][i]];
            for( int j = 0; j < l_; j++ )
                b += area[rlookup.config[c].background[rlookup.config[c].factor[q][i]][j]];
            bd += ZDIV( a, b ) * utility->Qb[q];
        }
#ifndef ALLOW_NEGATIVE_REDISTRIBUTION
        if( bd > utility->Q[q] ) utility->Qf[q] = 0;
        else
#endif
            utility->Qf[q] = utility->Q[q] - bd;
    }
}

void Init(rho_c_utility * utility, int w, int h)
{
    /* Utility frame */
    utility->width  = w;
    utility->height = h;

    /* Threshold Filter */
    RhoKalman.init(&utility->thresh_filter, 0, RHO_THRESH_LS, RHO_THRESH_VU, RHO_THRESH_BU, RHO_THRESH_SU );
    
    /* Density Filters */
    utility->density_map_pair.x.map = (int*)malloc(sizeof(int)*h);
    utility->density_map_pair.x.length = h;
    utility->density_map_pair.x.max[0] = 0;
    utility->density_map_pair.x.max[1] = 0;
    RhoKalman.init(&utility->density_map_pair.x.kalmans[0], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);
    RhoKalman.init(&utility->density_map_pair.x.kalmans[1], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);
    
    utility->density_map_pair.y.map = (int*)malloc(sizeof(int)*w);
    utility->density_map_pair.y.length = w;
    utility->density_map_pair.y.max[0] = 0;
    utility->density_map_pair.y.max[1] = 0;
    RhoKalman.init(&utility->density_map_pair.y.kalmans[0], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);
    RhoKalman.init(&utility->density_map_pair.y.kalmans[1], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);
    
    /* Background Maps */
    utility->background_map_pair.x.map = (int*)malloc(sizeof(int)*h);
    utility->background_map_pair.x.length = h;
    //memset( &utility->background_map_pair.x.map, 0, sizeof(int)*h );
    
    utility->background_map_pair.y.map = (int*)malloc(sizeof(int)*w);
    utility->background_map_pair.y.length = w;
    //memset( &utility->background_map_pair.y.map, 0, sizeof(int)*w );
    
    /* Prediction Filters */
    RhoKalman.init(&utility->prediction_pair.x.primary,   0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU);
    RhoKalman.init(&utility->prediction_pair.x.secondary, 0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU);
    RhoKalman.init(&utility->prediction_pair.y.primary,   0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU);
    RhoKalman.init(&utility->prediction_pair.y.secondary, 0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU);
    utility->prediction_pair.x.primary_new   = 0.;
    utility->prediction_pair.x.secondary_new = 0.;
    utility->prediction_pair.y.primary_new   = 0.;
    utility->prediction_pair.y.secondary_new = 0.;
    
    /* Prediction probabilities */
    memset(&utility->prediction_pair.x.probabilities, 0, sizeof(double)*3);
    memset(&utility->prediction_pair.y.probabilities, 0, sizeof(double)*3);
    
    /***** INTERRUPT MODEL CROSS-CONNECTOR VARIABLES START *****/
    /* Connect to Interrupt Model variable structure */
    RhoVariables.ram.Dx      =  utility->density_map_pair.x.map;
    RhoVariables.ram.Dy      =  utility->density_map_pair.y.map;
    RhoVariables.ram.Q       =  utility->Q;
    RhoVariables.ram.CX_ADDR = &utility->Cx;
    RhoVariables.ram.CY_ADDR = &utility->Cy;
    RhoVariables.ram.C_FRAME =  utility->cframe;
    RhoVariables.ram.THRESH_ADDR = &utility->thresh;
    
    RhoVariables.global.C_FRAME_MAX = C_FRAME_SIZE;
    RhoVariables.global.y_delimiter = Y_DEL;
    RhoVariables.global.W    =  utility->width;
    RhoVariables.global.H    =  utility->height;
    
    /* Frame Initializer routine */
    RhoInterrupts.FRAME_INIT();
    
    /* Interrupt model mutex initializer */
    pthread_mutex_init(&RhoVariables.global.rho_int_mutex, NULL);
    pthread_mutex_lock(&RhoVariables.global.rho_int_mutex);
    /*****  INTERRUPT MODEL CROSS-CONNECTOR VARIABLES END  *****/
}

/* Interrupt (Simulated Hardware-Driven) Density map generator */
void Generate_Density_Map_Using_Interrupt_Model( rho_c_utility * utility, cimage_t image, bool backgrounding )
{
    if( backgrounding )
    {
        RhoVariables.ram.Dx      =  utility->background_map_pair.x.map;
        RhoVariables.ram.Dy      =  utility->background_map_pair.y.map;
        RhoVariables.ram.CX_ADDR = &utility->By;
        RhoVariables.ram.CY_ADDR = &utility->Bx;
        RhoVariables.ram.Q       =  utility->Qb;
    }

    RhoVariables.ram.CAM_PORT = &test_port;
    RhoInterrupts.FRAME_START();
    pthread_create(&RhoVariables.global.loop_thread, NULL, (void *)RhoInterrupts.LOOP_THREAD, (void *)&RhoVariables.global.rho_int_mutex);
    int p = 0;
    for( int y = 0; y < image.height; y++ )
    {
        RhoInterrupts.ROW_INT();
        for( int x = 0; x < image.width; x++, p++ )
        {
            *(RhoVariables.ram.CAM_PORT) = image.pixels[p];
            RhoInterrupts.PCLK_INT();
        }
    }
    RhoInterrupts.FRAME_END();
    
    if( backgrounding )
    {
        /* Check for insignificant background */
        if( RhoVariables.ram.QT < BACKGROUND_COVERAGE_MIN )
        {
            memset(RhoVariables.ram.Q, 0, sizeof(RhoVariables.ram.Q[0])*4);
            utility->Bx = 0;
            utility->By = 0;
            utility->QbT = 0;
        }
        /* Otherwise calculate background centroids in X & Y */
        else
        {
            int xt = calculateCentroid( &utility->background_map_pair.x, &utility->Bx, BACKGROUND_CENTROID_CALC_THRESH );
            int yt = calculateCentroid( &utility->background_map_pair.y, &utility->By, BACKGROUND_CENTROID_CALC_THRESH );
            utility->QbT = MAX( xt, yt );
        }
    
        RhoVariables.ram.Dx      =  utility->density_map_pair.x.map;
        RhoVariables.ram.Dy      =  utility->density_map_pair.y.map;
        RhoVariables.ram.CX_ADDR = &utility->Cx;
        RhoVariables.ram.CY_ADDR = &utility->Cy;
        RhoVariables.ram.Q       =  utility->Q;
    }
    else
    {
        Redistribute_Densities( utility );
    }
}

/* Calculate and process data in variance band from density filter to generate predictions */
void Filter_and_Select( rho_c_utility * utility, DensityMapC * d, DensityMapC * b, PredictionC * r )
{
    int l = d->length,
        fpeak   = 0,
        fvar    = 0,
        fbandl  = 0,
        c       = 0,
        m       = 0,
        c1      = 0,
        c2      = 0,
        b1      = 0,
        count   = 0,
        gapc    = 0,
        has     = 0,
        denc    = 0,
        maxc    = 0,
        den[]   = {0,0},
        loc[]   = {0,0},
        max[]   = {0,0,0};
    
    double
    cavg    = 0.,
    mavg    = 0.;
    
    utility->QF = 0;
    utility->QT = 0;
    /* Find max and update kalman */
    int range[3] = { l, d->centroid, 0 };
#ifdef RHO_DEBUG
    printf("Range is <%d | %d | %d>\n", range[2], range[1], range[0]);
#endif
    int x1 = l, x2 = l;
    
    for( int i = 1, j = 2; i >= 0; i--, j-- )
    {
        m = 0;
        for( x1 = range[i]; x1 > range[j]; --x1, c = d->map[x1], b1 = b->map[x1] )
        {
            c = abs( b1 - c ) ;
            d->map[x1] = c;
            if( c < d->length/8 )
            {
                utility->QT += c;
                if( c > m ) m = c;
            }
        }
        
        RhoKalman.update(&d->kalmans[i], m, 0., true);
        d->max[i] = m;
        fpeak = d->kalmans[i].value;
        fvar = d->kalmans[i].variance;
        
        if( fvar < fpeak && fvar > 0)
        {
            fbandl = fpeak - fvar;
#ifdef RHO_DEBUG
//            printf("Map(%d): max>%d | peak>%d | var>%d | bandl>%d\n", i, m[i], fpeak, fvar, fbandl);
#endif
            /* Find blob centroids and sort out the top 2 */
            for( x2 = range[i]; x2 > range[j]; --x2, c1 = d->map[x2] )
            {
                /* Punish values above the filter peak */
                if(c1 > fpeak) c1 = fpeak - RHO_PUNISH_FACTOR*(c1 - fpeak);
                
                /* Check if CMA value is in band */
                if(c1 > fbandl)
                {
                    /* De-offset valid values */
                    c2 = c1 - fbandl;
                    
                    cma_M0_M1(c2, x2, &cavg, &mavg, &count);
                    if(c2 > maxc) maxc = c2;
                    has = 1; gapc = 0;
                }
                
                /* Process completed blobs */
                else if(has && gapc > RHO_GAP_MAX)
                {
                    denc = (int)cavg;
                    if( denc > den[0] )
                    {
                        loc[1] = loc[0]; loc[0] = (int)(mavg/cavg);
                        den[1] = den[0]; den[0] = denc;
                        max[2] = max[1]; max[1] = max[0]; max[0] = maxc;
                    }
                    else if( denc > den[1] )
                    {
                        loc[1] = (int)(mavg/cavg);
                        den[1] = denc;
                        max[2] = max[1]; max[1] = maxc;
                    }
                    utility->QF += cavg;
                    mavg = 0.; cavg = 0.;
                    has = 0; count = 0; gapc = 0;
                }
                
                /* Count gaps for all invalid values */
                else gapc++;
            }
        }
        utility->FT = ZDIV((double)utility->QF,(double)utility->QT);
#ifdef RHO_DEBUG
        printf("* Filtered coverage is %.5f%%\n", utility->FT*100);
#endif
    }
#ifdef RHO_DEBUG
    printf("Blobs: [0](%d,%d,%d) | [1](%d,%d,%d)\n", loc[0], den[0], max[0], loc[1], den[1], max[1]);
#endif
//    if( !loc[0] || !loc[1] ) return;
//    if( utility->FT > 1 || utility->FT < 0 ) utility->FT = 0;
    
    /* Update prediction with best peaks */
    r->primary_new   = loc[0];
    r->secondary_new = loc[1];
    
    double Ad, Af, Pf, Sf, Qf = (double)utility->QF;
    Ad = den[0] + den[1];
    Af = ( 1 - ( Ad / Qf ) ) * ALTERNATE_TUNING_FACTOR;
    Pf = den[0]/Qf;
    Sf = den[1]/Qf;
    
    if( utility->FT > FILTERED_COVERAGE_TARGET )
    {
        double v_ = 1.0;//ZDIV(1.0,fvar);
        r->probabilities.primary   = Pf * v_;
        r->probabilities.secondary = Sf * v_;
        r->probabilities.alternate = Af * v_;
        r->probabilities.absence   = 0.;
    }
    else
    {
        r->probabilities.primary   = 0.;
        r->probabilities.secondary = 0.;
        r->probabilities.alternate = 0.;
        r->probabilities.absence   = 1.;
    }
    
#ifdef RHO_DEBUG
    printf("Rho: pri-%.3f sec-%.3f alt-%.3f\n", r->probabilities.primary, r->probabilities.secondary, r->probabilities.alternate);
//    printf("Alternate probability is %.3f\n", r->probabilities.alternate);
#endif
}

void Filter_and_Select_Pairs( rho_c_utility * utility )
{
#ifdef RHO_DEBUG
    printf("X Map:\n");
#endif
    Filter_and_Select( utility, &utility->density_map_pair.x, &utility->background_map_pair.x, &utility->prediction_pair.x );
#ifdef RHO_DEBUG
    printf("Y Map:\n");
#endif
    Filter_and_Select( utility, &utility->density_map_pair.y, &utility->background_map_pair.y, &utility->prediction_pair.y );
}

/* Correct and factor predictions from variance band filtering into global model */
void Update_Prediction( rho_c_utility * utility )
{
    int
        Ax = utility->prediction_pair.y.primary_new,
        Ay = utility->prediction_pair.x.primary_new,
        Bx = utility->prediction_pair.y.secondary_new,
        By = utility->prediction_pair.x.secondary_new;
    if(Ax > Bx) iswap(&Ax, &Bx);
    if(Ay > By) iswap(&Ay, &By);
    
    int Cx = utility->Cx, Cy = utility->Cy;
    
    bool diag = true;
    if( ( Ax < Cx ) ^ ( Bx > Cx ) ) /* X ambiguous */
    {
        double x1 = utility->prediction_pair.x.primary.value + utility->prediction_pair.x.primary.velocity;
        double p1 = fabs(x1-Ax), p2 = fabs(x1-Bx);
        if( p1 > p2 ) iswap(&Ax, &Bx);
        diag = false;
#ifdef RHO_DEBUG
        printf("X Ambiguous (%d,%d,%d)\n", Ax, Bx, Cx);
#endif
    }
    if( ( Ay < Cy ) ^ ( By > Cy ) ) /* Y ambiguous */
    {
        double y1 = utility->prediction_pair.y.primary.value + utility->prediction_pair.y.primary.velocity;
        double p1 = fabs(y1-Ay), p2 = fabs(y1-By);
        if( p1 > p2 ) iswap(&Ay, &By);
        diag = false;
#ifdef RHO_DEBUG
        printf("Y Ambiguous (%d,%d,%d)\n", Ay, By, Cy);
#endif
    }

    if ( diag )
    {
        signed char qcheck = ( utility->Qf[0] > utility->Qf[1] ) + ( utility->Qf[2] < utility->Qf[3] ) - 1;
        if( Ax > Bx ) iswap(&Ax, &Bx);
        if ( ( qcheck > 0 ) ^ ( Ay < By ) ) iswap(&Ay, &By);
#ifdef RHO_DEBUG
        printf("Quadrant Check %d\n", qcheck);
#endif
    }
    
    if( Ax ) RhoKalman.update( &utility->prediction_pair.x.primary,   Ax, 0, false );
    if( Bx ) RhoKalman.update( &utility->prediction_pair.x.secondary, Bx, 0, false );
    if( Ay ) RhoKalman.update( &utility->prediction_pair.y.primary,   Ay, 0, false );
    if( By ) RhoKalman.update( &utility->prediction_pair.y.secondary, By, 0, false );
    
    Cx = (int)((utility->prediction_pair.x.primary.value + utility->prediction_pair.x.secondary.value)) / 2;
    Cy = (int)((utility->prediction_pair.y.primary.value + utility->prediction_pair.y.secondary.value)) / 2;
    
    Cx = INR(Cx, utility->width);
    Cy = INR(Cy, utility->height);
    
    utility->Cx = Cx;
    utility->Cy = Cy;
    utility->density_map_pair.x.centroid = Cy;
    utility->density_map_pair.y.centroid = Cx;
}

/* Use background and state information to update image threshold */
void Update_Threshold( rho_c_utility * utility )
{
    int thresh = utility->thresh;
    
    /* Hard-Tune on significant background */
#ifdef RHO_DEBUG
    printf("Coverage compare: Actual>%d vs. Target>%d\n", utility->QbT, BACKGROUND_COVERAGE_MIN);
#endif
    char cov = XRNG( utility->QbT, 0, BACKGROUND_COVERAGE_TOL_PX );
    switch(cov)
    {
        case -1:
            thresh -= THRESH_STEP*2;
            break;
        case 1:
            thresh += THRESH_STEP*4;
            break;
        case 0:
        default:
            break;
    }
    
    /* Soft-Tune on State */
    switch(utility->sys.state)
    {
        case UNSTABLE_MANY:
        case STABLE_MANY:
            thresh += THRESH_STEP;
            break;
        case UNSTABLE_NONE:
            thresh -= THRESH_STEP;
        case STABLE_NONE:
            thresh -= THRESH_STEP*2;
            break;
        case UNSTABLE_SINGLE:
        case STABLE_SINGLE:
            thresh -= THRESH_STEP;
            break;
        default:
            break;
    }
    thresh = BOUND(thresh, THRESHOLD_MIN, THRESHOLD_MAX);
    if(thresh != utility->thresh)
    {
        RhoKalman.update( &utility->thresh_filter, thresh, 0., false );
        utility->thresh = utility->thresh_filter.value;
#ifdef RHO_DEBUG
        printf("*** THRESH IS %d ***\n", thresh);
#endif
    }
}

/* Rho global functions */
const struct rho_functions RhoFunctions =
{
    .Init = Init,
    .Generate_Density_Map_Using_Interrupt_Model = Generate_Density_Map_Using_Interrupt_Model,
    .Redistribute_Densities = Redistribute_Densities,
    .Filter_and_Select_Pairs = Filter_and_Select_Pairs,
    .Filter_and_Select = Filter_and_Select,
    .Update_Prediction = Update_Prediction,
    .Update_Threshold = Update_Threshold
};

