/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *  File: rho_core.h
 *  Group: Rho Core
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#ifndef rho_core_h
#define rho_core_h

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                          Includes                                    *
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
#include "rho_utility.h"
#include "rho_track.h"

#ifdef __cplusplus
extern "C" {
#endif

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *                       Function Declarations                          *
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    typedef struct
    {
        void (*Initialize)(             rho_core_t *, index_t, index_t );
        void (*Perform)(                rho_core_t *, bool );
        void (*DetectPairs)(            rho_core_t * );
        void (*Detect)(                 rho_core_t *, density_map_t *, prediction_t * );
        void (*DetectRegions)(          rho_detection_variables *, density_map_t *, prediction_t * );
        void (*UpdatePrediction)(       prediction_t * );
        void (*UpdatePredictions)(      rho_core_t * );
        void (*UpdateThreshold)(        rho_core_t * );
        void (*GeneratePacket)(         rho_core_t * );
    } rho_core_functions;

    /* Rho global functions */
    extern const rho_core_functions RhoCore;

#ifdef __cplusplus
}
#endif

#endif /* rho_core_h */
