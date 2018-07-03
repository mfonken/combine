//
//  state_machine_utility.cpp
//  tau+
//
//  Created by Matthew Fonken on 2/8/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "state_machine_utility.h"

#define DISTANCE_SQ(X,Y)                ((X*X)+(Y*Y))
#define PROBABILITY_TUNING_FACTOR       0.5
#define PROBABILITY_THRESHOLD           0.1
#define PROBABILITY_ALTERNATE_THRESH    0.9
#define ABSENCE_FACTOR                  PROBABILITY_TUNING_FACTOR
#define PROBABILITY_TUNING_THRESHOLD    PROBABILITY_THRESHOLD * PROBABILITY_THRESHOLD
#define PROBABILITY_STABILITY_THRESHOLD 0.5
#define STATE_DISTANCE                  2.0
#define SHADOW_TOLERANCE                0.2

#define DOUBT_STABILITY                 0.5

#define INRANGE(X,Y,T)  (X>(Y-T)&&X<(Y+T))

/* Temporary macros */
#define numberOfSelectedPeaks 1

static double DOUBT( int i, state_t cs )
{
    double ret = ((double)(i<<1)+1.)/NUM_STATES;
    if( isStable(cs) && i < stateNumber(cs) )
        ret *= DOUBT_STABILITY;
    return ret;
}

static void initMap( bayesian_map_t * bm )
{
    printf("Initializing State Machine.\n");
    int l = NUM_STATES;
    bm->length = l;
    for( int i = 0; i < l; i++ )
    {
        for( int j = 0; j < l; j++ )
        {
            bm->map[i][j] = 0.0;
        }
        bm->map[i][i] = 1.0;
    }
}

static void normalizeMap( bayesian_map_t * bm )
{
    for( int i = 0; i < bm->length; i++ ) BayesianMap.normalizeState( bm, i );
}

static void normalizeState( bayesian_map_t * bm, int i )
{
    double total = 0.0;
    int l = bm->length;
    for( int j = 0; j < l; j++ ) total += bm->map[j][i];
    if(total) for( int j = 0; j < l; j++ ) bm->map[j][i] /= total;
    else bm->map[i][i] = 1.0;
}

static void resetState( bayesian_map_t * bm, int i )
{
    for( int j = 0; j < bm->length; j++ ) bm->map[j][i] = 0.0;
    bm->map[i][i] = 1.0;
    normalizeState( bm, i );
}

static void print( bayesian_map_t * bm, state_t s )
{
    int l = bm->length;
    for( int i = 0; i < l; i++ ) printf("\t\t %s-[%d]", stateString(i), i);
    for( int i = 0; i < l; i++ )
    {
        printf("\n%s-[%d]", stateString(i), i);
        for( int j = 0; j < l; j++ )
        {
            char c = ' ';
            if(j == (int)s) c = '|';
            printf("\t%c[%.2f]%c",c, bm->map[i][j],c);
        }
    }
    printf("\n");
}

const struct bayesian_map_functions BayesianMap =
{
    .initMap = initMap,
    .normalizeMap = normalizeMap,
    .normalizeState = normalizeState,
    .resetState = resetState,
    .print = print
};

static void init( bayesian_system_t * sys )
{
    sys->state                = STABLE_NONE;
    sys->prev                 = UNKNOWN_STATE;
    sys->next                 = UNKNOWN_STATE;
    sys->selection_index      = 0;
    sys->stability.primary    = 0.0;
    sys->stability.secondary  = 0.0;
    sys->stability.alternate  = 0.0;
    sys->stability.overall    = 0.0;
    
    BayesianMap.initMap( &sys->probabilities );
}

static void update( bayesian_system_t * sys, PredictionPairC * p )
{
    /** Assumptions
     *  - Column is current state
     *  - Row is next state
     */
    
    int l = sys->probabilities.length;
    int c = (int)sys->state;
    double max_v = 0.0;
    int max_i = 0;
    
    /* Find most probable next state */
    for( int i = 0; i < l; i++ )
    {
        double p = sys->probabilities.map[i][c];
        if( p > max_v )
        {
            max_v = p;
            max_i = i;
        }
    }
    if( max_i != sys->state ) sys->next = (state_t)max_i;
    
#ifdef STATEM_DEBUG
    printf("\n###### Current state is %s. ######\n\n", stateString((int)sys->state));
    printf("Next state is %s(%.2f).\n", stateString((int)sys->next), max_v);
#endif
    
    /* Update self-diagnostics based on state */
    double prob[] =
    {
        DISTANCE_SQ(p->x.probabilities.absence, p->y.probabilities.absence)/2,
        DISTANCE_SQ(p->x.probabilities.primary, p->y.probabilities.primary),
        DISTANCE_SQ(p->x.probabilities.secondary, p->y.probabilities.secondary),
        DISTANCE_SQ(p->x.probabilities.alternate, p->y.probabilities.alternate)
    };
    
#ifdef STATEM_DEBUG
    printf("Before: non-%.3f pri-%.3f sec-%.3f alt-%.3f\n", prob[0], prob[1], prob[2], prob[3]);
#endif
    
    int num_selections = 0;
//    prob[0] = ABSENCE_FACTOR;
    bool ch[4] = { 0,
                   prob[1] > PROBABILITY_TUNING_THRESHOLD,
                   prob[2] > PROBABILITY_TUNING_THRESHOLD,
                   prob[3] > PROBABILITY_TUNING_THRESHOLD };
    num_selections += ch[1] + ch[2] + ch[3];
//    ch[0] = ( num_selections > 0 );
//    for(int i = 0; i < num_selections; i++) prob[i] *= (double)ch[i];
    
#ifdef STATEM_DEBUG
    printf("After: non-%.3f pri-%.3f sec-%.3f alt-%.3f\n", prob[0], prob[1], prob[2], prob[3]);
#endif
    
    sys->selection_index      = num_selections;
//    sys->stability.primary    = 0.0;
//    sys->stability.secondary  = 0.0;
//    sys->stability.alternate  = 0.0;
//    
//    int k = sys->selection_index;
    
    double out[4] = { prob[0], prob[1], prob[2], prob[3] };
    double f;
    for(int i = 0; i <= num_selections; i++)
    {
        out[i] = 0.0;
        double p = prob[i] * PROBABILITY_TUNING_FACTOR;
        f = (double)DOUBT(i, sys->state);
        out[i] = p * f;
#ifdef STATEM_DEBUG
        printf("Punishing prob[%d]-%.3f by a factor of %.3f for a result of %.3f\n", i, p, f, out[i]);
#endif
    }
    
    /* Independent Alternate Check */
    if( prob[3] > PROBABILITY_ALTERNATE_THRESH )
        out[3] = prob[3] * PROBABILITY_TUNING_FACTOR * PROBABILITY_TUNING_FACTOR;
    
    BayesianSystem.updateProbabilities( sys, out );
    BayesianMap.normalizeState( &sys->probabilities, sys->state );
    BayesianSystem.updateState( sys );
#ifdef STATEM_DEBUG
    print( &sys->probabilities, sys->state );
#endif
}

static void updateProbabilities( bayesian_system_t * sys, double p[4] )
{
    int c = (int)sys->state;
    int x = stateNumber(c);
#ifdef STATEM_DEBUG
    for(int i = 0; i < 4; i++)
        if( p[i] > 10 )
            printf("!\n");
    printf("Probabilies are [0]%.2f [1]%.2f [2]%.2f [3]%.2f.\n", p[0], p[1], p[2], p[3]);
    printf("State %s is ", stateString(c));
    if(!isStable(c)) printf("not ");
    printf("stable.\n");
#endif
    for( int i = 0; i < 4; i++ )
    {
        int k = -1;
        int j = x - i;
        if(!isStable(c))
        {
            if( x == i )
                k = c - 1;
            else
                k = c - ( 2 * j );
        }
        else if( x != i )
            k = ( c + 1 ) - ( 2 * j );
        else
            k = c;
        if(k != -1)
        {
#ifdef STATEM_DEBUG
            printf("Updating %s by %.2f.\n", stateString(k), p[i]);
#endif
            sys->probabilities.map[k][c] += p[i];
        }
        else
            k = -100;
    }
}

static void updateState( bayesian_system_t * sys )
{
    if(sys->next != UNKNOWN_STATE )
    {
#ifdef STATEM_DEBUG
        printf("Updating state from %s to %s\n", stateString((int)sys->state), stateString((int)sys->next));
        if(sys->next != sys->state) printf("~~~ State is %s ~~~\n", stateString(sys->next));
#endif
        sys->prev = sys->state;
        sys->state = sys->next;
        sys->next = UNKNOWN_STATE;
        BayesianMap.resetState( &sys->probabilities, sys->prev );
    }
}

const struct bayesian_system_functions BayesianSystem =
{
    .init = init,
    .update = update,
    .updateProbabilities = updateProbabilities,
    .updateState = updateState
};
