//
//  state_machine_utility.cpp
//  tau+
//
//  Created by Matthew Fonken on 2/8/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "state_machine_utility.h"

typedef define_loop_variable_template_struct(state_dimension_t, state_global_t);
state_global_t _;

/* Temporary macros */
#define numberOfSelectedPeaks 1

static inline void reset_loop_variables( state_global_t * _, state_dimension_t l )
{ _->l = l; _->i = 0; _->j = 0; _->u = 0; _->v = 0.; }

void InitializeMarkovMap( markov_map_t * bm )
{
    LOG_STATEM(DEBUG_2, "Initializeializing State Machine.\n");
    reset_loop_variables( &_, NUM_STATES );
    bm->length = NUM_STATES;
    for( ; _.i < _.l; _.i++ )
    {
        for( _.j = 0; _.j < _.l; _.j++ )
            bm->map[_.i][_.j] = 0.0;
        bm->map[_.i][_.i] = 1.0;
    }
}

void NormalizeMarkovMap( markov_map_t * bm )
{
    reset_loop_variables( &_, bm->length );
    for(  _.i = 0; _.i < _.l; _.i++ )
        MarkovFunctions.Map.NormalizeState( bm, _.i );
}

void NormalizeMarkovState( markov_map_t * bm, state_dimension_t i )
{
    reset_loop_variables( &_, bm->length );
    floating_t * total = &_.v;
    for( _.j = 0; _.j < _.l; _.j++ )
    {
        _.u = bm->map[i][_.j];
        if( _.u >= 0. )
            *total += _.u;
        else
            bm->map[i][_.j] = 0.;
    }
    if(*total)
    {
        _.v = ZDIV( 1., (*total) );
        for( _.j = 0; _.j < _.l; _.j++ ) bm->map[i][_.j] *= _.v;
    }
    else bm->map[_.i][i] = 1.0;
}

void ResetMarkovState( markov_map_t * bm, state_dimension_t i )
{
    reset_loop_variables( &_, bm->length );
    for( _.j = 0; _.j < _.l; _.j++ ) bm->map[i][_.j] = 0.0;
    bm->map[i][i] = 1.0;
}

void PrintMarkovMap( markov_map_t * bm, state_t s )
{
#ifdef STATEM_DEBUG
    reset_loop_variables( &_, bm->length );
    for( _.i = 0; _.i < _.l; _.i++ ) printf("\t\t %s-[%d]", stateString((state_dimension_t)_.i), _.i);
    for( _.i = 0; _.i < _.l; _.i++ )
    {
        printf("\n%s-[%d]", stateString((state_dimension_t)_.i), _.i);
        for( _.j = 0; _.j < _.l; _.j++ )
        {
            char c = ' ';
            if(_.j == (state_dimension_t)s) c = '|';
            printf("\t%c[%.5f]%c",c, bm->map[_.j][_.i],c);
        }
    }
    printf("\n");
#endif
}

void InitializeMarkovSystem( markov_system_t * sys )
{
    sys->state                = STABLE_NONE;
    sys->prev                 = UNKNOWN_STATE;
    sys->next                 = UNKNOWN_STATE;
    sys->selection_index      = 0;
    sys->stability.primary    = 0.;
    sys->stability.secondary  = 0.;
    sys->stability.alternate  = 0.;
    sys->stability.overall    = 0.;
    
    MarkovFunctions.Map.InitializeMap( &sys->probabilities );
}

void DecayInactiveMarkovSystem( markov_system_t * sys )
{
    reset_loop_variables( &_, NUM_STATES );
    uint8_t c = sys->state;
    if( c == UNKNOWN_STATE ) return;
    for( _.i = 0; _.i < NUM_STATES; _.i++ )
    {
        if( _.i == c ||
           ( !isStable(c) && ( _.i == ( c - 1 ) ) ) ) continue;
        uint8_t state_distance = abs( _.i - c );
        for( _.j = 0; _.j < NUM_STATES; _.j++ ) // Punish based on relevance(distance) from current state
        {
            sys->probabilities.map[_.i][_.j] -= STATE_PUNISH * state_distance;
            if( sys->probabilities.map[_.i][_.j] < 0 ) sys->probabilities.map[_.i][_.j] = 0.;
        }
    }
}

void UpdateMarkovSystem( markov_system_t * sys, floating_t p[4] )
{
    reset_loop_variables( &_, NUM_STATES );
    
    MarkovFunctions.Sys.UpdateProbabilities( sys, p );
    MarkovFunctions.Map.NormalizeState( &sys->probabilities, sys->state );
    
    state_t next = sys->state;
    
    /* Find most probable next state */
    for( ; _.i < _.l; _.i++ )
    {
        _.v = sys->probabilities.map[(state_dimension_t)sys->state][_.i];
        if( _.v > _.u )
        {
            _.u = _.v;
            next = (state_t)_.i;
        }
    }
    /* Only update sys->next state on change */
    if( next != sys->state ) sys->next = next;
    
    MarkovFunctions.Sys.UpdateState( sys );
    MarkovFunctions.Sys.DecayInactive( sys );
    PrintMarkovMap( &sys->probabilities, sys->state );
}

void UpdateMarkovProbabilities( markov_system_t * sys, floating_t p[4] )
{
    state_t           c = sys->state;
    state_dimension_t x = stateNumber(c);
    int8_t            k = -1;
    bool              stable = isStable(c);

#ifdef STATEM_DEBUG
    for( _.i = 0; _.i < NUM_STATE_GROUPS; _.i++)
        if( p[_.i] > 10 )
            printf("!\n");
    printf("Probabilies are [0]%.2f [1]%.2f [2]%.2f [3]%.2f.\n", p[0], p[1], p[2], p[3]);
    printf("State %s is ", stateString(c));
    if(!isStable(c)) printf("not ");
        printf("stable.\n");
#endif
    
    _.u = 0.;
    
    for( _.i = 0; _.i < NUM_STATE_GROUPS; _.i++ )
    {
        _.j = x - _.i;
        if(!isStable(c))
        {
            if( !_.j ) k = c - 1;
            else k = c - ( _.j << 1 );
        }
        else if( x == _.i ) k = c;
        else k = ( c + 1 ) - ( _.j << 1);
        
        LOG_STATEM(DEBUG_2, "Updating %s by %.2f.\n", stateString(k), p[_.i]);
        if( stable )
        { // Stable states must return to unstable before dropping
            if( _.i < c )
                _.u += p[_.i];
            else if ( _.i == c + 1)
            { // Put all drop suggestions into instability for current stable state
                sys->probabilities.map[c][k] += _.u;
                _.u = 0.;
            }
        }
        sys->probabilities.map[c][k] += p[_.i];
        sys->probabilities.map[c][k] /= 2.;
        
        if( !stable && k == c - 1 )
        {
            floating_t transfer = sys->probabilities.map[c][c] * STATE_DECAY;
            sys->probabilities.map[c][k] += transfer;
            sys->probabilities.map[c][c] -= transfer;
        }
    }
}

void UpdateMarkovState( markov_system_t * sys )
{
    if(sys->next != UNKNOWN_STATE )
    {
        LOG_STATEM(DEBUG_2, "Updating state from %s to %s\n", stateString((int)sys->state), stateString((int)sys->next));
        if(sys->next != sys->state) {LOG_STATEM(DEBUG_2, "~~~ State is %s ~~~\n", stateString(sys->next));}
        sys->prev   = sys->state;
        sys->state  = sys->next;
        sys->next   = UNKNOWN_STATE;
//        MarkovFunctions.Map.ResetState( &sys->probabilities, sys->prev );
    }
}
