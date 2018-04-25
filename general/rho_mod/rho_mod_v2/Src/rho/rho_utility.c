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

// #define RHO_DEBUG

#define FOR(L)      for( int i = L; i > 0; --i)
#define FORA(L,A)   for( int i = L-1; i >= 0; i--, A[i] )
#define ZDIV(X,Y)   (!Y?2<<10:X/Y)

#ifdef RHO_DEBUG
#define PRINT1(A)			sprintf( out_buffer, A ); print(out_buffer);
#define PRINT2(A,B) 	sprintf( out_buffer, A, B ); print(out_buffer);
#define PRINT3(A,B,C) sprintf( out_buffer, A, B, C ); print(out_buffer);
#else
#define PRINT1(A)
#define PRINT2(A,B)
#define PRINT3(A,B,C)
#endif

#define UART_TIMEOUT 100
UART_HandleTypeDef * uart;

char out_buffer[100];

static void print(char * Buf )
{
	HAL_UART_Transmit( uart, (uint8_t *)Buf, strlen((const char *)Buf), UART_TIMEOUT );
}

void Init(rho_utility * utility, UART_HandleTypeDef * u, int w, int h)
{
	uart = u;
	print( "Initializing Rho\r\n" );

	utility->width  = w;
	utility->height = h;

	utility->density_map_pair.x.length = h;
	utility->density_map_pair.x.max = 0;
	utility->density_map_pair.x.variance = 0;
	RhoKalman.init(&utility->density_map_pair.x.kalman, 0., RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);

	utility->density_map_pair.y.length = w;
	utility->density_map_pair.y.max = 0;
	utility->density_map_pair.y.variance = 0;
	RhoKalman.init(&utility->density_map_pair.y.kalman, 0., RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);

	RhoKalman.init(&utility->prediction_pair.x.primary, 0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU);
	RhoKalman.init(&utility->prediction_pair.x.secondary, 0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU);
	RhoKalman.init(&utility->prediction_pair.y.primary, 0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU);
	RhoKalman.init(&utility->prediction_pair.y.secondary, 0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU);
	utility->prediction_pair.x.primary_new   = 0.;
	utility->prediction_pair.x.secondary_new = 0.;
	utility->prediction_pair.y.primary_new   = 0.;
	utility->prediction_pair.y.secondary_new = 0.;

	memset(&utility->prediction_pair.x.probabilities, 0, sizeof(FLOAT)*3);
	memset(&utility->prediction_pair.y.probabilities, 0, sizeof(FLOAT)*3);
}

void Find_Map_Max( density_map_t * d )
{
	uint32_t m = 0, c, l = d->length;
	/* Find max and update kalman */
	uint32_t i = l
	for( ; i < l; i++ )
	{
		c = d->map[i];
		if( c > m ) m = c;
	}
	d->max = m;
}

void Filter_and_Select( rho_utility * utility, density_map_t * d, prediction_t * r )
{
	utility->QF = 0;
	int     l       = d->length,
					count   = 0,
					gapc    = 0,
					has     = 0,
					denc    = 0,
					maxc    = 0,
					den[]   = {0,0},
					loc[]   = {0,0},
					max[]   = {0,0,0};

	FLOAT   t    		= 0.,
					v    		= 0.,
					g       = 0,
					c1      = 0,
					c2      = 0,
					cavg 		= 0.,
					mavg		= 0.;

	//PRINT2( "Max: %d\r\n", d->max );
	//PRINT2( "B:%f\r\n", d->kalman.value );
	RhoKalman.update(&d->kalman, (double)d->max, 0.);
	//PRINT2( "Value:%f\r\n", d->kalman.value );
	d->variance = v;
	t = d->kalman.value;

	FLOAT a, b, c;
	a = ( RHO_K_TARGET - d->kalman.K[0] );
	b = RHO_VARIANCE_SCALE * a;
	c = 1 + b;
	v = RHO_VARIANCE_NORMAL * c;


	PRINT2( "\tK:%f\r\n", d->kalman.K[0] );
	PRINT2( "\ta:%f\r\n", a );
	PRINT2( "\tb:%f\r\n", b );
	PRINT2( "\tc:%f\r\n", c );
	PRINT3( "\t %f(v) < %f(t)?\r\n", v, t );
	PRINT2( "Variance:%f\r\n", v );

	//v = RHO_VARIANCE_NORMAL * ( 1 + RHO_VARIANCE_SCALE * ( RHO_K_TARGET - d->kalman.K[0] ) );
	if( v < t && v > 0) g = t - v;
	else return;
	PRINT2( "g:%f\r\n", g );
	/* * * * * * * * * * * * * * */

	/* Find blob centers and sort out the top 2 */
	//FORA( l, c1 = d->map )
	for( uint32_t i = 0; i < l; i++, c1 = (double)d->map[i] )
	{
		PRINT3( "\t\t(%d)%f\r\n", i, c1 );
		if(c1 > (t+1))
		{
			c1 = t - RHO_PUNISH_FACTOR*(c1 - t);
			PRINT2( "\t\tcor>%f\r\n", c1 );
		}
		if(c1 > g)
		{
			c2 = c1 - g;
			cma_M0_M1(c2, i, &cavg, &mavg, &count);
			if(c2 > (double)maxc)
					maxc = (int)c2;
			has = 1;
			gapc = 0;
		}
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
		else gapc++;
	}
	utility->FT = utility->QF/(utility->width*utility->height*100);
	if(!utility->FT) return;

	PRINT2( "QF: %f\r\n", utility->QF);
	PRINT2( "Coverage: %f%%\r\n", utility->FT);
	PRINT3( "\t0:%d | %d\r\n", loc[0], den[0] );
	PRINT3( "\t1:%d | %d\r\n", loc[1], den[1] );


	if( !loc[0] && !loc[1] ) return;

	/* Update prediction with best peaks */
	r->primary_new   = loc[0];
	r->secondary_new = loc[1];

	FLOAT v_ = ZDIV(1.0,v);

	PRINT2( "v: %f\r\n", v);
	PRINT2( "v_: %f\r\n", v_);

	r->probabilities.primary   = ((FLOAT)max[0]) * v_;
	r->probabilities.secondary = ((FLOAT)max[1]) * v_;

	FLOAT comp = 1 - utility->FT/FILTERED_CONVERAGE_TARGET;
	if(comp < 0)
			r->probabilities.alternate = ((FLOAT)max[2]) * v_;
	else
			r->probabilities.alternate = comp;

	PRINT2( "  Primary probability: %f\r\n", r->probabilities.primary);
	PRINT2( "Secondary probability: %f\r\n", r->probabilities.secondary);
	PRINT2( "Alternate probability: %f\r\n", r->probabilities.alternate);

}

void Filter_and_Select_Pairs( rho_utility * utility )
{
	///Note: Dx max should be already calculated in density map generation.
	//print( "Processing Dx\r\n");
	Find_Map_Max( &utility->density_map_pair.x );
	Filter_and_Select( utility, &utility->density_map_pair.x, &utility->prediction_pair.x );

	//print( "Processing Dy\r\n");
	Find_Map_Max( &utility->density_map_pair.y );
	Filter_and_Select( utility, &utility->density_map_pair.y, &utility->prediction_pair.y );
}

void Update_Prediction( rho_utility * utility )
{
    int Ax = utility->prediction_pair.x.primary_new,
        Ay = utility->prediction_pair.y.primary_new,
        Bx = utility->prediction_pair.x.secondary_new,
        By = utility->prediction_pair.y.secondary_new;
    int Cx = utility->Cx, Cy = utility->Cy;

    bool nondiag = false;
    signed char qcheck = ( utility->Q[0] > utility->Q[1] ) + ( utility->Q[2] < utility->Q[3] ) - 1;
    if( ( Ax < Cx ) ^ ( Bx > Cx ) ) /* X ambiguous */
    {
        FLOAT x1 = utility->prediction_pair.x.primary.value + utility->prediction_pair.x.primary.velocity;
        FLOAT p1 = fabs(x1-Ax), p2 = fabs(x1-Bx);
        if( p1 > p2 ) iswap(&Ax, &Bx);
        nondiag = true;
#ifdef RHO_DEBUG
        printf("X Ambiguity (%d,%d,%d)\n", Ax, Bx, Cx);
#endif
    }
    if( ( Ay < Cy ) ^ ( By > Cy ) ) /* Y ambiguous */
    {
        FLOAT y1 = utility->prediction_pair.y.primary.value + utility->prediction_pair.y.primary.velocity;
        FLOAT p1 = fabs(y1-Ay), p2 = fabs(y1-By);
        if( p1 > p2 ) iswap(&Ay, &By);
        nondiag = true;
#ifdef RHO_DEBUG
        printf("Y Ambiguity (%d,%d,%d)\n", Ay, By, Cy);
#endif
    }

    if ( !nondiag )
    {
        if( Ax > Bx ) iswap(&Ax, &Bx);
        if ( ( qcheck > 0 ) ^ ( Ay < By ) ) iswap(&Ay, &By);

#ifdef RHO_DEBUG
        if( qcheck < 0 ) printf("Secondary diagonal\n");
        if( qcheck > 0 ) printf("Primary diagonal\n");
#endif
    }

    RhoKalman.update( &utility->prediction_pair.x.primary,   Ax, 0);
    RhoKalman.update( &utility->prediction_pair.x.secondary, Bx, 0);
    RhoKalman.update( &utility->prediction_pair.y.primary,   Ay, 0);
    RhoKalman.update( &utility->prediction_pair.y.secondary, By, 0);

    Cy = (int)((utility->prediction_pair.x.primary.value + utility->prediction_pair.x.secondary.value) / 2 );
    Cx = (int)((utility->prediction_pair.y.primary.value + utility->prediction_pair.y.secondary.value) / 2 );
    if( Cx < 0               ) Cx = 0;
    if( Cy < 0               ) Cy = 0;
    if( Cx > utility->width  ) Cx = utility->width;
    if( Cy > utility->height ) Cy = utility->height;
    utility->Cx = Cx;
    utility->Cy = Cy;

    /* Update State */
    BayesianSystem.update( &utility->sys, &utility->prediction_pair );
}

const struct rho_functions RhoFunctions =
{
	.Init = Init,
    .Find_Map_Max = Find_Map_Max,
	.Filter_and_Select_Pairs = Filter_and_Select_Pairs,
	.Filter_and_Select = Filter_and_Select,
	.Update_Prediction = Update_Prediction
};
