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
#include "global_types.h"


/** Key **
 *  S - Stable
 *  U - Unstable
 *  0 - Zero selections
 *  1 - One selections
 *  2 - Two selections
 *  N - Many selections
 */
    
#define NEUTRAL         1.0//NUM_STATES
#define GUARANTEED      4*NEUTRAL
#define HIGHLY_LIKELY   3*NEUTRAL
#define LIKELY          2*NEUTRAL
#define UNLIKELY        0.5*NEUTRAL
#define HIGHLY_UNLIKELY 0.3*NEUTRAL
    
#define DEF_SELF_CON GUARANTEED
#define DEF_S0_TO_U1 UNLIKELY
#define DEF_S0_TO_U2 UNLIKELY
#define DEF_S0_TO_UM UNLIKELY
#define DEF_U0_TO_S0 HIGHLY_LIKELY
#define DEF_S1_TO_U0 LIKELY
#define DEF_S1_TO_UM UNLIKELY
#define DEF_U1_TO_S1 HIGHLY_LIKELY
#define DEF_S2_TO_U0 LIKELY
#define DEF_S2_TO_U1 LIKELY
#define DEF_S2_TO_UM HIGHLY_UNLIKELY
#define DEF_U2_TO_S2 HIGHLY_LIKELY
#define DEF_UM_TO_S0 HIGHLY_LIKELY
#define DEF_UM_TO_U0 LIKELY
#define DEF_UM_TO_S1 HIGHLY_LIKELY
#define DEF_UM_TO_U1 LIKELY
#define DEF_UM_TO_S2 HIGHLY_LIKELY
#define DEF_UM_TO_U2 LIKELY
#define DEF_UM_TO_SN HIGHLY_UNLIKELY
#define DEF_SN_TO_UM HIGHLY_LIKELY
#define DEF_NOT_CONN 0.0 // SHOULD ALWAYS BE ZERO

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
bool isStable( state_t s );
void updateState( system_t * s );
state_t nextState(      state_t s, double pri, double sec, double alt );
state_t stabilityCheck( state_t s, double pri, double sec, double alt, double thresh );
void initDefaultBayesianMap( state_bayesian_map_t * b );
void normalizeBayesianMap(   state_bayesian_map_t * b );
void normalizeBayesianState( state_bayesian_map_t * b, int i );
void resetBayesianState(     state_bayesian_map_t * b, int i );
void printBayesianMap(       state_bayesian_map_t * b );
    
#ifdef __cplusplus
}
#endif


#endif /* statem_h */
