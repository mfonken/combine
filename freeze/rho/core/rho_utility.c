//
//  rho_utility.c
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_utility.h"
#include "rho_master.h"

static packet_offset_lookup_t packet_offset_lookup = PACKET_OFFSETS;
//static floating_t timestamp(void){struct timeval stamp; gettimeofday(&stamp, NULL); return stamp.tv_sec + stamp.tv_usec/1000000.0;}

static void cma_M0_M1( floating_t v, floating_t i, floating_t *m0, floating_t *m1, floating_t *n )
{
    floating_t n_=1/((floating_t)(++(*n)));
    *m0+=((v-*m0)*n_);
    *m1+=(((v*i)-*m1)*n_);
}
static void iswap( index_t *a, index_t *b ) { index_t t=(*a);*a=*b;*b=t; }

static inline void CalculateBlobScore( blob_t *b, density_t total_density, byte_t peak )
{
floating_t
    delta_d = ( (floating_t)b->den / (floating_t)total_density) - 0.5,
    delta_p = (floating_t)peak / (floating_t)b->max;
    b->scr = ( delta_d * delta_d ) + ( delta_p * delta_p );
}

static void printPacket( packet_t * p, int l )
{
    printf("Packet Size - %lubytes\n", sizeof(packet_t));
    for(int i = 0; i < sizeof(packet_t); )
    {
        printf("(%02d)", i);
        for( int j = 0; j < l && i < sizeof(packet_t); j++, i++ )
            printf(" 0x%02x", *(byte_t*)(&((byte_t*)&p->header)[i]));
        printf("\n");
    }
    uint32_t px = *(uint32_t*)p->data;
    floating_t pxf = *(floating_t*)p->data;
    uint8_t * ptr = (uint8_t*)&px;
    
    uint8_t pxr[4] = {ptr[3],ptr[2],ptr[1],ptr[0]};
    floating_t pxfr = *(floating_t*)&pxr;
    printf("{%02x}{%02x}{%02x}{%02x} %f:%f\n",ptr[0],ptr[1],ptr[2],ptr[3],pxf,pxfr);
}

#ifdef USE_INTERRUPT_MODEL
/* Universal Port for interrupt model */
pixel_base_t test_port = 0;
#endif

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
static floating_t CalculateCentroid( density_t * map, index_t l, index_t * C, register density_t thresh )
{
    floating_t avg = 0, mavg = 0, cnt = 0, tot = 0;
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

void InitRhoUtility( rho_utility * utility )
{
    /* Utility frame */
    index_t w = utility->Width;
    index_t h = utility->Height;
    
    /* Threshold Filter */
    RhoPID.Init(&utility->ThreshFilter, &(rho_pid_gain_t)DEFAULT_PID_GAIN );
    
    /* Coverage Filter */
    utility->TargetCoverageFactor  = (floating_t)FILTERED_COVERAGE_TARGET;
    RhoKalman.Init(&utility->TargetFilter, utility->TargetCoverageFactor, RHO_TARGET_LS, RHO_TARGET_VU, RHO_TARGET_BU, RHO_TARGET_SU );
    
    /* Density Filters */
    utility->DensityMapPair.x.map = (density_t*)malloc(sizeof(density_t)*h);
    utility->DensityMapPair.x.length = h;
    utility->DensityMapPair.x.max[0] = 0;
    utility->DensityMapPair.x.max[1] = 0;
    RhoKalman.Init(&utility->DensityMapPair.x.kalmans[0], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);
    RhoKalman.Init(&utility->DensityMapPair.x.kalmans[1], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);
    
    utility->DensityMapPair.y.map = (density_t*)malloc(sizeof(density_t)*w);
    utility->DensityMapPair.y.length = w;
    utility->DensityMapPair.y.max[0] = 0;
    utility->DensityMapPair.y.max[1] = 0;
    RhoKalman.Init(&utility->DensityMapPair.y.kalmans[0], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);
    RhoKalman.Init(&utility->DensityMapPair.y.kalmans[1], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU);
    
    utility->DensityMapPair.x.background = (density_t*)malloc(sizeof(density_t)*h);
    utility->DensityMapPair.y.background = (density_t*)malloc(sizeof(density_t)*w);
    utility->DensityMapPair.x.filtered   = (density_t*)malloc(sizeof(density_t)*h);
    utility->DensityMapPair.y.filtered   = (density_t*)malloc(sizeof(density_t)*w);
    
    /* Prediction Filters */
    for(uint8_t i = 0; i < MAX_TRACKING_FILTERS; i++)
    {
        RhoKalman.Init(&utility->PredictionPair.x.TrackingFilters[i], 0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU);
        RhoKalman.Init(&utility->PredictionPair.x.TrackingFilters[i], 0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU);
        utility->PredictionPair.x.TrackingFiltersOrder[i] = i;
        utility->PredictionPair.y.TrackingFiltersOrder[i] = i;
    }
    
    for(uint8_t i = 0; i < MAX_BLOBS; i++)
    {
        memset(&utility->PredictionPair.x.Blobs[i], 0, sizeof(blob_t));
        memset(&utility->PredictionPair.y.Blobs[i], 0, sizeof(blob_t));
        utility->PredictionPair.x.BlobsOrder[i] = i;
        utility->PredictionPair.y.BlobsOrder[i] = i;
    }
        
//    utility->PredictionPair.x.primary_new   = w/2;
//    utility->PredictionPair.x.secondary_new = w/2;
//    utility->PredictionPair.y.primary_new   = h/2;
//    utility->PredictionPair.y.secondary_new = h/2;
    
    /* Prediction probabilities */
    memset(&utility->PredictionPair.x.Probabilities, 0, sizeof(floating_t)*4);
    memset(&utility->PredictionPair.y.Probabilities, 0, sizeof(floating_t)*4);
    
    utility->Packet.header.ID       = PACKET_HEADER_ID;
    utility->Packet.header.includes = PACKET_INCLUDES;
    memset(utility->Packet.data, 0, sizeof(packet_offset_lookup_t));
    
    utility->BackgroundCounter = 0;
    utility->BackgroundPeriod = BACKGROUND_PERIOD;
}

void PerformRhoUtility( rho_utility * utility, bool background_event )
{
    if(background_event)
        RhoFunctions.GenerateBackground( utility );
    else
    {
        LOG_RHO("Filtering and selecting pairs.\n");
        RhoFunctions.FilterAndSelectPairs( utility );
        LOG_RHO("Updating predictions.\n");
        RhoFunctions.UpdatePredictions( utility );
        LOG_RHO("Updating threshold.\n");
        RhoFunctions.UpdateThreshold( utility );
        LOG_RHO("Generating packets.\n");
        RhoFunctions.GeneratePacket( utility );
    }
}

/* Calculate and process data in variance band from density filter to generate predictions */
void FilterAndSelectRhoUtility( rho_utility * utility, density_map_t * d, prediction_t * r )
{
    memset(&r->Probabilities, 0, sizeof(floating_t)*4);
    memset(&r->Probabilities, 0, sizeof(floating_t)*4);
    
    rho_selection_variables _ = {d->length, 0};
    
    utility->FilteredCoverage = 0;
    utility->TotalCoverage = 0;
    
    /* Find max and update kalman */
    
    index_t range[3] = { _.len, d->centroid, 0 };
    LOG_RHO("Range is <%d | %d | %d>\n", range[2], range[1], range[0]);
    
    DUAL_FILTER_CYCLE(cyc)
    {
        { /* Update Peak Filter - START */
            RhoKalman.Step(&d->kalmans[cyc], r->PreviousPeak, 0. );
            d->max[cyc] = _.cmax;
            _.fpeak = (index_t)d->kalmans[cyc].value;
            _.fpeak_2 = _.fpeak << 1;
            _.fvar = (index_t)RHO_VARIANCE( d->kalmans[cyc].K[0] );
            d->kalmans[cyc].variance = _.fvar;
        } /* Update Peak Filter - END */
        
        LOG_RHO("Map(%d): max>%d\n", cyc, _.cmax);
        
        { /* Analyze band - START */
            if( _.fvar > 0 && _.fpeak > _.fvar )
            {
                _.fbandl = _.fpeak - _.fvar;
                index_t start = range[cyc], end = range[cyc_];
                
                bool recalculate = false;
                do
                {
                    { /* Detect blobs - START */
                        /* Find blob centroids and sort out the top two */
                        BOUNDED_CYCLE_DUAL(start, end, x, _.c1, d->map, _.b, d->background)
                        {
                            _.c1 = abs( (int)_.c1 - (int)_.b );
                            if( _.c1 > _.cmax )  _.cmax = _.c1;
                            
                            /* Punish values above the filter peak */
                            if( _.c1 > _.fpeak_2 ) _.c1 = _.fpeak_2 - _.c1;
                            
                            /* Check if CMA value is in band */
                            if( _.c1 > _.fbandl )
                            {
                                /* De-offset valid values */
                                _.c2 = _.c1 - _.fbandl;
                                
                                /* Process new values into blob */
                                cma_M0_M1((floating_t)_.c2, (floating_t)x, &_.cavg, &_.mavg, &_.avgc);
                                
                                /* Update max */
                                if(_.c2 > _.cmax) _.cmax = _.c2;
                                
                                _.width++;
                                
                                /* Reset flag and counter */
                                _.has = 1; _.gapc = 0;
                            }
                            
                            /* Process completed blobs and increment count */
                            else if( !(_.width = 0) && ++_.gapc > RHO_GAP_MAX && _.has )
                            {
                                /* Get latest blob centroid */
                                _.cden = (density_2d_t)_.cavg;
                                _.fden += _.cden;
                                
                                /* Check if new blob is dense enough to be saved */
                                if( r->Blobs[_.blbi].den > MIN_BLOB_DENSITY)
                                {
                                    /* Create new blob at secondary */
                                    index_t loc = (index_t)ZDIV(_.mavg,_.cavg);
                                    r->Blobs[_.blbf++] = (blob_t){ d->map[loc], _.cden, loc, _.width };
                                    if(_.resc) r->BlobsOrder[_.blbi] = 0;
                                }
                                
                                /* Reset variables */
                                _.mavg = 0.; _.cavg = 0.; _.avgc = 0.;
                                _.has = 0; _.gapc = 0;
                            }
                        }
                    } /* Detect blobs - END */
                    
                    { /* Calculate Chaos - START */
                        _.pkp = (floating_t)r->PreviousPeak / (floating_t)_.cmax; // density percent
                        _.denp = (floating_t)r->PreviousDensity / (floating_t)_.fden; // peak percent
                        _.chaos = (density_t)( fabs( 1 - _.denp ) );
                        /* Assume no recalculations are needed */
                        recalculate = false;
                    } /* Calculate Chaos - END */
                    { /* Score blobs - START */
                        /* Return on empty frames */
                        if(_.fden == 0) return;
                        
                        /* Cycle blobs */
                        for(uint8_t i = 0; i < _.blbf; i++)
                        {
                            /* Get blob at index from order array and check validity */
                            uint8_t j = r->BlobsOrder[i];
                            if(j == MAX_BLOBS) continue;
                            blob_t curr = r->Blobs[j];
                            
                            /* Score current blob */
                            CalculateBlobScore(&curr, _.fden, _.cmax);
                            
                            /* Recalculate blobs with chaos */
                            if(curr.scr > _.chaos)
                            {
                                /* Recalculate around chaotic blob */
                                recalculate = true;
                                
                                /* Remove this index */
                                r->BlobsOrder[i] = MAX_BLOBS;
                                
                                /* Get centroid peak */
                                sdensity_t cpeak = (sdensity_t)d->map[curr.loc];
                                
                                /* Raise lower band half of blob centroid to band center when below */
                                if( cpeak < _.fpeak)
                                    _.fbandl += abs( (sdensity_t)_.fpeak - cpeak) >> 1;
                                /* Otherwise raise to centroid peak */
                                else
                                    _.fbandl = cpeak;
                                
                                /* Recalculate cycle width */
                                index_t w_2 = curr.wth >> 1;
                                if(curr.loc > w_2)
                                    start = curr.loc - w_2;
                                else start = 0;
                                if(curr.loc + w_2 < range[cyc_])
                                    end = curr.loc + w_2;
                                else end = range[cyc_];
                            }
                        }
                    } /* Score blobs - END */
                } while( recalculate );
                { /* Sort blobs - START */
                    uint8_t num_poor = 0;
                    for(uint8_t i = 0, j = 0; i < _.blbf; i++)
                    {
                        for(j = 0; j < _.blbf && r->Blobs[j].srt; j++);
                        floating_t min = r->Blobs[i].scr;
                        for(uint8_t k = 0; k < _.blbf; k++)
                        {
                            blob_t *curr = &r->Blobs[k];
                            if( curr->scr < min && !curr->srt )
                            {
                                min = curr->scr;
                                curr->srt = 1;
                                j = k;
                            }
                        }
                        if(r->Blobs[j].scr > MIN_BLOB_SCORE)
                            r->BlobsOrder[i-num_poor] = j;
                        else num_poor++;
                    }
                    /* Remove all unused blobs order indeces */
                    for(uint8_t i = _.blbf-num_poor; i < MAX_BLOBS; i++) r->BlobsOrder[i] = -1;
                } /* Sort blobs - END */
            } /* Analyze band - END */
        }
        r->NumBlobs = _.blbf;
        r->PreviousPeak = _.cmax;
        r->NuBlobs = _.denp * _.pkp * (floating_t)_.blbf;
    }
}

void GenerateRhoUtilityBackground( rho_utility * utility )
{
    floating_t xt = CalculateCentroid( utility->DensityMapPair.x.background, utility->DensityMapPair.x.length, &utility->Bx, BACKGROUND_CENTROID_CALC_THRESH );
    floating_t yt = CalculateCentroid( utility->DensityMapPair.y.background, utility->DensityMapPair.y.length, &utility->By, BACKGROUND_CENTROID_CALC_THRESH );
    LOG_RHO("^^^Total X density is %.2f & Total Y density is %.2f^^^\n", xt, yt);
    utility->QbT = MAX(xt, yt);
}

void RedistributeRhoUtilityDensities( rho_utility * utility )
{
    redistribution_variables _ =
    {
        { utility->Bx, abs(utility->Cx-utility->Bx), utility->Width - utility->Cx  },
        { utility->By, abs(utility->Cy-utility->By), utility->Height - utility->Cy },
        { 0 }, 0
    };
    if( utility->Cx < utility->Bx )
    {
        _.xl[0] = utility->Cx;
        _.xl[2] = utility->Width - utility->Bx;
        _.c |= 0x01;
    }
    if( utility->Cy < utility->By )
    {
        _.yl[0] = utility->Cy;
        _.yl[2] = utility->Width - utility->By;
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

void FilterAndSelectRhoUtilityPairs( rho_utility * utility )
{
    LOG_RHO("X Map:\n");
    RhoFunctions.FilterAndSelect( utility, &utility->DensityMapPair.x, &utility->PredictionPair.x );
    LOG_RHO("Y Map:\n");
    RhoFunctions.FilterAndSelect( utility, &utility->DensityMapPair.y, &utility->PredictionPair.y );
}

/* Correct and factor predictions from variance band filtering into global model */
void UpdateRhoUtilityPrediction( rho_utility * utility, prediction_t * r )
{
    /* Step predictions of all Kalmans */
    uint8_t valid_tracks = 0;
    for( valid_tracks = 0; valid_tracks < MAX_TRACKING_FILTERS; valid_tracks++ )
    {
        uint8_t index = r->TrackingFiltersOrder[valid_tracks];
        rho_kalman_t *curr = &r->TrackingFilters[index];
        if(RhoKalman.IsExpired(curr)) break;
        RhoKalman.Predict(curr, 0.);
    }
    
    /* Match blobs to Kalmans */
    floating_t A, B;
    for( uint8_t m = 0; m < valid_tracks; m += 2 )
    {
        for( uint8_t n = 0; n < r->NumBlobs; n += 2 )
        {
            rho_kalman_t
            *filterA = &r->TrackingFilters[m],
            *filterB = &r->TrackingFilters[m+1];
            floating_t
            blobA = (floating_t)r->Blobs[n].loc,
            blobB = (floating_t)r->Blobs[n+1].loc;
            
            A = fabs(filterA->value - blobA) * fabs(filterB->value - blobB);
            B = fabs(filterA->value - blobB) * fabs(filterB->value - blobA);
            if( A < B )
            {
                floating_t temp = blobA;
                blobA = blobB;
                blobB = temp;
            }
            RhoKalman.Update(filterA, blobA);
            RhoKalman.Update(filterB, blobB);
        }
    }
    
    /* Sort Kalmans by value */
    uint8_t old_order[MAX_TRACKING_FILTERS];
    for( uint8_t k = 0; k < MAX_TRACKING_FILTERS; k++ )
    {
        old_order[k] = r->TrackingFiltersOrder[k];
        r->TrackingFiltersOrder[k] = 0;
    }
    floating_t max = 0., curr = 0.;
    uint8_t i = 0, k = 0;
    for( ; k < valid_tracks; k++ )
    {
        max = RhoKalman.Score(&r->TrackingFilters[k]);
        i = k;
        for( uint8_t l = 0; l < valid_tracks; l++ )
        {
            if(k == l) continue;
            curr = RhoKalman.Score(&r->TrackingFilters[l]);
            if( curr > max )
            {
                max = curr;
                i = l;
            }
        }
        r->TrackingFiltersOrder[k] = i;
    }
    for( ; k < r->NumBlobs && k < MAX_TRACKING_FILTERS; k++ )
    {
        r->TrackingFilters[k].value = r->Blobs[k].loc;
    }
    for( ; k < MAX_TRACKING_FILTERS; k++ )
    {
        RhoKalman.Punish(&r->TrackingFilters[k]); /// TODO: Add punish
    }
}

void UpdateRhoUtilityPredictions( rho_utility * utility )
{
    UpdateRhoUtilityPrediction( utility, &utility->PredictionPair.x );
    UpdateRhoUtilityPrediction( utility, &utility->PredictionPair.y );
    
    prediction_update_variables _ =
    {
        utility->PredictionPair.y.TrackingFilters[0].value,
        utility->PredictionPair.x.TrackingFilters[0].value,
        utility->PredictionPair.y.TrackingFilters[1].value,
        utility->PredictionPair.x.TrackingFilters[1].value,
        utility->Cx,
        utility->Cy,
        0
    };
    if(_.Ax > _.Bx) iswap(&_.Ax, &_.Bx);
    if(_.Ay > _.By) iswap(&_.Ay, &_.By);
    
//    if( ( _.Ax < _.Cx ) ^ ( _.Bx > _.Cx ) ) /* ! X ambiguous */
//    {
//        _.x = utility->PredictionPair.x.primary.value + utility->PredictionPair.x.primary.velocity;
//        if( fabs( _.x - _.Ax ) > fabs( _.x - _.Bx ) ) iswap( &_.Ax, &_.Bx );
//        _.non_diag = true;
//        LOG_RHO("X Ambiguous (%d < %d > %d)\n", _.Ax, _.Cx, _.Bx);
//    }
//    if( ( _.Ay < _.Cy ) ^ ( _.By > _.Cy ) ) /* ! Y ambiguous */
//    {
//        _.y = utility->PredictionPair.y.primary.value + utility->PredictionPair.y.primary.velocity;
//        if( fabs( _.y - _.Ay ) > fabs( _.y -_. By ) ) iswap( &_.Ay, &_.By );
//        _.non_diag = true;
//        LOG_RHO("Y Ambiguous (%d < %d > %d)\n", _.Ay, _.Cy, _.By);
//    }
//    if ( !_.non_diag )
//    {
        //        printf("Rho: Redistributing densities.\n");
        RhoFunctions.RedistributeDensities( utility );
        _.qcheck = (  utility->Qf[0] > utility->Qf[1] ) + ( utility->Qf[2] < utility->Qf[3] ) - 1;
        if( ( _.Ax > _.Bx ) ^ ( ( _.qcheck > 0 ) ^ ( _.Ay < _.By ) ) ) iswap(&_.Ax, &_.Bx);
        LOG_RHO("Quadrant Check %d\n", _.qcheck);
//    }
    
    _.Cx = (index_t)(_.Ax + _.Bx) >> 1;
    _.Cy = (index_t)(_.Ay + _.By) >> 1;
    
    _.Cx = BOUNDU(_.Cx, utility->Width );
    _.Cy = BOUNDU(_.Cy, utility->Height);
    
    utility->Cx = _.Cx;
    utility->Cy = _.Cy;
    LOG_RHO("Cx>%d | Cy>%d\n", _.Cx, _.Cy);
    utility->DensityMapPair.x.centroid = _.Cy;
    utility->DensityMapPair.y.centroid = _.Cx;

    floating_t MaxNu = MAX(utility->PredictionPair.x.NuBlobs, utility->PredictionPair.x.NuBlobs);
    floating_t curr_CDF, prev_CDF = 0.;
    for( uint8_t i = 0; i <= NUM_STATE_GROUPS; i++ )
    {
        floating_t x = (floating_t)i / (floating_t)((uint8_t)NUM_STATE_GROUPS);
        curr_CDF = 1 - pow( 1 - pow( x, MaxNu ), NUM_STATE_GROUPS);
        if( i > 0 ) utility->PredictionPair.Probabilities.P[i-1] = curr_CDF - prev_CDF;
        prev_CDF = curr_CDF;
    }

    BayesianFunctions.Sys.Update( &utility->BayeSys, &utility->PredictionPair );
}

/* Use background and state information to update image threshold */
void UpdateRhoUtilityThreshold( rho_utility * utility )
{
    /* Hard-Tune on significant background */
    floating_t  hard_tune_factor = 0.;
    if( utility->QbT > BACKGROUND_COVERAGE_MIN )
    {
        floating_t background_coverage_factor = 1 - ZDIV( BACKGROUND_COVERAGE_MIN, utility->QbT );
        hard_tune_factor = THRESH_STEP_MAX*pow( BOUND(background_coverage_factor, -1, 1), 3);
        hard_tune_factor *= HARD_TUNING_PRESIDENCE*utility->CoverageFactor;
    }

    /* Soft-Tune on State */
    floating_t soft_tune_factor = SOFT_TUNING_PRESIDENCE*THRESH_STEP_BASE;
    switch(utility->BayeSys.state)
    {
        case UNSTABLE_NONE:
            soft_tune_factor *= 2;
        case UNSTABLE_SINGLE:
            soft_tune_factor *= utility->VarianceFactor;
            break;
        case STABLE_NONE:
            soft_tune_factor *= 2;
         case STABLE_SINGLE:
            soft_tune_factor *= utility->CoverageFactor;
            break;
        case STABLE_DOUBLE:
//            RhoPID
            RhoKalman.Step( &utility->TargetFilter, utility->FilteredPercentage, 0. );
            utility->TargetCoverageFactor = utility->TargetFilter.value;
            LOG_RHO("*** COVERAGE TARGET IS %.3f ***\n", utility->target_coverage_factor);
            soft_tune_factor = 0.;
            break;
        case UNSTABLE_MANY:
            soft_tune_factor *= -utility->VarianceFactor;
            break;
        case STABLE_MANY:
            soft_tune_factor *= -utility->CoverageFactor;
            break;
        default:
            break;
    }
    
    utility->TargetCoverageFactor -= 0.01*(utility->TargetCoverageFactor - (floating_t)FILTERED_COVERAGE_TARGET);
    floating_t proposed_thresh = *utility->Thresh + hard_tune_factor + soft_tune_factor;
    RhoPID.Update( &utility->ThreshFilter, proposed_thresh, *utility->Thresh);
    *utility->Thresh = (byte_t)BOUND(utility->ThreshFilter.Value, THRESH_MIN, THRESH_MAX);
    
    LOG_RHO("*** THRESH IS %d ***\n", utility->thresh);
    LOG_RHO("Hard factor is %.3f & soft factor is %.3f [c%.3f|v%.3f]\n", hard_tune_factor, soft_tune_factor, utility->coverage_factor, utility->variance_factor);
    LOG_RHO("Background coverage compare: Actual>%dpx vs. Target>%d±%dpx\n", utility->QbT, BACKGROUND_COVERAGE_MIN, BACKGROUND_COVERAGE_TOL_PX);
}

void GenerateRhoUtilityPacket( rho_utility * utility )
{
    packet_value_lookup_t  packet_value_lookup  = PACKET_ADDRESS_INITIALIZER(utility->PredictionPair);
    packet_generation_variables _ =
    {
        &utility->Packet,
        (address_t)&utility->Packet.data,
        (address_t)&packet_offset_lookup,
        (address_t*)&packet_value_lookup,
        0
    };
    _.packet->header.timestamp = RhoSystem.Functions.Platform.Time.Now();
    while( _.i++ < NUM_PACKET_ELEMENTS )
    {
        if( _.packet->header.includes & 0x01 )
        {
            if(!_.t) _.l = (*(packing_template_t*)_.llPtr).a;
            else          _.l = (*(packing_template_t*)_.llPtr).b;
            for( _.j = 0; _.j < _.l; _.j++)
                ((byte_t*)_.pdPtr)[_.j] = *(((byte_t*)*_.alPtr)+_.j);
             _.pdPtr += _.l;
        }
        _.alPtr++;
        _.includes >>= 1;
        if((_.t=!_.t )) ++_.llPtr;
    }
    printPacket( &utility->Packet, 3    );
}

// OLD PROBABILITY FUNCTIONS
//
////    if( !_.blobs[0].loc || !_.blobs[1].loc ) return;
//
///* Update prediction with best peaks */
//r->primary_new   = _.blobs[ _.sel].loc;
//r->secondary_new = _.blobs[!_.sel].loc;
//
///* Generate prediction probabilities */
//_.fdn_ = ZDIV(     1, _.fden);              // filtered density inverse
//
//_.pfac = _.blobs[ _.sel].den * _.fdn_;
//_.sfac = _.blobs[!_.sel].den * _.fdn_;
//
//floating_t n =  _.pfac + _.sfac;
//_.afac = pow( 1 - n, 1);
//n += _.afac;
//_.bfac = pow( 1 - n, 1);
//n += _.bfac;
//n = ZDIV( 1., n );
//
//_.fcov = BOUNDU(ZDIV(_.fden, _.tden), 2); // filtered coverage
//_.cfac = 1. - BOUNDU(ZDIV( _.fcov, utility->TargetCoverageFactor ), 2); // coverage factor
//_.vfac = ZDIV(_.fvar, _.fpeak); // variance factor
//
//r->probabilities.primary        = _.pfac * n;
//r->probabilities.secondary      = _.sfac * n;
//r->probabilities.alternate      = _.afac * n;
//r->probabilities.absence        = _.bfac * n;
//
//utility->FilteredCoverage      = _.fden;
//utility->TotalCoverage         = _.tden;
//utility->FilteredPercentage    = _.fcov;
//utility->CoverageFactor        = _.cfac;
//utility->VarianceFactor        = _.vfac;
//
//LOG_RHO("%.3f[cf] %.3f[vf] %.3f%%[fc] %.3f%%[tc] | %.3f[tf]\n", _.cfac, _.vfac, _.fcov*100, utility->target_coverage_factor*100, _.fdn_*_.tdnf );
//LOG_RHO("Rho: pri-%.3f sec-%.3f alt-%.3f abs-%.3f\n", r->probabilities.primary, r->probabilities.secondary, r->probabilities.alternate, r->probabilities.absence);
//LOG_RHO("* %.3f%% ( %d / %d )\n", _.fcov*100, _.fden, _.tden);
//LOG_RHO("Blobs: [0](%d,%d,%d) | [1](%d,%d,%d)\n", _.blobs[0].loc, _.blobs[0].den, _.blobs[0].max, _.blobs[1].loc, _.blobs[1].den, _.blobs[1].max);
//LOG_RHO("Normalized total probability: %.3f\n", r->probabilities.primary + r->probabilities.secondary + r->probabilities.alternate + r->probabilities.absence );

// OLD PREDICTION/KALMAN MATCH LOGIC
//prediction_update_variables _ =
//{
//    utility->PredictionPair.y.primary_new,
//    utility->PredictionPair.x.primary_new,
//    utility->PredictionPair.y.secondary_new,
//    utility->PredictionPair.x.secondary_new,
//    utility->Cx,
//    utility->Cy,
//    0
//};
//if(_.Ax > _.Bx) iswap(&_.Ax, &_.Bx);
//if(_.Ay > _.By) iswap(&_.Ay, &_.By);
//
//if( ( _.Ax < _.Cx ) ^ ( _.Bx > _.Cx ) ) /* ! X ambiguous */
//{
//    _.x = utility->PredictionPair.x.primary.value + utility->PredictionPair.x.primary.velocity;
//    if( fabs( _.x - _.Ax ) > fabs( _.x - _.Bx ) ) iswap( &_.Ax, &_.Bx );
//        _.non_diag = true;
//        LOG_RHO("X Ambiguous (%d < %d > %d)\n", _.Ax, _.Cx, _.Bx);
//        }
//if( ( _.Ay < _.Cy ) ^ ( _.By > _.Cy ) ) /* ! Y ambiguous */
//{
//    _.y = utility->PredictionPair.y.primary.value + utility->PredictionPair.y.primary.velocity;
//    if( fabs( _.y - _.Ay ) > fabs( _.y -_. By ) ) iswap( &_.Ay, &_.By );
//        _.non_diag = true;
//        LOG_RHO("Y Ambiguous (%d < %d > %d)\n", _.Ay, _.Cy, _.By);
//        }
//if ( !_.non_diag )
//{
//    //        printf("Rho: Redistributing densities.\n");
//    RhoFunctions.RedistributeDensities( utility );
//    _.qcheck = (  utility->Qf[0] > utility->Qf[1] ) + ( utility->Qf[2] < utility->Qf[3] ) - 1;
//    if( ( _.Ax > _.Bx ) ^ ( ( _.qcheck > 0 ) ^ ( _.Ay < _.By ) ) ) iswap(&_.Ax, &_.Bx);
//        LOG_RHO("Quadrant Check %d\n", _.qcheck);
//        }
//
//if( _.Ax ) RhoKalman.Step( &utility->PredictionPair.x.primary,   _.Ax, 0. );
//if( _.Bx ) RhoKalman.Step( &utility->PredictionPair.x.secondary, _.Bx, 0. );
//if( _.Ay ) RhoKalman.Step( &utility->PredictionPair.y.primary,   _.Ay, 0. );
//if( _.By ) RhoKalman.Step( &utility->PredictionPair.y.secondary, _.By, 0. );
//
//_.Cx = (index_t)((utility->PredictionPair.x.primary.value + utility->PredictionPair.x.secondary.value)) >> 1;
//_.Cy = (index_t)((utility->PredictionPair.y.primary.value + utility->PredictionPair.y.secondary.value)) >> 1;
//
//_.Cx = BOUNDU(_.Cx, utility->Width );
//_.Cy = BOUNDU(_.Cy, utility->Height);
//
//utility->Cx = _.Cx;
//utility->Cy = _.Cy;
//LOG_RHO("Cx>%d | Cy>%d\n", _.Cx, _.Cy);
//utility->DensityMapPair.x.centroid = _.Cy;
//utility->DensityMapPair.y.centroid = _.Cx;
//
//utility->PredictionPair.probabilities.primary   = sqrt( utility->PredictionPair.x.probabilities.primary   * utility->PredictionPair.y.probabilities.primary   );
//utility->PredictionPair.probabilities.secondary = sqrt( utility->PredictionPair.x.probabilities.secondary * utility->PredictionPair.y.probabilities.secondary );
//utility->PredictionPair.probabilities.alternate = sqrt( utility->PredictionPair.x.probabilities.alternate * utility->PredictionPair.y.probabilities.alternate );
//
//BayesianFunctions.Sys.Update( &utility->BayeSys, &utility->PredictionPair );
