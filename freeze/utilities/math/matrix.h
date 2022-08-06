//
//  matrix.h
//  ots-proto
//
//  Created by Matthew Fonken on 4/12/22.
//

#ifndef matrix_h
#define matrix_h

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "unilog.h"

//#define DEBUG_MATRIX
#define DEBUG_MATRIX_PRIO DEBUG_2

#ifdef DEBUG_MATRIX
#define LOG_MTX(L, ...) LOG(L, "<Matrix> " __VA_ARGS__)
#else
#define LOG_MTX(L, ...)
#endif

typedef double floatp;

typedef struct
{
    void (*eye)(    floatp *m, uint8_t rows, uint8_t cols );
    void (*dot)(    floatp a[], floatp b[], bool T_b, floatp r[], uint8_t rows, uint8_t cols, uint8_t inner );
    void (*addsub)( floatp a[], floatp b[], floatp r[], uint8_t rows, uint8_t cols, bool add );
    void (*zpad)(   floatp m[], uint8_t rows_in, uint8_t cols_in, floatp r[], uint8_t rows_out, uint8_t cols_out );
    void (*inv22)(  floatp m[2][2], floatp r[2][2] );
} matrix_functions;

extern const matrix_functions Matrix;


#endif /* matrix_h */
