//
//  ble_core_stubs.c
//  
//
//  Created by Matthew Fonken on 6/22/17.
//
//

#include "ble_core_stubs.h"

void SYSCLK_Init(){return;}

void Print_String(char*s){return;}
void Print_Int(int i){return;}
void Print_Double_Ascii(double d){return;}
void Print_Char(char c){return;}
void Print_Line(char*s){return;}

char Read_Char(void){return 0;}
void SYSCTL_Enable_Camera(void){return;}
void SYSCTL_Disable_Camera(void){return;}
void NVIC_EnableIRQ(int i){return;}
void NVIC_DisableIRQ(int i){return;}
void enableUARTInterrupt(void){return;}
void disableUARTInterrupt(void){return;}

bool I2C_Read(uint8_t a,uint8_t*b,uint8_t c){return false;};
bool I2C_Read_Reg(uint8_t a, uint8_t b, uint8_t*c, uint8_t d){return false;};
bool I2C_Write(uint8_t a, uint8_t*b, uint8_t c){return false;};


double timestamp(void){return 0;}
double seconds_since(double t){return 0;}
