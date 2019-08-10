//
//  rho_interrupt_model.c
//  tau+
//
//  Created by Matthew Fonken on 3/31/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_interrupt_model.h"
#include <string.h>

#define THRESH 0x11

void FRAME_START( void )
{
    memset(RhoVariables.ram.C_FRAME, 0, sizeof(int)*RhoVariables.global.C_FRAME_MAX);
    memset(RhoVariables.ram.Dy, 0, sizeof(int) * RhoVariables.ram.W);
    memset(RhoVariables.ram.Dx, 0, sizeof(int) * RhoVariables.ram.H);
    memset(RhoVariables.ram.Q,  0, sizeof(int) * 4);
    
    RhoVariables.registers.x    = 0;
    RhoVariables.registers.p    = 0;
    RhoVariables.registers.QS   = 0;
    RhoVariables.registers.PTOG = 0;
    RhoVariables.registers.Cx   = *RhoVariables.ram.CX_ADDR;
    RhoVariables.registers.Cy   = *RhoVariables.ram.CY_ADDR;
    RhoVariables.registers.wr   = RhoVariables.ram.C_FRAME;
    RhoVariables.registers.rd   = RhoVariables.ram.C_FRAME;
    
    RhoVariables.ram.y          = 0;
    RhoVariables.ram.QN         = 0;
    RhoVariables.ram.QN_        = 0;
    RhoVariables.ram.QT         = 0;
}

void FRAME_END( void )
{
    RhoVariables.ram.QT = 0;
    int * Qp = RhoVariables.ram.Q;
    for( int i = 0; i < 4; i++ )
        RhoVariables.ram.QT += *(Qp++);
}

void ROW_INT( void )
{
    *(RhoVariables.registers.wr++) = RhoVariables.global.y_delimiter;
    RhoVariables.registers.PTOG = !RhoVariables.registers.PTOG;
    RhoVariables.registers.x = 0;
}

void PCLK_INT( void )
{
    if( *RhoVariables.ram.CAM_PORT > THRESH )
        *(RhoVariables.registers.wr++) = RhoVariables.registers.x;
    RhoVariables.registers.x++;
}

void LOOP_THREAD( void )
{
    int x = 0, y = 0, s = 0;
    while( RhoVariables.registers.rd < ( RhoVariables.ram.C_FRAME + RhoVariables.global.C_FRAME_MAX ) )
    {
        if( RhoVariables.registers.rd < RhoVariables.registers.wr )
        {
            x = *(RhoVariables.registers.rd++);
            if( x == RhoVariables.global.y_delimiter || x == 0 )
            {
                if( y < RhoVariables.registers.Cy )
                        RhoVariables.registers.QS &= 0xfd;
                else    RhoVariables.registers.QS |= 0x02;
                (*(RhoVariables.ram.Dx+y))+=s;
                y++;
                s = 0;
            }
            else
            {
                if( x < RhoVariables.registers.Cx )
                        RhoVariables.registers.QS &= 0xfe;
                else    RhoVariables.registers.QS |= 0x01;
                (*(RhoVariables.ram.Dy+x))++;
                s++;
            }
            (*(RhoVariables.ram.Q+RhoVariables.registers.QS))++;
        }
    }
}

const rho_interrupts RhoInterrupts =
{
    .FRAME_START = FRAME_START,
    .FRAME_END   = FRAME_END,
    .ROW_INT     = ROW_INT,
    .PCLK_INT    = PCLK_INT,
    .LOOP_THREAD = LOOP_THREAD
};

rho_variables RhoVariables;
