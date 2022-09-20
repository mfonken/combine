//
//  tau_structures.hpp
//  tau+
//
//  Created by Matthew Fonken on 2/8/18.
//  Copyright © 2022 Marbl. All rights reserved.
//

#ifndef tau_structures_hpp
#define tau_structures_hpp

#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include <string>
#include <sstream>

#include "rho_global.h"
#include "rho_types.h"
#include "image_types.h"
#include "kalman.hpp"

#define ABSINV(X)   ((X<1)?1-X:X-1)
#define ABS(X)      ((X>=0)?X:-X)

#define GTTHR(X,Y,T)    (X>=(Y+T))

static inline double timeDiff( struct timeval a, struct timeval b )
{ return ((b.tv_sec  - a.tv_sec) + (b.tv_usec - a.tv_usec)/1000000.0) + 0.0005; }

template <typename T>
static std::string to_stringn(const T a_value, const int n = 6)
{
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}

typedef enum
{
    X_DIMENSION = 0,
    Y_DIMENSION
} dimension_t;

typedef enum
{
    UNWEIGHTED = 0,
    WEIGHTED = 1
} sorting_settings;

/* Tau filter types specifically for kalman matrix structure */
typedef enum
{
    NO_FILTER = 0,
    SOFT_FILTER,
    HARSH_FILTER,
    CHAOS_FILTER
} filter_t;

/* Stability tracking for selec tions */
class Stability
{
public:
    double primary;
    double secondary;
    double alternate;
    double overall;
};

#define PREDICTION_LIFESPAN             1.0
#define PREDICTION_VALUE_UNCERTAINTY    0.5
#define PREDICTION_BIAS_UNCERTAINTY     0.01
#define PREDICTION_SENSOR_UNCERTAINTY   0.001

typedef struct
{
    double  primary,
            secondary,
            alternate;
} prediction_probabilities_t;

typedef enum
{
    SIMILAR = 0,
    OPPOSITE
} selection_pair_t;

class Prediction
{
public:
    KalmanFilter    primary,
                    secondary;
    double          primary_new,
                    secondary_new;
    prediction_probabilities_t probabilities;
    
    Prediction();
};

class PredictionPair
{
public:
    Prediction x;
    Prediction y;
    selection_pair_t        selection_pair;
};

typedef float global_timestamp_t;
typedef struct
{
    uint8_t
    ID,
    includes,
    timestamp[sizeof(global_timestamp_t)];
} rho_packet_header_t;

typedef struct
{
    rho_packet_header_t header;
    uint32_t padding;
    float px;
    float py;
    float sx;
    float sy;
    float pp;
    float sp;
    float ap;
} rho_packet_t;

typedef struct
{
    uint32_t     CameraPort;                     /* Parallel port register to camera */
    uint32_t     HostTxPort;                     /* Output channel to host */
    index_t     *ThreshEnd;                      /* Actual end of thresh buffer */
    address_t    ThreshMax;                      /* Shared address of threshold value */
    address_t    PixelCount;                     /* Shared address of pixel count value */
    address_t    ProcessIndex;                      /* Address threhold buffer is processed */
    byte_t      *Capture;                       /* Address capture buffer is processed */
    byte_t      *CaptureEnd;                     /* Effective end address for capture buffer */
    byte_t      *CaptureMax;                     /* Actual end address for capture buffer */
    index_t     *ThreshFill;                        /* Address threshold buffer is filled */
    address_t   *DMapXI;
    
//    dma_info_t     *CameraDMA;                          /* Address to camera DMA info */
} rho_system_address_variables;

typedef struct
{
    capture_t      *Capture;                      /* Raw capture buffer for DMA */
    index_t      *Thresh;                       /* Threshold processing buffer */
//    sdensity_t   *DensityY;                     /* Processed density X array */
//    sdensity_t   *DensityX;                     /* Processed density Y array */
    density_2d_t *Quadrant;                     /* Quadrant density array */
//    packet_t      *BeaconPacket;                 /* Data packet for beacon comm */
} rho_system_buffer_variables;

typedef struct
{
    rho_core_t                      Utility;
    rho_system_address_variables    Addresses;
    rho_system_buffer_variables     Buffers;
//    camera_application_flags       *Flags;
} rho_system_variables;

typedef struct
{
    void (*Zero)( void );
} rho_system_memory_functions;

typedef struct
{
//    rho_perform_functions           Perform;
//    platform_interface_functions    Platform;
    rho_system_memory_functions     Memory;
} rho_system_functions;

/************************************************************************
 *                      Rho Core System Definition                      *
 ***********************************************************************/
typedef struct
{
    rho_system_variables Variables;
    rho_system_functions Functions;
} rho_system_t;

extern rho_system_t RhoSystem;

#endif /* tau_structures_hpp */
