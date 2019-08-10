//
//  kmat_print.h
//  tau+
//
//  Created by Matthew Fonken on 10/17/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef kmat_print_h
#define kmat_print_h

#include "kmat.h"
#include "global_types.h"

void printKM( kmat_t * m )
{
#ifdef DEBUG
    int ml = MAX_KALMANS;// m->k_index,
    int pl = MAX_PEAKS;
    //    printf("****KALMAN MATRIX*****\n");
    //    printf("Index\tSort\tValue\tSel\tPair\n");
    //    for(int i = 0, il = m->lookup[0]; i < ml; i++, il = m->lookup[i])
    //    {
    //        printf("%3d\t\t%3d",i,il);
    //        printf("\t\t%3.2f\t%d\t%d\n",m->value[il],m->selection[il],m->pair[il]);
    //    }
    
    /* Draw matrix */
    printf("%dx%d Matrix:\n\t\t",ml, pl);
    for(int j = 0; j < pl; j++) printf("\t\t\t\t\t%d", j);
    printf("\n");
    for(int i = 0, il = m->lookup[0]; i < ml; i++, il = m->lookup[i])
    {
        
        double s = m->value[il] + ( KALMAN_SORT_SCALE_STEP * m->pair[il] );
        printf("%3d[%3d](%.0f)",i, il, s);
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
            double p00 = k.P_k[0][0], p01 = k.P_k[0][1];
            char c = ' ';
            if(j == m->selection[il]) c = '|';
            printf("\t%cP00>%.1f|P01>%.1f%c\t",c,p00,p01,c);
        }
        printf("\n\t\t\t\t");
        for(int j = 0; j < pl; j++)
        {
            kalman_t k = m->kalmans[i][j];
            double p10 = k.P_k[1][0], p11 = k.P_k[1][1];
            char c = ' ';
            if(j == m->selection[il]) c = '|';
            printf("\t%cP10>%0.1f|P11>%0.1f%c\t",c,p10,p11,c);
        }
        printf("\n\n");
    }
    //    printf("**********************\n");
#endif
}


///* QUICKSORT MAX */
//void swapKalmanMatrixMax( kmat_t * m, kmat_p a, kmat_p b )
//{
//    kmat_p t = m->lookup[a];
//    m->lookup[a] = m->lookup[b];
//    m->lookup[b] = t;
//}
//
//int partitionKalmanMatrixMax(kmat_t * m, int l, int h)
//{
//    int pivot = m->value[m->lookup[h]];
//    int i = (l - 1);
//
//    for (int j = l; j <= h-1; j++)
//    {
//        double scaled_value = m->value[m->lookup[j]] + ( KALMAN_SORT_SCALE_STEP * m->pair[m->lookup[j]] );
//        if (scaled_value < pivot) swap(m->lookup,++i,j);
//    }
//    swap(m->lookup,i+1,h);
//    return (i + 1);
//}
//
//
//void quickSortKalmanMatrixMax(kmat_t * m, int l, int h)
//{
//    if( l < h )
//    {
//        int p = partitionKalmanMatrix(m, l, h);
//        quickSortKalmanMatrix(m, l, p - 1);
//        quickSortKalmanMatrix(m, p + 1, h);
//    }
//}



/************ QUICK SORT START *************
 link - (http://www.geeksforgeeks.org/quick-sort/
 */

/*
 void swap( lookup_t * l, int a, int b )
 {
 #ifdef KMAT_DEBUG
 printf("\tSwapping lookup at %d and %d: [%d][%d]",a,b,l[a],l[b]);
 #endif
 int t = l[a];
 l[a] = l[b];
 l[b] = t;
 #ifdef KMAT_DEBUG
 printf(" > [%d][%d]\n",l[a],l[b]);
 #endif
 }
 
 int partitionProbabilityList(probability_list_t * r, int l, int h)
 {
 int pivot = r->values[h];
 int i = (l - 1);
 
 for (int j = l; j <= h-1; j++)
 {
 if (r->values[j] <= pivot) swap(r->lookup,++i,j);
 }
 swap(r->lookup,i+1,h);
 return (i + 1);
 }
 
 void quickSortProbabilityList(probability_list_t * r, int l, int h)
 {
 if( l < h )
 {
 int p = partitionProbabilityList(r, l, h);
 quickSortProbabilityList(r, l, p - 1);
 quickSortProbabilityList(r, p + 1, h);
 }
 }
 
 void quickSortProbabilityListPair( probability_list_pair_t * r )
 {
 quickSortProbabilityList(&r->x.primary, 0, r->x.primary.length);
 quickSortProbabilityList(&r->x.secondary, 0, r->y.secondary.length);
 quickSortProbabilityList(&r->y.primary, 0, r->x.primary.length);
 quickSortProbabilityList(&r->y.secondary, 0, r->y.secondary.length);
 }
 */


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

void swap(int* a, int* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

/* This function takes last element as pivot, places
 the pivot element at its correct position in sorted
 array, and places all smaller (smaller than pivot)
 to left of pivot and all greater elements to right
 of pivot */
int partition (int arr[], int low, int high, int tab)
{
    int pivot = (int)arr[low];    // pivot
#ifdef EXT_DEBUG
    printNL(tab);
    printf("Pivot is %d at index %d", pivot, low);
#endif
    int i = high + 1;  // Index of smaller element
    
    for (int j = high; j > low; j--)
    {
#ifdef EXT_DEBUG
        printNL(tab);
        printf("i-%d and j-%d", i, j);
#endif
        // If current element is smaller than or
        // equal to pivot
        if ((int)arr[j] <= pivot)
        {
            if(--i!=j)
            {
                //                --i;    // increment index of smaller element
#ifdef EXT_DEBUG
                printNL(tab);
                printf("Swapping i-%d <> j-%d", i, j);
#endif
                swap((int*)&arr[i], (int*)&arr[j]);
#ifdef EXT_DEBUG
                printNL(tab);
                printA(arr, low, high, tab);
#endif
            }
        }
    }
#ifdef EXT_DEBUG
    printNL(tab);
#endif
    
    int b = low, a = i - 1;
#ifdef EXT_DEBUG
    printf("Pivot swapping i-%d <> j-%d", a, b);
#endif
    swap((int*)&arr[a], (int*)&arr[b]);
#ifdef EXT_DEBUG
    printNL(tab);
    printA(arr, low, high, tab);
#endif
    return (i - 1);
}

/* The main function that implements QuickSort
 arr[] --> Array to be sorted,
 low  --> Starting index,
 high  --> Ending index */
int quickSort(int arr[], int low, int high, int tab)
{
#ifdef EXT_DEBUG
    printNL(tab);
    printf("Sorting %d-%d:", low, high);
    printNL(tab);
    printA(arr, low, high, tab);
#endif
    int next_tab = tab+1;
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
         at right place */
        int pi = partition(arr, low, high, tab);
#ifdef EXT_DEBUG
        printNL(tab);
        printf("Partition split is %d",pi);
        // Separately sort elements before
        // partition and after partition
        printNL(tab);
        printf("Sorting below split %d-%d", low,pi);
#endif
        quickSort(arr, low, pi - 1, next_tab);
#ifdef EXT_DEBUG
        printNL(tab);
        printf("Sorting above split %d-%d", pi, high);
#endif
        quickSort(arr, pi + 1, high, next_tab);
    }
    return 1;
}
/*****************************************/

/* QUICK SORT END */

#endif /* kmat_print_h */
