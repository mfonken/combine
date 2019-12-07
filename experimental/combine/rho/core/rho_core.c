/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  File: rho_core.h
 *  Group: Rho Core
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                          Includes                                    *
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "rho_core.h"

#ifdef USE_DETECTION_MAP
#include "detection_map.h"
#endif
#ifndef TIMESTAMP
#include "timestamp.h"
#endif

static const char * X_INSTANCE_NAME = "X";
static const char * Y_INSTANCE_NAME = "Y";

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                       Local Instance                                 *
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
const rho_core_functions RhoCore =
{
    .Initialize         = InitializeRhoCore,
    .Perform            = PerformRhoCore,
    .DetectPairs        = DetectRhoCorePairs,
    .Detect             = DetectRhoCore,
    .UpdatePrediction   = UpdateRhoCorePrediction,
    .UpdatePredictions  = UpdateRhoCorePredictions,
    .UpdateThreshold    = UpdateRhoCoreThreshold,
    .GeneratePacket     = GenerateRhoCorePacket
};

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                      Functions Declarations                          *
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
void InitializeRhoCore( rho_core_t * core, index_t width, index_t height )
{
    /* Generic Data */
    RhoUtility.Initialize.Data( core, width, height );

    /* Filters */
    RhoUtility.Initialize.Filters( core );
    
    /* Density Data */
    RhoUtility.Initialize.DensityMap( &core->DensityMapPair.x, X_INSTANCE_NAME, height, core->Centroid.y );
    RhoUtility.Initialize.DensityMap( &core->DensityMapPair.y, Y_INSTANCE_NAME, width, core->Centroid.x  );

    /* Prediction Structures */
    RhoUtility.Initialize.Prediction( &core->PredictionPair.x, X_INSTANCE_NAME, core->Height );
    RhoUtility.Initialize.Prediction( &core->PredictionPair.y, Y_INSTANCE_NAME, core->Width  );

#ifdef USE_DETECTION_MAP
    /* Detection map */
    DetectionMapFunctions.Init( &core->DetectionMap, DETECTION_BUFFER_SIZE );
#endif
#ifdef USE_DECOUPLING
    /* Frame Conversion Model Connection */
    RhoInterrupts.INIT_FROM_CORE( core );
#endif
#ifdef __PSM__
    PSMFunctions.Initialize( &core->PredictiveStateModelPair.x, X_INSTANCE_NAME );
    PSMFunctions.Initialize( &core->PredictiveStateModelPair.y, Y_INSTANCE_NAME );
#endif
}

void PerformRhoCore( rho_core_t * core, bool background_event )
{
    if(background_event)
        RhoUtility.Generate.Background( core );
    else
    {
        RhoCore.DetectPairs( core );
        RhoCore.UpdatePredictions( core );
        RhoCore.UpdateThreshold( core );
//        RhoCore.GeneratePacket( core );
    }
}

/* Calculate and process data in variance band from density filter to generat
 e predictions */
void DetectRhoCore( rho_core_t * core, density_map_t * density_map, prediction_t * prediction )
{
    LOG_RHO(RHO_DEBUG_2, "Detecting %s Map:\n", density_map->name );
    
    rho_detection_variables _;
    RhoUtility.Reset.Detect( &_, density_map, prediction );
    core->TotalCoverage = 0;
    core->FilteredCoverage = 0;
    _.target_density = core->TargetFilter.value * (floating_t)TOTAL_RHO_PIXELS;

    /* Perform detect */
    LOG_RHO(RHO_DEBUG_2, "Performing detect:\n");
    RhoUtility.Detect.Perform( &_, density_map, prediction );
    
    /* Update frame statistics */
    LOG_RHO(RHO_DEBUG_2, "Calculating frame statistics:\n");
    RhoUtility.Detect.CalculateFrameStatistics( &_, prediction );

    /* Update core */
    core->TotalCoverage     += _.total_density;// target_density;
    core->FilteredCoverage  += _.filtered_density;
}

void DetectRhoCorePairs( rho_core_t * core )
{
    LOG_RHO(RHO_DEBUG_2,"Filtering and selecting pairs.\n");
    RhoCore.Detect( core, &core->DensityMapPair.x, &core->PredictionPair.x );
    RhoCore.Detect( core, &core->DensityMapPair.y, &core->PredictionPair.y );

    /* Calculate accumulated filtered percentage from both axes */
    core->FilteredPercentage        = ZDIV( (floating_t)core->FilteredCoverage, (floating_t)TOTAL_RHO_PIXELS );
    core->TotalPercentage           = ZDIV( (floating_t)core->TotalCoverage, (floating_t)TOTAL_RHO_PIXELS );
    core->PredictionPair.NumRegions = MAX( core->PredictionPair.x.NumRegions, core->PredictionPair.y.NumRegions );
    core->PredictionPair.NuRegions  = MAX( core->PredictionPair.x.NuRegions, core->PredictionPair.y.NuRegions );
}

/* Correct and factor predictions from variance band filtering into global model */
void UpdateRhoCorePrediction( prediction_t * prediction )
{
    LOG_RHO(RHO_DEBUG_PREDICT,"Updating %s Map:\n", prediction->Name);
    
    /* Step predictions of all Kalmans */
    RhoUtility.Predict.TrackingFilters( prediction );
    RhoUtility.Predict.TrackingProbabilities( prediction );
}

void UpdateRhoCorePredictions( rho_core_t * core )
{
    LOG_RHO(RHO_DEBUG_2,"Updating predictions.\n");
    RhoCore.UpdatePrediction( &core->PredictionPair.x );
    RhoCore.UpdatePrediction( &core->PredictionPair.y );
    
    RhoUtility.Predict.ReportObservationLists( core );

#ifdef USE_DETECTION_MAP
    DetectionMapFunctions.AddSet( &core->DetectionMap, &core->PredictionPair );
#endif
    
#ifdef __PSM__
    if( ISTIMEDOUT( core->Timestamp, PSM_UPDATE_PERIOD ) )
    {
        LOG_PSM(PSM_DEBUG, "Updating PSM\n");
        /* Process both dimensions' predictive state */
        RhoUtility.Predict.UpdatePredictiveStateModelPair( core );
        core->Timestamp = TIMESTAMP();
    }
#endif
    
    double state_intervals[NUM_STATE_GROUPS];
    KumaraswamyFunctions.GetVector( &core->Kumaraswamy, core->PredictionPair.NuRegions, state_intervals );
    FSMFunctions.Sys.Update( &core->StateMachine, state_intervals );

    prediction_predict_variables _;
    RhoUtility.Reset.Prediction( &_, &core->PredictionPair, core->Centroid );
    RhoUtility.Predict.CorrectAmbiguity( &_, core );
    RhoUtility.Predict.CombineProbabilities( &core->PredictionPair );
    RhoUtility.Predict.UpdateCorePredictionData( &_, core );
}

/* Use background and state information to update image threshold */
void UpdateRhoCoreThreshold( rho_core_t * core )
{
    LOG_RHO(RHO_DEBUG_2,"Updating threshold.\n");
    RhoUtility.Calculate.Tune( core );
}

void GenerateRhoCorePacket( rho_core_t * core )
{
    LOG_RHO(RHO_DEBUG_2,"Generating packets.\n");
    RhoUtility.Generate.Packet( core );
    RhoUtility.Print.Packet( &core->Packet, DEFAULT_PACKET_LENGTH );
}
