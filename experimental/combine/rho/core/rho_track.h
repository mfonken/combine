//
//  rho_track.h
//  ots-proto
//
//  Created by Matthew Fonken on 8/4/22.
//

#ifndef rho_track_h
#define rho_track_h

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                             Includes                                 *
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "../types/rho_types.h"
#include "timestamp.h"

#define RHO_TRACK_NAME "RhoTrack"
#define RHO_TRACK_NAME_BUILDER(DATA) NAME_BUILDER(RHO_TRACK_NAME, DATA)

#ifdef __cplusplus
extern "C" {
#endif

#define RHO_MIN_BLOB_CONFIDENCE 0.2
#define RHO_MIN_BLOB_DENSITY_CONTINUITY 0.8
#define RHO_BLOB_PADDING_FACTOR 2
#define RHO_MIN_TRACKER_AGE_SEC 1
#define RHO_MAX_TRACKER_VELOCITY 100

typedef struct
{
    void (*PairPredictions)( rho_core_t * );
    bool (*UpdateBlob)( blob_t *, tracker_t *, tracker_t *, index_pair_t * );
    void (*DisambiguatePair)( rho_core_t *, byte_pair_t[2] );
    void (*PairXY)( prediction_pair_t *, byte_pair_t );
    void (*RedistributeDensities)( rho_core_t * );
} rho_track_functions;

extern const rho_track_functions RhoTrack;

#ifdef __cplusplus
}
#endif

#endif /* rho_track_h */
