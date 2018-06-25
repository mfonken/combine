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

typedef struct
{
    density_t 	 *map,
                 *background;
    index_t    	length,
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
  blob_t
        blobs[2];
  index_t
        len,     /* data set/map length */
        fpeak,   /* filter peak */
        fvar,    /* filter variance */
        fbandl,  /* filter band lower edge value */
        c1,      /* current map value 1 */
        c2,      /* current map value 2 */
        b,       /* current background value */
        cloc,    /* current location value */
        gapc,    /* gap counter value */
        avgc;    /* averaging counter value */

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

#endif /* rho_c_types_h */
