//
//  rho_utility.h
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2019 Marbl. All rights reserved.
//

#ifndef rho_utility_h
#define rho_utility_h

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "rho_interrupt_model.h"
#include "rho_structure.h"
#include "rho_types.h"

#ifdef __cplusplus
extern "C" {
#endif
  static dmap_t
        FOREGROUND_DENSITY_MAP_Y[CAPTURE_WIDTH],
        FOREGROUND_DENSITY_MAP_X[CAPTURE_HEIGHT],
        BACKGROUND_DENSITY_MAP_Y[CAPTURE_WIDTH],
        BACKGROUND_DENSITY_MAP_X[CAPTURE_HEIGHT];
  
    void CumulateMomentsRhoUtility( floating_t, floating_t, floating_t *, floating_t *, floating_t * );
    void CalculateRegionScoreRhoUtility( region_t *, density_t, byte_t );
    density_2d_t CalculateCentroidRhoUtility( dmap_t *, index_t, index_t *, density_t );
    void PrintPacketRhoUtility( packet_t *, index_t );
    void GenerateBackgroundRhoUtility( rho_core_t * );
    
    void InitializeDataRhoUtility( rho_core_t *, index_t, index_t );
    void InitializeFiltersRhoUtility( rho_core_t * );
    void InitializePredictionRhoUtility( prediction_t *, index_t );
    void InitializeDensityMapRhoUtility( density_map_t *, index_t, index_t );

    void ResetForDetectRhoUtility( rho_detection_variables *, density_map_t *, prediction_t * );
    void ResetForPredictionRhoUtility( prediction_predict_variables *, prediction_pair_t *, index_t, index_t );

    void PredictPeakFilterRhoUtility( rho_detection_variables *, density_map_t *, prediction_t * );
    void PredictTrackingFiltersRhoUtility( prediction_t * );
    index_t CalculateValidTracksRhoUtility( prediction_t * );
    void SortTrackingFiltersRhoUtility( prediction_t * );
    void PredictTrackingProbabilitiesRhoUtility( prediction_t * );

    void PerformDetectRhoUtility( rho_detection_variables *, density_map_t *, prediction_t * );
    bool CalculateBandLowerBoundRhoUtility( rho_detection_variables * );
    void DetectRegionsRhoUtility( rho_detection_variables *, density_map_t *, prediction_t * );
    void DetectRegionRhoUtility( rho_detection_variables *, density_map_t *, prediction_t * );
    void SubtractBackgroundForDetectionRhoUtility( rho_detection_variables * );
    void CalculateChaosRhoUtility( rho_detection_variables *, prediction_t * );
    void ScoreRegionsRhoUtility( rho_detection_variables *, density_map_t *, prediction_t * );
    void SortRegionsRhoUtility( rho_detection_variables *, prediction_t * );
    void CalculatedFrameStatisticsRhoUtility( rho_detection_variables *, prediction_t * );

    void CorrectPredictionAmbiguityRhoUtility( prediction_predict_variables *, rho_core_t * );
    void RedistributeDensitiesRhoUtility( rho_core_t * );
    void CombineAxisProbabilitesRhoUtility( prediction_pair_t * );
    void UpdateCorePredictionDataRhoUtility( prediction_predict_variables *, rho_core_t * );

    void CalculateTuneRhoUtility( rho_core_t * );
    void CalculateBackgroundTuneFactorRhoUtility( rho_core_t * );
    void CalculateStateTuneFactorRhoUtility( rho_core_t * );
    void CalculateTargetTuneFactor( rho_core_t * );

    void GeneratePacketRhoUtility( rho_core_t * );

    void GenerateObservationListFromPredictionsRhoUtility( prediction_t *, uint8_t );
    void GenerateObservationListsFromPredictionsRhoUtility( rho_core_t * );

    typedef struct
    {
        void (*Data)( rho_core_t *, index_t, index_t );
        void (*Filters)( rho_core_t * );
        void (*Prediction)( prediction_t *, index_t );
        void (*DensityMap)( density_map_t *, index_t, index_t );
    } rho_utility_initializer_functions;

    typedef struct
    {
        void (*Detect)( rho_detection_variables *, density_map_t *, prediction_t * );
        void (*Prediction)( prediction_predict_variables *, prediction_pair_t *, index_t, index_t );
    } rho_utility_reset_functions;

    typedef struct
    {
        void (*PeakFilter)( rho_detection_variables *, density_map_t *, prediction_t * );
        void (*TrackingFilters)( prediction_t * );
        index_t (*CalculateValidTracks)( prediction_t * );
        void (*SortFilters)( prediction_t * );
        void (*TrackingProbabilities)( prediction_t * );
        void (*CorrectAmbiguity)( prediction_predict_variables *, rho_core_t * );
        void (*CombineProbabilities)( prediction_pair_t * );
        void (*RedistributeDensities)(  rho_core_t * );
        void (*UpdateCorePredictionData)( prediction_predict_variables *, rho_core_t * );
        void (*GenerateObservationList)( prediction_t *, uint8_t );
        void (*GenerateObservationLists)( rho_core_t * core );
    } rho_utility_predict_functions;

    typedef struct
    {
        void (*Perform)( rho_detection_variables *, density_map_t *, prediction_t * );
        bool (*LowerBound)( rho_detection_variables * );
        void (*Regions)( rho_detection_variables *, density_map_t *, prediction_t *);
        void (*Region)( rho_detection_variables *, density_map_t *, prediction_t * );
        void (*SubtractBackground)( rho_detection_variables *);
        void (*CalculateChaos)( rho_detection_variables *, prediction_t * );
        void (*ScoreRegions)( rho_detection_variables *, density_map_t *, prediction_t * );
        void (*SortRegions)( rho_detection_variables *, prediction_t * );
        void (*CalculateFrameStatistics)( rho_detection_variables *, prediction_t * );
    } rho_utility_detect_functions;

    typedef struct
    {
        void (*Tune)( rho_core_t * );
        void (*BackgroundTuneFactor)( rho_core_t * );
        void (*StateTuneFactor)( rho_core_t * );
        void (*TargetTuneFactor)( rho_core_t * );
    } rho_utility_calculate_functions;

    typedef struct
    {
        void (*CumulateMoments)( floating_t, floating_t, floating_t *, floating_t *, floating_t * );
        void (*CalculateRegionScore)( region_t *, density_t, byte_t );
        density_2d_t (*CalculateCentroid)( dmap_t *, index_t, index_t *, density_t );
        void (*PrintPacket)( packet_t *, index_t );
        void (*GenerateBackground)( rho_core_t * );
        void (*GeneratePacket)( rho_core_t * );
        rho_utility_initializer_functions Initialize;
        rho_utility_reset_functions Reset;
        rho_utility_predict_functions Predict;
        rho_utility_detect_functions Detect;
        rho_utility_calculate_functions Calculate;
    } rho_utility_functions;

    static const rho_utility_functions RhoUtility =
    {
        .CumulateMoments = CumulateMomentsRhoUtility,
        .CalculateRegionScore = CalculateRegionScoreRhoUtility,
        .CalculateCentroid = CalculateCentroidRhoUtility,
        .PrintPacket = PrintPacketRhoUtility,
        .GeneratePacket = GeneratePacketRhoUtility,

        .Initialize.Data = InitializeDataRhoUtility,
        .Initialize.Filters = InitializeFiltersRhoUtility,
        .Initialize.Prediction = InitializePredictionRhoUtility,
        .Initialize.DensityMap = InitializeDensityMapRhoUtility,

        .Reset.Detect = ResetForDetectRhoUtility,
        .Reset.Prediction = ResetForPredictionRhoUtility,

        .Predict.PeakFilter = PredictPeakFilterRhoUtility,
        .Predict.TrackingFilters = PredictTrackingFiltersRhoUtility,
        .Predict.CalculateValidTracks = CalculateValidTracksRhoUtility,
        .Predict.SortFilters = SortTrackingFiltersRhoUtility,
        .Predict.TrackingProbabilities = PredictTrackingProbabilitiesRhoUtility,
        .Predict.CorrectAmbiguity = CorrectPredictionAmbiguityRhoUtility,
        .Predict.CombineProbabilities = CombineAxisProbabilitesRhoUtility,
        .Predict.RedistributeDensities = RedistributeDensitiesRhoUtility,
        .Predict.UpdateCorePredictionData = UpdateCorePredictionDataRhoUtility,
        .Predict.GenerateObservationList = GenerateObservationListFromPredictionsRhoUtility,
        .Predict.GenerateObservationLists = GenerateObservationListsFromPredictionsRhoUtility,

        .Detect.Perform = PerformDetectRhoUtility,
        .Detect.LowerBound = CalculateBandLowerBoundRhoUtility,
        .Detect.Regions = DetectRegionsRhoUtility,
        .Detect.Region = DetectRegionRhoUtility,
        .Detect.SubtractBackground = SubtractBackgroundForDetectionRhoUtility,
        .Detect.CalculateChaos = CalculateChaosRhoUtility,
        .Detect.ScoreRegions = ScoreRegionsRhoUtility,
        .Detect.SortRegions = SortRegionsRhoUtility,
        .Detect.CalculateFrameStatistics = CalculatedFrameStatisticsRhoUtility,

        .Calculate.Tune = CalculateTuneRhoUtility,
        .Calculate.BackgroundTuneFactor = CalculateBackgroundTuneFactorRhoUtility,
        .Calculate.StateTuneFactor = CalculateStateTuneFactorRhoUtility,
        .Calculate.TargetTuneFactor = CalculateTargetTuneFactor
    };

#ifdef __cplusplus
}
#endif

#endif
