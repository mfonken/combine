//
//  kmat_utility.cpp
//  tau+
//
//  Created by Matthew Fonken on 2/8/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "kalman_matrix_utility.hpp"

#define STR(X) #X
#define PLURAL(X,Y) Y>1?(X STR(s)):X

KalmanMatrixPair::KalmanMatrixPair()
{
    printf("Initializing Kalman Matrix Pair.\n");
}

void KalmanMatrixPair::predict( PredictionPair * o )
{
    int xk = x.k_index, yk = y.k_index;
#ifdef KMAT_DEBUG
    printf("Getting KMat predictions: Xk-%d and Yk-%d\n", x.k_index, y.k_index);
#endif
    int xpv = 0, xsv = 0, ypv = 0, ysv = 0;
    double xpp = 0.0, xsp = 0.0, xap = 0.0, ypp = 0.0, ysp = 0.0, yap = 0.0;
    int il;
    KalmanFilter k;
    for(; xk >= 0; --xk)
    {
        il = x.lookup[xk];
        k = x.kalmans[il][x.selection[il]];
        if( xk >= 2) xap += k.K[0];
        else if( xk == 1 )
        {
            xsv = k.value;
            xsp = k.K[0];
        }
        else
        {
            xpv = k.value;
            xpp = k.K[0];
        }
    }
    for(; yk >= 0; --yk)
    {
        il = y.lookup[yk];
        k = y.kalmans[il][y.selection[il]];
        if( yk >= 2) yap += k.K[0];
        else if( yk == 1 )
        {
            ysv = k.value;
            ysp = k.K[0];
        }
        else
        {
            ypv = k.value;
            ypp = k.K[0];
        }
    }
    o->x.primary                = xpv;
    o->x.primary_probability    = xpp;
    o->x.secondary              = xsv;
    o->x.secondary_probability  = xsp;
    o->x.alternate_probability  = xap;
    
    o->y.primary                = ypv;
    o->y.primary_probability    = ypp;
    o->y.secondary              = ysv;
    o->y.secondary_probability  = ysp;
    o->y.alternate_probability  = yap;
    
#ifdef KMAT_DEBUG
    printf("O PREDICTION:\n\tVALUES: xp>%d & xs>%d | yp>%d & ys>%d\n", o->x.primary, o->x.secondary, o->y.primary, o->y.secondary);
    printf("\tPROBABILITIES:\n\t\tx - xpp>%f xsp>%f xap>%f\n", o->x.primary_probability, o->x.secondary_probability, o->x.alternate_probability);
    printf("\t\ty - ypp>%f ysp>%f yap>%f\n", o->y.primary_probability, o->y.secondary_probability, o->y.alternate_probability);
#endif
}

void KalmanMatrixPair::update( PeakListPair * p )
{
#ifdef KMAT_DEBUG
    printf("\n*=============================================== X ===============================================*\n");
#endif
    x.update( &p->x );
    
#ifdef KMAT_DEBUG
//    printf("\n*=============================================== Y ===============================================*\n");
#endif
//    y.update( &p->y );
}

KalmanMatrix::KalmanMatrix()
{
#ifdef KMAT_DEBUG
    printf(" k%d x p%d\n", MAX_KALMANS, MAX_PEAKS);
#endif
    for(int i = 0; i < MAX_KALMANS; i++)
    {
        lookup[i]            = i;
        selection[i]         = 0;
        pair[i].level        = 0;
        pair[i].persistence  = 0;
        density[i]           = 0;
        
#ifdef KMAT_DEBUG
        printf("\tk[%d]: lookup-%d selection-%d pair-%d density-%.1f\n", i, lookup[i], selection[i], pair[i].level, density[i]);
#endif
        for(int j = 0; j < MAX_PEAKS; j++)
        {
            KalmanFilter * k = &kalmans[i][j];
            (*k) = KalmanFilter(0.0, KMAT_LIFESPAN, VALUE_UNCERTAINTY, BIAS_UNCERTAINTY, SENSOR_UNCERTAINTY);
        }
    }
    k_index = 0;
    p_index = 0;
}

void KalmanMatrix::update( PeakList * p )
{
    if(p->length)
    {
        /*** 1) Add new kalmans for new peaks and update them ***/
        updateWithPeaks( p );
        
        /*** 2) Find highest kalman tracking values by tracking row ***/
        selectPeaks( p->length );
        
        /*** 3) Find density pairs in KalmanFilter matrix and give them a sorting bias ***/
        getCouples();
        
        /*** 4) Sort KalmanFilter Matrix ***/
        quickSort( 0, k_index-1, WEIGHTED );
    }
    
    /*** 5) Purge kalmans by selection if expired or best is still too small ***/
    purge();
    
#ifdef KMAT_DEBUG
    printf("*****************AFTER UPDATE****************\n");
    print();
#endif
}

/*** UPDATE ***/
void KalmanMatrix::updateWithPeaks( PeakList * p )
{
    KalmanFilter * kalp = NULL;
    
    /* Set length of peaks to check */
    ///TODO: Figure out how to check all peaks with MAX limit
    int pl = (p->length < MAX_PEAKS)?p->length:MAX_PEAKS; // Use all peaks
#ifdef KMAT_DEBUG
    printf("Receiving %d %s\n", pl, PLURAL("peak", pl));
    for(int i = 0; i < pl; i++) printf("#%d: %d/%d\n", i, p->map[i], p->den[i]);
#endif
    bool added = false;
    for(int i = k_index, il = lookup[0]; i < pl && i < MAX_KALMANS; i++, il = lookup[i])
    {
        kalp                = &kalmans[il][i];
        kalp->update(p->map[i], 0.);
//        kalp->value         = p->map[i];
        kalp->density       = p->den[i];
        selection[il]       = i;
        
#ifdef KMAT_DEBUG
        printf("i-%d il-%d\n",i, il);
#endif
        k_index++;
        // for(int j = 0; j < INIT_ITERATIONS; j++) Kalman_Update(kalp, p->map[i], (((rand() % 32) >> 5) - 0.5)); // Init with velocity variance
        
        added = true;
    }
    p_index = k_index;
    int ml = k_index;
    
#ifdef EXT_DEBUG
#ifdef KMAT_DEBUG
    if(added)
    {
        printf("*****************AFTER ADD****************\n");
        print();
    }
#endif
#endif
    
    for(int i = 0, il = lookup[0]; i < ml; i++, il = lookup[i])
    {
        KalmanFilter s = kalmans[il][selection[il]];
        /* Start new rows for valid new peaks */
        for(int j = 0; j < pl; j++)
        {
            KalmanFilter t = s;
            int val = p->map[j];
            int den = p->den[j];
#ifdef EXT_DEBUG
            printf("#%d-%d: %d|%d\n", il, j, val, den);
#endif
            if( den > MIN_PEAK )//&& INRANGE(val, t.value, MAX_VALUE_DIFF) )
            {
                double vel = val - s.value;
                t.update(val, vel);
                t.density = den;
            }
            else
            {
                t.last_update = -val;
            }
            kalmans[il][j] = t;
            density[il] = t.density;
            value[il] = t.K[0];
        }
    }
#ifdef EXT_DEBUG
#ifdef KMAT_DEBUG
    printf("*****************AFTER NEW PEAKS****************\n");
    print();
#endif
#endif
}

/*** SELECT ***/
void KalmanMatrix::selectPeaks( int pl )
{
    int ml = k_index;
    
    if( pl > p_index ) pl = p_index - 1;
    for(int i = 0, il = lookup[0]; i < ml; i++, il = lookup[i])
    {
        selectRow( pl, il );
#ifdef KMAT_DEBUG
        printf("\n");
#endif
    }
    
#ifdef EXT_DEBUG
#ifdef KMAT_DEBUG
    printf("*****************AFTER WEIGHTS****************\n");
    print();
#endif
#endif
}

void KalmanMatrix::selectRow( int pl, int il )
{
    KalmanFilter k;
    
    int s = 0;
    double v = kalmans[il][0].K[0];
    double ref = kalmans[il][0].density, d = ref;
    
#ifdef KMAT_DEBUG
    printf("Density referene is on kalman %d is %.2f\n",il, ref);
#endif
    
    for(int j = 1; j < pl; j++ )
    {
        k = kalmans[il][j];
        
        /* Adjust weights */
        double r = k.density / ref;
        if(r < 1000 && r > -1000) k.K[0] *= r;
        
#ifdef KMAT_DEBUG
        printf("\t[%d](%.3f>%.3f)",j,k.density,r);
#endif
        
        /* Find max */
        double n = k.K[0];
        if( GTTHR(n, v, SELECTION_THRESHOLD) ) //k.K[0] > v)
        {
            v = k.K[0];
            d = k.density;
            s = j;
        }
    }
#ifdef KMAT_DEBUG
    printf("\n");
#endif
    
#ifdef KMAT_DEBUG
    printf("Selecting %d for row %d\n", s, il);
#endif
    
    selection[il] = s;
    value[il] = v;
    density[il] = d;
}

/*** COUPLE ***/
void KalmanMatrix::getCouples()
{
    int ml = k_index;
    /* Sort matrix without weights */
    quickSort( 0, k_index-1, UNWEIGHTED );
    
    /* Start with higher pair level (higher sort bias) */
    int pair_level = KALMAN_PAIR_MAX;
    
    int min_den = -1;
    int curr_den = -1;
    int curr_lev = -1;
    
    int coup_ind[] = {0,0}; // coupling indeces
    int coup_lev = -1;      // coupling level
    
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
            il = lookup[i];
            curr_den = density[il];
            curr_lev = pair[il].level;
            if(curr_den < MIN_DENSITY) coup_lev = 0;
        }
        
        /* Finding first in density range (important: live matrix pair level should never go below 0) */
        if ( coup_lev == -1 )
        {
            min_den = curr_den - MAX_DENSITY_DIFF;
            coup_ind[0] = il;
            found_count = 1;
            coup_lev = pair[il].level;
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
                        discourageCouple( &pair[il] );
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
                    pair[coup_ind[0]].level = pair_level;
                    pair[coup_ind[1]].level = pair_level;
                    encourageCouple( &pair[coup_ind[0]] );
                    encourageCouple( &pair[coup_ind[1]] );
                    
                    /* Decrement pair level until zero */
                    !pair_level?0:pair_level--;
                    found_count = 0;
                    coup_found = false;
                }
                /* If no pair was found but an existing couple was found, discourage it */
                else if( coup_lev > 0 )
                {
                    discourageCouple( &pair[coup_ind[0]] );
                }
                coup_lev = -1;
                min_den = 0;
            }
        }
    }
}

/*** PURGE ***/
void KalmanMatrix::purge()
{
    int ml = k_index;
    int pl = p_index;
#ifdef KMAT_DEBUG
    printf("Purging Kalmans - k%dxp%d\n", ml, pl);
#endif
    int prev[ml-1];
    int pis[ml-1];
    int pi = 0;
    for(int i = 0, il = lookup[0]; i < ml; i++, il = lookup[i])
    {
        KalmanFilter * kalp = &kalmans[il][selection[il]];
        if( pl < CHAOS )
        {
#ifdef KMAT_DEBUG
            printf("Non-chaos similarity purge on row %d\n", il);
#endif
            int v = kalp->value;
            int lim = ml - 1;
            for(int j = 0; j < pi && j < ml; j++)
            {
#ifdef KMAT_DEBUG
                printf("Checking c>%d vs p>%d (%d)\n", v, prev[j], il);
#endif
                if( il != pis[j] && INRANGE(v,prev[j],SIMILARITY_TOLERANCE) )
                {
#ifdef KMAT_DEBUG
                    printf("k[%d](%.1f) ~= k[%d](%.1f)\n", il, kalmans[il][selection[il]].value, pis[j], kalmans[pis[j]][selection[pis[j]]].value);
#endif
                    kalp->K[0] *= PUNISH_FACTOR;
                    if(il < lim)
                    {
#ifdef KMAT_DEBUG
                        printf("Punishing row %d\n", il);
#endif
                        punishRow( il );
                    }
#ifdef KMAT_DEBUG
                    printf("Re-selecting row %d\n", il);
#endif
                    selectRow( pl, il );
                    il = lookup[++i];
                    if(il > 10 || il < 0)
                        il++;
                }
            }
            pis[pi] = il;
            prev[pi++] = v;
        }
        
        if(kalp->K[0] < MIN_PROB )
        {
            k_index = i;
            break;
        }
        else if( kalp->isExpired() )
        {
            (*kalp) = KalmanFilter(0.0, KMAT_LIFESPAN, VALUE_UNCERTAINTY, BIAS_UNCERTAINTY, SENSOR_UNCERTAINTY);
        }
    }
    /* Recount useful kalmans */
    int c = 0;
    for(int i = 0, il = lookup[0]; i < ml; i++, il = lookup[i] )
    {
        if( kalmans[il][selection[il]].K[0] >= MIN_PROB ) c++;
    }
    k_index = c;
//    printf("Useful kalmans: %d>%d \n", ki, c);
}

void KalmanMatrix::punishRow( int i )
{
    int lim = k_index - 1;
    int t = lookup[i];
    for(int k = i; k < lim; k++)
        lookup[k] = lookup[k+1];
    lookup[lim] = t;
}

double KalmanMatrix::weightedValue( int i )
{
    return value[lookup[i]] + ( KALMAN_SORT_SCALE_STEP * pair[lookup[i]].level );
}

/*** KalmanFilter Matrix Quicksort ***/
void KalmanMatrix::swap( kmat_p a, kmat_p b )
{
    if(a == b) return;
#ifdef EXT_DEBUG
#ifdef KMAT_DEBUG
    printf("\tSwapping lookup at %d and %d: [%d][%d]",a,b,lookup[a],lookup[b]);
    printf("\n");
#endif
#endif
    kmat_p t = lookup[a];
    lookup[a] = lookup[b];
    lookup[b] = t;
}
int KalmanMatrix::partition( int l, int h, bool w )
{
    int pivot = (w)?weightedValue(l):value[lookup[l]];
    int i = h+1;
    
    for (int j = h; j > l; j--)
    {
        if( (w)?weightedValue(j):value[lookup[j]] <= pivot) swap(--i,j);
    }
#ifdef EXT_DEBUG
#ifdef KMAT_DEBUG
    printf("Pivot ");
#endif
#endif
    swap(i-1,l);
    return (i - 1);
}
void KalmanMatrix::quickSort( int l, int h, bool w )
{
    if( l < h )
    {
        int p = partition( l, h, w);
        quickSort( l, p - 1, w);
        quickSort( p + 1, h, w);
    }
}
/*******************************/


void KalmanMatrix::print()
{
#ifdef DEBUG
    int ml = MAX_KALMANS;// k_index,
    int pl = MAX_PEAKS;
    
    /* Draw matrix */
    printf("%d(%d)x%d(%d) Matrix:\n\t\t",ml,k_index,pl,p_index);
    for(int j = 0; j < pl; j++) printf("\t\t\t\t\t%d", j);
    printf("\n");
    for(int i = 0, il = lookup[0]; i < ml; i++, il = lookup[i])
    {
        
        double s = value[il] + ( KALMAN_SORT_SCALE_STEP * pair[il].level );
        printf("%3d[%3d](%.0f)",i, il, s);
        for(int j = 0; j < pl; j++)
        {
            KalmanFilter k = kalmans[il][j];
            double v = (k.value < 1000)?k.value:999;
            double p = (k.prev < 1000)?k.prev:999;
            double d = (k.density < 1000)?k.density:999;
            double l = (k.last_update < 1000)?k.last_update:999;
            char c = ' ';
            if(j == selection[il]) c = '|';
            printf("\t\t%cv%3.0f|p%3.0f|l%3.0f|d%3.0f%c",c,v,p,l,d,c);
        }
        printf("\n");
        printf("s%3dv%3.2f\t", selection[il], value[il]);
        for(int j = 0; j < pl; j++)
        {
            KalmanFilter k = kalmans[il][j];
            double k0 = k.K[0], k1 = k.K[1];
            char c = ' ';
            if(j == selection[il]) c = '|';
            printf("\t\t%cK0>%1.2f|K1>%1.2f%c",c,k0,k1,c);
        }
        printf("\n\t\t\t\t");
        for(int j = 0; j < pl; j++)
        {
            KalmanFilter k = kalmans[i][j];
            double p00 = k.P[0][0], p01 = k.P[0][1];
            char c = ' ';
            if(j == selection[il]) c = '|';
            printf("\t%cP00>%.1f|P01>%.1f%c\t",c,p00,p01,c);
        }
        printf("\n\t\t\t\t");
        for(int j = 0; j < pl; j++)
        {
            KalmanFilter k = kalmans[i][j];
            double p10 = k.P[1][0], p11 = k.P[1][1];
            char c = ' ';
            if(j == selection[il]) c = '|';
            printf("\t%cP10>%.1f|P11>%.1f%c\t",c,p10,p11,c);
        }
        printf("\n\t\t\t\t");
        for(int j = 0; j < pl; j++)
        {
            KalmanFilter k = kalmans[i][j];
            double diff = now()-k.timestamp;
            if(diff>k.lifespan) diff = NAN;
    
            char c = ' ';
            if(j == selection[il]) c = '|';
            printf("\t%c%.3f%c\t\t\t\t",c,diff,c);
        }
        printf("\n\n");
    }
    //    printf("**********************\n");
#endif
}

void KalmanMatrix::printA(int arr[], int low, int high, int tab)
{
    for(int i = low; i <= high; i++) printf(" %2d   ", i);
    printf("\n");
    for(int i = 0; i < tab; i++) printf("\t");
    for(int i = low; i <= high; i++) printf("{%3d} ", arr[i]);
    printf("\n");
    for(int i = 0; i < tab; i++) printf("\t");
}
