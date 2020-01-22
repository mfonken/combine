//
//  DRV2605.c
//  C12_rtos
//
//  Created by Matthew Fonken on 8/29/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "DRV2605.h"

bool DRV2605_Start(void)
{
//    uint8_t status = DRV2605_ReadRegister( DRV2605_REG_STATUS);
    
    DRV2605_WriteRegister( DRV2605_REG_MODE,         0x00 );
    DRV2605_WriteRegister( DRV2605_REG_RTPIN,        0x00 );
    DRV2605_WriteRegister( DRV2605_REG_WAVESEQ1,     1 );
    DRV2605_WriteRegister( DRV2605_REG_WAVESEQ2,     0 );
    DRV2605_WriteRegister( DRV2605_REG_OVERDRIVE,    0 );
    DRV2605_WriteRegister( DRV2605_REG_OVERDRIVE,    0 );
    DRV2605_WriteRegister( DRV2605_REG_OVERDRIVE,    0 );
    DRV2605_WriteRegister( DRV2605_REG_AUDIOMAX,     0x64 );
    DRV2605_WriteRegister( DRV2605_REG_FEEDBACK,     0x00 );
    
    DRV2605_UseERM();

    uint8_t reg_control3 = DRV2605_ReadRegister( DRV2605_REG_CONTROL3);
    DRV2605_WriteRegister( DRV2605_REG_CONTROL3, ( reg_control3 | 0x20 ) );
    
    DRV2605_ReadRegister( DRV2605_REG_STATUS );
    return true;
}
void DRV2605_Go(void)
{
    DRV2605_WriteRegister( DRV2605_REG_GO, 1);
}
void DRV2605_Stop(void)
{
    DRV2605_WriteRegister( DRV2605_REG_GO, 0);
}
void DRV2605_SetWaveform( uint8_t slot, uint8_t w )
{
    DRV2605_WriteRegister( ( DRV2605_REG_WAVESEQ1 + slot ), w);
}
void DRV2605_SelectLibrary( uint8_t lib )
{
    DRV2605_WriteRegister( DRV2605_REG_LIBRARY, lib);
}
void DRV2605_SetMode( uint8_t mode )
{
    DRV2605_WriteRegister( DRV2605_REG_MODE, mode);
}
void DRV2605_SetRealtimeValue( uint8_t rtp )
{
    DRV2605_WriteRegister( DRV2605_REG_RTPIN, rtp);
}
void DRV2605_UseERM(void)
{
    uint8_t feedback = DRV2605_ReadRegister( DRV2605_REG_FEEDBACK );
    DRV2605_WriteRegister( DRV2605_REG_FEEDBACK, ( feedback & 0x7f ) );
}
void DRV2605_UseLRA(void)
{
    uint8_t feedback = DRV2605_ReadRegister( DRV2605_REG_FEEDBACK );
    DRV2605_WriteRegister( DRV2605_REG_FEEDBACK, ( feedback | 0x80 ) );
}
uint8_t DRV2605_ReadRegister( uint8_t reg )
{
    uint8_t data = 0;
    performI2CEvent( DRV2605_GetReadEvent( DRV2605_REG_MODE ), &data );
    return data;
}
void DRV2605_WriteRegister( uint8_t reg, uint8_t val )
{
    performI2CEventByte( DRV2605_GetWriteEvent( reg ), val );
}

