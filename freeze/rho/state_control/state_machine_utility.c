//
//  state_machine_utility.cpp
//  tau+
//
//  Created by Matthew Fonken on 2/8/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "state_machine_utility.h"
#include "rho_master.h"

typedef define_loop_variable_template_struct(state_dimension_t, state_global_t);
state_global_t _;

/* Temporary macros */
#define numberOfSelectedPeaks 1

static inline void reset_loop_variables( state_global_t * _, state_dimension_t l )
{ _->l = l; _->i = 0; _->j = 0; _->u = 0; _->v = 0.; }

static floating_t DOUBT( state_dimension_t i, state_t cs )
{
    floating_t ret = ( (floating_t)( ( i << 1 ) + 1 ) ) / NUM_STATES;
    if( isStable(cs) && ( i < stateNumber(cs) ) )
        ret *= (floating_t)DOUBT_STABILITY;
    return ret;
}

void InitBayesianMap( bayesian_map_t * bm )
{
    LOG_STATEM("Initializing State Machine.\n");
    reset_loop_variables( &_, NUM_STATES );
    bm->length = NUM_STATES;
    for( ; _.i < _.l; _.i++ )
    {
        for( _.j = 0; _.j < _.l; _.j++ )
            bm->map[_.i][_.j] = 0.0;
        bm->map[_.i][_.i] = 1.0;
    }
}

void NormalizeBayesianMap( bayesian_map_t * bm )
{
    reset_loop_variables( &_, bm->length );
    for(  _.i = 0; _.i < _.l; _.i++ )
        BayesianFunctions.Map.NormalizeState( bm, _.i );
}

void NormalizeBayesianState( bayesian_map_t * bm, state_dimension_t i )
{
    reset_loop_variables( &_, bm->length );
    floating_t * total = &_.v;
    for( _.j = 0; _.j < _.l; _.j++ ) *total += bm->map[_.j][i];
    if(*total)
    {
        _.v = ZDIV( 1., (*total) );
        for( _.j = 0; _.j < _.l; _.j++ ) bm->map[_.j][i] *= _.v;
    }
    else bm->map[_.i][i] = 1.0;
}

void ResetBayesianState( bayesian_map_t * bm, state_dimension_t i )
{
    reset_loop_variables( &_, bm->length );
    for( _.j = 0; _.j < _.l; _.j++ ) bm->map[_.j][i] = 0.0;
    bm->map[i][i] = 1.0;
}

void PrintBayesianMap( bayesian_map_t * bm, state_t s )
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
            printf("\t%c[%.2f]%c",c, bm->map[_.i][_.j],c);
        }
    }
    printf("\n");
#endif
}

void InitBayesianSystem( bayesian_system_t * sys )
{
    sys->state                = STABLE_NONE;
    sys->prev                 = UNKNOWN_STATE;
    sys->next                 = UNKNOWN_STATE;
    sys->selection_index      = 0;
    sys->stability.primary    = 0.;
    sys->stability.secondary  = 0.;
    sys->stability.alternate  = 0.;
    sys->stability.overall    = 0.;
    
    BayesianFunctions.Map.InitMap( &sys->probabilities );
}

void UpdateBayesianSystem( bayesian_system_t * sys, prediction_pair_t * p )
{
    reset_loop_variables( &_, NUM_STATES );
    state_t next = sys->state;

    bool ch[NUM_STATE_GROUPS] = { 0 };
    for(uint8_t i = 0; i < NUM_STATE_GROUPS; i++)
    {
        ch[i] = p->Probabilities.P[i] > PROBABILITY_TUNING_THRESHOLD;
    }
    
    floating_t out[NUM_STATE_GROUPS] = { 0. };
    /* Find most probable next state */
    for( ; _.i < _.l; _.i++ )
    {
        _.v = sys->probabilities.map[_.i][(state_dimension_t)sys->state];
        if( _.v > _.u )
        {
            _.u = _.v;
            next = (state_t)_.i;
        }
    }
    /* Only update sys->next state on change */
    if( next != sys->state ) sys->next = next;
    LOG_STATEM("\n###### Current state is %s. ######\n\n", stateString((state_dimension_t)sys->state));
    LOG_STATEM("Next state is %s(%.2f).\n", stateString((state_dimension_t)sys->next), _.u);
    _.l = ch[1] + ch[2] + ch[3];
    sys->selection_index = _.l;
    for( _.i = 0; _.i <= _.l; _.i++)
    {
        _.u = p->Probabilities.P[_.i];
        _.v = (floating_t)DOUBT(_.i, sys->state);
        out[_.i] = _.u * _.v;
        LOG_STATEM("Punishing prob[%d]-%.3f by a factor of %.3f for a result of %.3f\n", _.i, _.u, _.v, out[_.i]);
    }
    
    BayesianFunctions.Sys.UpdateProbabilities( sys, out );
    BayesianFunctions.Map.NormalizeState( &sys->probabilities, sys->state );
    BayesianFunctions.Sys.UpdateState( sys );
    PrintBayesianMap( &sys->probabilities, sys->state );
}

void UpdateBayesianProbabilities( bayesian_system_t * sys, floating_t p[4] )
{
    state_t           c = sys->state;
    state_dimension_t x = stateNumber(c);
    int8_t            k = -1;
    
#ifdef STATEM_DEBUG
    for( _.i = 0; _.i < 4; _.i++)
        if( p[_.i] > 10 )
            printf("!\n");
    printf("Probabilies are [0]%.2f [1]%.2f [2]%.2f [3]%.2f.\n", p[0], p[1], p[2], p[3]);
    printf("State %s is ", stateString(c));
    if(!isStable(c)) printf("not ");
        printf("stable.\n");
#endif
    for( _.i = 0; _.i < 4; _.i++ )
    {
        _.j = x - _.i;
        if(!isStable(c))
        {
            if( x == _.i ) k = c - 1;
            else k = c - ( _.j << 1 );
        }
        else if( x == _.i ) k = c;
        else k = ( c + 1 ) - ( _.j << 1);
        
        LOG_STATEM("Updating %s by %.2f.\n", stateString(k), p[_.i]);
        sys->probabilities.map[k][c] += p[_.i];
    }
}

void UpdateBayesianState( bayesian_system_t * sys )
{
    if(sys->next != UNKNOWN_STATE )
    {
        LOG_STATEM("Updating state from %s to %s\n", stateString((int)sys->state), stateString((int)sys->next));
        if(sys->next != sys->state) {LOG_STATEM("~~~ State is %s ~~~\n", stateString(sys->next));}
        sys->prev   = sys->state;
        sys->state  = sys->next;
        sys->next   = UNKNOWN_STATE;
        BayesianFunctions.Map.ResetState( &sys->probabilities, sys->prev );
    }
}
