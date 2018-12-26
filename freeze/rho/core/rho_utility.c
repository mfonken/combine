//
//  rho_utility.c
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_utility.h"

/* Universal Port for interrupt model */
pixel_base_t test_port = 0;

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
    b->scr = BLOB_SCORE_FACTOR*( ( delta_d * delta_d ) + ( delta_p * delta_p ) );
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

void InitRhoUtility( rho_utility * utility, index_t width, index_t height )
{
    size_t size;
    
    memset(utility, 0, sizeof(rho_utility));
    
    /* Utility frame */
    utility->Width = width;
    utility->Height = height;
    
    /* Threshold Filter */
    RhoPID.Init(&utility->ThreshFilter, DEFAULT_PID_GAIN );
    
    /* Coverage Filter */
    utility->TargetCoverageFactor  = (floating_t)FILTERED_COVERAGE_TARGET;
    RhoKalman.Init(&utility->TargetFilter, utility->TargetCoverageFactor, RHO_TARGET_LS, RHO_TARGET_VU, RHO_TARGET_BU, RHO_TARGET_SU, 0, MAX(width, height) );
    
    /* Density Filters */
    size = sizeof(density_t)*height;
    utility->DensityMapPair.x.map = (density_t*)malloc(size);
    utility->DensityMapPair.x.background = (density_t*)malloc(size);
    utility->DensityMapPair.x.filtered = (density_t*)malloc(size);
    memset(utility->DensityMapPair.x.map, 0, size);
    memset(utility->DensityMapPair.x.background, 0, size);
    memset(utility->DensityMapPair.x.filtered, 0, size);
    utility->DensityMapPair.x.length = height;
    utility->DensityMapPair.x.max[0] = 0;
    utility->DensityMapPair.x.max[1] = 0;
    RhoKalman.Init( &utility->DensityMapPair.x.kalmans[0], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU, 0, height );
    RhoKalman.Init( &utility->DensityMapPair.x.kalmans[1], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU, 0, height );
    
    size = sizeof(density_t)*width;
    utility->DensityMapPair.y.map = (density_t*)malloc(size);
    utility->DensityMapPair.y.background = (density_t*)malloc(size);
    utility->DensityMapPair.y.filtered = (density_t*)malloc(size);
    memset(utility->DensityMapPair.y.map, 0, size);
    memset(utility->DensityMapPair.y.background, 0, size);
    memset(utility->DensityMapPair.y.filtered, 0, size);
    utility->DensityMapPair.y.length = width;
    utility->DensityMapPair.y.max[0] = 0;
    utility->DensityMapPair.y.max[1] = 0;
    RhoKalman.Init( &utility->DensityMapPair.y.kalmans[0], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU, 0, width );
    RhoKalman.Init( &utility->DensityMapPair.y.kalmans[1], 0, RHO_DEFAULT_LS, RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU, 0, width );
    
    /* Prediction Filters */
    for(uint8_t i = 0; i < MAX_TRACKING_FILTERS; i++)
    {
        RhoKalman.Init( &utility->PredictionPair.x.TrackingFilters[i], 0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU, 0, height );
        RhoKalman.Init( &utility->PredictionPair.y.TrackingFilters[i], 0., RHO_PREDICTION_LS, RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU, 0, width );
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
        
    utility->Cx   = width/2;
    utility->Cy   = height/2;
    utility->DensityMapPair.x.centroid = utility->Cx;
    utility->DensityMapPair.y.centroid = utility->Cy;
    
    /* Prediction probabilities */
    memset(&utility->PredictionPair.x.Probabilities, 0, sizeof(floating_t)*4);
    memset(&utility->PredictionPair.y.Probabilities, 0, sizeof(floating_t)*4);
    
    utility->Packet.header.ID       = PACKET_HEADER_ID;
    utility->Packet.header.includes = PACKET_INCLUDES;
    memset(utility->Packet.data, 0, sizeof(packet_offset_lookup_t));
    
    utility->BackgroundCounter = 0;
    utility->BackgroundPeriod = BACKGROUND_PERIOD;
       
    /***** INTERRUPT MODEL CROSS-CONNECTOR VARIABLES START *****/
    /* Connect to Interrupt Model variable structure */
    RhoVariables.ram.Dx      =  utility->DensityMapPair.x.map;
    RhoVariables.ram.Dy      =  utility->DensityMapPair.y.map;
    RhoVariables.ram.Q       =  utility->Q;
    RhoVariables.ram.CX_ADDR = &utility->Cx;
    RhoVariables.ram.CY_ADDR = &utility->Cy;
    RhoVariables.ram.C_FRAME =  utility->cframe;
    RhoVariables.ram.THRESH_ADDR = (density_t *)&utility->Thresh;
    RhoVariables.ram.CAM_PORT = &test_port;
    
    RhoVariables.global.C_FRAME_MAX = C_FRAME_SIZE;
    RhoVariables.global.y_delimiter = Y_DEL;
    RhoVariables.global.W    =  utility->Width;
    RhoVariables.global.H    =  utility->Height;
    
    /* Frame Initializer routine */
    RhoInterrupts.FRAME_INIT();
    
    /* Interrupt model mutex initializer */
    pthread_mutex_init(&RhoVariables.global.rho_int_mutex, NULL);
    pthread_mutex_lock(&RhoVariables.global.rho_int_mutex);
    
    RhoVariables.connected = true;
    /*****  INTERRUPT MODEL CROSS-CONNECTOR VARIABLES END  *****/
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
//        LOG_RHO("Generating packets.\n");
//        RhoFunctions.GeneratePacket( utility );
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
    
    r->NuBlobs = 0;
    
    index_t range[3] = { _.len, d->centroid, 0 };
    LOG_RHO("Range is <%d | %d | %d>\n", range[2], range[1], range[0]);
    
    for(uint8_t i = 0; i < MAX_BLOBS; i++)
    {
        memset(&r->Blobs[i], 0, sizeof(blob_t));
        r->BlobsOrder[i] = MAX_BLOBS;
    }
    
    DUAL_FILTER_CYCLE(cyc)
    {
        { /* Update Peak Filter - START */
            _.fpeak = (index_t)RhoKalman.Step(&d->kalmans[cyc], r->PreviousPeak[cyc], d->kalmans[cyc].velocity );
            LOG_RHO("%d: %6d %6d %6.3f %6.3f\n", cyc, r->PreviousPeak[cyc], _.fpeak, d->kalmans[cyc].value, d->kalmans[cyc].velocity);
            d->max[cyc] = r->PreviousPeak[cyc];
            _.fpeak_2 = _.fpeak << 1;
            _.fvar = BOUND((index_t)RHO_VARIANCE( d->kalmans[cyc].K[0] ), MIN_VARIANCE, MAX_VARIANCE);
            d->kalmans[cyc].variance = _.fvar;
        } /* Update Peak Filter - END */
        
        { /* Analyze band - START */
            if( _.fvar > 0 )// && (_.fpeak == 0 || _.fpeak > _.fvar ))
            {
                if(_.fpeak > _.fvar)
                    _.fbandl = _.fpeak - _.fvar;
                else
                    _.fbandl = 0;
                index_t start = range[cyc], end = range[cyc_];

                do
                {
                    { /* Detect blobs - START */
                        BOUNDED_CYCLE_DUAL(start, end, x, _.c1, d->map, _.b, d->background)
                        {
                            if( _.c1 > _.b)
                            {
                                _.c1 = BOUNDU(_.c1 - _.b, d->length);
                                utility->TotalCoverage += _.c1;
                                if( _.c1 > _.cmax )
                                    _.cmax = _.c1;
                                
                                /* Punish values above the filter peak */
                                if( ( _.c1 > _.fpeak )
                                   && ( _.fpeak_2 > _.c2 ) )
                                    _.c1 = _.fpeak_2 - _.c1;
                            }
                            else
                                _.c1 = 0;

                            /* Check if CMA value is in band */
                            if( _.c1 > _.fbandl )
                            {
                                /* De-offset valid values */
                                _.c2 = _.c1 - _.fbandl;

                                /* Process new values into blob */
                                cma_M0_M1((floating_t)_.c2, (floating_t)x, &_.cavg, &_.mavg, &_.avgc);

                                /* Update max */
//                                if(_.c2 > _.cmax) _.cmax = _.c2;

                                _.width++;

                                /* Reset flag and counter */
                                _.has = 1; _.gapc = 0;
                            }

                            /* Process completed blobs and increment count */
                            else if( ++_.gapc > RHO_GAP_MAX && _.has )
                            {
                                /* Get latest blob centroid */
                                _.cden = (density_2d_t)_.cavg;
                                _.fden += _.cden;

                                /* Check if new blob is dense enough to be saved */
                                if( _.cden > MIN_BLOB_DENSITY)
                                {
                                    /* Create new blob at secondary */
                                    index_t loc = (index_t)ZDIV(_.mavg,_.cavg);
                                    r->Blobs[_.blbf] = (blob_t){ d->map[loc], _.cden, loc, _.width };
                                    r->BlobsOrder[_.blbf] = _.blbf;
                                    _.blbf++;
                                }

                                /* Reset variables */
                                _.mavg = 0.; _.cavg = 0.; _.avgc = 0.;
                                _.has = 0; _.gapc = 0;
                            }
                            
                            else if (!_.has)
                            {
                                _.width = 0;
                            }
                        }
                    } /* Detect blobs - END */
                    
                    { /* Calculate Chaos - START */
                        _.pkp = (floating_t)r->PreviousPeak[cyc] / (floating_t)_.cmax; // density percent
                        _.chaos = (floating_t)r->PreviousDensity[cyc] / (floating_t)_.fden; // peak percent
                        /* Assume no recalculations are needed */
                        _.rcal = false;
                    } /* Calculate Chaos - END */
                    if(_.fden > 10000)
                        _.fden++;
                    printf("Chaos: %.3f(%d/%d)\n", _.chaos, r->PreviousDensity[cyc], _.fden);
                    
                    { /* Score blobs - START */
                        /* Return on empty frames */
                        if(_.fden > 0 )
                        {
                            /* Cycle blobs */
                            for(uint8_t i = 0; i < _.blbf; i++)
                            {
                                /* Get blob at index from order array and check validity */
                                uint8_t j = r->BlobsOrder[i];
                                if(j >= MAX_BLOBS) continue;
                                blob_t * curr = &r->Blobs[j];

                                /* Score current blob */
                                CalculateBlobScore(curr, _.fden, _.cmax);

                                /* Recalculate blobs with chaos */
                                if(curr->scr > _.chaos)
                                {
                                    /* Recalculate around chaotic blob */
                                    _.rcal = true;

                                    /* Remove this index */
                                    r->BlobsOrder[i] = MAX_BLOBS+1;

                                    /* Get centroid peak */
                                    sdensity_t cpeak = (sdensity_t)d->map[curr->loc];
                                    _.fpeak_2 = cpeak << 1;
                                    /* Raise lower band half of blob centroid to band center when below */
                                    if( cpeak < _.fpeak)
                                        _.fbandl += abs( (sdensity_t)_.fpeak - cpeak) >> 1;
                                    /* Otherwise raise to centroid peak */
                                    else _.fbandl = cpeak;

                                    /* Recalculate cycle width */
                                    index_t w_2 = curr->wth >> 1;
                                    if(curr->loc - w_2 > range[cyc_])
                                        end = curr->loc - w_2;
                                    else end = range[cyc_];
                                    if(curr->loc + w_2 < range[cyc])
                                        start = curr->loc + w_2;
                                    else start = range[cyc];
                                    break;
                                }
                            }
                        }
                    } /* Score blobs - END */
                } while( _.rcal && ++_.rcal_c < MAX_RHO_RECALCULATION_LEVEL );
//                if( _.cmax == 0 ) continue;
                { /* Sort blobs - START */
                    uint8_t num_poor = 0;
                    for(uint8_t i = 0, j; i < _.blbf; i++)
                    {
                        for( j = 0; j < _.blbf && r->Blobs[j].srt;j++);
                        floating_t min = r->Blobs[j].scr;
                        for( uint8_t k = j+1; k < _.blbf; k++ )
                        {
                            blob_t *curr = &r->Blobs[k];
                            if( curr->scr < min && !curr->srt )
                            {
                                min = curr->scr;
                                curr->srt = 1;
                                j = k;
                            }
                        }
                        if( min == r->Blobs[j].scr )
                            r->Blobs[j].srt = 1;
                        if( r->Blobs[j].scr < MAX_BLOB_SCORE )
                            r->BlobsOrder[i-num_poor] = j;
                        else num_poor++;
                    }
//                    if(r->BlobsOrder[0] == r->BlobsOrder[1])
//                        printf("!\n");
                    /* Remove all unused blobs order indeces */
                    for(uint8_t i = _.blbf-num_poor; i < MAX_BLOBS; i++) r->BlobsOrder[i] = MAX_BLOBS;
                } /* Sort blobs - END */
            } /* Analyze band - END */
        }
        
//        for(uint8_t i = 0; i < _.blbf; i++)
//        {
//            uint8_t index = r->BlobsOrder[i];
//            printf("%d:%d\n", index, r->Blobs[index].loc);
//        }
//        printf("-------------\n");
         //LOG_RHO("Map(%d): max>%d\n", cyc, _.cmax);
        r->NumBlobs = _.blbf;
        for( uint8_t i = 0; i < _.blbf; i++ )
        {
            r->Blobs[i].srt = false;
        }
//        if( _.cmax == 0 ) return;
        
        r->PreviousPeak[cyc] = BOUND(_.cmax, 0, _.len);//d->length);
        r->PreviousDensity[cyc] = _.fden;
        utility->FilteredCoverage = _.fden;
        utility->FilteredPercentage = ZDIV( (floating_t)utility->FilteredCoverage, (floating_t)utility->TotalCoverage );
        
        printf("fc:%d(%.4f) tc:%d(%.4f)\n", utility->FilteredCoverage, utility->FilteredPercentage, utility->TotalCoverage, (floating_t)utility->TotalCoverage/(utility->Width*utility->Height));
        
        d->max[cyc] = _.cmax;
    
        if( isnormal(_.chaos) && isnormal(_.pkp) )
            r->NuBlobs += _.chaos * _.pkp * (floating_t)_.blbf;
        else
            r->NuBlobs = 0.;
    }
}

void FilterAndSelectRhoUtilityPairs( rho_utility * utility )
{
    LOG_RHO("X Map:\n");
    RhoFunctions.FilterAndSelect( utility, &utility->DensityMapPair.x, &utility->PredictionPair.x );
    LOG_RHO("Y Map:\n");
    RhoFunctions.FilterAndSelect( utility, &utility->DensityMapPair.y, &utility->PredictionPair.y );
}

void GenerateRhoUtilityBackground( rho_utility * utility )
{
    floating_t xt = CalculateCentroid( utility->DensityMapPair.x.background, utility->DensityMapPair.x.length, &utility->Bx, BACKGROUND_CENTROID_CALC_THRESH );
    floating_t yt = CalculateCentroid( utility->DensityMapPair.y.background, utility->DensityMapPair.y.length, &utility->By, BACKGROUND_CENTROID_CALC_THRESH );
    utility->QbT = MAX(xt, yt);
}

void RedistributeRhoUtilityDensities( rho_utility * utility )
{
    LOG_RHO("Rho: Redistributing densities.\n");
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

/* Correct and factor predictions from variance band filtering into global model */
void UpdateRhoUtilityPrediction( rho_utility * utility, prediction_t * r )
{
//    printf("------------------------\n");
    /* Step predictions of all Kalmans */
    uint8_t valid_tracks = 0;
    for( valid_tracks = 0; valid_tracks < MAX_TRACKING_FILTERS; valid_tracks++ )
    {
        uint8_t index = valid_tracks;
        if( index >= MAX_TRACKING_FILTERS ) break;
        rho_kalman_t *curr = &(r->TrackingFilters[index]);
        floating_t score = RhoKalman.Score(curr);
        if( RhoKalman.IsExpired(curr)
           || ( score < MIN_TRACKING_KALMAN_SCORE ) )
        {
//            printf("Bad score %.2f(%d)\n", score, index);
            break;
        }
        
        RhoKalman.Predict(curr, curr->velocity);
    }
//    printf("valid tracks %d\n", valid_tracks);
    /* Match blobs to Kalmans */
    floating_t A, B;
    uint8_t m, n;
    for( m = 0; m < ( valid_tracks - 1 ); m += 2 )
    {
        for( n = 0; n < ( r->NumBlobs - 1 ); n += 2 )
        {
            rho_kalman_t
            *filterA = &r->TrackingFilters[m],
            *filterB = &r->TrackingFilters[m+1];
            blob_t
            *blobA = &r->Blobs[r->BlobsOrder[n]],
            *blobB = &r->Blobs[r->BlobsOrder[n+1]];
            if( blobA->srt || blobB->srt ) continue;
            floating_t
            blocA = blobA->loc,
            blocB = blobB->loc;
            
            floating_t aa, bb, ab, ba;
            aa = fabs(filterA->value - blocA);
            bb = fabs(filterB->value - blocB);
            ab = fabs(filterA->value - blocB);
            ba = fabs(filterB->value - blocA);
            
            A = aa * bb;
            B = ab * ba;
            
            if( A > B )
            {
                SWAP(blocA, blocB);
//                printf("SWAP\n");
            }
//            printf("%.2fx%.2f = %.2f <> %.2fx%.2f = %.2f   %.2f:%.2f\n",aa,bb,A,ab,ba,B,blocA,blocB);
            
            RhoKalman.Update(filterA, blocA);
            RhoKalman.Update(filterB, blocB);
            
            blobA->srt = true;
            blobB->srt = true;
        }
        if( n > r->NumBlobs )
        { // Account for odd number of blobs
            rho_kalman_t
            *filter = &r->TrackingFilters[m];
            floating_t
            blob = (floating_t)r->Blobs[r->BlobsOrder[n-1]].loc;
            RhoKalman.Update(filter, blob);
        }
    }
    uint8_t k = valid_tracks;
    for( ; k < r->NumBlobs && k < MAX_TRACKING_FILTERS; k++ )
    {
        RhoKalman.Step( &r->TrackingFilters[k], r->Blobs[r->BlobsOrder[k]].loc, 0. );
    }
    for( ; k < MAX_TRACKING_FILTERS; k++ )
    {
        RhoKalman.Punish(&r->TrackingFilters[k]);
    }
    for( k = 0; k < MAX_TRACKING_FILTERS; k++ )
    {
        r->TrackingFilters[k].flag = false;
    }
    r->Primary   = r->TrackingFilters[0].value;
    r->Secondary = r->TrackingFilters[1].value;

    if( r->NuBlobs > 0. )
    {
        floating_t curr_CDF, prev_CDF = 0.;
        for( uint8_t i = 0; i <= NUM_STATE_GROUPS; i++ )
        {
            floating_t x = (floating_t)i / (floating_t)((uint8_t)NUM_STATE_GROUPS);
            curr_CDF = 1 - pow( 1 - pow( x, r->NuBlobs ), NUM_STATE_GROUPS);
            if( i > 0 ) r->Probabilities.P[i-1] = curr_CDF - prev_CDF;
            prev_CDF = curr_CDF;
        }
    }
    else
    {
        memset(r->Probabilities.P, 0, sizeof(r->Probabilities.P));
        r->Probabilities.P[0] = 1.;
    }
    
//    for( k = 0; k < MAX_TRACKING_FILTERS; k++ )
//    {
//        uint8_t index = k;
//        printf("%d(%d):%.2f(%.2f)\n", index, k, r->TrackingFilters[index].value, RhoKalman.Score(&r->TrackingFilters[index]));
//    }
}

void UpdateRhoUtilityPredictions( rho_utility * utility )
{
    UpdateRhoUtilityPrediction( utility, &utility->PredictionPair.x );
    UpdateRhoUtilityPrediction( utility, &utility->PredictionPair.y );
    
    prediction_update_variables _ =
    {
        utility->PredictionPair.x.Primary,
        utility->PredictionPair.y.Primary,
        utility->PredictionPair.x.Secondary,
        utility->PredictionPair.y.Secondary,
        utility->Cx,
        utility->Cy,
        0
    };
    LOG_RHO("Ax-%d Bx-%d   Ay-%d By-%d\n", _.Ax, _.Bx, _.Ay, _.By);
    if(   !( ( _.Ax < _.Cx ) ^ ( _.Bx > _.Cx ) )
       && !( ( _.Ay < _.Cy ) ^ ( _.By > _.Cy ) ) ) /* ! X and Y not ambiguous */
    {
        RhoFunctions.RedistributeDensities( utility );
        _.qcheck = (  utility->Qf[0] > utility->Qf[1] ) + ( utility->Qf[2] < utility->Qf[3] ) - 1;
        if( ( _.Ax > _.Bx ) ^ ( ( _.qcheck > 0 ) ^ ( _.Ay < _.By ) ) ) iswap(&_.Ax, &_.Bx);
    }
    
    _.Cx = BOUNDU((index_t)(_.Ax + _.Bx) >> 1, utility->Width );
    _.Cy = BOUNDU((index_t)(_.Ay + _.By) >> 1, utility->Height);
    LOG_RHO("Cx>%d | Cy>%d\n", _.Cx, _.Cy);
    
    utility->Cx = _.Cx;
    utility->Cy = _.Cy;

    utility->DensityMapPair.y.centroid = _.Cx;
    utility->DensityMapPair.x.centroid = _.Cy;
    /* NOTE: density maps invert axes */

    for( uint8_t i = 0; i < NUM_STATE_GROUPS; i++ )
    {
        floating_t pavg = ( utility->PredictionPair.x.Probabilities.P[i] + utility->PredictionPair.y.Probabilities.P[i] ) / 2.;
        utility->PredictionPair.Probabilities.P[i] = pavg;
    }

    BayesianFunctions.Sys.Update( &utility->BayeSys, &utility->PredictionPair );
}

/* Use background and state information to update image threshold */
void UpdateRhoUtilityThreshold( rho_utility * utility )
{
    /* Background-Tune on significant background */
    floating_t  background_tune_factor = 0.;
    if( utility->QbT > BACKGROUND_COVERAGE_MIN )
    {
        floating_t background_coverage_factor = 1 - ZDIV( BACKGROUND_COVERAGE_MIN, utility->QbT );
        background_tune_factor = pow( BOUND(background_coverage_factor, -1, 1), 3);
    }
    
    /* State-Tune by BayeSM state */
    floating_t state_tune_factor = 0.;
    utility->TargetCoverageFactor = FILTERED_COVERAGE_TARGET;
    switch(utility->BayeSys.state)
    {
        case STABLE_NONE:
            RhoKalman.Reset( &utility->DensityMapPair.x.kalmans[0], utility->PredictionPair.x.PreviousPeak[0] );
            RhoKalman.Reset( &utility->DensityMapPair.x.kalmans[1], utility->PredictionPair.x.PreviousPeak[1] );
            RhoKalman.Reset( &utility->DensityMapPair.y.kalmans[0], utility->PredictionPair.y.PreviousPeak[0] );
            RhoKalman.Reset( &utility->DensityMapPair.y.kalmans[1], utility->PredictionPair.y.PreviousPeak[1] );
        case STABLE_SINGLE:
        case STABLE_MANY:
        case UNSTABLE_NONE:
        case UNSTABLE_SINGLE:
        case UNSTABLE_MANY:
            state_tune_factor = (UNSTABLE_DOUBLE - (floating_t)utility->BayeSys.state ) / NUM_STATES;
            break;
        case STABLE_DOUBLE:
            utility->TargetCoverageFactor = utility->FilteredPercentage;
            break;
        default:
            break;
    }
    RhoKalman.Step( &utility->TargetFilter, utility->TargetCoverageFactor, 0. );
    RhoPID.Update( &utility->ThreshFilter, utility->FilteredPercentage, utility->TargetFilter.value);
    
    /* Filtered-Tune on target difference */
//    floating_t filtered_tune_factor = utility->ThreshFilter.Error;
    floating_t proposed_tune_factor = BOUND( ( 1. + ( background_tune_factor + state_tune_factor ) ) * ( PID_SCALE * -utility->ThreshFilter.Value ), -THRESH_STEP_MAX, THRESH_STEP_MAX);
    utility->Thresh = (byte_t)BOUND(utility->Thresh + proposed_tune_factor, THRESH_MIN, THRESH_MAX);

    printf("*** THRESH IS %d(%.2f) ***\n", utility->Thresh, utility->ThreshFilter.Value);
    printf("btf:%.3f | stf%.3f | e%.6f \n", background_tune_factor, state_tune_factor, proposed_tune_factor);
    printf("thf.v:%.3f | thf.e:%.3f | tgf.v:%.3f | tc.f:%.3f\n", utility->ThreshFilter.Value, utility->ThreshFilter.Error, utility->TargetFilter.value, utility->TargetCoverageFactor);
//    printf("Background coverage compare: Actual>%dpx vs. Target>%d±%dpx\n", utility->QbT, BACKGROUND_COVERAGE_MIN, BACKGROUND_COVERAGE_TOL_PX);
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
    _.packet->header.timestamp = timestamp();
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
//    printPacket( &utility->Packet, 3    );
}
