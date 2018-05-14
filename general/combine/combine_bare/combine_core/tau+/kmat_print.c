//
//  kmat_print.h
//  tau+
//
//  Created by Matthew Fonken on 10/17/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#include "kmat_print.h"

void printKM( kmat_t * m )
{
#ifdef DEBUG
    int ml = MAX_KALMANS;// m->k_index,
    int pl = MAX_PEAKS;

    /* Draw matrix */
    printf("%d(%d)x%d(%d) Matrix:\n\t\t",ml,m->k_index,pl,m->p_index);
    for(int j = 0; j < pl; j++) printf("\t\t\t\t\t%d", j);
    printf("\n");
    for(int i = 0, il = m->lookup[0]; i < ml; i++, il = m->lookup[i])
    {

        double s = m->value[il] + ( KALMAN_SORT_SCALE_STEP * m->pair[il].level );
        printf("%3d[%3d](%.3f)",i, il, s);
        for(int j = 0; j < pl; j++)
        {
            kalman_t k = m->kalmans[il][j];
            double v = (k.value < 1000)?k.value:999;
            double p = (k.prev < 1000)?k.prev:999;
            double d = (k.density < 1000)?k.density:999;
            char c = ' ';
            if(j == m->selection[il]) c = '|';
            printf("\t\t%cv%3.0f|p%3.0f|d%3.0f%c",c,v,p,d,c);
        }
        printf("\n");
        printf("s%3dv%3.2f\t", m->selection[il], m->value[il]);
        for(int j = 0; j < pl; j++)
        {
            kalman_t k = m->kalmans[il][j];
            double k0 = k.K[0], k1 = k.K[1];
            char c = ' ';
            if(j == m->selection[il]) c = '|';
            printf("\t\t%cK0>%1.2f|K1>%1.2f%c",c,k0,k1,c);
        }
        printf("\n\t\t\t\t");
        for(int j = 0; j < pl; j++)
        {
            kalman_t k = m->kalmans[i][j];
            double p00 = k.P[0][0], p01 = k.P[0][1];
            char c = ' ';
            if(j == m->selection[il]) c = '|';
            printf("\t%cP00>%.1f|P01>%.1f%c\t",c,p00,p01,c);
        }
        printf("\n\t\t\t\t");
        for(int j = 0; j < pl; j++)
        {
            kalman_t k = m->kalmans[i][j];
            double p10 = k.P[1][0], p11 = k.P[1][1];
            char c = ' ';
            if(j == m->selection[il]) c = '|';
            printf("\t%cP10>%.1f|P11>%.1f%c\t",c,p10,p11,c);
        }
        printf("\n\t\t\t\t");
        for(int j = 0; j < pl; j++)
        {
            kalman_t k = m->kalmans[i][j];
            double diff = timestamp()-k.timestamp;
            if(diff>MAX_KALMAN_LIFE) diff = NAN;
            char c = ' ';
            if(j == m->selection[il]) c = '|';
            printf("\t%c%.3f%c\t\t\t\t",c,diff,c);
        }
        printf("\n\n");
    }
    //    printf("**********************\n");
#endif
}

void printNL(int tab)
{
    printf("\n");
    for(int i = 0; i < tab; i++) printf("\t");
}

void printA(int arr[], int low, int high, int tab)
{
    for(int i = low; i <= high; i++) printf(" %2d   ", i);
    printNL(tab);
    for(int i = low; i <= high; i++) printf("{%3d} ", arr[i]);
    printNL(tab);
}

