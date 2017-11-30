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
    
#include "test_setup.h"
    
#include <stdint.h>
#include <stdbool.h>
#include "kalman.h"
#include "gaussian.h"
    
#define ABSINV(X)   ((X<1)?1-X:X-1)
#define ABS(X)      ((X>=0)?X:-X)
    
#define GTTHR(X,Y,T)    X>=(Y+T)
#define INRANGE(X,Y,T)  (X>(Y-T)&&X<(Y+T))
    
/* Kalman Defaults */
#define IMAGE_MAX_KALMAN_LIFE     5.0 // (seconds)
#define IMAGE_VALUE_UNCERTAINTY   0.1//0.01
#define IMAGE_BIAS_UNCERTAINTY    0.005//0.003
#define IMAGE_SENSOR_UNCERTAINTY  0.005//0.02
    
    typedef int16_t         image_dimension_base_t;
    typedef uint8_t         pixel_base_t;
    typedef pixel_base_t*   cimage_t;
    
    typedef double          FLOAT;
    
    static FLOAT timeDiff( struct timeval a, struct timeval b ) { return ((b.tv_sec  - a.tv_sec) + (b.tv_usec - a.tv_usec)/1000000.0) + 0.0005; }
    
    static FLOAT seconds_since( double time )
    {
        struct timeval now;
        gettimeofday( &now, NULL );
        return (now.tv_sec + now.tv_usec/1000000.0) - time;
    }
    
    static void updateThreshold( int * threshold, int selections )
    {
        if( selections > NUM_OF_SELECTIONS ) *threshold += THRESH_STEP;
        if( selections < NUM_OF_SELECTIONS ) *threshold -= THRESH_STEP;
        if( *threshold > THRESHOLD_MAX) *threshold = THRESHOLD_MAX;
        if( *threshold < THRESHOLD_MIN) *threshold = THRESHOLD_MIN;
    }
    
    typedef enum
    {
        UNWEIGHTED = 0,
        WEIGHTED = 1
    } sorting_settingss;
    
#define MAX_PERSISTENCE ( 1 << 7 ) - 1 // MAX OF SIGNED BYTE
    
    /* Kalman Internal Lookup Pointer Type */
    typedef uint8_t kmat_p;
    
    typedef uint8_t lookup_t;
    
    /* Kalman Internal Float Type */
    typedef FLOAT kmat_f;
    
    /* Kalman Probabilities Type */
    typedef struct
    {
        kmat_f  drop;
        kmat_f  prev_weight;
        kmat_f  curr_weight;
    } kmat_probabilities_t;
    
    typedef struct
    {
        uint8_t level;
        int8_t  persistence;
    } kmat_coupling_t;
    
#define COUPLING_DEGREDATION 100
    static inline void discourageCouple( kmat_coupling_t * c )
    {
        if(c->persistence > 0) c->persistence -= COUPLING_DEGREDATION;
        else c->level = 0;
    }
    
    static inline void encourageCouple( kmat_coupling_t * c )
    {
        if(c->persistence < MAX_PERSISTENCE) c->persistence++;
    }
    
    /* Kalman Matrix Type */
    typedef struct
    {
        kalman_t        kalmans[MAX_KALMANS][MAX_PEAKS];
        lookup_t        lookup[MAX_KALMANS];            /**< Tracks Kalman resorting >*/
        kmat_p          selection[MAX_KALMANS];         /**< Tracks current peak id selection >*/
        FLOAT           value[MAX_KALMANS];             /**< Tracks current tracking value >*/
        FLOAT           density[MAX_KALMANS];           /**< Tracks current density value >*/
        kmat_coupling_t pair[MAX_KALMANS];
        kmat_p          k_index;
        kmat_p          p_index;
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
    
    typedef enum
    {
        SIMILAR = 0,
        OPPOSITE
    } selection_pair_t;
    
    typedef struct
    {
        image_dimension_base_t  primary;
        FLOAT                  primary_probability;
        image_dimension_base_t  secondary;
        FLOAT                  secondary_probability;
        FLOAT                  alternate_probability;
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
        int *           map;
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
        int8_t*   dir;
    }peak_list_t;
    
    typedef struct
    {
        peak_list_t x;
        peak_list_t y;
    } peak_list_pair_t;
    
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
        
        NUM_STATES
    } state_t;
    
    static inline int stateToSelection(int s) {return ((int)((s+1)/2) - 1);};
    static inline const char *stateString(int s)
    {
        static const char *strings[] = { "UN", "S0", "U0", "S1", "U1", "S2", "U2", "SM", "UM" };
        return strings[s];
    }
    
    /* Markov state tree with bayesian base */
    typedef struct
    {
        FLOAT   map[NUM_STATES][NUM_STATES];
        uint8_t length;
        //    bool    scheduled_normalize;
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
        FLOAT primary;
        FLOAT secondary;
        FLOAT alternate;
        FLOAT overall;
    } stability_t;
    
    /* System self-diagnostic state control type */
    typedef struct
    {
        state_t     state;
        state_t     prev;
        state_t     next;
        uint8_t     selection_index;
        stability_t stability;
        //    filter_t    filter;
        state_bayesian_map_t probabilities;
    } system_t;
    
    /* Universal Rho Type */
    typedef struct
    {
        uint16_t                width;
        uint16_t                height;
        density_map_pair_t      density_map_pair;
        peak_list_pair_t        peak_list_pair;
        gaussian_t              gaussian;
    } rho_t;
    
    /* Universal Sigma Type */
    typedef struct
    {
        uint16_t                width;
        uint16_t                height;
        kmat_pair_t             loc_km;
    } sigma_t;
    
    /* Universal Tau Type */
    typedef struct
    {
        uint16_t                width;                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
        uint16_t                height;
        system_t                sys;
        rho_t                   rho;
        sigma_t                 sigma;
        prediction_pair_t       predictions;
    } tau_t;
    
#endif /* global_types_h */
    
#ifdef __cplusplus
}
#endif
