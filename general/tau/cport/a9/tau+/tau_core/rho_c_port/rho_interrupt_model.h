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
    int C_FRAME_MAX,
        y_delimiter,
        W,
        H,
        counter;
    
    pthread_t       loop_thread;
    pthread_mutex_t rho_int_mutex;
} rho_global_variables;

typedef struct
{
    int x,
        p,
        QS,
        PTOG,
        Cx,
        Cy,
       *wr,
       *rd,
        THRESH;
} rho_register_variables;

typedef struct
{
    int y,
        QT,
        QN,
        QN_;

    density_t
        *Dx,
        *Dy;
    int *Q,
        *CX_ADDR,
        *CY_ADDR,
        *C_FRAME,
        *C_FRAME_END,
        *THRESH_ADDR;
    unsigned char *CAM_PORT;
} rho_sram_variables;

typedef struct
{
    rho_global_variables    global;
    rho_register_variables  registers;
    rho_sram_variables      ram;
} rho_variables;
extern rho_variables RhoVariables;

#endif /* rho_interrupt_h */
