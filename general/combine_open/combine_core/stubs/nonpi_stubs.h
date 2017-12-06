#ifndef nonpi_stubs_h
#define nonpi_stubs_h

#ifdef __cplusplus
extern "C" {
#endif
    
#include <stdio.h>
#include <stdint.h>
#include "../kinetic/LSM9DS1_types.h"
    
#define nil         0x00
#define I2C_SLAVE   nil
#define O_RDWR      nil
#define RETURN      0xff
    
    static int I2C_Read_Reg(char a, char b){return RETURN;}
    static int I2C_Read_Regs(char a, char b, unsigned char *c, char d){return RETURN;}
    static int I2C_Write(char a, unsigned char *b, int c){return RETURN;}
#ifdef __cplusplus
}
#endif

#endif
