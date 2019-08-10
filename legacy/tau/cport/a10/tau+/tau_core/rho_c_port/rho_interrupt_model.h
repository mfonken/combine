//
//  rho_interrupt.h
//  tau+
//
//  Created by Matthew Fonken on 3/31/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_interrupt_model_h
#define rho_interrupt_model_h

#include <pthread.h>
#include "rho_c_types.h"

void PERFORM_RHO_C( cimage_t );
void PERFORM_RHO_FUNCTION( cimage_t );

void FRAME_INIT(  void );
void FRAME_START( void );
void FRAME_END(   void );
void ROW_INT(     void );
void PCLK_INT(    void );
void LOOP_THREAD( void * );

typedef struct rho_interrupts rho_interrupts;
struct rho_interrupts
{
    void (*FRAME_INIT)(  void );
    void (*FRAME_START)( void );
    void (*FRAME_END)(   void );
    void (*ROW_INT)(     void );
    void (*PCLK_INT)(    void );
    void (*LOOP_THREAD)( void * );
};
extern const rho_interrupts RhoInterrupts;

typedef struct
{
    uint32_t
        C_FRAME_MAX;
    index_t
        counter,
        y_delimiter,
        W,
        H;
    
    pthread_t       loop_thread;
    pthread_mutex_t rho_int_mutex;
} rho_global_variables;

typedef struct
{
    index_t
        x,
        p,
        Cx,
        Cy,
        *wr,
        *rd;
    density_t
        THRESH;
    byte_t
        QS,
        PTOG;
} rho_register_variables;

typedef struct
{
    density_t
        *Dx,
        *Dy,
        *THRESH_ADDR;
    density_2d_t
        *Q,
        QT,
        QN,
        QN_;
    index_t
        y,
        *CX_ADDR,
        *CY_ADDR,
        *C_FRAME,
        *C_FRAME_END;
    
    pixel_base_t
        *CAM_PORT;
} rho_sram_variables;

typedef struct
{
    rho_global_variables    global;
    rho_register_variables  registers;
    rho_sram_variables      ram;
} rho_variables;
extern rho_variables RhoVariables;

#endif /* rho_interrupt_h */
