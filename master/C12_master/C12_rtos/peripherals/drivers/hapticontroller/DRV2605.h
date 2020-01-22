//
//  DRV2605.h
//  C12_rtos
//
//  Created by Matthew Fonken on 8/28/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef DRV2605_h
#define DRV2605_h

#include "i2c_template.h"
#include <stdint.h>
#include <stdbool.h>

#define DRV2605_ADDR                0x5A

#define DRV2605_COMM_LEN            1

#define DRV2605_REG_STATUS          0x00
#define DRV2605_REG_MODE            0x01
#define DRV2605_MODE_INTTRIG        0x00
#define DRV2605_MODE_EXTTRIGEDGE    0x01
#define DRV2605_MODE_EXTTRIGLVL     0x02
#define DRV2605_MODE_PWMANALOG      0x03
#define DRV2605_MODE_AUDIOVIBE      0x04
#define DRV2605_MODE_REALTIME       0x05
#define DRV2605_MODE_DIAGNOS        0x06
#define DRV2605_MODE_AUTOCAL        0x07

#define DRV2605_REG_RTPIN           0x02
#define DRV2605_REG_LIBRARY         0x03
#define DRV2605_REG_WAVESEQ1        0x04
#define DRV2605_REG_WAVESEQ2        0x05
#define DRV2605_REG_WAVESEQ3        0x06
#define DRV2605_REG_WAVESEQ4        0x07
#define DRV2605_REG_WAVESEQ5        0x08
#define DRV2605_REG_WAVESEQ6        0x09
#define DRV2605_REG_WAVESEQ7        0x0A
#define DRV2605_REG_WAVESEQ8        0x0B

#define DRV2605_REG_GO              0x0C
#define DRV2605_REG_OVERDRIVE       0x0D
#define DRV2605_REG_SUSTAINPOS      0x0E
#define DRV2605_REG_SUSTAINNEG      0x0F
#define DRV2605_REG_BREAK           0x10
#define DRV2605_REG_AUDIOCTRL       0x11
#define DRV2605_REG_AUDIOLVL        0x12
#define DRV2605_REG_AUDIOMAX        0x13
#define DRV2605_REG_RATEDV          0x16
#define DRV2605_REG_CLAMPV          0x17
#define DRV2605_REG_AUTOCALCOMP     0x18
#define DRV2605_REG_AUTOCALEMP      0x19
#define DRV2605_REG_FEEDBACK        0x1A
#define DRV2605_REG_CONTROL1        0x1B
#define DRV2605_REG_CONTROL2        0x1C
#define DRV2605_REG_CONTROL3        0x1D
#define DRV2605_REG_CONTROL4        0x1E
#define DRV2605_REG_VBAT            0x21
#define DRV2605_REG_LRARESON        0x22

bool DRV2605_Start(void);
void DRV2605_Go(void);
void DRV2605_Stop(void);
void DRV2605_SetWaveform(uint8_t, uint8_t);
void DRV2605_SelectLibrary(uint8_t);
void DRV2605_SetMode(uint8_t);
void DRV2605_SetRealtimeValue(uint8_t);
void DRV2605_UseERM(void);
void DRV2605_UseLRA(void);
uint8_t DRV2605_ReadRegister(uint8_t);
void DRV2605_WriteRegister(uint8_t, uint8_t);

static i2c_event_t DRV2605_GetReadEvent(uint8_t reg) { return (i2c_event_t){ I2C_READ_REG_EVENT, reg, DRV2605_COMM_LEN, DRV2605_ADDR }; }
static i2c_event_t DRV2605_GetWriteEvent(uint8_t reg) { return (i2c_event_t){ I2C_WRITE_REG_EVENT, reg, DRV2605_COMM_LEN, DRV2605_ADDR }; }


#endif /* DRV2605_h */
