//
//  rho_utility.h
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_utility_h
#define rho_utility_h

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "timestamp.h"

#include "rho_structure.h"

#include "rho_types.h"

#ifdef __PCR__
#include "rho_interrupt_model.h"
#else
//#include "rho_client.h"
#endif

#define KUMARASWAMY_CDF(X,A,B) (1 - pow( 1 - pow( X, A ), B))

#ifdef __cplusplus
extern "C" {
#endif
    
    void CumulateMomentsRhoUtility( floating_t, floating_t, floating_t *, floating_t *, floating_t * );
    void CalculateBlobScoreRhoUtility( blob_t *, density_t, byte_t );
    floating_t CalculateCentroidRhoUtility( density_t *, index_t, index_t *, density_t );
    void PrintPacketRhoUtility( packet_t *, index_t );
    void GenerateBackgroundRhoUtility( rho_core_t * );
    
    void InitializeDensityMapRhoUtility( density_map_t *, index_t );
    
    void ResetForDetectRhoUtility( rho_detection_variables *, density_map_t *, prediction_t * );
    void ResetForPredictionRhoUtility( prediction_predict_variables *, prediction_pair_t *, index_t, index_t );
    
    void PredictPeakFilterRhoUtility( rho_detection_variables *, density_map_t *, prediction_t * );
    void PredictTrackingFiltersRhoUtility( prediction_t * );
    index_t CalculateValidTracksRhoUtility( prediction_t * );
    void SortTrackingFiltersRhoUtility( prediction_t * );
    void PredictTrackingProbabilitiesRhoUtility( prediction_t * );
    
    void PerformDetectRhoUtility( rho_detection_variables *, density_map_t *, prediction_t * );
    bool CalculateBandLowerBoundRhoUtility( rho_detection_variables * );
    void DetectBlobsRhoUtility( rho_detection_variables *, density_map_t *, prediction_t * );
    void DetectBlobRhoUtility( rho_detection_variables *, density_map_t *, prediction_t * );
    void SubtractBackgroundForDetectionRhoUtility( rho_detection_variables *, density_map_t * );
    void CalculateDetectionChaosRhoUtility( rho_detection_variables *, prediction_t *  );
    void ScoreBlobsRhoUtility( rho_detection_variables *, density_map_t *, prediction_t * );
    void SortBlobsRhoUtility( rho_detection_variables *, prediction_t * );
    
    void CorrectPredictionAmbiguityRhoUtility( prediction_predict_variables *, rho_core_t * );
    void CombineAxisProbabilitesRhoUtility( prediction_pair_t * );
    void RedistributeDensitiesRhoUtility( rho_core_t * );
    
    typedef struct
    {
        void (*DensityMap)( density_map_t *, index_t );
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
    } rho_utility_predict_functions;
    
    typedef struct
    {
        void (*Perform)( rho_detection_variables *, density_map_t *, prediction_t * );
        bool (*LowerBound)( rho_detection_variables * );
        void (*Blobs)( rho_detection_variables *, density_map_t *, prediction_t *);
        void (*Blob)( rho_detection_variables *, density_map_t *, prediction_t * );
        void (*SubtractBackground)( rho_detection_variables *, density_map_t * );
        void (*CalculateChaos)( rho_detection_variables *, prediction_t * );
        void (*ScoreBlobs)( rho_detection_variables *, density_map_t *, prediction_t * );
        void (*SortBlobs)( rho_detection_variables *, prediction_t * );
    } rho_utility_detect_functions;
    
    typedef struct
    {
        void (*CumulateMoments)( floating_t, floating_t, floating_t *, floating_t *, floating_t * );
        void (*CalculateBlobScore)( blob_t *, density_t, byte_t );
        floating_t (*CalculateCentroid)( density_t *, index_t, index_t *, density_t );
        void (*PrintPacket)( packet_t *, index_t );
        void (*GenerateBackground)( rho_core_t * );
        rho_utility_initializer_functions Initialize;
        rho_utility_reset_functions Reset;
        rho_utility_predict_functions Predict;
        rho_utility_detect_functions Detect;
    } rho_utility_functions;
    
    static const rho_utility_functions RhoUtility =
    {
        .CumulateMoments = CumulateMomentsRhoUtility,
        .CalculateBlobScore = CalculateBlobScoreRhoUtility,
        .CalculateCentroid = CalculateCentroidRhoUtility,
        .PrintPacket = PrintPacketRhoUtility,
        
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
        
        .Detect.Perform = PerformDetectRhoUtility,
        .Detect.LowerBound = CalculateBandLowerBoundRhoUtility,
        .Detect.Blobs = DetectBlobsRhoUtility,
        .Detect.Blob = DetectBlobRhoUtility,
        .Detect.SubtractBackground = SubtractBackgroundForDetectionRhoUtility,
        .Detect.CalculateChaos = CalculateDetectionChaosRhoUtility,
        .Detect.ScoreBlobs = ScoreBlobsRhoUtility,
        .Detect.SortBlobs = SortBlobsRhoUtility
    };
    
#ifdef __cplusplus
}
#endif

#endif

