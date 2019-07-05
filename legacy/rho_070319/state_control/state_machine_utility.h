//
//  state_machine_utility.hpp
//  tau+
//
//  Created by Matthew Fonken on 2/8/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef state_machine_utility_h
#define state_machine_utility_h

#include <stdint.h>
#include "rho_global.h"

#ifdef __cplusplus
extern "C" {
#endif

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
#define STATE_DECAY                     0.95
#define STATE_PUNISH                    0.025
#define STABILITY_FACTOR                3.0
    
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
    
    #define NUM_STATE_GROUPS ((uint8_t)NUM_STATES/2)
    
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
    
    /* Markov state tree with markov base */
    typedef struct
    {
        floating_t map[NUM_STATES][NUM_STATES];
        state_dimension_t length;
    } markov_map_t;
    
    /* System self-diagnostic state control type */
    typedef struct
    {
        state_t         state;
        state_t         prev;
        state_t         next;
        state_dimension_t  selection_index;
        stability_t     stability;
        markov_map_t  probabilities;
    } markov_system_t;
    
    struct markov_map_functions
    {
        void (*InitializeMap)(      markov_map_t * );
        void (*NormalizeMap)(       markov_map_t * );
        void (*NormalizeState)(     markov_map_t *, state_dimension_t );
        void (*ResetState)(         markov_map_t *, state_dimension_t );
        void (*Print)(              markov_map_t *, state_t s );
    };
    
    struct markov_system_functions
    {
        void (*Initialize)(          markov_system_t *                );
        void (*DecayInactive)(       markov_system_t *                );
        void (*UpdateProbabilities)( markov_system_t *, floating_t[4] );
        void (*UpdateState)(         markov_system_t *                );
        void (*Update)(              markov_system_t *, floating_t[4] );
    };
    
    struct markov_functions
    {
        struct markov_map_functions    Map;
        struct markov_system_functions Sys;
    };
    
    void InitializeMarkovMap(         markov_map_t *                            );
    void NormalizeMarkovMap(          markov_map_t *                            );
    void NormalizeMarkovState(        markov_map_t *,       state_dimension_t   );
    void ResetMarkovState(            markov_map_t *,       state_dimension_t   );
    void PrintMarkovMap(              markov_map_t *,       state_t             );
    void InitializeMarkovSystem(      markov_system_t *                         );
    void DecayInactiveMarkovSystem(   markov_system_t *                         );
    void UpdateMarkovSystem(          markov_system_t *,    floating_t[4]       );
    void UpdateMarkovProbabilities(   markov_system_t *,    floating_t[4]       );
    void UpdateMarkovState(           markov_system_t *                         );
    
    static const struct markov_functions MarkovFunctions =
    {
        { /* Map functions */
            .InitializeMap          = InitializeMarkovMap,
            .NormalizeMap           = NormalizeMarkovMap,
            .NormalizeState         = NormalizeMarkovState,
            .ResetState             = ResetMarkovState,
            .Print                  = PrintMarkovMap
        },
        { /* System functions */
            .Initialize             = InitializeMarkovSystem,
            .DecayInactive          = DecayInactiveMarkovSystem,
            .Update                 = UpdateMarkovSystem,
            .UpdateProbabilities    = UpdateMarkovProbabilities,
            .UpdateState            = UpdateMarkovState
        }
    };
    
    static inline void copymax(floating_t * a, floating_t * b) { if(*a>*b)*b=*a;else*a=*b; }
    static inline bool isStable( state_t s ) { return ((state_dimension_t)s % 2); }
    static inline state_dimension_t stateNumber( state_t s ) { return (state_dimension_t)( ( s - 1 ) / 2 ); }
    
#define define_loop_variable_template_struct(T, N)struct { T l, i, j; floating_t u, v;}N;
    
#ifdef __cplusplus
}
#endif

#endif /* state_machine_utility_hpp */
