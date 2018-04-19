//
//  kmat_print.h
//  tau+
//
//  Created by Matthew Fonken on 10/17/17.
//  Copyright © 2017 Marbl. All rights reserved.
//

#ifndef kmat_print_h
#define kmat_print_h

#ifdef __cplusplus
extern "C" {
#endif

#include "kmat.h"
#include "test_setup.h"
#include "global_types.h"

void printKM( kmat_t * m );
void printNL(int tab);
void printA(int arr[], int low, int high, int tab);
void swap(int* a, int* b);
int partition (int arr[], int low, int high, int tab);
int quickSort(int arr[], int low, int high, int tab);

/* QUICK SORT END */
#ifdef __cplusplus
}
#endif
    
#endif /* kmat_print_h */
