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

void PERFORM_RHO_C( cimage_t image )
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
    LOG_RHO(">>>frame density is %d<<<\n", RhoVariables.ram.QT);
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

#define RPC(X) if(X&0xf0)
#define RPCB(X,Y,N) {RPC(X){Q##Y++;N[x]++;}}

void PERFORM_RHO_FUNCTION( cimage_t image )
{
    index_t w = image.width, h = image.height;
//    int * mapx = density_map_pair.x.map, * mapy = density_map_pair.y.map;
//    density_map_pair.y.length = w;
//    density_map_pair.x.length = h;
    
    index_t y, x;
    int p;

    FRAME_START();
    
    RhoVariables.ram.QT = p = 0;
    density_2d_t Q0 = 0, Q1 = 0, Q2 = 0, Q3 = 0, QN = 0, QN_ = 0;
    for( y = 0, p = 0; y < RhoVariables.registers.Cy; y++ )
    {
        for( x = 0; x < RhoVariables.registers.Cx; x++, p++ )
        {
            RPC(image.pixels[p])
            {
                Q0++;
                RhoVariables.ram.Dy[x]++;
            }
        }
        for(      ; x <  w; x++, p++ )
        {
            RPC(image.pixels[p])
            {
                Q1++;
                RhoVariables.ram.Dy[x]++;
            }
        }
        QN = Q0 + Q1;
        RhoVariables.ram.Dx[y] = QN - QN_;
        QN_ = QN;
    }
    for( ; y < h; y++ )
    {
        for( x = 0; x < RhoVariables.registers.Cx; x++, p++ )
        {
            RPC(image.pixels[p])
            {
                Q2++;
                RhoVariables.ram.Dy[x]++;
            }
        }
        for(      ; x <  w; x++, p++ )
        {
            RPC(image.pixels[p])
            {
                Q3++;
                RhoVariables.ram.Dy[x]++;
            }
        }
        QN = Q2 + Q3;
        RhoVariables.ram.Dx[y] = QN - QN_;
        QN_ = QN;
    }

    RhoVariables.ram.Q[0] = Q0;
    RhoVariables.ram.Q[1] = Q1;
    RhoVariables.ram.Q[2] = Q2;
    RhoVariables.ram.Q[3] = Q3;
    RhoVariables.ram.QT = Q0 + Q1 + Q2 + Q3;
    
    LOG_RHO("Quadrants are [%d][%d][%d][%d] (%d|%d)\n", Q0, Q1, Q2, Q3, RhoVariables.registers.Cx, RhoVariables.registers.Cy);
    LOG_RHO("# Total coverage is %.3f%%\n", ((double)RhoVariables.ram.QT)/((double)w*h)*100);
}

rho_variables RhoVariables;
