//
//  kmat_utility.hpp
//  tau+
//
//  Created by Matthew Fonken on 2/8/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef kmat_utility_hpp
#define kmat_utility_hpp

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "test_setup.h"

#include "tau_structures.hpp"
#include "rho_utility.hpp"

//#include "global_types.h"
#include "kalman.hpp"

/* KALMAN / KMAT SETUP */
#define KMAT_LIFESPAN       1.0
#define VALUE_UNCERTAINTY   0.1//0.01
#define BIAS_UNCERTAINTY    0.05//0.003
#define SENSOR_UNCERTAINTY  0.001

#define MAX_KALMANS             3
//#define KALMAN_IS_TIMELESS            // NOTE: Currently breaks kalman gain and thus state machine
#define MAX_VALUE_DIFF          20
#define MAX_DENSITY_DIFF        10      // Pixels
#define KALMAN_SORT_SCALE_STEP  10       // Pixels
#define KALMAN_PAIR_MAX         3       // Pair/sorting steps
#define MIN_PROB                0.4     // Gain units
#define SELECTION_THRESHOLD     0.01    // Gain units
#define MIN_PEAK                10      // Pixels
#define SIMILARITY_TOLERANCE    3       // Pixels
#define PUNISH_FACTOR           0.8     // Gain / Gain

typedef int lookup_t;
typedef int kmat_p; /* Kalman Internal Lookup Pointer Type */
typedef double  kmat_f; /* Kalman Internal Float Type */

typedef struct
{
    int level;
    int  persistence;
} kmat_coupling_t;

#define COUPLING_DEGREDATION 10
#define MAX_PERSISTENCE ( 1 << 7 ) - 1 // MAX OF SIGNED BYTE
static inline void discourageCouple( kmat_coupling_t * c )
{
    if(c->persistence > 0) c->persistence -= COUPLING_DEGREDATION;
    else c->level = 0;
}

static inline void encourageCouple( kmat_coupling_t * c )
{
    if(c->persistence < MAX_PERSISTENCE) c->persistence++;
}

/* Kalman Matrix Type */
class KalmanMatrix
{
    void    updateWithPeaks( PeakList * );
    void    selectPeaks( int );
    void    selectRow( int, int );
    void    getCouples();
    void    purge();
    void    punishRow(int);
    double  weightedValue(int);
    void    swap( kmat_p, kmat_p );
    int     partition( int, int, bool );
    void    quickSort( int, int, bool );
    void    printA( int *, int, int, int );
    
public:
    KalmanFilter    kalmans[MAX_KALMANS][MAX_PEAKS];
    lookup_t        lookup[MAX_KALMANS];            /**< Tracks Kalman resorting >*/
    kmat_p          selection[MAX_KALMANS];         /**< Tracks current peak id selection >*/
    double          value[MAX_KALMANS];             /**< Tracks current tracking value >*/
    double          density[MAX_KALMANS];           /**< Tracks current density value >*/
    kmat_coupling_t pair[MAX_KALMANS];
    kmat_p          k_index;
    kmat_p          p_index;
    
    KalmanMatrix();
    void    update(PeakList * );
    void    print();

};

/* Kalman Matrix Pair Type */
class KalmanMatrixPair
{
public:
    KalmanMatrix  x;
    KalmanMatrix  y;
    //    kmat_probabilities_t      probabilities;
    
    KalmanMatrixPair();
    void update(PeakListPair *);
    void predict(PredictionPair *);
};


#endif /* kmat_utility_hpp */
