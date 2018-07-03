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
#else
*/
#define PRINT1(A)			;
#define PRINT2(A,B)		;
#define PRINT3(A,B,C)	;
//#endif
//*/

#define UART_TIMEOUT 100
UART_HandleTypeDef * uart;

byte_t packet_buffer[PACKET_SIZE];
char out_buffer[125];

static void print(const char * Buf )
{
	HAL_UART_Transmit( uart, (uint8_t *)Buf, strlen((const char *)Buf), UART_TIMEOUT );
}

/* Quadrant density redistribution lookup table */
const density_redistribution_lookup_t rlookup =
{{{{{0,1,3,4},{2,5},{6,7},{8}},{{0},{1,2},{3,6},{4,5,7,8}},{{0,1,2,3},{1,3},{2,3},{3}},{4,2,2,1}},{{{0,3},{1,2,4,5},{6},{7,8}},{{0,1},{2},{3,4,6,7},{5,8}},{{0,2},{0,1,2,3},{2},{2,3}},{2,4,1,2}},{{{0,1},{2},{3,4,6,7},{5,8}},{{0,3},{1,2,4,5},{6},{7,8}},{{0,1},{1},{0,1,2,3},{1,3}},{2,1,4,2}},{{{0},{1,2},{3,6},{4,5,7,8}},{{0,1,3,4},{2,5},{6,7},{8}},{{0},{0,1},{0,2},{0,1,2,3}},{1,2,2,4}}}};

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
	memset(&utility->prediction_pair.x.probabilities, 0, sizeof(FLOAT)*3);
	memset(&utility->prediction_pair.y.probabilities, 0, sizeof(FLOAT)*3);

	utility->packet.data 		 				= PACKET_HEADER_ID;
	utility->packet.header.includes = PACKET_INCLUDES;
	memset(utility->packet.data, 0, sizeof(packet_offset_lookup_t));
	packet_offset_lookup 						= PACKET_OFFSETS;
	packet_address_lookup = PACKET_ADDRESS_INITIALIZER(utility->prediction_pair);
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

/* Generic centroid and mass calculator */
static index_t calculateCentroid( density_t * map, index_t l, index_t * C, byte_t thresh )
{
	centroid_calculation_variables _ = { 0 };
	for( _.i; _.i < l; _.i++ )
	{
		_.c = map[_.i];
		if( _.c > thresh )
		{
			cma_M0_M1(_.c, _.i, &_.avg, &_.mavg, &_.cnt);
			_.tot += _.c;
		}
	}
	*C = (index_t)ZDIV(_.mavg, _.avg);
	return _.tot;
}

void Redistribute_Densities( rho_utility * utility )
{
	redistribution_variables _ =
	{
		{ utility->Bx, abs(utility->Cx-utility->Bx), utility->width - utility->Cx  },
		{ utility->By, abs(utility->Cy-utility->By), utility->height - utility->Cy },
		{ 0 }, 0
	}
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
	rho_selection_variables _ =
	{
		d->length, /* len, */
		{ d->length, d->centroid, 0 }, /* range[3] */
		1, 2,	/* cyc, cyc_ */
		0
	};
#ifdef RHO_DEBUG
	printf("Range is <%d | %d | %d>\n", _.range[2], _.range[1], _.range[0]);
#endif
	for( ; _.cyc_ > 0; _.cyc--, _.cyc_-- ) /* Find max and update kalman */
	{
		_.cmax = 0;
		for( _.x1 = _.range[_.cyc]; _.x1 > _.range[_.cyc_]; --_.x1, _.c1 = d->map[_.x1], _.b = d->background[_.x1] )
		{
			_.c1 = abs( _.b - _.c1 ) ;
			d->map[_.x1] = _.c1;
			_.tden += _.c1;
			if( _.c1 > _.cmax ) _.cmax = _.c1;
		}
		RhoKalman.update(&d->kalmans[_.cyc], _.cmax, 0.);
		d->max[_.cyc] = _.cmax;
		_.fpeak = d->kalmans[_.cyc].value;
		_.fvar = RHO_VARIANCE(  d->kalmans[_.cyc].K[0] );

		if( _.fvar < _.fpeak && _.fvar > 0) /* Check for valid variance band */
		{
			_.fbandl = _.fpeak - _.fvar; /* Find blobs in band - centroids, densities, and maximums */
#ifdef RHO_DEBUG
			// printf("Map(%d): max>%d | peak>%d | var>%d | bandl>%d\n", i, m[i], fpeak, fvar, fbandl);
#endif
			/* Find blob centroids and sort out the top 2 */
			for( _.x2 = _.range[_.cyc]; _.x2 > _.range[_.cyc_]; --_.x2, _.c1 = d->map[_.x2] )
			{
				if( _.c1 > _.fpeak ) /* Punish values above the filter peak */
					_.c1 = _.fpeak - RHO_PUNISH( _.c1 - _.fpeak );
				if( _.c1 > _.fbandl ) /* Check if CMA value is in band */
				{
					_.c2 = _.c1 - _.fbandl; /* De-offset valid values */
					cma_M0_M1(_.c2, _.x2, &_.cavg, &_.mavg, &_.avgc);
					if(_.c2 > _.cmax) _.cmax = _.c2;
						_.has = 1; _.gapc = 0;
				}
				else if(_.has && _.gapc > RHO_GAP_MAX) /* Process completed blobs */
				{
					_.cden = (density_t)_.cavg;
					if( _.cden > _.blobs[!_.sel].den )
					{
						if( _.cden > _.blobs[_.sel].den )
						{
							_.sel = !_.sel;
							_.amax = _.blobs[_.sel].max;
						}
						_.blobs[_.sel] = (blob_t){ _.cmax, _.cden, (index_t)ZDIV(_.mavg,_.cavg) };
					}
					_.fden += _.cavg;
					_.mavg = 0.; _.cavg = 0.;
					_.has = 0; _.avgc = 0; _.gapc = 0;
				}
				else _.gapc++; /* Count gaps for all invalid values */
			}
		}
	}
	//    if( !loc[0] || !loc[1] ) return;
	//    if( utility->FT > 1 || utility->FT < 0 ) utility->FT = 0;

	/* Update prediction with best peaks */
	r->primary_new   = _.blobs[_.sel].loc;
	r->secondary_new = _.blobs[!_.sel].loc;

	/* Find coverage values */
	_.fcov = ZDIV(_.fden, _.tden);
	_.fdnf = ZDIV(     1, _.fden);
	_.tdnf = _.blobs[0].den + _.blobs[1].den;
	_.pfac = _.blobs[ _.sel].den * _.fdnf;
	_.sfac = _.blobs[!_.sel].den * _.fdnf;
	_.afac = ( 1. - ( _.tdnf * _.fdnf ) ) * ALTERNATE_TUNING_FACTOR;

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
	preduction_update_variables _ =
	{
		utility->prediction_pair.y.primary_new,
		utility->prediction_pair.x.primary_new,
		utility->prediction_pair.y.secondary_new,
		utility->prediction_pair.x.secondary_new,
		utility->Cx,
		utility->Cy,
		0
	}
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

	_.Cx = INR(_.Cx, utility->width);
	_.Cy = INR(_.Cy, utility->height);

	utility->Cx = _.Cx;
	utility->Cy = _.Cy;
	utility->density_map_pair.x.centroid = _.Cy;
	utility->density_map_pair.y.centroid = _.Cx;

	utility->prediction_pair.probabilities.primary 	 = sqrt( utility->prediction_pair.x.primary   * utility->prediction_pair.y.primary   );
	utility->prediction_pair.probabilities.secondary = sqrt( utility->prediction_pair.x.secondary * utility->prediction_pair.y.secondary );
	utility->prediction_pair.probabilities.alternate = sqrt( utility->prediction_pair.x.alternate * utility->prediction_pair.y.alternate );

	BayesianFunctions.sys.update( &utility->sys, &utility->prediction_pair );
}

/* Use background and state information to update image threshold */
void Update_Threshold( rho_utility * utility )
{
	byte_t thresh = utility->thresh;

	/* Hard-Tune on significant background */
#ifdef RHO_DEBUG
	printf("Coverage compare: Actual>%d vs. Target>%d\n", utility->QbT, BACKGROUND_COVERAGE_MIN);
#endif
	switch( XRNG( utility->QbT, 0, BACKGROUND_COVERAGE_TOL_PX ) )
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

void Generate_Packet( rho_utility * utility )
{
	packet_generation_variables _ =
	{
		&utility->packet,
		(address_t)&packet->data,
		(address_t)&packet_offset_lookup,
		(address_t*)&packet_value_lookup,
	};
	packet->timestamp = timestamp();
	while( _.i++ < NUM_PACKET_ELEMENTS )
	{
		if( _.includes & 1 )
		{
			if(!_.t) _.l = (*(packing_template_t*)_.llPtr).a;
			else 		 _.l = (*(packing_template_t*)_.llPtr).b;
			for( _.j = 0; _.j < _.l; _.j++)
				((byte_t*)_.pdPtr)[_.j] = *(((byte_t*)*_.alPtr)+_.j);
			_.pdPtr += _.l;
		}
		_.alPtr++;
		_.includes >>= 1;
		if((_.t=!_.t )) _.llPtr++;
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
	.Update_Threshold = Update_Threshold,
	.Generate_Packet = Generate_Packet
};
