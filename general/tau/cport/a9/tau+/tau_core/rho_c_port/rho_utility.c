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

#define INRU(X,H)    ((X>H)?(H-1):X)
#define INR(X,H)     (X<0?0:((X>H)?(H-1):X))
#define MAX(A,B)     ((A>B)?A:B)
#define XRNG(A,B,C)  (A<(B-C)?-1:(A>(B+C)?1:0)) //Check if exceeds range
#define ALLOW_NEGATIVE_REDISTRIBUTION
#define ZDIV(X,Y)    (!X?0:(!Y?2<<10:(floating_t)X/(floating_t)Y))
#define BOUND(X,MIN,MAX) ((X<MIN)?MIN:((X>MAX)?MAX:X))

static void cma_M0_M1( floating_t v, floating_t i, floating_t *m0, floating_t *m1, density_2d_t * n )
{
    floating_t n_=1/((floating_t)(++(*n)));
    *m0+=((v-*m0)*n_);
    *m1+=(((v*i)-*m1)*n_);
}
static void iswap( index_t *a, index_t *b ) { index_t t=(*a);*a=*b;*b=t; }

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
static index_t calculateCentroid( density_t * map, index_t l, index_t * C, register density_t thresh )
{
    floating_t avg = 0, mavg = 0;
    density_2d_t cnt = 0, tot = 0;
    for( index_t i = 0; i < l; i++ )
    {
        density_t c = map[i];
        if( c > thresh )
        {
            cma_M0_M1((floating_t)c, (floating_t)i, &avg, &mavg, &cnt);
            tot += c;
        }
    }
    *C = (index_t)(mavg/avg);
    return tot;
}

void Perform( rho_c_utility * utility, bool background_event )
{
    if(background_event)
        RhoFunctions.Generate_Background( utility );
    else
    {
        RhoFunctions.Redistribute_Densities( utility );
        RhoFunctions.Filter_and_Select_Pairs( utility );
        RhoFunctions.Update_Prediction( utility );
//        BayesianFunctions.sys.update( &utility->sys, &utility->prediction_pair );
        RhoFunctions.Update_Threshold( utility );
    }
}

/* Interrupt (Simulated Hardware-Driven) Density map generator */
void Generate_Background( rho_c_utility * utility )
{
    /* Check for insignificant background */
    if( utility->QT < BACKGROUND_COVERAGE_MIN )
    {
        memset(utility->Q, 0, sizeof(utility->Q[0])*4);
        utility->Bx = 0;
        utility->By = 0;
        utility->QbT = 0;
    }
    /* Otherwise calculate background centroids in X & Y */
    else
    {
        index_t xt = calculateCentroid( utility->density_map_pair.x.background, utility->density_map_pair.x.length, &utility->Bx, BACKGROUND_CENTROID_CALC_THRESH );
        index_t yt = calculateCentroid( utility->density_map_pair.y.background, utility->density_map_pair.y.length, &utility->By, BACKGROUND_CENTROID_CALC_THRESH );
        utility->QbT = MAX( xt, yt );
    }
}

static void Redistribute_Densities( rho_c_utility * utility )
{
    redistribution_variables _ =
    {
        { utility->Bx, abs(utility->Cx-utility->Bx), utility->width - utility->Cx  },
        { utility->By, abs(utility->Cy-utility->By), utility->height - utility->Cy },
        { 0 }, 0
    };
    if( utility->Cx < utility->Bx )
    {
        _.xl[0] = utility->Cx;
        _.xl[2] = utility->width - utility->Bx;
        _.c |= 0x01;
    }
    if( utility->Cy < utility->By )
    {
        _.yl[0] = utility->Cy;
        _.yl[2] = utility->width - utility->By;
        _.c |= 0x02;
    }
    
    while( _.y < 3 )
        for( _.x = 0; _.x < 3; )
            _.area[_.p++] = _.xl[_.x++]* _.yl[_.y++];
    
    for( ; _.q < 4; _.q++ )
    {
        _.l  = rlookup.config[_.c].length[    _.q];
        _.l_ = rlookup.config[_.c].length[3 - _.q];
        for( _.x = 0, _.b = 0; _.x < _.l; _.x++ )
        {
            _.a = _.area[rlookup.config[_.c].current[_.q][_.x]];
            for( _.y = 0; _.y < _.l_; _.y++ )
                _.b += _.area[rlookup.config[_.c].background[rlookup.config[_.c].factor[_.q][_.x]][_.y]];
            _.d += ZDIV( _.a, _.b ) * utility->Qb[_.q];
        }
#ifndef ALLOW_NEGATIVE_REDISTRIBUTION
        if( _.d > utility->Q[_.q] ) utility->Qf[_.q] = 0;
        else
#endif
            utility->Qf[_.q] = utility->Q[_.q] - _.d;
    }
}

void Init(rho_c_utility * utility, index_t w, index_t h)
{
    /* Utility frame */
    utility->width  = w;
    utility->height = h;

    /* Threshold Filter */
    RhoKalman.init(&utility->thresh_filter, 0, RHO_THRESH_LS, RHO_THRESH_VU, RHO_THRESH_BU, RHO_THRESH_SU );
    
    /* Density Filters */
    utility->density_map_pair.x.map = (density_t*)malloc(sizeof(density_t)*h);
    utility->density_map_pair.x.length = h;
    utility->density_map_pair.x.max[0] = 0;
    utility->density_map_pair.x.max[1] = 0;
    RhoKalman.init(&utility->density_map_pair.x.kalmans[0], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);
    RhoKalman.init(&utility->density_map_pair.x.kalmans[1], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);
    
    utility->density_map_pair.y.map = (density_t*)malloc(sizeof(density_t)*w);
    utility->density_map_pair.y.length = w;
    utility->density_map_pair.y.max[0] = 0;
    utility->density_map_pair.y.max[1] = 0;
    RhoKalman.init(&utility->density_map_pair.y.kalmans[0], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);
    RhoKalman.init(&utility->density_map_pair.y.kalmans[1], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);
    
    utility->density_map_pair.x.background = (density_t*)malloc(sizeof(density_t)*h);
    utility->density_map_pair.y.background = (density_t*)malloc(sizeof(density_t)*w);
    utility->density_map_pair.x.filtered   = (density_t*)malloc(sizeof(density_t)*h);
    utility->density_map_pair.y.filtered   = (density_t*)malloc(sizeof(density_t)*w);
    
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
    memset(&utility->prediction_pair.x.probabilities, 0, sizeof(floating_t)*3);
    memset(&utility->prediction_pair.y.probabilities, 0, sizeof(floating_t)*3);
    
    /***** INTERRUPT MODEL CROSS-CONNECTOR VARIABLES START *****/
    /* Connect to Interrupt Model variable structure */
    RhoVariables.ram.Dx       =  utility->density_map_pair.x.map;
    RhoVariables.ram.Dy       =  utility->density_map_pair.y.map;
    RhoVariables.ram.Q        =  utility->Q;
    RhoVariables.ram.CX_ADDR  = &utility->Cx;
    RhoVariables.ram.CY_ADDR  = &utility->Cy;
    RhoVariables.ram.C_FRAME  =  utility->cframe;
    RhoVariables.ram.THRESH_ADDR = &utility->thresh;
    RhoVariables.ram.CAM_PORT = &test_port;
    
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

/* Calculate and process data in variance band from density filter to generate predictions */
void Filter_and_Select( rho_c_utility * utility, density_map_t * d, prediction_t * r )
{
    rho_selection_variables _ = {d->length, 0};
    
    utility->QF = 0;
    utility->QT = 0;
    
    /* Find max and update kalman */
    byte_t cyc, cyc_;
    index_t x1 = _.len,
            x2 = _.len,
            range[3] = { _.len, d->centroid, 0 };
#ifdef RHO_DEBUG
//    printf("Range is <%d | %d | %d>\n", range[2], range[1], range[0]);
#endif
    
    for( cyc = 1, cyc_ = 2; cyc_ > 0; cyc--, cyc_-- )
    {
        _.cmax = 0;
        if( d->has_background )
        {
            for( x1 = range[cyc]; x1 > range[cyc_]; --x1, _.c1 = d->map[x1], _.b = d->background[x1] )
            {
                _.c1 = abs( _.c1 -_.b );
                if( _.c1 < (d->length>>2) )
                {
                    utility->QT += _.c1;
                    if( _.c1 > _.cmax ) _.cmax = _.c1;
                    d->filtered[x1] = _.c1;
                }
                else
                    d->filtered[x1] = 0;
            }
        }
        else
        {
            for( x1 = range[cyc]; x1 > range[cyc_]; --x1, _.c1 = d->map[x1] )
            {
                if( _.c1 < (d->length>>2) )
                {
                    utility->QT += _.c1;
                    if( _.c1 > _.cmax ) _.cmax = _.c1;
                    d->filtered[x1] = _.c1;
                }
                else
                    d->filtered[x1] = 0;
            }
        }
        
        RhoKalman.update(&d->kalmans[cyc], _.cmax, 0. );
        d->max[cyc] = _.cmax;
        _.fpeak = d->kalmans[cyc].value;
        _.fvar = RHO_VARIANCE( d->kalmans[cyc].K[0] );
        d->kalmans[cyc].variance = _.fvar;
        
        if( _.fpeak > _.fvar && _.fvar > 0)
        {
            _.fbandl = _.fpeak - _.fvar;
#ifdef RHO_DEBUG
//            printf("Map(%d): max>%d | peak>%d | var>%d | bandl>%d\n", i, m[i], fpeak, fvar, fbandl);
#endif
            /* Find blob centroids and sort out the top 2 */
            for( x2 = range[cyc]; x2 > range[cyc_]; --x2, _.c1 = d->filtered[x2] )
            {
                /* Punish values above the filter peak */
                if(_.c1 > _.fpeak)
                {
                    _.p = RHO_PUNISH_FACTOR*(_.c1 - _.fpeak);
                    if( _.fpeak > _.p ) _.c1 = _.fpeak - _.p;
                    else _.c1 = 0;
                }
                /* Check if CMA value is in band */
                if(_.c1 > _.fbandl)
                {
                    /* De-offset valid values */
                    _.c2 = _.c1 - _.fbandl;
        
                    /* Process new values into blob */
                    cma_M0_M1((floating_t)_.c2, (floating_t)x2, &_.cavg, &_.mavg, &_.avgc);
                    if(_.c2 > _.cmax) _.cmax = _.c2;
                    _.has = 1; _.gapc = 0;
                }
                
                /* Process completed blobs */
                else if(_.has && _.gapc > RHO_GAP_MAX)
                {
                    _.cden = (density_2d_t)_.cavg;
                    
                    bool sel_ = !_.sel;
                    if( _.cden > _.blobs[sel_].den )
                    {
                        if( _.cden > _.blobs[_.sel].den || !_.blobs[sel_].den)
                        {
                            _.sel = sel_;
                            _.amax = _.blobs[_.sel].max;
                        }
                        _.blobs[_.sel] = (blob_t){ _.cmax, _.cden, (density_2d_t)ZDIV(_.mavg,_.cavg) };
                    }
                    
                    utility->QF += _.cavg;
                    _.mavg = 0.; _.cavg = 0.;
                    _.has = 0; _.avgc = 0; _.gapc = 0;
                }
                
                /* Count gaps for all invalid values */
                else if( _.has )
                    _.gapc++;
            }
        }
    }
    
//    if( !_.blobs[0].loc || !_.blobs[1].loc ) return;
    
    utility->FT = ZDIV((floating_t)utility->QF,(floating_t)utility->QT);
#ifdef RHO_DEBUG
    printf("* %.3f%% ( %d | %d )\n", utility->FT*100, utility->QF, utility->QT);
#endif
#ifdef RHO_DEBUG
    printf("Blobs: [0](%d,%d,%d) | [1](%d,%d,%d)\n", _.blobs[_.sel].loc, _.blobs[_.sel].den, _.blobs[_.sel].max, _.blobs[!_.sel].loc, _.blobs[!_.sel].den, _.blobs[!_.sel].max);
//    printf("Blobs: [0](%d,%d,%d) | [1](%d,%d,%d)\n", _.loc[0], _.den[0], _.max[0], _.loc[1], _.den[1], _.max[1]);
#endif
    
    /* Update prediction with best peaks */
    r->primary_new   = _.blobs[_.sel].loc;
    r->secondary_new = _.blobs[!_.sel].loc;
    
    floating_t Ad, Af, Pf, Sf, Qf_ = ZDIV(1,utility->QF);
    Ad = _.blobs[0].den + _.blobs[1].den;
    Af = ( 1 - ( Ad * Qf_ ) ) * ALTERNATE_TUNING_FACTOR;
    Pf = (floating_t)_.blobs[ _.sel].den * Qf_;
    Sf = (floating_t)_.blobs[!_.sel].den * Qf_;
    
    if( utility->FT > FILTERED_COVERAGE_TARGET )
    {
        floating_t v_ = 1.0;//ZDIV(1.0,fvar);
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
        r->probabilities.absence   = MAX_ABSENCE_PROBABIILITY;
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
    Filter_and_Select( utility, &utility->density_map_pair.x, &utility->prediction_pair.x );
#ifdef RHO_DEBUG
    printf("Y Map:\n");
#endif
    Filter_and_Select( utility, &utility->density_map_pair.y, &utility->prediction_pair.y );
}

/* Correct and factor predictions from variance band filtering into global model */
void Update_Prediction( rho_c_utility * utility )
{
    prediction_update_variables _ =
    {
        utility->prediction_pair.y.primary_new,
        utility->prediction_pair.x.primary_new,
        utility->prediction_pair.y.secondary_new,
        utility->prediction_pair.x.secondary_new,
        utility->Cx,
        utility->Cy,
        0
    };
    if(_.Ax > _.Bx) iswap(&_.Ax, &_.Bx);
    if(_.Ay > _.By) iswap(&_.Ay, &_.By);
    
    if( ( _.Ax < _.Cx ) ^ ( _.Bx > _.Cx ) ) /* X ambiguous */
    {
        _.x1 = utility->prediction_pair.x.primary.value + utility->prediction_pair.x.primary.velocity;
        if( fabs( _.x1 - _.Ax ) > fabs( _.x1 - _.Bx ) ) iswap( &_.Ax, &_.Bx );
        _.non_diag = true;
#ifdef RHO_DEBUG
        printf("X Ambiguous (%d,%d,%d)\n", _.Ax, _.Bx, _.Cx);
#endif
    }
    if( ( _.Ay < _.Cy ) ^ ( _.By > _.Cy ) ) /* Y ambiguous */
    {
        _.y1 = utility->prediction_pair.y.primary.value + utility->prediction_pair.y.primary.velocity;
        if( fabs( _.y1 - _.Ay ) > fabs( _.y1 -_. By ) ) iswap( &_.Ay, &_.By );
        _.non_diag = true;
#ifdef RHO_DEBUG
        printf("Y Ambiguous (%d,%d,%d)\n", _.Ay, _.By, _.Cy);
#endif
    }
    
    if ( !_.non_diag )
    {
        _.qcheck = ( utility->Qf[0] > utility->Qf[1] ) + ( utility->Qf[2] < utility->Qf[3] ) - 1;
        if( ( _.Ax > _.Bx ) ^ ( ( _.qcheck > 0 ) ^ ( _.Ay < _.By ) ) ) iswap(&_.Ax, &_.Bx);
#ifdef RHO_DEBUG
        printf("Quadrant Check %d\n", _.qcheck);
#endif
    }
    
    if( _.Ax ) RhoKalman.update( &utility->prediction_pair.x.primary,   _.Ax, 0 );
    if( _.Bx ) RhoKalman.update( &utility->prediction_pair.x.secondary, _.Bx, 0 );
    if( _.Ay ) RhoKalman.update( &utility->prediction_pair.y.primary,   _.Ay, 0 );
    if( _.By ) RhoKalman.update( &utility->prediction_pair.y.secondary, _.By, 0 );
    
    _.Cx = (index_t)((utility->prediction_pair.x.primary.value + utility->prediction_pair.x.secondary.value)) >> 1;
    _.Cy = (index_t)((utility->prediction_pair.y.primary.value + utility->prediction_pair.y.secondary.value)) >> 1;
    
    _.Cx = INRU(_.Cx, utility->width);
    _.Cy = INRU(_.Cy, utility->height);
    
    utility->Cx = _.Cx;
    utility->Cy = _.Cy;
    utility->density_map_pair.x.centroid = _.Cy;
    utility->density_map_pair.y.centroid = _.Cx;
    
    utility->prediction_pair.probabilities.primary   = sqrt( utility->prediction_pair.x.probabilities.primary   * utility->prediction_pair.y.probabilities.primary   );
    utility->prediction_pair.probabilities.secondary = sqrt( utility->prediction_pair.x.probabilities.secondary * utility->prediction_pair.y.probabilities.secondary );
    utility->prediction_pair.probabilities.alternate = sqrt( utility->prediction_pair.x.probabilities.alternate * utility->prediction_pair.y.probabilities.alternate );
    
    BayesianFunctions.sys.update( &utility->sys, &utility->prediction_pair );
}

/* Use background and state information to update image threshold */
void Update_Threshold( rho_c_utility * utility )
{
    density_t thresh = utility->thresh;
    
    /* Hard-Tune on significant background */
#ifdef RHO_DEBUG
//    printf("Background coverage compare: Actual>%d vs. Target>%d\n", utility->QbT, BACKGROUND_COVERAGE_MIN);
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
        RhoKalman.update( &utility->thresh_filter, thresh, 0. );
        utility->thresh = utility->thresh_filter.value;
#ifdef RHO_DEBUG
//        printf("*** THRESH IS %d ***\n", thresh);
#endif
    }
}

/* Rho global functions */
const struct rho_functions RhoFunctions =
{
    .Init = Init,
    .Perform = Perform,
    .Generate_Background = Generate_Background,
    .Redistribute_Densities = Redistribute_Densities,
    .Filter_and_Select_Pairs = Filter_and_Select_Pairs,
    .Filter_and_Select = Filter_and_Select,
    .Update_Prediction = Update_Prediction,
    .Update_Threshold = Update_Threshold
};

