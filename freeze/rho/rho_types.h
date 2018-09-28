//
//  rho_c_types.h
//  tau+
//
//  Created by Matthew Fonken on 4/22/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_c_types_h
#define rho_c_types_h

#include <stdint.h>
#include <stdbool.h>
#include "global_config.h"
#include "rho_kalman.h"

//#define USE_SHORTHAND_TYPES

#ifdef USE_SHORTHAND_TYPES
typedef uint8_t   uh;
typedef uint16_t  uw;
typedef uint32_t  ul;
typedef int8_t    sh;
typedef int16_t   sw;
typedef int32_t   sl;
#endif

typedef double          floating_t;
typedef uint8_t         byte_t;
typedef uint16_t        index_t;
typedef uint16_t        density_t;
typedef int16_t         variance_t;
typedef uint32_t        density_2d_t;
typedef void *          address_t;
typedef floating_t      timestamp_t;
typedef uint8_t         capture_t;
typedef volatile bool   flag_t;

#define byte_t_max          ( (sizeof(byte_t)       << 3 ) - 1 )
#define index_t_max         ( (sizeof(index_t)      << 3 ) - 1 )
#define density_t_max       ( (sizeof(density_t)    << 3 ) - 1 )
#define density_2d_t_max    ( (sizeof(density_2d_t) << 3 ) - 1 )

#define RHO_PUNISH_FACTOR   1

#define ALTERNATE_TUNING_FACTOR    0.5
#define FILTERED_COVERAGE_TARGET   0.004
#define FILTERED_COVERAGE_PIXELS   ((index_t)(FILTERED_COVERAGE_TARGET*FRAME_SIZE))

#define MAX_ABSENCE_PROBABIILITY   0.5

#define TUNING_PRESIDENCE       0.75
#define HARD_TUNING_PRESIDENCE  TUNING_PRESIDENCE
#define SOFT_TUNING_PRESIDENCE  (1 - TUNING_PRESIDENCE)

#define RHO_PREDICTION_LS   1.0
#define RHO_PREDICTION_VU   0.05
#define RHO_PREDICTION_BU   0.001
#define RHO_PREDICTION_SU   0.01

#define RHO_DEFAULT_LS      5
#define RHO_DEFAULT_VU      0.001
#define RHO_DEFAULT_BU      0.5
#define RHO_DEFAULT_SU      0.7

#define RHO_THRESH_LS      10
#define RHO_THRESH_VU      0.5
#define RHO_THRESH_BU      0.01
#define RHO_THRESH_SU      0.05

#define RHO_TARGET_LS      5
#define RHO_TARGET_VU      0.5
#define RHO_TARGET_BU      0.01
#define RHO_TARGET_SU      0.05

#define DYNAMIC_BUFFER

/* Capture Config */
#define CAPTURE_DIV             4
#define CAPTURE_WIDTH           (CAMERA_WIDTH>>CAPTURE_DIV)
#define CAPTURE_HEIGHT          (CAMERA_HEIGHT>>CAPTURE_DIV)
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
#define RHO_K_TARGET        RHO_K_TARGET_IND+(10/RHO_SQRT_HEIGHT*RHO_DIM_INFLUENCE)          //0.3
#define RHO_VARIANCE_NORMAL RHO_SQRT_HEIGHT/5.0             //5
#define RHO_VARIANCE_SCALE  RHO_SQRT_HEIGHT/3.0//1.32        //20
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
#define THRESH_MIN          0
#define THRESH_MAX          255

/* Packet Generation Settings */
#define YES 1
#define NO  0

#define PACKET_HEADER_SIZE (sizeof(packet_header_t));
#define PACKET_HEADER_ID    0xab

#define PACKET_DATA_DECIMAL       floating_t
#define PACKET_DATA_INTEGER       uint8_t
#define PACKET_DATA_DECIMAL_SIZE  ((byte_t)sizeof(PACKET_DATA_DECIMAL))
#define PACKET_DATA_INTEGER_SIZE  ((byte_t)sizeof(PACKET_DATA_INTEGER))

#define PACKET_PX_DATA_OFFSET 0
#define PACKET_PY_DATA_OFFSET 1
#define PACKET_SX_DATA_OFFSET 2
#define PACKET_SY_DATA_OFFSET 3
#define PACKET_PP_DATA_OFFSET 4
#define PACKET_SP_DATA_OFFSET 5
#define PACKET_AP_DATA_OFFSET 6

#define PACKET_PX_DATA_SIZE PACKET_DATA_DECIMAL_SIZE
#define PACKET_PY_DATA_SIZE PACKET_DATA_DECIMAL_SIZE
#define PACKET_SX_DATA_SIZE PACKET_DATA_DECIMAL_SIZE
#define PACKET_SY_DATA_SIZE PACKET_DATA_DECIMAL_SIZE
#define PACKET_PP_DATA_SIZE PACKET_DATA_DECIMAL_SIZE
#define PACKET_SP_DATA_SIZE PACKET_DATA_DECIMAL_SIZE
#define PACKET_AP_DATA_SIZE PACKET_DATA_DECIMAL_SIZE

#define INCLUDE_PACKET_PX_DATA   YES
#define INCLUDE_PACKET_PY_DATA   YES
#define INCLUDE_PACKET_SX_DATA   YES
#define INCLUDE_PACKET_SY_DATA   YES
#define INCLUDE_PACKET_PP_DATA   YES
#define INCLUDE_PACKET_SP_DATA   YES
#define INCLUDE_PACKET_AP_DATA   YES

#define PACKET_INCLUDES  (  INCLUDE_PACKET_PX_DATA << PACKET_PX_DATA_OFFSET \
| INCLUDE_PACKET_PY_DATA << PACKET_PY_DATA_OFFSET \
| INCLUDE_PACKET_SX_DATA << PACKET_SX_DATA_OFFSET \
| INCLUDE_PACKET_SY_DATA << PACKET_SY_DATA_OFFSET \
| INCLUDE_PACKET_PP_DATA << PACKET_PP_DATA_OFFSET \
| INCLUDE_PACKET_SP_DATA << PACKET_SP_DATA_OFFSET \
| INCLUDE_PACKET_AP_DATA << PACKET_AP_DATA_OFFSET \
)
#define PACKET_SIZE    (  INCLUDE_PACKET_PX_DATA * PACKET_PX_DATA_SIZE \
+ INCLUDE_PACKET_PY_DATA * PACKET_PY_DATA_SIZE \
+ INCLUDE_PACKET_SX_DATA * PACKET_SX_DATA_SIZE \
+ INCLUDE_PACKET_SY_DATA * PACKET_SY_DATA_SIZE \
+ INCLUDE_PACKET_PP_DATA * PACKET_PP_DATA_SIZE \
+ INCLUDE_PACKET_SP_DATA * PACKET_SP_DATA_SIZE \
+ INCLUDE_PACKET_AP_DATA * PACKET_AP_DATA_SIZE \
)
#define PACKET_OFFSETS  {                    \
PACKET_PX_DATA_SIZE, \
PACKET_PY_DATA_SIZE, \
PACKET_SX_DATA_SIZE, \
PACKET_SY_DATA_SIZE, \
PACKET_PP_DATA_SIZE, \
PACKET_SP_DATA_SIZE, \
PACKET_AP_DATA_SIZE, \
}
#define LOG2_MAX_PACKET_TYPE_SIZE 4
#define PACKET_OFFSET_WIDTH LOG2_MAX_PACKET_TYPE_SIZE
typedef struct
{
    byte_t px:PACKET_OFFSET_WIDTH;
    byte_t py:PACKET_OFFSET_WIDTH;
    byte_t sx:PACKET_OFFSET_WIDTH;
    byte_t sy:PACKET_OFFSET_WIDTH;
    byte_t pp:PACKET_OFFSET_WIDTH;
    byte_t sp:PACKET_OFFSET_WIDTH;
    byte_t ap:PACKET_OFFSET_WIDTH;
} packet_offset_lookup_t;

typedef struct
{
byte_t
    a:PACKET_OFFSET_WIDTH,
    b:PACKET_OFFSET_WIDTH;
}packing_template_t;

typedef struct
{
address_t
    px,
    py,
    sx,
    sy,
    pp,
    sp,
    ap;
} packet_value_lookup_t;

#define NUM_PACKET_ELEMENTS ((byte_t)( sizeof(packet_offset_lookup_t) * ( 8 / PACKET_OFFSET_WIDTH ) ))

typedef struct
{
    byte_t  ID,
    includes;
    timestamp_t
    timestamp;
    uint8_t padding[6];
} packet_header_t;

typedef struct
{
    packet_header_t header;
    byte_t          data[PACKET_SIZE];
} packet_t;

/* Rho Structures* */
typedef struct
{
    density_t      *map,
                   *background,
                   *filtered,
                    max[2];
    index_t         length,
                    centroid;
    bool            has_background;
    rho_kalman_t    kalmans[2];
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
    floating_t  primary,
                secondary,
                alternate,
                absence;
} prediction_probabilities;

typedef struct
{
    rho_kalman_t    primary,
                    secondary;
    floating_t      primary_new,
                    secondary_new;
    prediction_probabilities probabilities;
} prediction_t;

typedef struct
{
    prediction_t    x,y;
    prediction_probabilities probabilities;
} prediction_pair_t;

#endif /* rho_c_types_h */
