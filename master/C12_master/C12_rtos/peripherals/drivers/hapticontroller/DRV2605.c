//
//  DRV2605.c
//  C12_rtos
//
//  Created by Matthew Fonken on 8/29/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "DRV2605.h"

bool DRV2605_Start( comm_host_t * host )
{
//    uint8_t status = DRV2605_ReadRegister( DRV2605_REG_STATUS);
    
    DRV2605_WriteRegister( host, DRV2605_REG_MODE,         0x00 );
    DRV2605_WriteRegister( host, DRV2605_REG_RTPIN,        0x00 );
    DRV2605_WriteRegister( host, DRV2605_REG_WAVESEQ1,     1 );
    DRV2605_WriteRegister( host, DRV2605_REG_WAVESEQ2,     0 );
    DRV2605_WriteRegister( host, DRV2605_REG_OVERDRIVE,    0 );
    DRV2605_WriteRegister( host, DRV2605_REG_OVERDRIVE,    0 );
    DRV2605_WriteRegister( host, DRV2605_REG_OVERDRIVE,    0 );
    DRV2605_WriteRegister( host, DRV2605_REG_AUDIOMAX,     0x64 );
    DRV2605_WriteRegister( host, DRV2605_REG_FEEDBACK,     0x00 );
    
    DRV2605_UseERM( host );

    uint8_t reg_control3 = DRV2605_ReadRegister( host, DRV2605_REG_CONTROL3);
    DRV2605_WriteRegister( host, DRV2605_REG_CONTROL3, ( reg_control3 | 0x20 ) );
    
    DRV2605_ReadRegister( host, DRV2605_REG_STATUS );
    return true;
}
void DRV2605_Go( comm_host_t * host )
{
    DRV2605_WriteRegister( host, DRV2605_REG_GO, 1);
}
void DRV2605_Stop( comm_host_t * host )
{
    DRV2605_WriteRegister( host, DRV2605_REG_GO, 0);
}
void DRV2605_SetWaveform( comm_host_t * host, uint8_t slot, uint8_t w )
{
    DRV2605_WriteRegister( host, ( DRV2605_REG_WAVESEQ1 + slot ), w);
}
void DRV2605_SelectLibrary( comm_host_t * host, uint8_t lib )
{
    DRV2605_WriteRegister( host, DRV2605_REG_LIBRARY, lib);
}
void DRV2605_SetMode( comm_host_t * host, uint8_t mode )
{
    DRV2605_WriteRegister( host, DRV2605_REG_MODE, mode);
}
void DRV2605_SetRealtimeValue( comm_host_t * host, uint8_t rtp )
{
    DRV2605_WriteRegister( host, DRV2605_REG_RTPIN, rtp);
}
void DRV2605_UseERM( comm_host_t * host )
{
    uint8_t feedback = DRV2605_ReadRegister( host, DRV2605_REG_FEEDBACK );
    DRV2605_WriteRegister( host, DRV2605_REG_FEEDBACK, ( feedback & 0x7f ) );
}
void DRV2605_UseLRA( comm_host_t * host )
{
    uint8_t feedback = DRV2605_ReadRegister( host, DRV2605_REG_FEEDBACK );
    DRV2605_WriteRegister( host, DRV2605_REG_FEEDBACK, ( feedback | 0x80 ) );
}
uint8_t DRV2605_ReadRegister( comm_host_t * host, uint8_t reg )
{
    uint8_t data = 0;
    PerformI2CEvent( DRV2605_GetReadEvent( host, DRV2605_REG_MODE, &data ) );
    return data;
}
void DRV2605_WriteRegister( comm_host_t * host, uint8_t reg, uint8_t val )
{
    PerformI2CEvent( DRV2605_GetWriteEvent( host, reg, &val ) );
}

