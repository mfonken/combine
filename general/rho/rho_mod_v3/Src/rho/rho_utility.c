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
#include <stdarg.h>

//#define RHO_DEBUG 0

#define ALLOW_NEGATIVE_REDISTRIBUTION

#define POW2(X) 			<<X
#define INR(X,H)     (X<0?0:((X>H)?(H-1):X))
#define MAX(A,B)     ((A>B)?A:B)
#define XRNG(A,B,C)  (A<(B-C)?-1:(A>(B+C)?1:0)) //Check if exceeds range
#define ZDIV(X,Y)    (!X?0:(!Y?2 POW2(10):(FLOAT)X/(FLOAT)Y))
#define BOUND(X,MIN,MAX) ((X<MIN)?MIN:((X>MAX)?MAX:X))
#define UDIFF(A,B)	 	 (B<A?A-B:B-A)



//#ifdef RHO_DEBUG
/*
#define PRINT1(A)			sprintf( out_buffer, A ); \
											print(out_buffer);
#define PRINT2(A,B) 	sprintf( out_buffer, A, B ); \
											print(out_buffer);
#define PRINT3(A,B,C) sprintf( out_buffer, A, B, C ); \
											print(out_buffer);
*/
/*
#else
*/
#define PRINT1(A)			;
#define PRINT2(A,B)		;
#define PRINT3(A,B,C)	;
//#endif
//*/

#define UART_TIMEOUT 100
UART_HandleTypeDef * uart;

char out_buffer[125];

static void print(const char * Buf )
{
	HAL_UART_Transmit( uart, (uint8_t *)Buf, strlen((const char *)Buf), UART_TIMEOUT );
}

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
static index_t calculateCentroid( density_t * map, index_t l, index_t * C, byte_t thresh )
{
    FLOAT avg = 0, mavg = 0;
    index_t cnt = 0, tot = 0, i;
		density_t c;
    for( i = 0; i < l; i++ )
    {
        c = map[i];
        if( c > thresh )
        {
            cma_M0_M1(c, i, &avg, &mavg, &cnt);
            tot += c;
        }
    }
    *C = (index_t)ZDIV(mavg, avg);
    return tot;
}

static void Redistribute_Densities( rho_utility * utility )
{
    uint16_t
				x0 = utility->Bx,
				x1 = utility->Cx,
				y0 = utility->By,
				y1 = utility->Cy,
        area[9];
    if( utility->Cx < utility->Bx ) { x0 = utility->Cx; x1 = utility->Bx; }
    if( utility->Cy < utility->By ) { y0 = utility->Cy; y1 = utility->By; }

    index_t
				xl[3] = {x0, x1-x0, utility->width-x1},
        yl[3] = {y0, y1-y0, utility->height-y1};

		byte_t a, b, c, d, l, l_, p, q, x, y;
    for( y = 0, p = 0; y < 3; y++ )
        for( x = 0; x < 3; x++ )
            area[p++] = xl[x]* yl[y];

    c = ( (byte_t)( utility->Cy < utility->By ) << 1 ) + (byte_t)( utility->Cx < utility->Bx );
    for( q = 0; q < 4; q++ )
    {
        d = 0, l = rlookup.config[c].length[q], l_ = rlookup.config[c].length[3-q];
        for( x = 0; x < l; x++ )
        {
            a = 0, b = 0;
            a = area[rlookup.config[c].current[q][x]];
            for( y = 0; y < l_; y++ )
                b += area[rlookup.config[c].background[rlookup.config[c].factor[q][x]][y]];
            d += ZDIV( a, b ) * utility->Qb[q];
        }
#ifndef ALLOW_NEGATIVE_REDISTRIBUTION
        if( d > utility->Q[q] ) utility->Qf[q] = 0;
        else
#endif
				utility->Qf[q] = utility->Q[q] - d;
    }
}

void Init(rho_utility * utility, index_t  w, index_t h)
{
    /* Utility frame */
    utility->width  = w;
    utility->height = h;

    /* Threshold Filter */
    RhoKalman.init(&utility->thresh_filter, 0., RHO_THRESH_LS, RHO_THRESH_VU, RHO_THRESH_BU, RHO_THRESH_SU );

    /* Density Filters */
    utility->density_map_pair.x.map = (density_t *)malloc(sizeof(density_t)*h);
    utility->density_map_pair.x.length = h;
    utility->density_map_pair.x.max[0] = 0;
    utility->density_map_pair.x.max[1] = 0;
    RhoKalman.init(&utility->density_map_pair.x.kalmans[0], 0., RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);
    RhoKalman.init(&utility->density_map_pair.x.kalmans[1], 0., RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);

    utility->density_map_pair.y.map = (density_t *)malloc(sizeof(density_t)*w);
    utility->density_map_pair.y.length = w;
    utility->density_map_pair.y.max[0] = 0;
    utility->density_map_pair.y.max[1] = 0;
    RhoKalman.init(&utility->density_map_pair.y.kalmans[0], 0., RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);
    RhoKalman.init(&utility->density_map_pair.y.kalmans[1], 0., RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);

    /* Background Maps */
    utility->density_map_pair.x.background = (density_t *)malloc(sizeof(density_t)*h);
    utility->density_map_pair.y.background = (density_t *)malloc(sizeof(density_t)*w);

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
}

void Perform( rho_utility * utility, bool background_event )
{
	if(background_event)
	{
		RhoFunctions.Generate_Background( utility );
	}
	else
	{
		RhoFunctions.Redistribute_Densities( utility );
		RhoFunctions.Filter_and_Select_Pairs( utility );
		RhoFunctions.Update_Prediction( utility );
		RhoFunctions.Update_Threshold( utility );
	}
}

/* Interrupt (Simulated Hardware-Driven) Density map generator */
void Generate_Background( rho_utility * utility )
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


/* Calculate and process data in variance band from density filter to generate predictions */
void Filter_and_Select( rho_utility * utility, density_map_t * d, prediction_t * r )
{
		rho_selection_variables _ = {d->length, 0};

    index_t
				x1 = _.len, x2 = _.len, cyc, cyc_,
				range[3] = { _.len, d->centroid, 0 };

#ifdef RHO_DEBUG
    printf("Range is <%d | %d | %d>\n", range[2], range[1], range[0]);
#endif
		/* Find max and update kalman */
    for( cyc = 1, cyc_ = 2; cyc_ > 0; cyc--, cyc_-- )
    {
        _.cmax = 0;
        for( x1 = range[cyc]; x1 > range[cyc_]; --x1, _.c1 = d->map[x1], _.b = d->background[x1] )
        {
            _.c1 = abs( _.b - _.c1 ) ;
            d->map[x1] = _.c1;
            _.tden += _.c1;
            if( _.c1 > _.cmax ) _.cmax = _.c1;
        }
        RhoKalman.update(&d->kalmans[cyc], _.cmax, 0.);
        d->max[cyc] = _.cmax;
        _.fpeak = d->kalmans[cyc].value;
        _.fvar = RHO_VARIANCE(  d->kalmans[cyc].K[0] );

				/* Check for valid variance band */
        if( _.fvar < _.fpeak && _.fvar > 0)
        {
						/* Find blobs in band - centroids, densities, and maximums */
            _.fbandl = _.fpeak - _.fvar;
#ifdef RHO_DEBUG
//            printf("Map(%d): max>%d | peak>%d | var>%d | bandl>%d\n", i, m[i], fpeak, fvar, fbandl);
#endif
            /* Find blob centroids and sort out the top 2 */
            for( x2 = range[cyc]; x2 > range[cyc_]; --x2, _.c1 = d->map[x2] )
            {
                /* Punish values above the filter peak */
                if( _.c1 > _.fpeak )
                    _.c1 = _.fpeak - RHO_PUNISH( _.c1 - _.fpeak );

                /* Check if CMA value is in band */
                if( _.c1 > _.fbandl )
                {
                    /* De-offset valid values */
                    _.c2 = _.c1 - _.fbandl;

                    cma_M0_M1(_.c2, x2, &_.cavg, &_.mavg, &_.avgc);
                    if(_.c2 > _.cmax) _.cmax = _.c2;
                    _.has = 1; _.gapc = 0;
                }
                /* Process completed blobs */
                else if(_.has && _.gapc > RHO_GAP_MAX)
                {
                    _.cden = (density_t)_.cavg;
										bool sel_ = !_.sel;
										if( _.cden > _.blobs[sel_].den )
                    {
                        if( _.cden > _.blobs[_.sel].den )
                        {
                            _.sel = sel_;
                            _.amax = _.blobs[_.sel].max;
                        }
                        _.blobs[_.sel] = (blob_t){ _.cmax, _.cden, (index_t)ZDIV(_.mavg,_.cavg) };
                    }
                    _.fden += _.cavg;
                    _.mavg = 0.; _.cavg = 0.;
                    _.has = 0; _.avgc = 0; _.gapc = 0;
                }
                /* Count gaps for all invalid values */
                else _.gapc++;
            }
        }
    }
//    if( !loc[0] || !loc[1] ) return;
//    if( utility->FT > 1 || utility->FT < 0 ) utility->FT = 0;

    /* Update prediction with best peaks */
    r->primary_new   = _.blobs[_.sel].loc;
    r->secondary_new = _.blobs[!_.sel].loc;

		_.fcov = ZDIV(_.fden, _.tden);

		/* Find coverage values */
			_.fdnf 	= ZDIV(1, _.fden);
	    _.tdnf 	= _.blobs[0].den + _.blobs[1].den;
	    _.afac	= ( 1. - ( _.tdnf * _.fdnf ) ) * ALTERNATE_TUNING_FACTOR;
	    _.pfac	= _.blobs[_.sel].den * _.fdnf;
	    _.sfac	= _.blobs[!_.sel].den * _.fdnf;

    if( _.fcov > FILTERED_COVERAGE_TARGET )
    {
        _.fvf_ = 1.0;//ZDIV( 1.0, _.fvar );
        r->probabilities.primary   = _.pfac * _.fvf_;
        r->probabilities.secondary = _.sfac * _.fvf_;
        r->probabilities.alternate = _.afac * _.fvf_;
        r->probabilities.absence   = 0.;
    }
    else
    {
        r->probabilities.primary   = 0.;
        r->probabilities.secondary = 0.;
        r->probabilities.alternate = 0.;
        r->probabilities.absence   = MAX_ABSENCE_PROBABILITY;
    }
#ifdef RHO_DEBUG
		printf("Blobs: [0](%d,%d,%d) | [1](%d,%d,%d)\n", _.loc[0], _.den[0], _.max[0], _.loc[1], _.den[1], _.max[1]);
#endif

#ifdef RHO_DEBUG
    printf("Rho: pri-%.3f sec-%.3f alt-%.3f\n", r->probabilities.primary, r->probabilities.secondary, r->probabilities.alternate);
//    printf("Alternate probability is %.3f\n", r->probabilities.alternate);
#endif
}

void Filter_and_Select_Pairs( rho_utility * utility )
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
void Update_Prediction( rho_utility * utility )
{
		FLOAT x1, y1;
    index_t
				Ax = utility->prediction_pair.y.primary_new,
        Ay = utility->prediction_pair.x.primary_new,
        Bx = utility->prediction_pair.y.secondary_new,
        By = utility->prediction_pair.x.secondary_new,
				Cx = utility->Cx,
				Cy = utility->Cy;
    if(Ax > Bx) iswap(&Ax, &Bx);
    if(Ay > By) iswap(&Ay, &By);

    bool diag = true;
    if( ( Ax < Cx ) ^ ( Bx > Cx ) ) /* X ambiguous */
    {
        x1 = utility->prediction_pair.x.primary.value + utility->prediction_pair.x.primary.velocity;
        if( fabs( x1 - Ax ) > fabs( x1 - Bx ) ) iswap( &Ax, &Bx );
        diag = false;
#ifdef RHO_DEBUG
        printf("X Ambiguous (%d,%d,%d)\n", Ax, Bx, Cx);
#endif
    }
    if( ( Ay < Cy ) ^ ( By > Cy ) ) /* Y ambiguous */
    {
        y1 = utility->prediction_pair.y.primary.value + utility->prediction_pair.y.primary.velocity;
        if( fabs( y1 - Ay ) > fabs( y1 - By ) ) 
					iswap( &Ay, &By );
        diag = false;
#ifdef RHO_DEBUG
        printf("Y Ambiguous (%d,%d,%d)\n", Ay, By, Cy);
#endif
    }

    if ( diag )
    {
        signed char qcheck = ( utility->Qf[0] > utility->Qf[1] ) + ( utility->Qf[2] < utility->Qf[3] ) - 1;
        if( ( Ax > Bx ) ^ ( ( qcheck > 0 ) ^ ( Ay < By ) ) ) iswap(&Ax, &Bx);
#ifdef RHO_DEBUG
        printf("Quadrant Check %d\n", qcheck);
#endif
    }

    if( Ax ) RhoKalman.update( &utility->prediction_pair.x.primary,   Ax, 0 );
    if( Bx ) RhoKalman.update( &utility->prediction_pair.x.secondary, Bx, 0 );
    if( Ay ) RhoKalman.update( &utility->prediction_pair.y.primary,   Ay, 0 );
    if( By ) RhoKalman.update( &utility->prediction_pair.y.secondary, By, 0 );

    Cx = (index_t)((utility->prediction_pair.x.primary.value + utility->prediction_pair.x.secondary.value)) >> 1;
    Cy = (index_t)((utility->prediction_pair.y.primary.value + utility->prediction_pair.y.secondary.value)) >> 1;

    Cx = INR(Cx, utility->width);
    Cy = INR(Cy, utility->height);

    utility->Cx = Cx;
    utility->Cy = Cy;
    utility->density_map_pair.x.centroid = Cy;
    utility->density_map_pair.y.centroid = Cx;

		BayesianSystem.update( &utility->sys, &utility->prediction_pair );
}

/* Use background and state information to update image threshold */
void Update_Threshold( rho_utility * utility )
{
    byte_t thresh = utility->thresh;

    /* Hard-Tune on significant background */
#ifdef RHO_DEBUG
    printf("Coverage compare: Actual>%d vs. Target>%d\n", utility->QbT, BACKGROUND_COVERAGE_MIN);
#endif
    char cov = XRNG( utility->QbT, 0, BACKGROUND_COVERAGE_TOL_PX );
    switch(cov)
    {
        case -1:
            thresh -= THRESH_STEP POW2(1);
            break;
        case 1:
            thresh += THRESH_STEP POW2(2);
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
            thresh -= THRESH_STEP POW2(1);
            break;
        case UNSTABLE_SINGLE:
        case STABLE_SINGLE:
            thresh -= THRESH_STEP;
            break;
        default:
            break;
    }
    thresh = BOUND(thresh, THRESH_MIN, THRESH_MAX);
    if(thresh != utility->thresh)
    {
        RhoKalman.update( &utility->thresh_filter, thresh, 0. );
        utility->thresh = utility->thresh_filter.value;
#ifdef RHO_DEBUG
        printf("*** THRESH IS %d ***\n", thresh);
#endif
    }
}

const struct rho_functions RhoFunctions =
{
	.Init = Init,
	.Perform = Perform,
	.Redistribute_Densities = Redistribute_Densities,
	.Generate_Background = Generate_Background,
	.Filter_and_Select_Pairs = Filter_and_Select_Pairs,
	.Filter_and_Select = Filter_and_Select,
	.Update_Prediction = Update_Prediction,
	.Update_Threshold = Update_Threshold
};
