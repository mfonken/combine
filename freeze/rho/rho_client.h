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
#include "rho_utility.h"
#include "stm32_interface.h"

typedef struct
{
flag_t
    Active,
    IRQ,
    Frame,
    Row,
    Backgrounding,
    UARTBusy;
} rho_system_flags_variables;

typedef struct
{
address_t
    CameraPort,
    UartTx,
    CaptureEnd,
    CaptureMax,
    ThreshEnd,                    /* Max threshold buffering size */
    ThreshMax,
    RhoMemEnd;
} rho_system_address_variables;

typedef struct
{
density_t
    *DensityY,                          /* Processed density X array */
    *DensityX;                          /* Processed density Y array */
density_2d_t
    *Quadrant;                          /* Quadrant density array */
capture_t
    Capture[CAPTURE_BUFFER_SIZE];       /* Raw capture buffer for DMA */
index_t
    Thresh[THRESH_BUFFER_SIZE];         /* Threshold processing buffer */
} rho_system_buffer_variables;

typedef struct
{
    index_t pixels, rows;
    density_2d_t left, right;
} section_process_t;

typedef struct
{
    rho_utility                     Utility;
    rho_system_address_variables    Address;
    rho_system_flags_variables      Flag;
    rho_system_buffer_variables     Buffer;
} rho_system_variables;

typedef struct
{
    void (*Init)( void );
    void (*FrameCapture)( void );
    void (*RhoProcess)( void );
} rho_perform_functions;

typedef struct
{
    void (*InitPCLKDMA)( void );
    void (*PauseDMA)( void );
    void (*ResumeDMA)( void );
    void (*ResetDMA)( void );
} rho_system_utility_functions;

typedef struct
{
    void (*Init)( void );
    void (*Zero)( void );
} rho_system_memory_functions;

typedef struct
{
    rho_perform_functions           Perform;
    rho_system_utility_functions    Utility;
    rho_system_memory_functions     Memory;
} rho_system_functions;

typedef struct
{
    rho_system_variables Variables;
    rho_system_functions Functions;
} system_t;

void ProcessRhoSystemFrameCapture( void );
void PerformRhoSystemProcess( void );
void InitRhoSystem( void );
void ZeroRhoSystemMemory( void );

static system_t RhoSystem =
{
    { /* VARIABLES */
        { /* Utility */
            CAPTURE_WIDTH,
            CAPTURE_HEIGHT
        },
        { /* Address */
            (address_t)CAMERA_PORT,
            (address_t)UART_TX_PORT,
            (address_t)CAPTURE_WIDTH
        },
        { 0 },/* Flag */
        { /* Buffer */
            
        }
    },
    { /* FUNCTIONS */
        { /* Perform */
            .Init                   = InitRhoSystem,
            .FrameCapture           = ProcessRhoSystemFrameCapture,
            .RhoProcess             = PerformRhoSystemProcess
        },
        { /* Utility */
            .InitPCLKDMA    = STMInitPCLKDMA,
            .PauseDMA       = STMPauseDMA,
            .ResumeDMA      = STMResumeDMA,
            .ResetDMA       = STMResetDMA,
        },
        { /* Memory */
            .Init = InitRhoSystem,
            .Zero = ZeroRhoSystemMemory
        }
    }
};

static inline void   activityEnable( void ) { RhoSystem.Variables.Flag.Active = 1; }
static inline void   activityDisable(void ) { RhoSystem.Variables.Flag.Active = 0; }
static inline void   irqEnable(      void ) { RhoSystem.Variables.Flag.IRQ = 1; }
static inline void   irqDisable(     void ) { RhoSystem.Variables.Flag.IRQ = 0; }
static inline void   rowFlagSet(     void ) { RhoSystem.Variables.Flag.Row = 1; }
static inline void   rowFlagReset(   void ) { RhoSystem.Variables.Flag.Row = 0; }
static inline flag_t rowFlagGet(     void ) { return RhoSystem.Variables.Flag.Row; }
static inline void   frameFlagSet(   void ) { RhoSystem.Variables.Flag.Frame = 1; }
static inline void   frameFlagReset( void ) { RhoSystem.Variables.Flag.Frame = 0; }

#endif /* rho_client_h */
