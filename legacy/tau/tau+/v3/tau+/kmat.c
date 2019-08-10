//
//  kmat.c
//  kmatrix
//
//  Created by Matthew Fonken on 10/9/17.
//  Copyright © 2017 Marbl. All rights reserved.
//


///TODO: Implement pair perging!!! (And the other perging)

#include "kmat_print.h"

#define GTTHR(X,Y,T) X>(Y+T)
#define STR(X) #X
#define PLURAL(X,Y) Y>1?(X STR(s)):X

void initKMatPair( kmat_pair_t * m )
{
#ifdef KMAT_DEBUG
    printf("Initializing Kalman Matrix -");
    printf("\n\tX:");
#endif
    initKMat(&m->x);
#ifdef KMAT_DEBUG
    printf("\n\tY:");
#endif
    initKMat(&m->y);
#ifdef KMAT_DEBUG
    printf("\n");
#endif
}

void getKMatPrediction( kmat_pair_t * m, prediction_pair_t * o )
{
    o->x.primary = m->x.kalmans[m->x.lookup[0]][m->x.selection[m->x.lookup[0]]].value;
    o->x.secondary = m->x.kalmans[m->x.lookup[1]][m->x.selection[m->x.lookup[1]]].value;
    o->x.primary_probability = m->x.kalmans[m->x.lookup[0]][m->x.selection[m->x.lookup[0]]].K[0];
    o->x.secondary_probability = m->x.kalmans[m->x.lookup[1]][m->x.selection[m->x.lookup[1]]].K[0];
    
    o->y.primary = m->y.kalmans[m->y.lookup[0]][m->y.selection[m->y.lookup[0]]].value;
    o->y.secondary = m->y.kalmans[m->y.lookup[1]][m->y.selection[m->y.lookup[1]]].value;
    o->y.primary_probability = m->y.kalmans[m->y.lookup[0]][m->y.selection[m->y.lookup[0]]].K[0];
    o->y.secondary_probability = m->y.kalmans[m->y.lookup[1]][m->y.selection[m->y.lookup[1]]].K[0];
    
#ifdef KMAT_DEBUG
    printf("O: xp>%d & xs>%d | yp>%d & ys>%d\n", o->x.primary, o->x.secondary, o->y.primary, o->y.secondary);
#endif
}

void updateKMatPair( kmat_pair_t * m, peak_list_pair_t * p )
{
#ifdef KMAT_DEBUG
    printf("\n*=============================================== X ===============================================*\n");
#endif
    updateKMat( &m->x, &p->x );
    
#ifdef KMAT_DEBUG
    printf("\n*=============================================== Y ===============================================*\n");
#endif
    updateKMat( &m->y, &p->y );
}

void initKMat( kmat_t * m )
{
#ifdef KMAT_DEBUG
    printf(" k%d x p%d\n", MAX_KALMANS, MAX_PEAKS);
#endif
    for(int i = 0; i < MAX_KALMANS; i++)
    {
        m->lookup[i]            = i;
        m->selection[i]         = 0;
        m->pair[i].level        = 0;
        m->pair[i].persistence  = 0;
        m->density[i]           = 0;
        
#ifdef KMAT_DEBUG
        printf("\tk[%d]: lookup-%d selection-%d pair-%d density-%.1f\n", i, m->lookup[i], m->selection[i], m->pair[i].level, m->density[i]);
#endif
        for(int j = 0; j < MAX_PEAKS; j++)
        {
            kalman_t * k = &m->kalmans[i][j];
            Kalman_Init(k, 0.0);
        }
    }
    m->k_index = 0;
    m->p_index = 0;
}

int updateKMat( kmat_t * m, peak_list_t * p )
{
    kalman_t kal;
    kalman_t * kalp = &kal;
    Kalman_Init(kalp, 0);
    
    ///TODO: Combine for loops!!!
    
    /* Set length of peaks to check */
    ///TODO: Figure out how to check all peaks with MAX limit
    int pl = (p->length < MAX_PEAKS)?p->length:MAX_PEAKS; // Use all peaks
    
#ifdef KMAT_DEBUG
    printf("Receiving %d %s\n", pl, PLURAL("peak", pl));
#endif
    
    /*** Add new kalmans for new peaks ***/
    int i = m->k_index;
    for(int il = m->lookup[0]; i < pl && i < MAX_KALMANS; i++, il = m->lookup[i])
    {
        kalp                = &m->kalmans[il][i];
        kalp->value         = p->map[i];
        kalp->density       = p->den[i];
        m->selection[il]    = i;
        
#ifdef KMAT_DEBUG
        printf("i-%d il-%d\n",i, il);
#endif
        
//        for(int j = 0; j < INIT_ITERATIONS; j++) Kalman_Update(kalp, p->map[i], (((rand() % 32) >> 5) - 0.5)); // Init with velocity variance
    }
    m->k_index = i;
    m->p_index = pl;
    
#ifdef EXT_DEBUG
    printf("*****************AFTER ADD****************\n");
    printKM( m );
#endif
//    for(int j = 0; j < MAX_PEAKS; pl += (r->values[r->lookup[j]] > MIN_PROB)?1:0, j++); // Use most probable peaks (only if sorted)
    
    /*** Perge all unused kalmans by row - reset all in row after expired kalman (make sure the cols are being sorted) ***/
    for(int j = m->p_index; j > pl && 0; j--)
    {
        bool expired = false;
        for(int i = 0, il = m->lookup[0]; i < m->k_index; i++, il = m->lookup[i])
        {
            kalp = &m->kalmans[il][j];
            if(expired || (expired = isExpired(kalp)))
            {
                Kalman_Init(kalp, 0.0);
                
                /* If entire row is perged, stop tracking */
                if(i == 0) m->p_index--;
            }
            else
            {
                /* Add new kalman row for new interesting objects */
                m->k_index++;
            }
        }
    }
    
    /* Get post-perged kalman count */
    int ml = m->k_index;
    
    /*** Update kalmans with new peaks ***/
    for(int i = 0, il = m->lookup[0]; i < ml; i++, il = m->lookup[i])
    {
        kalman_t s = m->kalmans[il][m->selection[il]];
        /* Start new rows for valid new peaks */
        for(int j = 0; j < pl; j++)
        {
            int val = p->map[j];
            int den = p->den[j];
            if( den > MIN_PEAK )
            {
                double vel = val - s.value;
                Kalman_Update(&s, val, vel);
                s.density = den;
            }
            m->kalmans[il][j] = s;
        }
    }
#ifdef EXT_DEBUG
    printf("*****************AFTER NEW PEAKS****************\n");
    printKM( m );
#endif
    //    for(int j = 0; j < MAX_PEAKS; pl += (r->values[r->lookup[j]] > MIN_PROB)?1:0, j++); // Use most probable peaks (only if sorted)
    
    /* Adjust Kalman weights */
    for(int i = 0, il = m->lookup[0]; i < ml; i++, il = m->lookup[i])
    {
        double ref = m->kalmans[m->lookup[il]][0].density;
#ifdef KMAT_DEBUG
        printf("Density referene is on kalman %d is %.2f\n",il, ref);
        printf("[%d]",i);
#endif
        for(int j = 0; j < pl; j++, kalp = &m->kalmans[il][j])
        {
            double r = kalp->density / ref;
            if(r < 1000 && r > -1000) kalp->K[0] *= r;
#ifdef KMAT_DEBUG
            printf("\t[%d](%.3f>%.3f)",j,kalp->density,r);
#endif
        }
#ifdef KMAT_DEBUG
        printf("\n");
#endif
    }
    
#ifdef EXT_DEBUG
    printf("*****************AFTER WEIGHTS****************\n");
    printKM( m );
#endif
    
    /*** Find highest kalman tracking values by tracking row ***/
    for(int i = 0, il = m->lookup[0]; i < ml; i++, il = m->lookup[i])
    {
        int s = 0;
        double v = 0.0;
        
        /* Find highest kalman gain */
        for(int j = 0; j < pl; j++, kalp = &m->kalmans[il][j])
        {
            if(kalp->K[0] > v)
            {
                v = kalp->K[0];
                s = j;
            }
        }
        
        if(GTTHR(m->value[il], v, SELECTION_THRESHOLD))
        {
#ifdef KMAT_DEBUG
            printf("Setting kalman %d to selection-%d value-%.2f density-%.2f\n",i,s,v,kalp->density);
#endif
            m->selection[il] = s;
            m->value[il] = v;
            m->density[il] = kalp->density;
       }
    }
    
    /*** Find density pairs in Kalman matrix and give them a sorting bias ***/
    /* Sort matrix without weights */
    quickSortKMat( m, 0, m->k_index-1, UNWEIGHTED );
    
    /* Start with higher pair level (higher sort bias) */
    uint8_t pair_level = KALMAN_PAIR_MAX;

    int min_den = -1;
    int curr_den = -1;
    int curr_lev = -1;
    
    int coup_ind[2];  // coupling indeces
    int coup_lev = -1;  // coupling level
    
    int found_count = 0;
    bool coup_found = false;
    
#define MIN_DENSITY 10
    for( int i = 0, il = 0; i <= ml; i++ )
    {
        if(i == ml)
        {
            curr_den = 0;
            coup_lev = 0;
        }
        else
        {
            il = m->lookup[i];
            curr_den = p->den[i];// m->density[il];
            curr_lev = m->pair[il].level;
            if(curr_den < MIN_DENSITY) coup_lev = 0;
        }
        
        /* Finding first in density range (important: live matrix pair level should never go below 0) */
        if ( coup_lev == -1 )
        {
            min_den = curr_den - MAX_DENSITY_DIFF;
            coup_ind[0] = il;
            found_count = 1;
            coup_lev = m->pair[il].level;
        }
        /* Finding similar to first in density range */
        else
        {
            /* If current is within first's density range */
            if( curr_den >= min_den )
            {
                /* If already exist coupling */
                if( curr_lev > 0 )
                {
                    /* If two existing couple haven't already been found */
                    if( !coup_found )
                    {
                        /* If first with existing coupling in density range, set as first */
                        if( curr_lev > coup_lev )
                        {
                            coup_lev = curr_lev;
                            
                            /* Save if uncoupled indeces were saved in the first spot */
                            if( found_count == 1 )
                            {
                                coup_ind[1] = coup_ind[0];
                                found_count++;
                            }
                            coup_ind[0] = il;
                        }
                        /* Otherwise set as second */
                        else
                        {
                            coup_ind[1] = il;
                            found_count = 2;
                            coup_found = true;
                        }
                    }
                    /* Otherwise discourage any remaining existing couplings in density range */
                    else
                    {
                        discourageCouple( &m->pair[il] );
                    }
                }
                /* If has no coupling */
                else
                {
                    /* If two pairs haven't already been found */
                    if( !coup_found )
                    {
                        coup_ind[1] = il;
                        found_count = 2;
                    }
                }
            }
            /* Otherwise reset to look for new first and density range */
            else
            {
                /* If a pair was found in range... */
                if(found_count == 2)
                {
                    m->pair[coup_ind[0]].level = pair_level;
                    m->pair[coup_ind[1]].level = pair_level;
                    encourageCouple( &m->pair[coup_ind[0]] );
                    encourageCouple( &m->pair[coup_ind[1]] );
                    
                    /* Decrement pair level until zero */
                    !pair_level?0:pair_level--;
                    found_count = 0;
                    coup_found = false;
                }
                /* If no pair was found but an existing couple was found, discourage it */
                else if( coup_lev > 0 )
                {
                    discourageCouple( &m->pair[coup_ind[0]] );
                }
                coup_lev = -1;
                min_den = 0;
            }
        }
    }
    
    /*** Sort Kalman Matrix ***/
    quickSortKMat( m, 0, m->k_index-1, WEIGHTED );
    
    /** Perge kalmans by column if best is still too small */
    for(int i = 0, il = m->lookup[0]; i < ml; i++, il = m->lookup[i])
    {
        if(m->kalmans[il][m->selection[il]].K[0] < MIN_PROB)
        {
            m->k_index = i;
            break;
        }
    }
    
#ifdef KMAT_DEBUG
    printf("*****************AFTER UPDATE****************\n");
    printKM( m );
#endif
   
    /* Select top from Kalman Matrix */
    int il = m->lookup[0];
    int c = m->selection[il];
    int s = (int)(m->kalmans[il][c].value);
    return s;
}

double weightedValue( kmat_t * m, int i )
{
    return m->value[m->lookup[i]] + ( KALMAN_SORT_SCALE_STEP * m->pair[m->lookup[i]].level );
}

bool isExpired( kalman_t * k )
{
    return ((timestamp() - k->timestamp) > MAX_KALMAN_LIFE);
}

/*** Kalman Matrix Quicksort ***/
void swapKMat( kmat_t * m, kmat_p a, kmat_p b )
{
    if(a == b) return;
#ifdef EXT_DEBUG
    printf("\tSwapping lookup at %d and %d: [%d][%d]",a,b,m->lookup[a],m->lookup[b]);
    printf("\n");
#endif
    kmat_p t = m->lookup[a];
    m->lookup[a] = m->lookup[b];
    m->lookup[b] = t;
}
int partitionKMat(kmat_t * m, int l, int h, bool w )
{
    int pivot = (w)?weightedValue(m, l):m->value[m->lookup[l]];
    int i = h+1;
    
    for (int j = h; j > l; j--)
    {
        if( weightedValue(m, j) <= pivot) swapKMat(m,--i,j);
    }
#ifdef EXT_DEBUG
    printf("Pivot ");
#endif
    swapKMat(m,i-1,l);
    return (i - 1);
}
void quickSortKMat(kmat_t * m, int l, int h, bool w )
{
    if( l < h )
    {
        int p = partitionKMat(m, l, h, w);
        quickSortKMat(m, l, p - 1, w);
        quickSortKMat(m, p + 1, h, w);
    }
}
/*******************************/
