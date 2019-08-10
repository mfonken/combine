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

void FRAME_START( void );
void FRAME_END(   void );
void ROW_INT(     void );
void PCLK_INT(    void );
void LOOP_THREAD( void );

typedef struct rho_interrupts rho_interrupts;
struct rho_interrupts
{
    void (*FRAME_START)( void );
    void (*FRAME_END)(   void );
    void (*ROW_INT)(     void );
    void (*PCLK_INT)(    void );
    void (*INIT_VARIABLES)(void);
    void (*LOOP_THREAD)( void );
};
extern const rho_interrupts RhoInterrupts;

typedef struct
{
    int C_FRAME_MAX,
        y_delimiter;
    
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
       *rd;
} rho_register_variables;

typedef struct
{
    int y,
        QT,
        QN,
        QN_;

    int *Q,
        *Dx,
        *Dy,
        *CX_ADDR,
        *CY_ADDR,
        *C_FRAME;
    unsigned char *CAM_PORT;
    int W, H;
} rho_sram_variables;

typedef struct
{
    rho_global_variables    global;
    rho_register_variables  registers;
    rho_sram_variables      ram;
//    void (*init)( void );
} rho_variables;
extern rho_variables RhoVariables;

#endif /* rho_interrupt_h */
