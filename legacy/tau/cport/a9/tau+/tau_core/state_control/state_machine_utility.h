//
//  state_machine_utility.hpp
//  tau+
//
//  Created by Matthew Fonken on 2/8/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef state_machine_utility_h
#define state_machine_utility_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include "rho_c_types.h"
    
    /***************************************************************************************/
    /*                          DEFINITIONS & MACROS                                       */
    /***************************************************************************************/
#define PROBABILITY_TUNING_FACTOR       0.5
#define PROBABILITY_TUNING_FACTOR_SQ    ( PROBABILITY_TUNING_FACTOR*PROBABILITY_TUNING_FACTOR )
#define PROBABILITY_THRESHOLD           0.1
#define PROBABILITY_ALTERNATE_THRESH    0.9
#define ABSENCE_FACTOR                  PROBABILITY_TUNING_FACTOR
#define PROBABILITY_TUNING_THRESHOLD    ( PROBABILITY_THRESHOLD * PROBABILITY_THRESHOLD )
#define PROBABILITY_STABILITY_THRESHOLD 0.5
#define STATE_DISTANCE                  2.0
#define SHADOW_TOLERANCE                0.2
#define DOUBT_STABILITY                 0.5
    
#define SQUARE(X)                       ( X * X )
#define DISTANCE_SQ(X,Y)                ( SQUARE(X) + SQUARE(Y) )
#define INRANGE(X,Y,T)                  ( abs( X - Y ) < T )
    
    /***************************************************************************************/
    
    /** Goals **
     *  - Column is current state
     *  - Row is next state
     *  - Diagonal is probability of remaining in state, should tend to be most probable
     *  - System should try to stay in S2
     *  - All probabilities are <= 1.0 (100%) and...
     *  - ...all row probabilities add to <= 1.0 (100%)
     */
    
    /* Tau states */
#define state_dimension_t uint8_t
#define loop_variables_state_dimension_t loop_variables_uint8_t //##state_dimension_t
    
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
    
    static inline state_dimension_t stateToSelection(state_t s) {return ((state_dimension_t)((s+1)/2) - 1);};
    static inline const char *stateString(state_dimension_t s)
    {
        static const char *strings[] = { "UN", "S0", "U0", "S1", "U1", "S2", "U2", "SM", "UM" };
        return strings[(state_dimension_t)s];
    }
    
    /* Stability tracking for selec tions */
    typedef struct
    {
        floating_t primary;
        floating_t secondary;
        floating_t alternate;
        floating_t overall;
    } stability_t;
    
    /* Markov state tree with bayesian base */
    typedef struct
    {
        floating_t map[NUM_STATES][NUM_STATES];
        state_dimension_t length;
    } bayesian_map_t;
    
    /* System self-diagnostic state control type */
    typedef struct
    {
        state_t         state;
        state_t         prev;
        state_t         next;
        state_dimension_t  selection_index;
        stability_t     stability;
        bayesian_map_t  probabilities;
    } bayesian_system_t;
    
    struct bayesian_map_functions
    {
        void (*initMap)(            bayesian_map_t * );
        void (*normalizeMap)(       bayesian_map_t * );
        void (*normalizeState)(     bayesian_map_t *, state_dimension_t );
        void (*resetState)(         bayesian_map_t *, state_dimension_t );
        void (*print)(              bayesian_map_t *, state_t s );
    };
    
    struct bayesian_system_functions
    {
        void (*init)(                bayesian_system_t * );
        void (*updateProbabilities)( bayesian_system_t *, floating_t[4] );
        void (*updateState)(         bayesian_system_t * );
        void (*update)(              bayesian_system_t *, prediction_pair_t * );
    };
    
    struct bayesian_functions
    {
        struct bayesian_map_functions    map;
        struct bayesian_system_functions sys;
    };
    
    extern const struct bayesian_functions BayesianFunctions;
    
    static inline void copymax(floating_t * a, floating_t * b) { if(*a>*b)*b=*a;else*a=*b; }
    static inline bool isStable( state_t s ) { return ((state_dimension_t)s % 2); }
    static inline state_dimension_t stateNumber( state_t s ) { return (state_dimension_t)( ( s - 1 ) / 2 ); }
    
#define define_loop_variable_template_struct(T, N)struct { T l, i, j; floating_t u, v;}N;
    
#ifdef __cplusplus
}
#endif

#endif /* state_machine_utility_hpp */
