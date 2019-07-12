//
//  statem.h
//  taustates
//
//  Created by Matthew Fonken on 10/17/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef statem_h
#define statem_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <math.h>
    
#include "test_setup.h"
    
#include "global_types.h"

/** Goals **
 *  - Column is current state
 *  - Row is next state
 *  - Diagonal is probability of remaining in state, should tend to be most probable
 *  - System should try to stay in S2
 *  - All probabilities are <= 1.0 (100%) and...
 *  - ...all row probabilities add to <= 1.0 (100%)
 */


/*** Functions ***/
void initSystem(   system_t * s );
void updateSystem( system_t * s, prediction_pair_t * p );
void updateProbabilities( system_t * s, double p[3] );
void updateState( system_t * s );
void initDefaultBayesianMap( state_bayesian_map_t * b );
void normalizeBayesianMap(   state_bayesian_map_t * b );
void normalizeBayesianState( state_bayesian_map_t * b, int i );
void resetBayesianState(     state_bayesian_map_t * b, int i );
void printBayesianMap( state_bayesian_map_t * b, state_t s);
    
static inline bool isStable( int s ) { return (s % 2); }
static inline int stateNumber( int s ) { return (int)( ( s - 1 ) / 2 ); }
    
#ifdef __cplusplus
}
#endif


#endif /* statem_h */
