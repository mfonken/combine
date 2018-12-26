//
//  global_config.h
//  rho_client
//
//  Created by Matthew Fonken on 9/19/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef global_config_h
#define global_config_h

#define __PCR__

/* Camera Config */
#define RHO_WIDTH        1280
#define RHO_HEIGHT       800

#define BACKGROUND_PERIOD   10 // Frames
#define RHO_PUNISH_FACTOR   1

#define PIXEL_COUNT_TRUST_FACTOR   0.2
#define PIXEL_COUNT_DROP_FACTOR    0.75

#define ALTERNATE_TUNING_FACTOR    0.5
#define FILTERED_COVERAGE_TARGET   0.007
#define FILTERED_COVERAGE_PIXELS   ((index_t)(FILTERED_COVERAGE_TARGET*FRAME_SIZE))

#define MAX_ABSENCE_PROBABIILITY   0.5

#define TUNING_PRESIDENCE       0.75
#define HARD_TUNING_PRESIDENCE  TUNING_PRESIDENCE
#define SOFT_TUNING_PRESIDENCE  (1 - TUNING_PRESIDENCE)

#define RHO_PREDICTION_LS   1.
#define RHO_PREDICTION_VU   0.05
#define RHO_PREDICTION_BU   0.001
#define RHO_PREDICTION_SU   0.1

#define RHO_DEFAULT_LS      5.
#define RHO_DEFAULT_VU      0.001
#define RHO_DEFAULT_BU      0.001
#define RHO_DEFAULT_SU      0.01

#define RHO_THRESH_LS       10.
#define RHO_THRESH_VU       0.5
#define RHO_THRESH_BU       0.01
#define RHO_THRESH_SU       0.05

#define RHO_TARGET_LS       5.
#define RHO_TARGET_VU       0.001
#define RHO_TARGET_BU       0.05
#define RHO_TARGET_SU       0.9

#define DYNAMIC_BUFFER

/* Capture Config */
#define CAPTURE_DIV             4
#define CAPTURE_WIDTH           (RHO_WIDTH>>CAPTURE_DIV)
#define CAPTURE_HEIGHT          (RHO_HEIGHT>>CAPTURE_DIV)
#define FRAME_SIZE              (CAPTURE_WIDTH*CAPTURE_HEIGHT)
#define CAPTURE_BUFFER_WIDTH    (uint32_t)CAPTURE_WIDTH
#define CAPTURE_BUFFER_HEIGHT   (uint32_t)CAPTURE_HEIGHT
#ifdef DYNAMIC_BUFFER
#define CAPTURE_BUFFER_SIZE     CAPTURE_BUFFER_WIDTH    //*CAPTURE_BUFFER_HEIGHT
#else
#define CAPTURE_BUFFER_SIZE     (CAPTURE_BUFFER_WIDTH*CAPTURE_BUFFER_HEIGHT)
#endif

#define COVERAGE_NORMAL_MAX     0.45
#define THRESH_BUFFER_SIZE      1//((uint32_t)((CAPTURE_BUFFER_WIDTH*(CAPTURE_BUFFER_HEIGHT+1.))*COVERAGE_NORMAL_MAX)+2)
#define THRESH_BUFFER_MAX       ((index_t)(sizeof(index_t)*(index_t)(THRESH_BUFFER_SIZE-CAPTURE_HEIGHT)))
#define DEFAULT_THRESH          0xfa

#define MAX_COVERAGE            1
#define C_FRAME_SIZE            ((int)(MAX_COVERAGE * FRAME_SIZE))
#define Y_DEL                   0xaaaa

#define BACKGROUND_PERCENT_MIN  0.02
#define BACKGROUND_COVERAGE_MIN ((int)(BACKGROUND_PERCENT_MIN*FRAME_SIZE))
#define BACKGROUND_COVERAGE_TOL_PR   0.001
#define BACKGROUND_COVERAGE_TOL_PX   ((int)(BACKGROUND_COVERAGE_TOL_PR*FRAME_SIZE))

#define BACKGROUND_CENTROID_CALC_THRESH 10 // pixels

#define RHO_SQRT_HEIGHT     sqrt(CAPTURE_HEIGHT)
#define RHO_DIM_INFLUENCE   0.1
#define RHO_K_TARGET_IND    0.3
#define RHO_K_TARGET        0.4//RHO_K_TARGET_IND+(10/RHO_SQRT_HEIGHT*RHO_DIM_INFLUENCE)
#define RHO_VARIANCE_NORMAL 5.//RHO_SQRT_HEIGHT/5.0
#define RHO_VARIANCE_SCALE  8.//RHO_SQRT_HEIGHT/3.0//1.32
#define RHO_VARIANCE(X)     RHO_VARIANCE_NORMAL * ( 1 + RHO_VARIANCE_SCALE * ( RHO_K_TARGET - X ) )

#define RHO_PUNISH(X)       ((X)<<1)

#define RHO_GAP_MAX 2//10

#define FILTERED_CONVERAGE_TARGET       0.002

#define MAX_ABSENCE_PROBABILITY         0.5

#define BACKGROUND_CENTROID_CALC_THRESH 10 // pixels

#define FRAME_ROW_DELIMETER             0xabab
#define FRAME_ROW_END                   0xef

#define THRESH_STEP_MAX     10
#define THRESH_STEP_BASE    6
#define THRESH_STEP         1
#define THRESH_MIN          1
#define THRESH_MAX          255

#define MIN_VARIANCE        3
#define MAX_VARIANCE        40

#define MAX_BLOBS           10
#define MIN_BLOB_DENSITY    5
#define MAX_BLOB_SCORE      4
#define BLOB_SCORE_FACTOR   0.5

#define MAX_RHO_RECALCULATION_LEVEL 0

#define MAX_TRACKING_FILTERS 4
#define MIN_TRACKING_KALMAN_SCORE 0.02

#define PID_SCALE   10

#endif /* global_config_h */
