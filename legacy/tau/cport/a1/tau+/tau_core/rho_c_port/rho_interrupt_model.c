//
//  rho_interrupt_model.c
//  tau+
//
//  Created by Matthew Fonken on 3/31/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_interrupt_model.h"
#include <string.h>

#define THRESH 0xf8

void FRAME_START( void )
{
    memset(RhoVariables.ram.Dy, 0, sizeof(int) * RhoVariables.ram.W);
    memset(RhoVariables.ram.Dx, 0, sizeof(int) * RhoVariables.ram.H);
    memset(RhoVariables.ram.Q,  0, sizeof(int) * 4);
    
    RhoVariables.registers.x    = 0;
    RhoVariables.registers.p    = 0;
    RhoVariables.registers.QS   = 0;
    RhoVariables.registers.PTOG = 0;
    RhoVariables.registers.Cx   = *RhoVariables.ram.CX_ADDR;
    RhoVariables.registers.Cy   = *RhoVariables.ram.CY_ADDR;
    
    RhoVariables.ram.y          = 0;
    RhoVariables.ram.QN         = 0;
    RhoVariables.ram.QN_        = 0;
    RhoVariables.ram.QT         = 0;
}

void FRAME_END( void )
{
    RhoVariables.ram.QT = 0;
    for( int i = 0; i < 4; i++ )
        RhoVariables.ram.QT += RhoVariables.ram.Q[i];
}

void ROW_START( void )
{
    if( RhoVariables.ram.y < RhoVariables.registers.Cy ) RhoVariables.registers.QS &= 0xfd;
    else RhoVariables.registers.QS |= 0x02;
    RhoVariables.ram.y++;
    RhoVariables.registers.x = 0;
}

void ROW_END( void )
{
    if( RhoVariables.registers.QS & 0x02 ) RhoVariables.ram.QN = RhoVariables.ram.Q[2] + RhoVariables.ram.Q[3];
    else RhoVariables.ram.QN = RhoVariables.ram.Q[0] + RhoVariables.ram.Q[1];
    RhoVariables.ram.Dx[RhoVariables.ram.y] = RhoVariables.ram.QN - RhoVariables.ram.QN_;
    RhoVariables.ram.QN_ = RhoVariables.ram.QN;
    RhoVariables.registers.PTOG = !RhoVariables.registers.PTOG;
}

void PCLK_INT( void )
{
    RhoVariables.registers.PTOG = !RhoVariables.registers.PTOG;
//    if( RhoVariables.registers.PTOG )
        CAPTURE_INT();
}

void CAPTURE_INT( void )
{
    RhoVariables.registers.x++;
    RhoVariables.registers.p++;
    if( RhoVariables.registers.x < RhoVariables.registers.Cx ) RhoVariables.registers.QS &= 0xfe;
    else RhoVariables.registers.QS |= 0x01;
    RPCB_SUB();
}

void RPCB_SUB( void )
{
    if( *RhoVariables.ram.CAM_PORT & THRESH )
    {
        RhoVariables.ram.Q[RhoVariables.registers.QS]++;
        RhoVariables.ram.Dy[RhoVariables.registers.x]++;
    }
}

const rho_interrupts RhoInterrupts =
{
    .FRAME_START = FRAME_START,
    .FRAME_END   = FRAME_END,
    .ROW_START   = ROW_START,
    .ROW_END     = ROW_END,
    .PCLK_INT    = PCLK_INT,
    .CAPTURE_INT = CAPTURE_INT,
    .RPCB_SUB    = RPCB_SUB
};

rho_variables RhoVariables;
