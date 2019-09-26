/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  File: rho_types.c
 *  Group: Rho Core
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef rho_c_types_h
#define rho_c_types_h

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*                                    Includes                                         */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __PSM__
#include "psm.h"
#else
#include "fsm.h"
#endif

#include "rho_packet.h"
#include "kalman.h"
#include "pid.h"

//#ifndef MAX_OBSERVATIONS
//#define MAX_OBSERVATIONS        (1 << 3) // Length of history
//#endif

typedef struct
{
    index_t x,y;
} index_pair_t;

typedef struct
{
density_t
    maximum,
    density;
index_t
    location,
    width,
    sort;
floating_t
    score;
byte_t
    tracking_id;
} region_t;

/* Rho Structures* */
typedef struct
{
    density_map_unit_t
        *map,
        *background,
        max[2];
    index_t
        length,
        centroid;
    bool
        has_background;
    kalman_filter_t
        kalmans[2];
    const char *
        name;
} density_map_t;

typedef struct
{
    density_map_t x,y;
} density_map_pair_t;

typedef struct
{
    byte_t
        background[4][4],
        current[4][4],
        factor[4][4],
        length[4];
} density_redistribution_lookup_config_t;

typedef struct
{
    density_redistribution_lookup_config_t config[4];
} density_redistribution_lookup_t;

typedef struct
{
    floating_t  P[NUM_STATES],
                confidence;
} prediction_probabilities;

typedef struct
{
    kalman_filter_t    TrackingFilters[MAX_TRACKING_FILTERS];
    uint8_t         TrackingFiltersOrder[MAX_TRACKING_FILTERS];
    region_t        Regions[MAX_REGIONS];
    uint8_t         RegionsOrder[MAX_REGIONS],
                    NumRegions;
    floating_t      NuRegions,
                    Primary,
                    Secondary,
                    AverageDensity;
    density_t       PreviousPeak[2],
                    PreviousCentroid;
    density_2d_t    PreviousDensity[2],
                    TotalDensity,
                    FilterDensity[2];
    observation_list_t ObservationList;
    prediction_probabilities Probabilities;
    const char *    Name;
} prediction_t;

typedef struct
{
    prediction_t    x,y;
    prediction_probabilities Probabilities;

    floating_t      NuRegions,
//                    BestConfidence,
                    AverageDensity;
} prediction_pair_t;

typedef struct
{
    psm_t x,y;
    uint8_t current_state;
    double
        best_confidence,
        proposed_nu,
        proposed_avg_den;
} psm_pair_t;

typedef struct
{
    density_2d_t left, right;       /* Direction density values */
    bool complete;
} section_process_t;

typedef struct
{
  void (*Init)(void);
  void (*Pause)(void);
  void (*Resume)(void);
  void (*Reset)(void);
} platform_dma_interface_functions;

typedef struct
{
  uint8_t (*Transmit)( byte_t *, index_t);
} rho_platform_uart_interace_functions;

#ifndef USE_DECOUPLING
typedef struct
{
  void (*Activate)( camera_application_flags * );
} rho_platform_flag_interace_functions;
#endif

typedef struct
{
  uint32_t (*Now)( void );
} rho_platform_time_interace_functions;

typedef struct
{
    index_t xl[3], yl[3];
    density_2d_t area[9];
    density_2d_t a, b, c, d, l, l_, p, q, x, y;
} redistribution_variables;
extern const density_redistribution_lookup_t rlookup;

typedef struct
{
    floating_t
        background,
        state,
        target,
        proposed;
} rho_tune_t;

typedef struct
{
    index_t
      len,
      range[3],
      cycle,
      cycle_,
      gap_counter,
      width,
      region_total,            /* Region fill */
      x,               /* Generic index */
      start,
      end;
    density_t
      filter_peak,
      filter_peak_2,
      filter_band_lower,
      curr,
      background_curr,
      recalculation_counter;
    variance_t
      filter_variance;
    density_2d_t
      maximum,
      current_density,
      total_density,
      filtered_density,
      first_filtered_density; /* Initial filtered density before retries */
    bool
      has_region,
      recalculate;
    floating_t
      average_counter,
      average_curr,   /* cumulative average */
      average_moment,   /* moment average */
      chaos,
      target_density,
      assumed_regions;
} rho_detection_variables;

typedef struct
{
    index_pair_t
        Primary,
        Secondary,
        Centroid;
    int8_t quadrant_check;
} prediction_predict_variables;

typedef struct
{
    packet_t *packet;
    address_t pdPtr, llPtr, *alPtr;
    byte_t includes, i, j, l, t;
} packet_generation_variables;

#define DETECTION_BUFFER_SIZE ( 1 << 9 )
#define DETECTION_BUFFER_MASK ( DETECTION_BUFFER_SIZE - 1 )
#define MAX_DENSITY           ( 1 << 9 )

typedef struct
{
    uint8_t thresh;
    index_t density;
    uint8_t tracking_id;
} detection_element_t;

typedef struct
{
    uint16_t index, fill, length, first;
    detection_element_t buffer[DETECTION_BUFFER_SIZE];
} detection_ring_buffer_t;

typedef detection_ring_buffer_t detection_map_t;

typedef struct
{
    density_map_pair_t  DensityMapPair;
    index_t
        Width,
        Height,
        Subsample,
        RowsLeft;
    index_pair_t
        Primary,
        Secondary,
        Centroid,
        BackgroundCentroid;
//        Px, Py, Sx, Sy, Cx, Cy, Bx, By;
    byte_t
        ThreshByte,
        BackgroundCounter;
    density_2d_t
        Q[4], Qb[4], Qf[4], QbT,
        TotalCoverage,
        FilteredCoverage,
        TargetCoverage,
        BackgroundPeriod;
    floating_t
        TotalPercentage,
        FilteredPercentage,
        TargetCoverageFactor,
        CoverageFactor,
        VarianceFactor,
        PreviousThreshFilterValue,
        Thresh;
    rho_tune_t          Tune;
    prediction_pair_t   PredictionPair;
    pid_filter_t        ThreshFilter;
    kalman_filter_t     TargetFilter;
    detection_map_t     DetectionMap;

#ifdef __PSM__
    psm_pair_t          PredictiveStateModelPair;
#else
    kumaraswamy_t       Kumaraswamy;
    fsm_system_t        StateMachine;
#endif
    packet_t            Packet;

#ifdef USE_DECOUPLING
    uint8_t             cframe[C_FRAME_SIZE];
#endif
    
    double             Timestamp;
} rho_core_t;

#endif /* rho_c_types_h */
