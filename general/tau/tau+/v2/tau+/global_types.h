//
//  global_types.h
//  tau+
//
//  Created by Matthew Fonken on 8/22/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifdef __cplusplus
extern "C" {
#endif

#ifndef global_types_h
#define global_types_h

#include <stdint.h>
#include <stdbool.h>
#include "kalman.h"

#define ABSINV(X)   ((X<1)?1-X:X-1)
#define ABS(X)      ((X>=0)?X:-X)

#define MAX_KALMANS 3
#define MAX_PEAKS   3 
    
/***PROBABILITY BIASES***/
#define LOCATION_BIAS   0.5
#define DENSITY_BIAS    1 - LOCATION_BIAS
/************************/

/*******PEAK TUNING******/
/************************/

typedef int16_t         image_dimension_base_t;
typedef uint8_t         pixel_base_t;
typedef pixel_base_t**  cimage_t;

typedef double          FLOAT;
    
/* Kalman Internal Lookup Pointer Type */
typedef uint8_t kmat_p;

typedef uint8_t lookup_t;

/* Kalman Internal Float Type */
typedef double kmat_f;

/* Kalman Probabilities Type */
typedef struct
{
    kmat_f  drop;
    kmat_f  prev_weight;
    kmat_f  curr_weight;
} kmat_probabilities_t;

/* Kalman Matrix Type */
typedef struct
{
    kalman_t    kalmans[MAX_KALMANS][MAX_PEAKS];
    lookup_t    lookup[MAX_KALMANS];            /**< Tracks Kalman resorting >*/
    kmat_p      selection[MAX_KALMANS];         /**< Tracks current peak id selection >*/
    double      value[MAX_KALMANS];             /**< Tracks current tracking value >*/
    double      density[MAX_KALMANS];           /**< Tracks current density value >*/
    uint8_t     pair[MAX_KALMANS];
    kmat_p      k_index;
    kmat_p      p_index;
} kmat_t;

/* Kalman Matrix Pair Type */
typedef struct
{
    kmat_t  x;
    kmat_t  y;
//    kmat_probabilities_t      probabilities;
} kmat_pair_t;

    
typedef struct
{
    image_dimension_base_t x;
    image_dimension_base_t y;
} coordinate_t;

typedef struct
{
    image_dimension_base_t i;
    image_dimension_base_t j;
} vector_t;
    
typedef struct
{
    FLOAT   *array;
    uint16_t length;
    FLOAT    sigma;
} gaussian_t;
    
typedef enum
{
    SIMILAR = 0,
    OPPOSITE
} selection_pair_t;
    
typedef struct
{
    image_dimension_base_t  primary;
    double                  primary_probability;
    image_dimension_base_t  secondary;
    double                  secondary_probability;
    double                  alternate_probability;
} prediction_t;

typedef struct
{
    prediction_t x;
    prediction_t y;
    selection_pair_t        selection_pair;
} prediction_pair_t;

typedef struct
{
    kalman_t primary;
    kalman_t secondary;
} prediction_kalman_t;

typedef struct
{
    prediction_kalman_t x;
    prediction_kalman_t y;
} prediction_kalman_pair_t;

typedef struct
{
    float*          map;
    float*          vel;
    float*          acc;
    float*          jrk;
    float*          jnc;
    uint16_t        length;
}density_map_t;

typedef struct
{
    density_map_t   x;
    density_map_t   y;
} density_map_pair_t;

typedef struct
{
    uint16_t  length;
    uint16_t* map;
    uint16_t* den;
    int*      dir;
}peak_list_t;

typedef struct
{
    peak_list_t x;
    peak_list_t y;
} peak_list_pair_t;

typedef struct
{
    uint16_t  length;
    FLOAT* primary;
    FLOAT* secondary;
}probability_list_t;
    
typedef struct
{
    probability_list_t x;
    probability_list_t y;
}probability_list_pair_t;
    
/* Tau states */
typedef enum
{
    UNKNOWN_STATE = 0,
    STABLE_NONE,
    UNSTABLE_NONE,
    STABLE_SINGLE,
    UNSTABLE_SINGLE,
    STABLE_DOUBLE,
    UNSTABLE_DOUBLE,
    STABLE_MANY,
    UNSTABLE_MANY,
    //    COLLAPSING,
    
    NUM_STATES
} state_t;

static inline int stateToSelection(state_t s) {return ((int)((s+1)/2) - 1);};

static inline const char *stateString(state_t s)
{
    static const char *strings[] = { "UN", "S0", "U0", "S1", "U1", "S2", "U2", "SM", "UM", "CO" };
    return strings[s];
}

/* Markov state tree with bayesian base */
typedef struct
{
    double  map[NUM_STATES][NUM_STATES];
    int     length;
    bool    scheduled_normalize;
} state_bayesian_map_t;

/* Tau filter types specifically for kalman matrix structure */
typedef enum
{
    NO_FILTER = 0,
    SOFT_FILTER,
    HARSH_FILTER,
    CHAOS_FILTER
} filter_t;

/* Stability tracking for selec tions */
typedef struct
{
    double primary;
    double secondary;
    double alternate;
    double overall;
} stability_t;

/* System self-diagnostic state control type */
typedef struct
{
    state_t     state;
    state_t     prev;
    state_t     next;
    int         selection_index;
    stability_t stability;
    filter_t    filter;
    state_bayesian_map_t probabilities;
} system_t;

#endif /* global_types_h */
    
#ifdef __cplusplus
}
#endif
