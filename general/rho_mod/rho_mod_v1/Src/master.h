#ifndef master_h
#define master_h

#include "OV9712.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"
#include "usb_device.h"
#include "usbd_cdc.h"

#incldue "rho_utility.h"

void master_init( TIM_HandleTypeDef *, DMA_HandleTypeDef *, I2C_HandleTypeDef * );
void master_test( void );

/* Camera Config */
#define CAMERA_WIDTH 	640
#define CAMERA_HEIGHT	400

/* Capture Config */
#define CAPTURE_DIV			1
#define CAPTURE_WIDTH 	(CAMERA_WIDTH/CAPTURE_DIV)
#define CAPTURE_HEIGHT	(CAMERA_HEIGHT/CAPTURE_DIV)
#define CAPTURE_BUFFER_SIZE 36
#define CAPTURE_BUFFER_WIDTH CAPTURE_BUFFER_SIZE/6
#define THRESH_BUFFER_SIZE CAPTURE_BUFFER_SIZE

/* DMA Config */
#define TIM2_DMA_ID 	TIM_DMA_ID_CC2
#define TIM2_DMA_CC		TIM_DMA_CC2
#define TIM2_IT_CC		TIM_IT_CC2
#define TIM2_CHANNEL 	TIM_CHANNEL_2

typedef uint8_t		byte_t;
typedef uint16_t	index_t;
typedef uint8_t 	capture_t;
typedef uint32_t	density_t;
typedef uint32_t	address_t;


void init_memory( void );
void initTimerDMA( TIM_HandleTypeDef *, DMA_HandleTypeDef * );
void zero_memory( void );
void printBuffers( void );
void printAddress( const char *, uint32_t );
void printAddresses( void );
void printBuffer( index_t *, int );
void drawDensityMap( density_t *, int );

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
	HAL_Delay(30);
  return result;
}

extern rho_c_utility Rho;

#endif
