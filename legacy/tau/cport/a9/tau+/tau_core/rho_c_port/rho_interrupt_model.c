//
//  rho_interrupt_model.c
//  tau+
//
//  Created by Matthew Fonken on 3/31/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_interrupt_model.h"
#include <stdio.h>
#include <string.h>

#define BURN_ROWS 0
#define BURN_COLS 0

inline void PERFORM_RHO_C( cimage_t image )
{
    RhoInterrupts.FRAME_START();
    pthread_create(&RhoVariables.global.loop_thread, NULL, (void *)RhoInterrupts.LOOP_THREAD, (void *)&RhoVariables.global.rho_int_mutex);
    uint32_t p = BURN_ROWS * image.height;
    for( index_t y = BURN_ROWS, x; y < image.height; y++ )
    {
        RhoInterrupts.ROW_INT();
        p += BURN_COLS;
        for( x = BURN_COLS; x < image.width; x++, p++ )
        {
            *(RhoVariables.ram.CAM_PORT) = image.pixels[p];
            RhoInterrupts.PCLK_INT();
        }
        
    }
    RhoInterrupts.FRAME_END();
}

void FRAME_INIT( void )
{

}

void FRAME_START( void )
{
    RhoVariables.registers.x    = 0;
    RhoVariables.registers.p    = 0;
    RhoVariables.registers.QS   = 0;
    RhoVariables.registers.PTOG = 0;
    
    RhoVariables.ram.y          = 0;
    RhoVariables.ram.QN         = 0;
    RhoVariables.ram.QN_        = 0;
    RhoVariables.ram.QT         = 0;
    RhoVariables.ram.C_FRAME_END = RhoVariables.ram.C_FRAME + RhoVariables.global.C_FRAME_MAX;
    
    memset(RhoVariables.ram.C_FRAME, 0, sizeof(*RhoVariables.ram.C_FRAME)*RhoVariables.global.C_FRAME_MAX);
    memset(RhoVariables.ram.Dy, 0, sizeof(*RhoVariables.ram.Dy) * RhoVariables.global.W);
    memset(RhoVariables.ram.Dx, 0, sizeof(*RhoVariables.ram.Dx) * RhoVariables.global.H);
    memset(RhoVariables.ram.Q,  0, sizeof(*RhoVariables.ram.Q) * 4);
    RhoVariables.registers.Cx   = *RhoVariables.ram.CX_ADDR;
    RhoVariables.registers.Cy   = *RhoVariables.ram.CY_ADDR;
    RhoVariables.registers.wr   = RhoVariables.ram.C_FRAME;
    RhoVariables.registers.rd   = RhoVariables.ram.C_FRAME;
    RhoVariables.registers.THRESH = *RhoVariables.ram.THRESH_ADDR;
}

void FRAME_END( void )
{
    if(pthread_mutex_trylock(&RhoVariables.global.rho_int_mutex))
        pthread_mutex_unlock(&RhoVariables.global.rho_int_mutex);
    RhoVariables.ram.QT = 0;
    density_2d_t * Qp = RhoVariables.ram.Q;
    for( uint8_t i = 0; i < 4; i++ )
        RhoVariables.ram.QT += Qp[i];
    printf(">>>frame density is %d<<<\n", RhoVariables.ram.QT);
}

void ROW_INT( void )
{
    *(RhoVariables.registers.wr++) = RhoVariables.global.y_delimiter;
//    RhoVariables.registers.PTOG = !RhoVariables.registers.PTOG;
    RhoVariables.registers.x = 0;
}

void PCLK_INT( void )
{
    if( *RhoVariables.ram.CAM_PORT > RhoVariables.registers.THRESH )
        *(RhoVariables.registers.wr++) = RhoVariables.registers.x;
    RhoVariables.registers.x++;
}

void LOOP_THREAD( void * mutex )
{
    pthread_mutex_t * m = (pthread_mutex_t *)mutex;
    index_t rx = 0, ry = 0;
    RhoVariables.global.counter = 0;
    while( RhoVariables.registers.rd != RhoVariables.ram.C_FRAME_END
           && pthread_mutex_trylock(m)
          )
    {
        RhoVariables.global.counter++;
        if( RhoVariables.registers.rd < RhoVariables.registers.wr )
        {
            rx = *(RhoVariables.registers.rd++);
            
            if( rx == RhoVariables.global.y_delimiter )
            {
                if( ry < RhoVariables.registers.Cy )
                {
                    RhoVariables.registers.QS &= 0xfd;
                    RhoVariables.ram.QN = *(RhoVariables.ram.Q  ) + *(RhoVariables.ram.Q+1);
                }
                else
                {
                    RhoVariables.registers.QS |= 0x02;
                    RhoVariables.ram.QN = *(RhoVariables.ram.Q+2) + *(RhoVariables.ram.Q+3);
                }
                
                (*(RhoVariables.ram.Dx+ry++)) = RhoVariables.ram.QN - RhoVariables.ram.QN_;
                RhoVariables.ram.QN_ = RhoVariables.ram.QN;
            }
            else
            {
                if( rx < RhoVariables.registers.Cx )
                        RhoVariables.registers.QS &= 0xfe;
                else    RhoVariables.registers.QS |= 0x01;
                (*(RhoVariables.ram.Dy+rx))++;
                (*(RhoVariables.ram.Q+RhoVariables.registers.QS))++;
            }
        }
    }
}

const rho_interrupts RhoInterrupts =
{
    .FRAME_INIT  = FRAME_INIT,
    .FRAME_START = FRAME_START,
    .FRAME_END   = FRAME_END,
    .ROW_INT     = ROW_INT,
    .PCLK_INT    = PCLK_INT,
    .LOOP_THREAD = LOOP_THREAD
};

rho_variables RhoVariables;
