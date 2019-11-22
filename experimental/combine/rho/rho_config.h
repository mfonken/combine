/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  File: rho_config.h
 *  Group: Rho Core
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef rho_config_h
#define rho_config_h

#include "rho_global.h"

//#define SPOOF_STATE_BANDS
#define USE_DETECTION_MAP
#define RHO_DRAWER

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                               CAPTUE PARAMETERS                                     */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#define IS_RGGB_ELIMINATE_G     true

#define __USE_RUNNING_AVERAGE__

#ifdef __CAM__
/* Camera Config */
#define RHO_WIDTH               1920
#define RHO_HEIGHT              1080

#define CAPTURE_DIV             4

#define CAPTURE_SUB_SAMPLE      SUBSAMPLE_APPLICATION
#else
#define CAPTURE_DIV             0
#define RHO_WIDTH               FRAME_WIDTH
#define RHO_HEIGHT              FRAME_HEIGHT
#endif

#define CAPTURE_WIDTH           (RHO_WIDTH>>CAPTURE_DIV)
#define CAPTURE_HEIGHT          (RHO_HEIGHT>>CAPTURE_DIV)
#define FRAME_SIZE              (CAPTURE_WIDTH*CAPTURE_HEIGHT)

#if defined __linux || defined __APPLE__
#ifdef __CAM__
#define DENSITY_MAP_X_SIZE       CAPTURE_WIDTH
#define DENSITY_MAP_Y_SIZE       CAPTURE_HEIGHT
#else
#define DENSITY_MAP_X_SIZE       RHO_WIDTH
#define DENSITY_MAP_Y_SIZE       RHO_HEIGHT
#endif
#else
#define DENSITY_MAP_X_SIZE       DENSITY_MAP_X_LENGTH
#define DENSITY_MAP_Y_SIZE       DENSITY_MAP_Y_LENGTH
#endif

#define CAPTURE_BUFFER_WIDTH    (uint32_t)CAPTURE_WIDTH
#define CAPTURE_BUFFER_HEIGHT   (uint32_t)CAPTURE_HEIGHT
#define CWL                     80 // CAPTURE_BUFFER_WIDTH
#define CAPTURE_BUFFER_SIZE     CAPTURE_BUFFER_LENGTH

#define SS                      6 // DEFAULT_SUBSAMPLE

#define COVERAGE_NORMAL_MAX     0.45
#define THRESH_BUFFER_SIZE      THRESH_BUFFER_LENGTH
#define THRESH_BUFFER_MAX       THRESH_BUFFER_LENGTH//((index_t)(sizeof(index_t)*(index_t)THRESH_BUFFER_SIZE))
#define DEFAULT_THRESH          THRESH_MIN// 170//250

#define FILTERED_COVERAGE_TARGET   0.007
#define MAX_COVERAGE            1
#define C_FRAME_SIZE            ((int)(MAX_COVERAGE * FRAME_SIZE))
#define Y_DEL                   0xaaaa

//#define USE_STATIC_BACKGROUNDING    

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                                RHO PARAMETERS                                       */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#define THRESH_STEP_MAX     5
#define THRESH_MIN          10
#define THRESH_MAX          250

#define MIN_VARIANCE        1
#define MAX_VARIANCE        100

#define MAX_REGION_HEIGHT   700//200
#define RHO_GAP_MAX         5

#define BACKGROUND_CENTROID_CALC_THRESH 10 // pixels

#ifdef __PSM__
#define PSM_UPDATE_PERIOD     0.5
#endif
#define BACKGROUNDING_PERIOD  100000 // Frames

#define EXPECTED_NUM_REGIONS  2
#define MAX_REGIONS           4
#define MIN_REGION_DENSITY    2
#define MAX_REGION_SCORE      10
#define REGION_SCORE_FACTOR   0.5
#define MAX_NU_REGIONS        NUM_STATE_GROUPS+1
#define MAX_OBSERVATIONS      (1 << 4)
#define MIN_CHAOS             1.0

#define MAX_RHO_RECALCULATION_LEVEL 3

#define MAX_TRACKING_FILTERS MAX_REGIONS
#define MIN_TRACKING_KALMAN_SCORE 0.02
#define MAX_TRACKING_MATCH_DIFFERENCE 500
#define TRACKING_MATCH_TRUST 0.4

#define TARGET_TUNE_FACTOR  1.0
#define STATE_TUNE_FACTOR   0.2

#define PIXEL_COUNT_TRUST_FACTOR    0.2
#define PIXEL_COUNT_DROP_FACTOR     0.75

#define FRAME_QUADRANT_TOP_LEFT_INDEX   0
#define FRAME_QUADRANT_TOP_RIGHT_INDEX  1
#define FRAME_QUADRANT_BTM_LEFT_INDEX   2
#define FRAME_QUADRANT_BTM_RIGHT_INDEX  3

#define TOTAL_RHO_PIXELS    ( CAPTURE_WIDTH * CAPTURE_HEIGHT )

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                              FILTER PARAMETERS                                      */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/* Kalman Filter Configs */
#define RHO_K_TARGET        0.02
#define RHO_VARIANCE_NORMAL 20.
#define RHO_VARIANCE_SCALE  10.
#define RHO_VARIANCE(X)     RHO_VARIANCE_NORMAL * ( 1 + RHO_VARIANCE_SCALE * ( RHO_K_TARGET - X ) )

#define RHO_DEFAULT_LS      5.          // Lifespan
#define RHO_DEFAULT_VU      0.001       // Value uncertainty
#define RHO_DEFAULT_BU      0.1         // Bias uncertainty
#define RHO_DEFAULT_SU      0.02        // Sensor uncertainty
#define DEFAULT_KALMAN_UNCERTAINTY \
(kalman_uncertainty_c){ RHO_DEFAULT_VU, RHO_DEFAULT_BU, RHO_DEFAULT_SU }

#define RHO_PREDICTION_LS   1.
#define RHO_PREDICTION_VU   0.005
#define RHO_PREDICTION_BU   0.001
#define RHO_PREDICTION_SU   0.001
#define DEFAULT_PREDICTION_UNCERTAINTY \
(kalman_uncertainty_c){ RHO_PREDICTION_VU, RHO_PREDICTION_BU, RHO_PREDICTION_SU }

#define RHO_TARGET_LS       5.
#define RHO_TARGET_VU       0.001
#define RHO_TARGET_BU       0.001
#define RHO_TARGET_SU       0.025
#define DEFAULT_TARGET_UNCERTAINTY \
(kalman_uncertainty_c){ RHO_TARGET_VU, RHO_TARGET_BU, RHO_TARGET_SU }
#define RHO_TARGET_FILTER_MAX   0.5
#define RHO_TARGET_FILTER_MIN   0.001

#define MIN_STATE_CONFIDENCE        0.01 //0.5
#define BACKGROUND_PERCENT_MIN      0.02
#define BACKGROUND_COVERAGE_MIN     ((int)(BACKGROUND_PERCENT_MIN*FRAME_SIZE))
#define BACKGROUND_TUNE_MAX         2
#define BACKGROUND_TUNE_EXPONENT    3
//#define BACKGROUND_COVERAGE_TOL_PR   0.001
//#define BACKGROUND_COVERAGE_TOL_PX   ((int)(BACKGROUND_COVERAGE_TOL_PR*FRAME_SIZE))

#define DEFAULT_KUMARASWAMY_BANDS   { 0.25, 0.5, 0.75, 1.0 }

#ifdef __PSM__
#define USE_2D_OBSERVATIONS

#ifdef USE_2D_OBSERVATIONS
#define DEFAULT_OBSERVATION_LIST \
{ \
    { { 0.5, 225 },  { 0.5, 0., 0., 10. }, 0. }, \
    { { 1.0, 175 },  { 0.5, 0., 0., 10. }, 0. }, \
    { { 2.0, 100 },  { 0.5, 0., 0., 10. }, 0. }, \
    { { 4.0,  40 },  { 0.5, 0., 0., 10. }, 0. }  \
}
#else
#define DEFAULT_OBSERVATION_LIST \
{ \
    { 0.5, 0.5 }, \
    { 1.0, 0.5 }, \
    { 2.0, 0.5 }, \
    { 4.0, 0.5 } \
}
#endif
#define DEFAULT_STATE_VECTOR        { 0.25, 0.5, 0.2, 0.05 }
#else
#define SPOOF_STATE_BANDS           { 0.2, 0.5, 0.75, 1.0 }
#endif

#define DEFAULT_PACKET_LENGTH       3

#endif /* rho_config_h */
