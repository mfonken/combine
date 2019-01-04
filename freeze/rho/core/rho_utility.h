//
//  rho_utility.h
//  tau+
//
//  Created by Matthew Fonken on 2/7/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_utility_h
#define rho_utility_h

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

#include "timestamp.h"

#include "rho_structure.h"

#include "state_machine_utility.h"

#ifdef __PCR__
#include "rho_interrupt_model.h"
#else
#include "rho_client.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif
    
    void CumulateMomentsRhoUtility( floating_t, floating_t, floating_t *, floating_t *, floating_t * );
    void CalculateBlobScoreRhoUtility( blob_t *, density_t, byte_t );
    floating_t CalculateCentroidRhoUtility( density_t *, index_t, index_t *, density_t );
    void PrintPacketRhoUtility( packet_t *, index_t );
    
    typedef struct
    {
        void (*CumulateMoments)( floating_t, floating_t, floating_t *, floating_t *, floating_t * );
        void (*CalculateBlobScore)( blob_t *, density_t, byte_t );
        floating_t (*CalculateCentroid)( density_t *, index_t, index_t *, density_t );
        void (*PrintPacket)( packet_t *, index_t );
    } rho_utility_functions;
    
    static const rho_utility_functions RhoUtility =
    {
        .CumulateMoments = CumulateMomentsRhoUtility,
        .CalculateBlobScore = CalculateBlobScoreRhoUtility,
        .CalculateCentroid = CalculateCentroidRhoUtility,
        .PrintPacket = PrintPacketRhoUtility
    };
    
#ifdef __cplusplus
}
#endif

#endif

