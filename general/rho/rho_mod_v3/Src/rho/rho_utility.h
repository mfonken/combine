//
//  rho_utility.h
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_utility_h
#define rho_utility_h

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "stm32l4xx_hal.h"
#include "rho_types.h"
#include "rho_kalman.h"
#include "state_machine_utility.h"

/***************************************************************************************/
/*                           DEFINITIONS & MACROS                                      */
/***************************************************************************************/

typedef struct
{
  index_t
  	xl[3],
  	yl[3],
    area[9];

	byte_t a, b, c, d, l, l_, p, q, x, y;
} redistribution_variables;

typedef struct
{
  index_t
        len,     /* data set/map length           */
        range[3],
        cyc,
        cyc_,
        x1,
        x2,
        fpeak,   /* filter peak                   */
        fvar,    /* filter variance               */
        fbandl,  /* filter band lower edge value  */
        c1,      /* current map value 1           */
        c2,      /* current map value 2           */
        b,       /* current background value      */
        p,       /* punishment value              */
        cloc,    /* current location value        */
        gapc,    /* gap counter value             */
        avgc;    /* averaging counter value       */
    blob_t
        blobs[2];

    density_t
        cmax,   /* current maximum value */
        amax,   /* alternate blob maximum */

        cden,   /* current density value */
        fden,   /* filtered density */
        tden;   /* Total density */

    byte_t
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
	byte_t non_diag;
  int8_t qcheck;
} prediction_update_variables;

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
   		camera_port,
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
} rho_utility;

typedef struct
{
  system_flags_variables    flags;
  system_counter_variables  counters;
  system_data_variables     data;
  system_buffer_variables 	buffers;
  rho_utility     					rho;
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
  void (*printd)( uint8_t *, uint16_t );
} system_utility_functions;

typedef struct
{
  void (*Init_Memory)( void );
  void (*Zero_Memory)( void );
} system_memory_functions;

typedef struct
{
  system_perform_functions 	perform;
  system_utility_functions 	utility;
  system_memory_functions 	memory;
} system_functions;

typedef struct
{
  system_variables variables;
  system_functions functions;
} system_t;

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

#define svf rho_system.variables.flags
#define svc rho_system.variables.counters
#define svd rho_system.variables.data
#define svb rho_system.variables.buffers

#define PACKET_ADDRESS_INITIALIZER(r)																\
{																																		\
	(address_t)&r.x.primary.value,         /* px */										\
	(address_t)&r.y.primary.value,         /* py */										\
	(address_t)&r.x.secondary.value,       /* sx */										\
	(address_t)&r.y.secondary.value,       /* sy */										\
	(address_t)&r.probabilities.primary,   /* pp */										\
	(address_t)&r.probabilities.secondary, /* ap */										\
	(address_t)&r.probabilities.alternate  /* ap */										\
}

/***************************************************************************************/

/***************************************************************************************/
/*                            FUNCTIONS & TYPES                                        */
/***************************************************************************************/

static void cma( FLOAT new_val, FLOAT *avg, index_t num ) { *avg+=(new_val-*avg)/(FLOAT)(num+1); }
static void cma_M0_M1( FLOAT v, FLOAT i, FLOAT *m0, FLOAT *m1, index_t * n )
{FLOAT n_=1/(++(*n));*m0+=(v-*m0)*n_;*m1+=((v*i)-*m1)*n_;}
static inline void iswap(uint16_t*a,uint16_t*b){uint16_t t=*a;*a=*b;*b=t;}

struct rho_functions
{
	void (*Init)( rho_utility *, UART_HandleTypeDef *, index_t, index_t );
  void (*Perform)( rho_utility *, bool );
	void (*Redistribute_Densities)( rho_utility * );
	void (*Generate_Background)( rho_utility * );
	void (*Filter_and_Select_Pairs)( rho_utility * );
	void (*Filter_and_Select)( rho_utility *, density_map_t *, prediction_t * );
	void (*Update_Prediction)( rho_utility * );
  void (*Update_Threshold)( rho_utility * );
  void (*Generate_Packet)( rho_utility * );
};

extern system_t rho_system;
extern const struct rho_functions RhoFunctions;
extern const density_redistribution_lookup_t rlookup;

/***************************************************************************************/

#endif
