#ifndef kalman_h
#define kalman_h

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

//#include "rho_global.h"
#include "unilog.h"

#include <stdio.h>
#include <string.h>

#include "matrix.h"

#ifdef MATVEC_LIB
#include "matvec.h"
#endif

//#define MATVEC_LIB
#define SET_DT_SEC 0.25

//#define KALMAN_MATURATION     1 // Seconds

#ifndef BOUND
#define BOUNDU(X,MAX)           ( ( X > MAX ) ? MAX : X )         // Bound in upper range
#define BOUND(X,MIN,MAX)        ( ( X < MIN ) ? MIN : BOUNDU( X, MAX ) ) // Bound in upper and lower range
#endif

//#define DEBUG_KALMAN
#define DEBUG_KALMAN_PRIO DEBUG_2

#ifdef DEBUG_KALMAN
#define LOG_KALMAN(L, ...) LOG(L, "<Kalman> " __VA_ARGS__)
#else
#define LOG_KALMAN(L, ...)
#endif

#define KALMAN_TIME_UNITS TIME_SEC

#define KALMAN_AUGMENT

#ifdef __cplusplus
extern "C" {
#endif

#ifndef floatp
typedef double floatp;
#endif

    typedef struct
    {
        floatp p;
        floatp v;
        floatp a;
    } physical_t;
    
    /*~ Kalman structure */
    typedef struct
    {
        physical_t x;   // State
        floatp A[2][2]; // State Transition
        floatp B[2];    // Control Input Matrix
        floatp H[2];    // Measurement Mapping Matrix
        floatp Q[2][2]; // Process Noise Covariance
        floatp R;       // Measurement Noise Covariance
        floatp P[2][2]; // Covariance Matrix
        floatp K[2];    // Kalman Gain
        floatp process_noise_sq;
        floatp t;
    } kalman_t;
    
    struct kalman_functions
    {
        void (       *Init)( kalman_t *, floatp, floatp, floatp, floatp, floatp, floatp );
        void (       *Reset)( kalman_t *, floatp );
        physical_t * (*Test)( kalman_t *, floatp x_new[2], bool update_A );
        floatp (*TestPosition)( kalman_t * k, floatp p_new, bool update_A );
        void (     *Predict)( kalman_t * );
        void (      *Update)( kalman_t *, floatp );
        physical_t * (*Step)( kalman_t *, floatp );
        void (       *Print)( kalman_t * );
#ifdef KALMAN_AUGMENT
        bool (   *IsExpired)( kalman_t * );
        floatp ( *Score)( kalman_t * );
        void (      *Punish)( kalman_t * );
#endif
//#ifdef MATVEC_LIB
//        gaussian1d_t (*Gaussian1D)( kalman_filter_t * );
//#endif
    };
    
extern const struct kalman_functions Kalman;
    
#ifdef __cplusplus
}
#endif

#endif /* kalman_h */

