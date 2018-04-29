//
//  state_machine_utility.cpp
//  tau+
//
//  Created by Matthew Fonken on 2/8/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "state_machine_utility.hpp"

#define DISTANCE_SQ(X,Y)                ((X*X)+(Y*Y))
#define PROBABILITY_TUNING_FACTOR       0.2
#define PROBABILITY_THRESHOLD           0.1
#define ABSENCE_FACTOR                  PROBABILITY_TUNING_FACTOR
#define PROBABILITY_TUNING_THRESHOLD    PROBABILITY_THRESHOLD * PROBABILITY_THRESHOLD
#define PROBABILITY_STABILITY_THRESHOLD 0.5
#define STATE_DISTANCE                  2.0

#define SHADOW_TOLERANCE 0.2

/* Temporary macros */
#define numberOfSelectedPeaks 1

BayesianMap::BayesianMap()
{
    printf("Initializing State Machine.\n");
    int l = NUM_STATES;
    length = l;
    for( int i = 0; i < l; i++ )
    {
        for( int j = 0; j < l; j++ )
        {
            map[i][j] = 0.0;
        }
        map[i][i] = 1.0;
    }
}

void BayesianMap::normalizeMap()
{
    for( int i = 0; i < length; i++ ) normalizeState( i );
}

void BayesianMap::normalizeState( int i )
{
    double total = 0.0;
    for( int j = 0; j < length; j++ ) total += map[j][i];
    if(total) for( int j = 0; j < length; j++ ) map[j][i] /= total;
    else map[i][i] = 1.0;
}

void BayesianMap::resetState( int i )
{
    for( int j = 0; j < length; j++ ) map[j][i] = 0.0;
    map[i][i] = 1.0;
    normalizeState( i );
}

void BayesianMap::print( state_t s)
{
    int l = length;
    for( int i = 0; i < l; i++ ) printf("\t\t %s-[%d]", stateString(i), i);
    for( int i = 0; i < l; i++ )
    {
        printf("\n%s-[%d]", stateString(i), i);
        for( int j = 0; j < l; j++ )
        {
            char c = ' ';
            if(j == (int)s) c = '|';
            printf("\t%c[%.2f]%c",c, map[i][j],c);
        }
    }
    printf("\n");
}

System::System()
{
    state                = STABLE_NONE;
    prev                 = UNKNOWN_STATE;
    next                 = UNKNOWN_STATE;
    selection_index      = 0;
    stability.primary    = 0.0;
    stability.secondary  = 0.0;
    stability.alternate  = 0.0;
    stability.overall    = 0.0;
    //    filter               = NO_FILTER;
}

void System::update( PredictionPair * p )
{
    /** Assumptions
     *  - Column is current state
     *  - Row is next state
     */
    
    int l = probabilities.length;
    int c = (int)state;
    double max_v = 0.0;
    int max_i = 0;
    
    /* Find most probable next state */
    for( int i = 0; i < l; i++ )
    {
        double p = probabilities.map[i][c];
        if( p > max_v )
        {
            max_v = p;
            max_i = i;
        }
    }
    if( max_i != state ) next = (state_t)max_i;
    
#ifdef STATEM_DEBUG
    printf("\n###### Current state is %s. ######\n\n", stateString((int)state));
    printf("Next state is %s(%.2f).\n", stateString((int)next), max_v);
#endif
    
    /* Update self-diagnostics based on state */
    double prob[] = {0.0, 0.0, 0.0, 0.0};
    double xpp = p->x.probabilities.primary, xsp = p->x.probabilities.secondary, ypp = p->y.probabilities.primary, ysp = p->y.probabilities.secondary;
    
    if( ( ( xpp > SHADOW_TOLERANCE ) ^ ( xsp > SHADOW_TOLERANCE ) ) && ( ( ypp > SHADOW_TOLERANCE ) ^ ( ysp > SHADOW_TOLERANCE ) ) )
    {
        bool xc = INRANGE(xpp, xsp, SHADOW_TOLERANCE),
             yc = INRANGE(ypp, ysp, SHADOW_TOLERANCE);
        if( xc && !yc ) copymax(&ypp, &ysp);
        if( !xc && yc ) copymax(&xpp, &xsp);
#ifdef STATEM_DEBUG
        printf("\tShadow> a: xp>%.2f xs>%.2f yp%.2f ys>%.2f\n", xpp, xsp, ypp, ysp);
#endif
    }
    if( p->selection_pair == SIMILAR )
    {
        prob[1] = DISTANCE_SQ(xpp, ypp);
        prob[2] = DISTANCE_SQ(xsp, ysp);
    }
    else if( p->selection_pair == OPPOSITE )
    {
        prob[1] = DISTANCE_SQ(xpp, ysp);
        prob[2] = DISTANCE_SQ(xsp, ypp);
    }
    /* Alternate probability is 2D chaos related, thus dependent on both existing */
    prob[3] = p->x.probabilities.alternate * p->y.probabilities.alternate;
    
#ifdef STATEM_DEBUG
    printf("Before: non-%.3f pri-%.3f sec-%.3f alt-%.3f\n", prob[0], prob[1], prob[2], prob[3]);
#endif
    
    int num_selections = 0;
    prob[0] = ABSENCE_FACTOR;
    bool ch[4] = { 0,
                   prob[1] > PROBABILITY_TUNING_THRESHOLD,
                   prob[2] > PROBABILITY_TUNING_THRESHOLD,
                   prob[3] > PROBABILITY_TUNING_THRESHOLD };
    num_selections += ch[1] + ch[2] + ch[3];
    ch[0] = ( num_selections > 0 );
    for(int i = 0; i < num_selections; i++) prob[i] *= ch[i];
    
#ifdef STATEM_DEBUG
    printf("After: non-%.3f pri-%.3f sec-%.3f alt-%.3f\n", prob[0], prob[1], prob[2], prob[3]);
#endif
    
    selection_index      = num_selections;
    stability.primary    = 0.0;
    stability.secondary  = 0.0;
    stability.alternate  = 0.0;
    
    int k = selection_index;
    
    double out[4];
    double f;
    for(int i = 0; i <= 3 /*k*/; i++)
    {
        out[i] = 0.0;
        double p = prob[i] * PROBABILITY_TUNING_FACTOR;
        f = (double)( i ) / 3.0;
        out[i] = p * f;
#ifdef STATEM_DEBUG
        printf("Punishing prob[%d]-%.3f by a factor of %.3f for a result of %.3f\n", i, p, f, out[i]);
#endif
    }
    updateProbabilities( out );
    probabilities.normalizeState(state);
    updateState();
}

void System::updateProbabilities( double p[4] )
{
    int c = (int)state;
    int x = stateNumber(c);
#ifdef STATEM_DEBUG
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
            probabilities.map[k][c] += p[i];
        }
    }
}

void System::updateState()
{
    if(next != UNKNOWN_STATE )
    {
#ifdef STATEM_DEBUG
        printf("Updating state from %s to %s\n", stateString((int)state), stateString((int)next));
        if(next != state) printf("~~~ State is %s ~~~\n", stateString(next));
#endif
        prev = state;
        state = next;
        next = UNKNOWN_STATE;
        probabilities.resetState(prev);
    }
}
