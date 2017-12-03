//
//  kmat.c
//  kmatrix
//
//  Created by Matthew Fonken on 10/9/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "kmat_print.h"


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
    int xk = m->x.k_index-1, yk = m->y.k_index-1;
#ifdef KMAT_DEBUG
    printf("Getting KMat predictions: Xk-%d and Yk-%d\n", m->x.k_index, m->y.k_index);
#endif
    int xpv = 0, xsv = 0, ypv = 0, ysv = 0;
    double xpp = 0.0, xsp = 0.0, xap = 0.0, ypp = 0.0, ysp = 0.0, yap = 0.0;
    int il;
    kalman_t * kalp;
    for(; xk >= 0; --xk)
    {
        il = m->x.lookup[xk];
        kalp = &m->x.kalmans[il][m->x.selection[il]];
        if( xk >= 2) xap += kalp->K[0];
        else if( xk == 1 )
        {
            xsv = kalp->value;
            xsp = kalp->K[0];
        }
        else
        {
            xpv = kalp->value;
            xpp = kalp->K[0];
        }
    }
    for(; yk >= 0; --yk)
    {
        il = m->y.lookup[yk];
        kalp = &m->y.kalmans[il][m->y.selection[il]];
        if( yk >= 2) yap += kalp->K[0];
        else if( yk == 1 )
        {
            ysv = kalp->value;
            ysp = kalp->K[0];
        }
        else
        {
            ypv = kalp->value;
            ypp = kalp->K[0];
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
    printf("Kalman matrix init:\n");
    for(int i = 0; i < MAX_KALMANS; i++)
    {
        m->lookup[i]            = i;
        m->selection[i]         = 0;
        m->pair[i].level        = 0;
        m->pair[i].persistence  = 0;
        m->density[i]           = 0;

#ifdef KMAT_DEBUG
        //printf("\tk[%d]: lookup-%d selection-%d pair-%d density-%.1f\n", i, m->lookup[i], m->selection[i], m->pair[i].level, m->density[i]);
#endif
        for(int j = 0; j < MAX_PEAKS; j++)
        {
            
            kalman_t k;
            Kalman_Init(&k, 0.0, IMAGE_MAX_KALMAN_LIFE, IMAGE_VALUE_UNCERTAINTY, IMAGE_BIAS_UNCERTAINTY, IMAGE_SENSOR_UNCERTAINTY);
            m->kalmans[i][j] = k;
            printf("ID:%p", &m->kalmans[i][j]);
        }
        printf("\n");
    }
    
    m->k_index = 0;
    m->p_index = 0;
}

void updateKMat( kmat_t * m, peak_list_t * p )
{
    /*** 1) Add new kalmans for new peaks and update them ***/
    updateKMatWithPeaks( m, p );

    /*** 2) Find highest kalman tracking values by tracking row ***/
    selectKMatPeaks( m, p->length );

    /*** 3) Find density pairs in Kalman matrix and give them a sorting bias ***/
    //getKMatCouples( m );

    /*** 4) Sort Kalman Matrix ***/
    printf("Sorting WEIGHTED\n");
    quickSortKMat( m, 0, m->k_index-1, WEIGHTED );
    printKM( m );

    /*** 5) Purge kalmans by selection if expired or best is still too small ***/
    purgeKMat( m );

#ifdef KMAT_DEBUG
    printf("*****************AFTER UPDATE****************\n");
    printKM( m );
#endif
}

/*** UPDATE ***/
void updateKMatWithPeaks( kmat_t * m, peak_list_t * p )
{
    kalman_t * kalp = NULL;

    /* Set length of peaks to check */
    ///TODO: Figure out how to check all peaks with MAX limit
    int pl = (p->length < MAX_PEAKS)?p->length:MAX_PEAKS; // Use all peaks
    p->length = pl;
#ifdef KMAT_DEBUG
    printf("Receiving %d %s\n", pl, PLURAL("peak", pl));
#endif

    for(int i = m->k_index, il = m->lookup[0]; i < pl && i < MAX_KALMANS; i++, il = m->lookup[i])
    {
        kalp                = &m->kalmans[il][i];
        kalp->value         = p->map[i];
        kalp->density       = p->den[i];
        m->selection[il]    = i;

#ifdef KMAT_DEBUG
        printf("i-%d il-%d\n",i, il);
#endif
        m->k_index++;
#define INIT_ITERATIONS 100
         for(int j = 0; j < INIT_ITERATIONS; j++) Kalman_Update(kalp, p->map[i], (((rand() % 32) >> 5) - 0.5)); // Init with velocity variance
    }
    m->p_index = m->k_index;
    int ml = m->k_index;

#ifdef EXT_DEBUG
#ifdef KMAT_DEBUG
    printf("*****************AFTER ADD****************\n");
    printKM( m );
#endif
#endif

    for(int i = 0, il = m->lookup[0]; i < ml; i++, il = m->lookup[i])
    {
        for(int j = 0; j < pl; j++)
        {
            kalman_t s = m->kalmans[il][m->selection[il]];
            int val = p->map[j];
            int den = p->den[j];
            if( den > MIN_PEAK )
            {
                double vel = val - s.value;
                Kalman_Update(&s, val, vel);
                s.density = den;
            }
            m->kalmans[il][j] = s;
//            kalman_copy(&s, &m->kalmans[il][j]);
//            printf("ID:%p ", &m->kalmans[il][j]);
            m->density[il] = den;
            m->value[il] = s.K[0];
        }
//        printf("\n");
    }
#ifdef EXT_DEBUG
#ifdef KMAT_DEBUG
    printf("*****************AFTER NEW PEAKS****************\n");
    printKM( m );
#endif
#endif
}

/*** SELECT ***/
void selectKMatPeaks( kmat_t * m, int pl )
{
    printf("Running with a pl of %d\n", pl);
    int ml = m->k_index;
    for(int i = 0, il = m->lookup[0]; i < ml; i++, il = m->lookup[i])
    {
        selectRow( m, pl, il );
#ifdef KMAT_DEBUG
        printf("\n");
#endif
    }

#ifdef EXT_DEBUG
#ifdef KMAT_DEBUG
    printf("*****************AFTER WEIGHTS****************\n");
    printKM( m );
#endif
#endif
}

double getConfidence( kalman_t * k)
{
    return k->K[0] + k->K[1];// * ((k->value > k->prev)?(k->prev/k->value):(k->value/k->prev));
}

void selectRow( kmat_t * m, int pl, int il )
{
    kalman_t k;

    int s = 0;
    double v = getConfidence( &m->kalmans[il][0] );
    double ref = m->kalmans[il][0].density;

#ifdef KMAT_DEBUG
    printf("Density referene is on kalman %d is %.2f\n",il, ref);
#endif
    
    for(int j = 1; j < pl; j++ )
    {
        k = m->kalmans[il][j];

        /* Adjust weights */
        double r = k.density / ref;
//        if(r < 1000 && r > -1000) k.K[0] *= r;

#ifdef KMAT_DEBUG
        printf("\t[%d](%.3f>%.3f): ",j,k.density,r);
#endif

        /* Find max */
        double n = getConfidence( &k );
        
        /* If first until selection look like or are less than selection, update
         * or check if current is threshold greater than selection */
        printf("n(%.4f) vs v(%.4f) %s equal each other\n", n, v, EQTHR(n,v, SELECTION_THRESHOLD)?"":"do not" );
        printf("Selection is %d and j is %d\n", m->selection[il], j);
        if( ( j <= m->selection[il] && EQTHR(n,v, 0.001) ) || GTTHR(n, v, SELECTION_THRESHOLD) )
        {
            printf("Setting new values\n");
            v = n;
            s = j;
        }
    }
#ifdef KMAT_DEBUG
    printf("\n");
#endif

    printf("Selection on row %d is %d\n", il,s);
    m->selection[il] = s;
    m->value[il] = v;
    m->density[il] = k.density;
}

/*** COUPLE ***/
void getKMatCouples( kmat_t * m )
{
    int ml = m->k_index;
    /* Sort matrix without weights */
    printf("Sorting UNWEIGHTED\n");
    quickSortKMat( m, 0, m->k_index-1, UNWEIGHTED );

    /* Start with higher pair level (higher sort bias) */
    uint8_t pair_level = KALMAN_PAIR_MAX;

    int min_den = -1;
    int curr_den = -1;
    int curr_lev = -1;

    int coup_ind[] = {0,0}; // coupling indeces
    int coup_lev = -1;      // coupling level

    int found_count = 0;
    bool coup_found = false;

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
            curr_den = m->density[il];
            curr_lev = m->pair[il].level;
            if(curr_den < MIN_DENSITY) coup_lev = 0;
        }

        /* Finding first in density range (important: live matrix couple level should never go below 0) */
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
}

/*** PURGE ***/
void purgeKMat( kmat_t * m )
{
    int ml = m->k_index;
    int pl = m->p_index;
#ifdef KMAT_DEBUG
    printf("Purging Kalmans - k%dxp%d\n", ml, pl);
#endif
    int prev[ml-1];
    int pis[ml-1];
    int pi = 0;
    for(int i = 0, il = m->lookup[0]; i < ml; i++, il = m->lookup[i])
    {
        kalman_t * kalp = &m->kalmans[il][m->selection[il]];
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
                if( il != pis[j] && EQTHR(v,prev[j],SIMILARITY_TOLERANCE) )
                {
#ifdef KMAT_DEBUG
                    printf("k[%d](%.1f) ~= k[%d](%.1f)\n", il, m->kalmans[il][m->selection[il]].value, pis[j], m->kalmans[pis[j]][m->selection[pis[j]]].value);
#endif
                    kalp->K[0] *= PUNISH_FACTOR;
                    if(il < lim)
                    {
#ifdef KMAT_DEBUG
                        printf("Punishing row %d\n", il);
#endif
                        punishRow(m, il);
                        v = 0.0;
                    }
//#ifdef KMAT_DEBUG
//                    printf("Re-selecting row %d\n", il);
//#endif
//                    selectRow( m, pl, il );
//                    printf("New selection on row %d is %d\n", il, m->selection[il]);
                    il = m->lookup[++i];
                }
            }
            pis[pi] = il;
            prev[pi++] = v;
        }

        if(getConfidence(kalp) < MIN_PROB )
        {
            m->k_index = i;
            break;
        }
        else if( isExpired(kalp) )
        {
            Kalman_Init(kalp, 0.0, IMAGE_MAX_KALMAN_LIFE, IMAGE_VALUE_UNCERTAINTY, IMAGE_BIAS_UNCERTAINTY, IMAGE_SENSOR_UNCERTAINTY);
        }
    }
    /* Recount useful kalmans */

    int c = 0;
    for(int i = 0, il = m->lookup[0]; i < ml; i++, il = m->lookup[i])
    {
        kalman_t * kalp = &m->kalmans[il][m->selection[il]];
        if( kalp->K[0] >= MIN_PROB ) c++;
    }
#ifdef KMAT_DEBUG
    printf("Recounted useful kalmans: Was-%d | Now-%d \n", m->k_index, c);
#endif
    m->k_index = c;
}

void punishRow( kmat_t * m, int i )
{
    int lim = m->k_index - 1;
    int t = m->lookup[i];
    for(int k = i; k < lim; k++)
        m->lookup[k] = m->lookup[k+1];
    m->lookup[lim] = t;
    printf("Setting kalman at %d(%d) to 0\n", m->lookup[lim], lim);
    m->value[m->lookup[lim]] = 0.0;
}

double weightedValue( kmat_t * m, int i )
{
    return m->value[m->lookup[i]] + ( KALMAN_SORT_SCALE_STEP * m->pair[m->lookup[i]].level );
}

/*** Kalman Matrix Quicksort ***/
void swapKMat( kmat_t * m, kmat_p a, kmat_p b )
{
    if(a == b) return;
#ifdef EXT_DEBUG
#ifdef KMAT_DEBUG
    printf("\tSwapping lookup at %d and %d: [%d][%d]",a,b,m->lookup[a],m->lookup[b]);
    printf("\n");
#endif
#endif
    kmat_p t = m->lookup[a];
    m->lookup[a] = m->lookup[b];
    m->lookup[b] = t;
}

/* Partition using density */
int partitionKMatUnweighted(kmat_t * m, int l, int h )
{
    int pivot = m->density[m->lookup[l]];
    int i = h+1;

    for (int j = h; j > l; j--)
    {
        if( m->density[m->lookup[j]] <= pivot) swapKMat(m,--i,j);
    }
#ifdef EXT_DEBUG
#ifdef KMAT_DEBUG
    printf("Pivot ");
#endif
#endif
    swapKMat(m,i-1,l);
    return (i - 1);
}

/* Partition using weightedValue() function */
int partitionKMatWeighted(kmat_t * m, int l, int h)
{
    int pivot = weightedValue(m, l);
    int i = h+1;

    for (int j = h; j > l; j--)
    {
        if( weightedValue(m, j) <= pivot) swapKMat(m,--i,j);
    }
#ifdef EXT_DEBUG
#ifdef KMAT_DEBUG
    printf("Pivot ");
#endif
#endif
    swapKMat(m,i-1,l);
    return (i - 1);
}
void quickSortKMat(kmat_t * m, int l, int h, bool w )
{
    if( l < h )
    {
        int p;
        if(w) p = partitionKMatUnweighted(m, l, h);
        else p = partitionKMatWeighted(m, l, h);
        quickSortKMat(m, l, p - 1, w);
        quickSortKMat(m, p + 1, h, w);
    }
}
/*******************************/
