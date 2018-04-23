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

/** Goals **
 *  - Column is current state
 *  - Row is next state
 *  - Diagonal is probability of remaining in state, should tend to be most probable
 *  - System should try to stay in S2
 *  - All probabilities are <= 1.0 (100%) and...
 *  - ...all row probabilities add to <= 1.0 (100%)
 */

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

/* Stability tracking for selec tions */
typedef struct
{
    double primary;
    double secondary;
    double alternate;
    double overall;
} stability_t;

/* Markov state tree with bayesian base */
typedef struct
{
    double  map[NUM_STATES][NUM_STATES];
    int     length;
} bayesian_map_t;

struct bayesian_map_functions
{
    void    (*initMap)(        bayesian_map_t * );
    void    (*normalizeMap)(   bayesian_map_t * );
    void    (*normalizeState)( bayesian_map_t *, int );
    void    (*resetState)(     bayesian_map_t *, int );
    void    (*print)(          bayesian_map_t *, state_t s );
};
extern const struct bayesian_map_functions BayesianMap;

/* System self-diagnostic state control type */
typedef struct
{
    state_t     state;
    state_t     prev;
    state_t     next;
    int         selection_index;
    stability_t stability;
    bayesian_map_t probabilities;
} bayesian_system_t;

struct bayesian_system_functions
{
    void (*init)(                bayesian_system_t * );
    void (*updateProbabilities)( bayesian_system_t *, double[4] );
    void (*updateState)(         bayesian_system_t * );
    void (*update)(              bayesian_system_t *, PredictionPairC * );
};

extern const struct bayesian_system_functions BayesianSystem;

static inline void copymax(double * a, double * b) { if(*a>*b)*b=*a;else*a=*b; }
static inline bool isStable( int s ) { return (s % 2); }
static inline int stateNumber( int s ) { return (int)( ( s - 1 ) / 2 ); }

#ifdef __cplusplus
}
#endif

#endif /* state_machine_utility_hpp */
