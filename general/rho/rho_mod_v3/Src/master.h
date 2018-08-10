#ifndef master_h
#define master_h

#include "OV9712.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"

#include "Rho\rho_utility.h"
#include "printers.h"

void spoofFrameProcessor( void );
void frameProcessor( void );
void master_init( I2C_HandleTypeDef *, TIM_HandleTypeDef *, UART_HandleTypeDef * );
void master_test( void );
void frameTx( void );
void spoofFrameTx( void );

/* DMA Config */
#define TIM2_DMA_ID 	TIM_DMA_ID_CC2
#define TIM2_DMA_CC		TIM_DMA_CC2
#define TIM2_IT_CC		TIM_IT_CC2
#define TIM2_CHANNEL 	TIM_CHANNEL_2

void pauseDMA( void );
void resumeDMA( void );
void resetDMA( void );

void init_memory( void );
void zero_memory( void );
void initTimerDMA( void );

void performBackgroundEvent( void );

void spoofPixels( void );
void spoofDensityMaps( void );

void dprint( uint8_t *, uint16_t);
void dprintBuffers( void );
void dprintCapture( void );
void printBuffers( uint32_t );
void printAddress( const char *, uint32_t );
void printAddresses( void );
void printCapture( void );
void printBuffer( index_t *, int );
void drawDensityMap( density_t *, int );
void UART_Clear( void );
void print( const uint8_t* Buf);
void printPredictionPair( prediction_pair_t * pr );

#endif
