//
//  rho_interrupt.h
//  tau+
//
//  Created by Matthew Fonken on 3/31/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef rho_interrupt_model_h
#define rho_interrupt_model_h

void FRAME_START( void );
void FRAME_END(   void );
void ROW_START(   void );
void ROW_END(     void );
void PCLK_INT(    void );
void CAPTURE_INT( void );
void RPCB_SUB(    void );

typedef struct rho_interrupts rho_interrupts;
struct rho_interrupts
{
    void (*FRAME_START)( void );
    void (*FRAME_END)(   void );
    void (*ROW_START)(   void );
    void (*ROW_END)(     void );
    void (*PCLK_INT)(    void );
    void (*CAPTURE_INT)( void );
    void (*RPCB_SUB)(    void );
    void (*INIT_VARIABLES)(void);
};
extern const rho_interrupts RhoInterrupts;

typedef struct
{
    int x,
        p,
        QS,
        PTOG,
        Cx,
        Cy;
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
        *DX_ADDR,
        *DY_ADDR,
        *CX_ADDR,
        *CY_ADDR;
    unsigned char *CAM_PORT;
    int W, H;
} rho_sram_variables;

typedef struct
{
    rho_register_variables  registers;
    rho_sram_variables      ram;
//    void (*init)( void );
} rho_variables;
extern rho_variables RhoVariables;

#endif /* rho_interrupt_h */
