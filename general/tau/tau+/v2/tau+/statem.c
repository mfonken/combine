//
//  statem.c
//  taustates
//
//  Created by Matthew Fonken on 10/17/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "statem.h"

#define __TEMP_PROB__   0.0
#define PROBABILITY_TUNING_FACTOR       0.5
#define PROBABILITY_THRESHOLD           0.7
#define PROBABILITY_TUNING_THRESHOLD    PROBABILITY_THRESHOLD * PROBABILITY_THRESHOLD
#define PROBABILITY_STABILITY_THRESHOLD 0.5

/* Temporary macros */
#define numberOfSelectedPeaks 1

double DEF_BAYESIAN_MAP[NUM_STATES+1][NUM_STATES+1] =
{                 /*   UNKNOWN_STAT  STABLE_NONE   UNSTABLE_NON  STABLE_SINGL  UNSTABLE_SIN  STABLE_DOUBL  UNSTABLE_DOU  STABLE_MANY   UNSTABLE_MAN  COLLAPSING  */
    /* UNKNOWN_STA */{ DEF_SELF_CON, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN },
    /* STABLE_NONE */{ DEF_NOT_CONN, DEF_SELF_CON, DEF_U0_TO_S0, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_UM_TO_S0, DEF_NOT_CONN },
    /* UNSTABLE_NO */{ DEF_NOT_CONN, DEF_NOT_CONN, DEF_SELF_CON, DEF_S1_TO_U0, DEF_NOT_CONN, DEF_S2_TO_U0, DEF_NOT_CONN, DEF_NOT_CONN, DEF_UM_TO_U0, DEF_NOT_CONN },
    /* STABLE_SING */{ DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_SELF_CON, DEF_U1_TO_S1, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_UM_TO_S1, DEF_NOT_CONN },
    /* UNSTABLE_SI */{ DEF_NOT_CONN, DEF_S0_TO_U1, DEF_NOT_CONN, DEF_NOT_CONN, DEF_SELF_CON, DEF_S2_TO_U1, DEF_NOT_CONN, DEF_NOT_CONN, DEF_UM_TO_U1, DEF_NOT_CONN },
    /* STABLE_DOUB */{ DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_SELF_CON, DEF_U2_TO_S2, DEF_NOT_CONN, DEF_UM_TO_S2, DEF_NOT_CONN },
    /* UNSTABLE_DO */{ DEF_NOT_CONN, DEF_S0_TO_U2, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_SELF_CON, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN },
    /* STABLE_MANY */{ DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_SELF_CON, DEF_UM_TO_SN, DEF_NOT_CONN },
    /* UNSTABLE_MA */{ DEF_NOT_CONN, DEF_S0_TO_UM, DEF_NOT_CONN, DEF_S1_TO_UM, DEF_NOT_CONN, DEF_S2_TO_UM, DEF_NOT_CONN, DEF_SN_TO_UM, DEF_SELF_CON, DEF_NOT_CONN },
    /* COLLAPSING  */{ DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_NOT_CONN, DEF_SELF_CON }
};

void initSystem( system_t * s )
{
    s->state                = STABLE_NONE;
    s->prev                 = UNKNOWN_STATE;
    s->next                 = UNKNOWN_STATE;
    s->selection_index      = 0;
    s->stability.primary    = 0.0;
    s->stability.secondary  = 0.0;
    s->stability.alternate  = 0.0;
    s->stability.overall    = 0.0;
    s->filter               = NO_FILTER;
    initDefaultBayesianMap( &s->probabilities );
}

void updateSystem( system_t * s, prediction_pair_t * p )
{
    /** Assumptions
     *  - Column is current state
     *  - Row is next state
     */
    if( s->probabilities.scheduled_normalize ) normalizeBayesianMap( &s->probabilities );
    
    int l = s->probabilities.length;
    int c = (int)s->state;
    double max_v = 0.0;
    int max_i = 0;
    
    /* Find most probable next state */
    for( int i = 0; i < l; i++ )
    {
        double p = s->probabilities.map[i][c];
        if( p > max_v )
        {
            max_v = p;
            max_i = i;
        }
    }
    if( max_i != s->state ) s->next = max_i;
    
    printf("Current state is %s.\n", stateString(s->state));
    printf("Next state is %s(%.2f).\n", stateString(s->next), max_v);
    
    /* Update self-diagnostics based on state */
    
    double pri = 0.0, sec = 0.0, alt = 0.0;
    if( p->selection_pair == SIMILAR )
    {
        pri = p->x.primary_probability * p->y.primary_probability;
        sec = p->x.secondary_probability * p->y.secondary_probability;
    }
    else if( p->selection_pair == OPPOSITE )
    {
        pri = p->x.primary_probability * p->y.secondary_probability;
        sec = p->x.secondary_probability * p->y.primary_probability;
    }
    alt = p->x.alternate_probability * p->y.alternate_probability;
    
    printf("Before: pri-%.3f sec-%.3f alt-%.3f\n", pri, sec, alt);
    int num_selections = 0;
    if( pri < PROBABILITY_TUNING_THRESHOLD ) pri = 0.0;
    else num_selections++;
    if( sec < PROBABILITY_TUNING_THRESHOLD ) sec = 0.0;
    else num_selections++;
    if( alt < PROBABILITY_TUNING_THRESHOLD ) alt = 0.0;
    else num_selections++;
    printf("After: pri-%.3f sec-%.3f alt-%.3f\n", pri, sec, alt);
    
    s->selection_index      = num_selections;
    s->stability.primary    = 0.0;
    s->stability.secondary  = 0.0;
    s->stability.alternate  = 0.0;
    
    printf("Current state %s is ", stateString(s->state));
    if(!isStable(s->state)) printf("not ");
    printf("stable\n");
    ///TODO: Update correctly through state map
    state_t update = nextState(s->state, pri, sec, alt);
    printf("Update state %s\n", stateString(update));
    switch((int)s->selection_index)
    {
        default:
            s->stability.alternate  = alt;
            s->probabilities.map[update][s->state] += alt * PROBABILITY_TUNING_FACTOR;
        case 2:
            s->stability.secondary  = sec;
            s->probabilities.map[update][s->state] += sec * PROBABILITY_TUNING_FACTOR;
        case 1:
            s->stability.primary    = pri; // stable primary tracking
            s->probabilities.map[update][s->state] += pri * PROBABILITY_TUNING_FACTOR;
        case 0:
            s->stability.primary    = pri; // unstable primary tracking, needs some influence to leave U/S0
            break;
    }
    normalizeBayesianState( &s->probabilities, s->state );
}

state_t nextState( state_t s, double pri, double sec, double alt )
{
    state_t r = UNKNOWN_STATE;
    switch( s )
    {
        case STABLE_NONE:
            r = UNSTABLE_SINGLE;
            break;
        case UNSTABLE_NONE:
            r = STABLE_NONE;
            break;
        case UNSTABLE_DOUBLE:
        case STABLE_DOUBLE:
        case UNSTABLE_SINGLE:
        case STABLE_SINGLE:
            r = stabilityCheck( s, pri, sec, alt, PROBABILITY_STABILITY_THRESHOLD );
            break;
        default:
            break;
    }
    return r;
}

state_t stabilityCheck( state_t s, double pri, double sec, double alt, double thresh )
{
    int n = stateToSelection(s);
    if( pri > thresh )
    {
        if( sec > thresh )
        {
            if( alt > thresh ) return UNSTABLE_MANY;
            return (n==2)?STABLE_DOUBLE:UNSTABLE_DOUBLE;
        }
        return (n==1)?STABLE_SINGLE:UNSTABLE_SINGLE;
    }
    return UNSTABLE_NONE;
}

bool isStable( state_t s )
{
    if(s == STABLE_NONE) return false;
    return (s % 2);
}

void updateState( system_t * s )
{
    if(s->next != UNKNOWN_STATE )
    {
        printf("Updating state from %s to %s\n", stateString(s->state), stateString(s->next));
        s->prev = s->state;
        s->state = s->next;
        s->next = UNKNOWN_STATE;
        resetBayesianState( &s->probabilities, s->prev );
    }
}

void initDefaultBayesianMap( state_bayesian_map_t * b )
{
    int l = NUM_STATES;
    b->length = l;
    for( int i = 0; i < l; i++ )
    {
        for( int j = 0; j < l; j++ )
        {
            b->map[i][j] = DEF_BAYESIAN_MAP[i][j];
        }
    }
    normalizeBayesianMap(b);
}

void normalizeBayesianMap( state_bayesian_map_t * b )
{
    int l = b->length;
    
    for( int i = 0; i < l; i++ )
    {
        normalizeBayesianState( b, i );
    }
    b->scheduled_normalize = false;
}

void normalizeBayesianState( state_bayesian_map_t * b, int i )
{
    int l = b->length;
    double total = 0.0;
    for( int j = 0; j < l; j++ ) total += b->map[j][i];
    if(total) for( int j = 0; j < l; j++ ) b->map[j][i] /= total;
    else b->map[i][i] = 1.0;
}

void resetBayesianState( state_bayesian_map_t * b, int i )
{
    int l = b->length;
    for( int j = 0; j < l; j++ ) b->map[j][i] = DEF_BAYESIAN_MAP[j][i];
    normalizeBayesianState( b, i );
}

void printBayesianMap( state_bayesian_map_t * b )
{
    int l = b->length;
    for( int i = 0; i < l; i++ ) printf("\t\t%s-[%d]", stateString(i), i);
    for( int i = 0; i < l; i++ )
    {
        printf("\n%s-[%d]", stateString(i), i);
        for( int j = 0; j < l; j++ )
        {
            printf("\t[%.2f]\t",b->map[i][j]);
        }
    }
    printf("\n");
}
