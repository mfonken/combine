//
//  rho_types.h
//  tau+
//
//  Created by Matthew Fonken on 4/22/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_types_h
#define rho_types_h

#include "rho_kalman.h"

#define USE_SHORTHAND_TYPES

#ifdef USE_SHORTHAND_TYPES
typedef uint8_t   uh;
typedef uint16_t  uw;
typedef uint32_t  ul;
typedef int8_t    sh;
typedef int16_t   sw;
typedef int32_t   sl;
#endif

typedef float FLOAT;
typedef uint8_t		byte_t;
typedef uint16_t	index_t;
typedef uint8_t 	capture_t;
typedef uint32_t	density_t;
typedef uint32_t	address_t;
typedef volatile bool flag_t;
typedef FLOAT     timestamp_t;

/* Packet Generation Settings */
#define YES 1
#define NO  0

typedef struct
{
    byte_t  ID,
            includes,
            timestamp[sizeof(timestamp_t)];
} packet_header_t;

#define PACKET_HEADER_SIZE (sizeof(packet_header_t));
#define PACKET_HEADER_ID    0xab

#define PACKET_DATA_DECIMAL       int16_t//FLOAT
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
byte_t  a:PACKET_OFFSET_WIDTH,
        b:PACKET_OFFSET_WIDTH;
}packing_template_t;

typedef struct
{
    address_t px;
    address_t py;
    address_t sx;
    address_t sy;
    address_t pp;
    address_t sp;
    address_t ap;
} packet_value_lookup_t;

#define NUM_PACKET_ELEMENTS ((byte_t)( sizeof(packet_offset_lookup_t) * ( 8 / PACKET_OFFSET_WIDTH ) ))

typedef struct
{
    packet_header_t header;
    byte_t          data[PACKET_SIZE];
} packet_t;

static packet_offset_lookup_t packet_offset_lookup = {0};
static packet_value_lookup_t  packet_value_lookup  = {0};

/* Rho Structures* */
typedef struct
{
    density_t 	 *map,
                 *background;
    index_t    		length,
									centroid;
    density_t  		max[2];
    rho_kalman_t 	kalmans[2];
} density_map_t;

typedef struct
{
    density_map_t x,y;
} density_map_pair_t;

typedef struct
{
    FLOAT   primary,
            secondary,
            alternate,
						absence;
} prediction_probabilities;

typedef struct
{
    rho_kalman_t    primary,
                    secondary;
    FLOAT           primary_new,
                    secondary_new;
    prediction_probabilities probabilities;
} prediction_t;

typedef struct
{
    prediction_t    x,y;
    prediction_probabilities
                    probabilities;
} prediction_pair_t;

typedef struct
{
    int area[9];
} area_list_t;

typedef struct
{
    unsigned char
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
  density_t
    max,
    den;
  index_t
    loc;
} blob_t;

typedef struct
{
  index_t
  	xl[3],
  	yl[3],
    area[9],

	byte_t a, b, c, d, l, l_, p, q, x, y;
} redistribution_variables;

typedef struct
{
  index_t
        len,     /* data set/map length */
        range[3],
        cyc,
        cyc_,
        x1,
        x2,
        fpeak,   /* filter peak */
        fvar,    /* filter variance */
        fbandl,  /* filter band lower edge value */
        c1,      /* current map value 1 */
        c2,      /* current map value 2 */
        b,       /* current background value */
        cloc,    /* current location value */
        gapc,    /* gap counter value */
        avgc;    /* averaging counter value */
    blob_t
        blobs[2];

    density_t
        cmax,   /* current maximum value */
        amax,   /* alternate blob maximum */

        cden,   /* current density value */
        fden,   /* filtered density */
        tden;   /* Total density */

    bool
        has,    /* has blob, is tracking */
        sel;

    FLOAT
        fcov,   /* Filtered coverage ratio */
        cavg,   /* cumulative average */
        mavg,   /* moment average */
        afac,   /* alternate factor */
        pfac,   /* prinary factor */
        sfac,   /* secondary factor */
        fdnf,   /* filtered density (float) */
        tdnf,   /* target density (float) */
        fvf_;   /* filtered variance inverse (float) */
} rho_selection_variables;

typedef struct
{
	index_t
		Ax,
		Ay,
		Bx,
		By,
		Cx,
		Cy;
  FLOAT
    x1,
    y1;
	byte_t non_diag,
  int8_t q_check;
} preduction_update_variables;

typedef struct
{
  FLOAT
    avg,
    mavg;
  density_t
    c;
  index_t
    cnt,
    tot,
    i;
} centroid_calculation_variables;

typedef struct
{
  packet_t * packet;
  address_t pdPtr,
            llPtr,
					* alPtr;
	byte_t    includes,
            i,
            j,
            l,
            t;
} packet_generation_variables;

typedef struct
{
  flag_t
    irq,
  	frame,
  	row,
  	uart_dma_busy;
} system_flags_variables;

typedef struct
{
  volatile uint16_t
    v,
    w,
    h,
    f,
    t,
    p;
} system_counter_variables;

typedef struct
{
  index_t
      CENTROID_X,
      CENTROID_Y;
  density_t
      quadrant_total,                       /* Total density */
      quadrant_previous,                    /* Previous row density */
      density_x_max;
  address_t
      capture_end,
      capture_max,
      thresh_end,                    /* Max threshold buffering size */
      thresh_max,
      thresh_value,                         /* Shared threshold value */
      rho_mem_end,
   		campera_port,
  		uart_tx_address;
} system_data_variables;

typedef struct
{
  density_t
      density_y[CAPTURE_WIDTH],          		/* Processed density X array */
      density_x[CAPTURE_HEIGHT],            /* Processed density Y array */
      quadrant[4];                   /* Quadrant density array */
  capture_t
      capture[CAPTURE_BUFFER_SIZE];  /* Raw capture buffer for DMA */
  index_t
      thresh[THRESH_BUFFER_SIZE];    /* Threshold processing buffer */
} system_buffer_variables;

typedef struct
{
    density_map_pair_t  density_map_pair;
    prediction_pair_t   prediction_pair;
    bayesian_system_t   sys;
    rho_kalman_t        thresh_filter;
    index_t
            width,
            height,
            Cx,
            Cy,
            Bx,
            By;
    byte_t thresh;
    density_t
            Q[4],
            Qb[4],
            Qf[4],
            QT,
            QbT;
    packet_t packet;
    // double  QF, FT;
} rho_utility;

typedef struct
{
  system_flags_variables    flags;
  system_counter_variables  counters
  system_data_variables     data;
  system_buffers_variables  buffers;
  rho_utility     rho;
} system_variables;

typedef struct
{
  void (*Process_Frame)( void );
  void (*Perform_Background_Event)( void );
} system_perform_functions;

typedef struct
{
  void (*Init_Pixel_DMA)( void );
  void (*Pause_DMA)( void );
  void (*Resume_DMA)( void );
  void (*Reset_DMA)( void );
  void (*printd)( void );
} system_utility_functions;

typedef struct
{
  void (*Init_Memory)( void );
  void (*Zero_Memory)( void );
} system_memory_functions;

typedef struct
{
  system_perform_functions;
  system_utility_functions;
  system_memory_functions;
} system_functions;

typedef struct
{
  system_variables variables;
  system_functions functions;
} system;

#define DEFAULT_SYSTEM_INITIALIZATION                               \
{                                                                   \
  {0}, /* VARIABLES */                                              \
  { /* FUNCTIONS */                                                 \
    { /* Perform */                                                 \
      .Process_Frame            = frameProcessor,                   \
  		.Perform_Background_Event = performBackgroundEvent            \
    },                                                              \
    { /* Utility */                                                 \
      .Init_Pixel_DMA = initTimerDMA,                               \
      .Pause_DMA      = pauseDMA,                                   \
      .Resume_DMA     = resumeDMA,                                  \
      .Reset_DMA      = resetDMA,                                   \
      .printd         = dprint                                      \
    },                                                              \
    { /* Memory */                                                  \
      .Init_Memory = init_memory,                                   \
      .Zero_Memory = zero_memory                                    \
    }                                                               \
  }                                                                 \
}

#define svf system.variables.flags
#define svc system.variables.counters
#define svd system.variables.data
#define svb system.variables.buffers

#define PACKET_ADDRESS_INITIALIZER(r)
{
 (address_t)&r.x.primary.value,         /* px */
 (address_t)&r.y.primary.value,         /* py */
 (address_t)&r.x.secondary.value,       /* sx */
 (address_t)&r.y.secondary.value,       /* sy */
 (address_t)&r.probabilities.primary,   /* pp */
 (address_t)&r.probabilities.secondary, /* ap */
 (address_t)&r.probabilities.alternate  /* ap */
}

#endif /* rho_c_types_h */
