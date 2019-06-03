//
//  rho_client.h
//  rho_client
//
//  Created by Matthew Fonken on 9/19/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_client_h
#define rho_client_h

#include <stdio.h>
#include "rho_types.h"

/************************************************************************
 *                      Global Function Prototypes                      *
 ***********************************************************************/
void PixelThreshLoop( register byte_t *, register index_t *, const register byte_t, const register byte_t );
index_t CaptureFrame( void );
section_process_t ProcessFrameSection( register index_t, register index_t, register index_t, register index_t );
void ActivateBackgrounding( void );
void DeactivateBackgrounding( void );
void FilterPixelCount( index_t *, index_t );
bool HasPixelCountDrop( index_t *, index_t );
void ProcessFrame( index_t );
void ProcessRhoSystemFrameCapture( void );
void PerformRhoSystemProcess( void );
void ActivateRhoSystem( void );
void DeactivateRhoSystem( void );
void InitRhoSystem( address_t, address_t );
void ZeroRhoSystemMemory( void );
void ConnectRhoSystemPlatformInterface( platform_interface_functions * );
void TransmitRhoSystemPacket( void );

/************************************************************************
 *                      Global Buffers                                  *
 ***********************************************************************/
static capture_t _capture_buffer_internal[CAPTURE_BUFFER_SIZE];
static index_t _thresh_buffer_internal[THRESH_BUFFER_SIZE];

/************************************************************************
 *                      Rho Core Variables                              *
 ***********************************************************************/
typedef struct
{
address_t
  CameraPort,                     /* Parallel port register to camera */
  CaptureEnd,                     /* Effective end address for capture buffer */
  CaptureMax,                     /* Actual end address for capture buffer */
  ThreshEnd,                      /* Actual end of thresh buffer */
  ThreshMax,                      /* Shared address of threshold value */
  PixelCount,                     /* Shared address of pixel count value */
  HostTxPort;
} rho_system_address_variables;

typedef struct
{
capture_t
    *Capture;                       /* Raw capture buffer for DMA */
index_t
    *Thresh;                        /* Threshold processing buffer */
density_t
    *DensityY,                      /* Processed density X array */
    *DensityX;                      /* Processed density Y array */
density_2d_t
    *Quadrant;                      /* Quadrant density array */
packet_t
    *BeaconPacket;                  /* Data packet for beacon comm */
} rho_system_buffer_variables;

typedef struct
{
    rho_core_t                      Utility;
    rho_system_address_variables    Addresses;
    rho_system_flags_variables      Flags;
    rho_system_buffer_variables     Buffers;
} rho_system_variables;

/************************************************************************
 *                      Rho Core Functions                              *
 ***********************************************************************/
typedef struct
{
    void (*Init)( address_t, address_t );
    void (*FrameCapture)( void );
    void (*RhoProcess)( void );
    void (*ConnectToInterface)( platform_interface_functions * );
    void (*TransmitPacket)( void );
} rho_perform_functions;

typedef struct
{
    void (*Zero)( void );
} rho_system_memory_functions;

typedef struct
{
    rho_perform_functions           Perform;
    platform_interface_functions    Platform;
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

static rho_system_t RhoSystem =
{
    { /* VARIABLES */
        { /* Utility */
            CAPTURE_WIDTH,
            CAPTURE_HEIGHT
        },
        { 0 },/* Addresses */
        { 0 },/* Flags */
        { /* Buffers */
            _capture_buffer_internal,
            _thresh_buffer_internal
        }
    },
    { /* FUNCTIONS */
        { /* Perform */
            .Init               = InitRhoSystem,
            .FrameCapture       = ProcessRhoSystemFrameCapture,
            .RhoProcess         = PerformRhoSystemProcess,
            .ConnectToInterface = ConnectRhoSystemPlatformInterface,
            .TransmitPacket     = TransmitRhoSystemPacket
        },
        { 0 }, /* Utility */
        { /* Memory */
            .Zero               = ZeroRhoSystemMemory
        }
    }
};

static inline void   activityEnable( void ) { RhoSystem.Variables.Flags.Active  = 1; }
static inline void   activityDisable(void ) { RhoSystem.Variables.Flags.Active  = 0; }
static inline void   irqEnable(      void ) { RhoSystem.Variables.Flags.IRQ     = 1; }
static inline void   irqDisable(     void ) { RhoSystem.Variables.Flags.IRQ     = 0; }
static inline void   rowFlagSet(     void ) { RhoSystem.Variables.Flags.Row     = 1; }
static inline void   rowFlagReset(   void ) { RhoSystem.Variables.Flags.Row     = 0; }
static inline void   frameFlagSet(   void ) { RhoSystem.Variables.Flags.Frame   = 1; }
static inline void   frameFlagReset( void ) { RhoSystem.Variables.Flags.Frame   = 0; }

#endif /* rho_client_h */
