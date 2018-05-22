#ifndef master_h
#define master_h

#include "OV9712.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"

#include "Rho\rho_utility.h"

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
void initTimerDMA( void );
void zero_memory( void );

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
void print(uint8_t* Buf);
void printPredictionPair( prediction_pair_t * pr );

static void printc(uint8_t* Buf)
{
	uint16_t Len = 0;
	uint8_t c = 0xff;
	for( ; c != '\0'; c = Buf[Len++] ) ITM_SendChar(c);
}

/*
static uint8_t USB_TX(uint8_t* Buf)
{
	uint16_t Len = 0;
	uint8_t c = 0xff;
	for( ; c != '\0'; c = Buf[Len]) Len++;

  uint8_t result = USBD_OK;

  USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  if ((Len%64==0) && (result==USBD_OK))
  {
    USBD_CDC_SetTxBuffer(&hUsbDeviceFS, Buf, 0);
    result = USBD_CDC_TransmitPacket(&hUsbDeviceFS);
  }
	HAL_Delay(0);
  return result;
}
*/

extern rho_utility Rho;

#endif
