//
//  rho_interrupt.c
//  tau+
//
//  Created by Matthew Fonken on 3/31/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "rho_interrupt.h"

#define W  640
#define H  400

#define THRESH 0xf8

#define NULL_ADDR 0
#define DX_ADDR     NULL_ADDR
#define DY_ADDR     NULL_ADDR
#define CX_ADDR     NULL_ADDR
#define CY_ADDR     NULL_ADDR
#define CAM_DPORT   NULL_ADDR

int *mapx = DX_ADDR;
int *mapy = DY_ADDR;

int x,
    p,
    QC,
    Cx,
    Cy,
    PTOG;

int y,
    QT,
    QS,
    QN,
    QN_,
    Q[4];

void FRAME_START( void )
{
    memset(mapy, 0, sizeof(int) * W);
    memset(mapx, 0, sizeof(int) * H);
    memset(Q,    0, sizeof(int) * 4);
    
    y = 0, x = 0, p = 0;
    QC = 0, QS = 0, QN = 0, QN_ = 0, QT = 0, Cx = *(int *)CX_ADDR, Cy = *(int *)CY_ADDR;
}

void FRAME_END( void )
{
    QT = 0;
    for( int i = 0; i < 4; i++ )
    {
//        utility->Q[i] = Q[i];
        QT += Q[i];
    }
}

void ROW_START( void )
{
    if( y < Cy ) QS &= 0xfd;
    else         QS |= 0x02;
    y++;
    x = 0;
}

void ROW_END( void )
{
    if( QS & 0x02 ) QN = Q[2] + Q[3];
    else            QN = Q[0] + Q[1];
    mapx[y] = QN - QN_;
    QN_ = QN;
    PTOG = !PTOG;
}

void PCLK_INT( void )
{
    PTOG = !PTOG;
    if( PTOG ) CAPTURE_INT();
}

void CAPTURE_INT( void )
{
    x++;
    p++;
    if( x < Cx ) QS &= 0xf7;
    else         QS |= 0x01;
}

void RPCB_INT( void )
{
    if( *(int *)CAM_DPORT & THRESH )
    {
        Q[QS]++;
        mapy[x]++;
    }
}
