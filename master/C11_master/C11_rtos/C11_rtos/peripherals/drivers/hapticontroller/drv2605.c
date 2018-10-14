//
//  drv2605.c
//  C11_rtos
//
//  Created by Matthew Fonken on 8/29/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#include "drv2605.h"

bool DRV2605Start(void)
{
//    uint8_t status = DRV2605ReadRegister( DRV2605_REG_STATUS);
    
    DRV2605WriteRegister( DRV2605_REG_MODE,         0x00 );
    DRV2605WriteRegister( DRV2605_REG_RTPIN,        0x00 );
    DRV2605WriteRegister( DRV2605_REG_WAVESEQ1,     1 );
    DRV2605WriteRegister( DRV2605_REG_WAVESEQ2,     0 );
    DRV2605WriteRegister( DRV2605_REG_OVERDRIVE,    0 );
    DRV2605WriteRegister( DRV2605_REG_OVERDRIVE,    0 );
    DRV2605WriteRegister( DRV2605_REG_OVERDRIVE,    0 );
    DRV2605WriteRegister( DRV2605_REG_AUDIOMAX,     0x64 );
    DRV2605WriteRegister( DRV2605_REG_FEEDBACK,     0x00 );
    
    DRV2605UseERM();

    uint8_t reg_control3 = DRV2605ReadRegister( DRV2605_REG_CONTROL3);
    DRV2605WriteRegister( DRV2605_REG_CONTROL3, ( reg_control3 | 0x20 ) );
    
    DRV2605ReadRegister( DRV2605_REG_STATUS );
    return true;
}
void DRV2605Go(void)
{
    DRV2605WriteRegister( DRV2605_REG_GO, 1);
}
void DRV2605Stop(void)
{
    DRV2605WriteRegister( DRV2605_REG_GO, 0);
}
void DRV2605SetWaveform( uint8_t slot, uint8_t w )
{
    DRV2605WriteRegister( ( DRV2605_REG_WAVESEQ1 + slot ), w);
}
void DRV2605SelectLibrary( uint8_t lib )
{
    DRV2605WriteRegister( DRV2605_REG_LIBRARY, lib);
}
void DRV2605SetMode( uint8_t mode )
{
    DRV2605WriteRegister( DRV2605_REG_MODE, mode);
}
void DRV2605SetRealtimeValue( uint8_t rtp )
{
    DRV2605WriteRegister( DRV2605_REG_RTPIN, rtp);
}
void DRV2605UseERM(void)
{
    uint8_t feedback = DRV2605ReadRegister( DRV2605_REG_FEEDBACK );
    DRV2605WriteRegister( DRV2605_REG_FEEDBACK, ( feedback & 0x7f ) );
}
void DRV2605UseLRA(void)
{
    uint8_t feedback = DRV2605ReadRegister( DRV2605_REG_FEEDBACK );
    DRV2605WriteRegister( DRV2605_REG_FEEDBACK, ( feedback | 0x80 ) );
}
uint8_t DRV2605ReadRegister( uint8_t reg )
{
    uint8_t data = 0;
    performI2CEvent( DRV2605GetReadEvent( DRV2605_REG_MODE ), &data );
    return data;
}
void DRV2605WriteRegister( uint8_t reg, uint8_t val )
{
    performI2CEventByte( DRV2605GetWriteEvent( reg ), val );
}

