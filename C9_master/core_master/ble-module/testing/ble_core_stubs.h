#ifndef ble_core_stubs_h
#define ble_core_stubs_h

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define USART0_RX_IRQn 0

void SYSCLK_Init();

void Print_String(char*s);
void Print_Int(int i);
void Print_Double_Ascii(double d);
void Print_Char(char c);
void Print_Line(char*s);

char Read_Char(void);
void SYSCTL_Enable_Camera(void);
void SYSCTL_Disable_Camera(void);
void NVIC_EnableIRQ(int i);
void NVIC_DisableIRQ(int i);
void enableUARTInterrupt(void);
void disableUARTInterrupt(void);

bool I2C_Read(uint8_t a,uint8_t*b,uint8_t c);
bool I2C_Read_Reg(uint8_t a, uint8_t b, uint8_t*c, uint8_t d);
bool I2C_Write(uint8_t a, uint8_t*b, uint8_t c);

double timestamp(void);
double seconds_since(double t);

#endif /* ble_core_stubs_h */
