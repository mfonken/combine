//
//  rho_core.c
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_core.h"

#ifdef USE_INTERRUPT_MODEL
/* Universal Port for interrupt model */
pixel_base_t test_port = 0;
#endif

void InitializeRhoCore( rho_core_t * core, index_t width, index_t height )
{
    memset(core, 0, sizeof(rho_core_t));
    
    /* core frame */
    core->Width = width;
    core->Height = height;
    
    /* Threshold Filter */
    RhoPID.Initialize(&core->ThreshFilter, DEFAULT_PID_GAIN );
    
    /* Coverage Filter */
    core->TargetCoverageFactor  = (floating_t)FILTERED_COVERAGE_TARGET;
    RhoKalman.Initialize(&core->TargetFilter, core->TargetCoverageFactor, RHO_TARGET_LS, 0, MAX(width, height), DEFAULT_TARGET_UNCERTAINTY );
    
    /* Density Filters */
    RhoUtility.Initialize.DensityMap( &core->DensityMapPair.x, height );
    RhoUtility.Initialize.DensityMap( &core->DensityMapPair.y, width  );
    
    /* Prediction Filters */
    for(uint8_t i = 0; i < MAX_TRACKING_FILTERS; i++)
    {
        RhoKalman.Initialize( &core->PredictionPair.x.TrackingFilters[i], 0., RHO_PREDICTION_LS, 0, height, DEFAULT_PREDICTION_UNCERTAINTY );
        RhoKalman.Initialize( &core->PredictionPair.y.TrackingFilters[i], 0., RHO_PREDICTION_LS, 0, width, DEFAULT_PREDICTION_UNCERTAINTY  );
        core->PredictionPair.x.TrackingFiltersOrder[i] = i;
        core->PredictionPair.y.TrackingFiltersOrder[i] = i;
    }
    
    /* Blobs */
    for(uint8_t i = 0; i < MAX_BLOBS; i++)
    {
        memset(&core->PredictionPair.x.Blobs[i], 0, sizeof(blob_t));
        memset(&core->PredictionPair.y.Blobs[i], 0, sizeof(blob_t));
        core->PredictionPair.x.BlobsOrder[i] = i;
        core->PredictionPair.y.BlobsOrder[i] = i;
    }
    
    /* Centroid */
    core->Cx   = width/2;
    core->Cy   = height/2;
    core->DensityMapPair.x.centroid = core->Cx;
    core->DensityMapPair.y.centroid = core->Cy;
    
    /* Prediction probabilities */
    memset(&core->PredictionPair.x.Probabilities, 0, sizeof(floating_t)*4);
    memset(&core->PredictionPair.y.Probabilities, 0, sizeof(floating_t)*4);
    
    /* Packet */
    core->Packet.header.ID       = PACKET_HEADER_ID;
    core->Packet.header.includes = PACKET_INCLUDES;
    memset(core->Packet.data, 0, sizeof(packet_offset_lookup_t));
    
    /* Background */
    core->BackgroundCounter = 0;
    core->BackgroundPeriod = BACKGROUND_PERIOD;
    
    /***** INTERRUPT MODEL CROSS-CONNECTOR VARIABLES START *****/
    /* Connect to Interrupt Model variable structure */
    RhoVariables.ram.Dx      =  core->DensityMapPair.x.map;
    RhoVariables.ram.Dy      =  core->DensityMapPair.y.map;
    RhoVariables.ram.Q       =  core->Q;
    RhoVariables.ram.CX_ADDR = &core->Cx;
    RhoVariables.ram.CY_ADDR = &core->Cy;
    RhoVariables.ram.C_FRAME =  core->cframe;
    RhoVariables.ram.THRESH_ADDR = (density_t *)&core->ThreshByte;
    RhoVariables.ram.CAM_PORT = &test_port;
    
    RhoVariables.global.C_FRAME_MAX = C_FRAME_SIZE;
    RhoVariables.global.y_delimiter = Y_DEL;
    RhoVariables.global.W    =  core->Width;
    RhoVariables.global.H    =  core->Height;
    
    /* Frame Initializeializer routine */
    RhoInterrupts.FRAME_INIT();
    
    /* Interrupt model mutex Initializeializer */
    pthread_mutex_init(&RhoVariables.global.rho_int_mutex, NULL);
    pthread_mutex_lock(&RhoVariables.global.rho_int_mutex);
    
    RhoVariables.connected = true;
    /*****  INTERRUPT MODEL CROSS-CONNECTOR VARIABLES END  *****/
}

void PerformRhoCore( rho_core_t * core, bool background_event )
{
    if(background_event)
        return;
//        RhoUtility.GenerateBackground( core );
    else
    {
        LOG_RHO("Filtering and selecting pairs.\n");
        RhoCore.DetectPairs( core );
        LOG_RHO("Updating predictions.\n");
        RhoCore.UpdatePredictions( core );
        LOG_RHO("Updating threshold.\n");
        RhoCore.UpdateThreshold( core );
//        LOG_RHO("Generating packets.\n");
//        RhoCore.GeneratePacket( core );
    }
}

/* Calculate and process data in variance band from density filter to generate predictions */
void DetectRhoCore( rho_core_t * core, density_map_t * d, prediction_t * r )
{
    rho_detection_variables *_ = (rho_detection_variables*)malloc(sizeof(rho_detection_variables));
    RhoUtility.Reset.Detect( _, d, r );
    
    DUAL_FILTER_CYCLE(_->cyc)
    {
        _->cmax = 0;
        _->start = _->range[_->cyc];
        _->end = _->range[_->cyc_];
        
        RhoUtility.Update.PeakFilter( _, d, r );
        
        if( _->fvar > 0 )// && (_->fpeak == 0 || _->fpeak > _->fvar ))
        {
            if(_->fpeak > _->fvar)
                _->fbandl = _->fpeak - _->fvar;
            else
                _->fbandl = 0;
            
            do
            {
                RhoUtility.Detect.Blobs( _, d, r);
                RhoUtility.Detect.CalculateChaos( _, r );
                RhoUtility.Detect.ScoreBlobs( _, d, r );
            } while( _->rcal && ++_->rcal_c < MAX_RHO_RECALCULATION_LEVEL );
            
            RhoUtility.Detect.SortBlobs( _, r );
        }
    
        r->PreviousPeak[_->cyc] = BOUND(_->cmax, 0, _->len);
        r->PreviousDensity[_->cyc] = _->fden;
        d->max[_->cyc] = _->cmax;
    }
    
    floating_t target_density = core->TargetFilter.value * core->Width * core->Height;// / band_factor;
    r->NumBlobs = _->blbf;
    r->NuBlobs = BOUNDU( ZDIV( (floating_t)_->tden * (floating_t)_->blbf, target_density ), 10.);
    r->TotalDensity = _->tden;
    /* Reset sort flags */
    for( uint8_t i = 0; i < MAX_BLOBS; i++ )
        r->Blobs[i].srt = false;
    
    core->TotalCoverage = _->tcov;
    core->FilteredCoverage   = _->fden;
    core->FilteredPercentage = ZDIV( (floating_t)core->FilteredCoverage, (floating_t)core->TotalCoverage );
    
//    floating_t A = (floating_t)r->TotalDensity, B = target_density;
//    floating_t band_width = _->fvar * 2, band_factor = r->PreviousPeak[0] / band_width;
//    printf("A:%6d B:%6d | Nu:%2.4f Bn:%d Dt:%d(%2.4f) Dc:%d Bw:%3.4f Bf:%3.4f\n", (int)A, (int)B, r->NuBlobs, r->NumBlobs, (int)target_density, core->TargetFilter.value, r->TotalDensity, band_width, band_factor);
}

void DetectRhoCorePairs( rho_core_t * core )
{
    LOG_RHO("Detecting X Map:\n");
    RhoCore.Detect( core, &core->DensityMapPair.x, &core->PredictionPair.x );
    LOG_RHO("Detecting Y Map:\n");
    RhoCore.Detect( core, &core->DensityMapPair.y, &core->PredictionPair.y );
}

/* Correct and factor predictions from variance band filtering into global model */
void UpdateRhoCorePrediction( prediction_t * r )
{
    /* Step predictions of all Kalmans */
    RhoUtility.Update.TrackingFilters( r );
    RhoUtility.Update.TrackingProbabilities( r );
    
    r->Primary   = r->TrackingFilters[0].value;
    r->Secondary = r->TrackingFilters[1].value;
}

void UpdateRhoCorePredictions( rho_core_t * core )
{
    LOG_RHO("Updating X Map:\n");
    RhoCore.UpdatePrediction( &core->PredictionPair.x );
    LOG_RHO("Updating Y Map:\n");
    RhoCore.UpdatePrediction( &core->PredictionPair.y );
    
    prediction_update_variables *_ = (prediction_update_variables*)malloc(sizeof(prediction_update_variables));
    RhoUtility.Reset.Prediction( _, &core->PredictionPair, core->Cx, core->Cy );

    _->Cx = BOUNDU((index_t)(_->Ax + _->Bx) >> 1, core->Width );
    _->Cy = BOUNDU((index_t)(_->Ay + _->By) >> 1, core->Height);
    
    LOG_RHO("Cx>%d | Cy>%d\n", _->Cx, _->Cy);
    
    core->Cx = _->Cx;
    core->Cy = _->Cy;
    
    /* NOTE: density maps invert axes */
    core->DensityMapPair.y.centroid = _->Cx;
    core->DensityMapPair.x.centroid = _->Cy;
    
    /* Average X & Y probabilities */
    for( uint8_t i = 0; i < NUM_STATE_GROUPS; i++ )
        core->PredictionPair.Probabilities.P[i] = ( core->PredictionPair.x.Probabilities.P[i] + core->PredictionPair.y.Probabilities.P[i] ) / 2.;
    
    BayesianFunctions.Sys.Update( &core->BayeSys, core->PredictionPair.Probabilities.P );
}

/* Use background and state information to update image threshold */
void UpdateRhoCoreThreshold( rho_core_t * core )
{
    /* Background-Tune on significant background */
    floating_t  background_tune_factor = 1.;
    if( core->QbT > BACKGROUND_COVERAGE_MIN )
    {
        floating_t background_coverage_factor = 1 - ZDIV( BACKGROUND_COVERAGE_MIN, core->QbT );
        background_tune_factor = pow( BOUND(background_coverage_factor, -1, 1), 3);
    }
    
    /* State-Tune by BayeSM state */
    floating_t state_tune_factor = 1.;
    core->TargetCoverageFactor = FILTERED_COVERAGE_TARGET;
    switch(core->BayeSys.state)
    {
        case STABLE_NONE:
            RhoKalman.Reset( &core->DensityMapPair.x.kalmans[0], core->PredictionPair.x.PreviousPeak[0] );
            RhoKalman.Reset( &core->DensityMapPair.x.kalmans[1], core->PredictionPair.x.PreviousPeak[1] );
            RhoKalman.Reset( &core->DensityMapPair.y.kalmans[0], core->PredictionPair.y.PreviousPeak[0] );
            RhoKalman.Reset( &core->DensityMapPair.y.kalmans[1], core->PredictionPair.y.PreviousPeak[1] );
        case STABLE_MANY:
//            state_tune_factor = 10;
        case STABLE_SINGLE:
        case UNSTABLE_NONE:
        case UNSTABLE_SINGLE:
        case UNSTABLE_MANY:
            state_tune_factor *= ((floating_t)UNSTABLE_DOUBLE - (floating_t)core->BayeSys.state ) / (floating_t)NUM_STATES;
            break;
        case STABLE_DOUBLE:
            core->TargetCoverageFactor = core->FilteredPercentage;
            break;
        default:
            break;
    }
    RhoKalman.Step( &core->TargetFilter, core->TargetCoverageFactor, 0. );
    RhoPID.Update( &core->ThreshFilter, core->FilteredPercentage, core->TargetFilter.value);
    
    /* Filtered-Tune on target difference */
    floating_t target_tune_factor = -ZDIV( core->ThreshFilter.Value, core->TargetFilter.value );
    floating_t proposed_tune_factor = BOUND( background_tune_factor * state_tune_factor * target_tune_factor, -THRESH_STEP_MAX, THRESH_STEP_MAX);
    core->Thresh = BOUND(core->Thresh - proposed_tune_factor, THRESH_MIN, THRESH_MAX);
    core->ThreshByte = (byte_t)core->Thresh;
    
        printf("(%s) STune:%3.4f TFilter.V:%3.4f Proposed:%3.4f\n", stateString(core->BayeSys.state), state_tune_factor, target_tune_factor, proposed_tune_factor);
    //    if(proposed_tune_factor < 0.001) proposed_tune_factor *= PID_DRIFT;
    //    printf("*** THRESH IS %.2f(%.2f) ***\n", core->Thresh, core->ThreshFilter.Value);
    //    printf("btf:%.3f | stf%.3f | ptf%.6f \n", background_tune_factor, state_tune_factor, proposed_tune_factor);
    //    printf("thf.v:%.3f | thf.e:%.3f | tgf.v:%.3f | tc.f:%.3f\n", core->ThreshFilter.Value, core->ThreshFilter.Error, core->TargetFilter.value, core->TargetCoverageFactor);
    //    printf("Background coverage compare: Actual>%dpx vs. Target>%d±%dpx\n", core->QbT, BACKGROUND_COVERAGE_MIN, BACKGROUND_COVERAGE_TOL_PX);
}

void GenerateRhoCorePacket( rho_core_t * core )
{
    packet_value_lookup_t  packet_value_lookup  = PACKET_ADDRESS_INITIALIZER(core->PredictionPair);
    packet_offset_lookup_t packet_offset_lookup = PACKET_OFFSETS;
    packet_generation_variables _ =
    {
        &core->Packet,
        (address_t)&core->Packet.data,
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
    
    RhoUtility.PrintPacket( &core->Packet, 3    );
}


