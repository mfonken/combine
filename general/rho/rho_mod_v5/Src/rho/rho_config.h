//
//  rho_config.h
//
//  Created by Matthew Fonken on 9/19/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_config_h
#define rho_config_h

#include "rho_global.h"

#define __PCR__
//#define __PSM__

//#define SPOOF_STATE_BANDS

/* Camera Config */
#define RHO_WIDTH               1280
#define RHO_HEIGHT              800

#define BACKGROUNDING_PERIOD   10000 // Frames

#define FILTERED_COVERAGE_TARGET   0.007
//#define FILTERED_COVERAGE_PIXELS   ((index_t)(FILTERED_COVERAGE_TARGET*FRAME_SIZE))

/* Kalman Filter Configs */
#define RHO_DEFAULT_LS      5.
#define RHO_DEFAULT_VU      0.001
#define RHO_DEFAULT_BU      0.001
#define RHO_DEFAULT_SU      0.01
#define DEFAULT_KALMAN_UNCERTAINTY \
(rho_kalman_uncertainty_c){ RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU }

#define RHO_PREDICTION_LS   1.
#define RHO_PREDICTION_VU   0.05
#define RHO_PREDICTION_BU   0.001
#define RHO_PREDICTION_SU   0.05
#define DEFAULT_PREDICTION_UNCERTAINTY \
(rho_kalman_uncertainty_c){ RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU }

#define RHO_TARGET_LS       5.
#define RHO_TARGET_VU       0.001
#define RHO_TARGET_BU       0.05
#define RHO_TARGET_SU       0.0001
#define DEFAULT_TARGET_UNCERTAINTY \
(rho_kalman_uncertainty_c){ RHO_TARGET_VU, RHO_TARGET_BU, RHO_TARGET_SU }

//#define RHO_SQRT_HEIGHT     sqrt(CAPTURE_HEIGHT)
//#define RHO_DIM_INFLUENCE   0.1
//#define RHO_K_TARGET_IND    0.3
#define RHO_K_TARGET        10//0.12//RHO_K_TARGET_IND+(10/RHO_SQRT_HEIGHT*RHO_DIM_INFLUENCE)
#define RHO_VARIANCE_NORMAL 20.//RHO_SQRT_HEIGHT/5.0
#define RHO_VARIANCE_SCALE  10.//RHO_SQRT_HEIGHT/3.0//1.32
#define RHO_VARIANCE(X)     RHO_VARIANCE_NORMAL * ( 1 + RHO_VARIANCE_SCALE * ( RHO_K_TARGET - X ) )

/* PID Filter Configs */
#define PID_SCALE   1//10
#define PID_DRIFT   1.1

//#define STRICT_PROPORTIONAL_FACTOR 0.98
//#define STRICT_INTEGRAL_FACTOR     0.02
//#define STRICT_DERIVATIVE_FACTOR   0.001
#define DEFAULT_PROPORTIONAL_FACTOR 0.5
#define DEFAULT_INTEGRAL_FACTOR     0.002
#define DEFAULT_DERIVATIVE_FACTOR   0.001
#define DEFAULT_PID_GAIN (rho_pid_gain_t){ DEFAULT_PROPORTIONAL_FACTOR, DEFAULT_INTEGRAL_FACTOR, DEFAULT_DERIVATIVE_FACTOR }

/* Capture Config */
//#define DYNAMIC_BUFFER
#define CAPTURE_DIV             4
#define CAPTURE_WIDTH           (RHO_WIDTH>>CAPTURE_DIV)
#define CAPTURE_HEIGHT          (RHO_HEIGHT>>CAPTURE_DIV)
#define FRAME_SIZE              (CAPTURE_WIDTH*CAPTURE_HEIGHT)

//#ifndef USE_INTERRUPT_MODEL
#define CAPTURE_BUFFER_WIDTH    (uint32_t)CAPTURE_WIDTH
#define CAPTURE_BUFFER_HEIGHT   (uint32_t)CAPTURE_HEIGHT
#ifdef DYNAMIC_BUFFER
#define CAPTURE_BUFFER_SIZE     CAPTURE_BUFFER_WIDTH    //*CAPTURE_BUFFER_HEIGHT
#else
#define CAPTURE_BUFFER_SIZE     (CAPTURE_BUFFER_WIDTH*CAPTURE_BUFFER_HEIGHT)
//#endif

#define COVERAGE_NORMAL_MAX     0.45
#define THRESH_BUFFER_SIZE      1//((uint32_t)((CAPTURE_BUFFER_WIDTH*(CAPTURE_BUFFER_HEIGHT+1.))*COVERAGE_NORMAL_MAX)+2)
#define THRESH_BUFFER_MAX       ((index_t)(sizeof(index_t)*(index_t)(THRESH_BUFFER_SIZE-CAPTURE_HEIGHT)))
#define DEFAULT_THRESH          0xfa
#endif

#define MAX_COVERAGE            1
#define C_FRAME_SIZE            ((int)(MAX_COVERAGE * FRAME_SIZE))
#define Y_DEL                   0xaaaa

#define MIN_STATE_CONFIDENCE    0.5
#define BACKGROUND_PERCENT_MIN  0.02
#define BACKGROUND_COVERAGE_MIN ((int)(BACKGROUND_PERCENT_MIN*FRAME_SIZE))
//#define BACKGROUND_COVERAGE_TOL_PR   0.001
//#define BACKGROUND_COVERAGE_TOL_PX   ((int)(BACKGROUND_COVERAGE_TOL_PR*FRAME_SIZE))

#define BACKGROUND_CENTROID_CALC_THRESH 10 // pixels

#define RHO_GAP_MAX 10

#define BACKGROUND_CENTROID_CALC_THRESH 10 // pixels

//#define FRAME_ROW_DELIMETER             0xabab //
//#define FRAME_ROW_END                   0xef //

#define THRESH_STEP_MAX     5
#define THRESH_MIN          1
#define THRESH_MAX          254

#define MIN_VARIANCE        3
#define MAX_VARIANCE        20

#define MAX_BLOB_HEIGHT     200

#define EXPECTED_NUM_BLOBS  2
#define MAX_BLOBS           4
#define MIN_BLOB_DENSITY    2
#define MAX_BLOB_SCORE      10
#define BLOB_SCORE_FACTOR   0.5
#define MAX_NU_BLOBS        NUM_STATE_GROUPS+1
#define MAX_OBSERVATIONS    1 << 4

#define MAX_RHO_RECALCULATION_LEVEL 0

#define MAX_TRACKING_FILTERS MAX_BLOBS
#define MIN_TRACKING_KALMAN_SCORE 0.02
#define MAX_TRACKING_MATCH_DIFFERNCE 500
#define TRACKING_MATCH_TRUST 0.4

#define TARGET_TUNE_FACTOR  1.0
#define STATE_TUNE_FACTOR   0.2

#define PIXEL_COUNT_TRUST_FACTOR   0.2
#define PIXEL_COUNT_DROP_FACTOR    0.75

#endif /* rho_config_h */
