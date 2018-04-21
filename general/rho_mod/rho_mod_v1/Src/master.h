#ifndef master_h
#define master_h

#include "OV9712.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"
#include "usb_device.h"
#include "usbd_cdc.h"

#include "rho\rho_utility.h"

void master_init( TIM_HandleTypeDef *, DMA_HandleTypeDef *, I2C_HandleTypeDef * );
void master_test( void );

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

void pauseDMA( TIM_HandleTypeDef * );
void resumeDMA( TIM_HandleTypeDef * );
void init_memory( void );
void initTimerDMA( TIM_HandleTypeDef *, DMA_HandleTypeDef * );
void zero_memory( void );
void printBuffers( uint32_t );
void printAddress( const char *, uint32_t );
void printAddresses( void );
void printBuffer( index_t *, int );
void drawDensityMap( density_t *, int );

/***************************** Memory *****************************/
capture_t
    CAPTURE_BUFFER[CAPTURE_BUFFER_SIZE];    /* Raw capture buffer for DMA */
index_t
    THRESH_BUFFER[THRESH_BUFFER_SIZE],        /* Threshold processing buffer */
    CENTROID_X,
    CENTROID_Y;
density_t
    DENSITY_X[CAPTURE_WIDTH],                                        /* Processed density X array */
    DENSITY_Y[CAPTURE_HEIGHT],                                        /* Processed density Y array */
    QUADRANT_BUFFER[4],                                                                    /* Quadrant density array */
    QUADRANT_TOTAL,                                                            /* Total density */
    QUADRANT_PREVIOUS,                                                                /* Previous row density */
    DENSITY_X_MAX;
address_t
    CAPTURE_BUFFER_END,
    CAPTURE_BUFFER_MAX,
    THRESH_BUFFER_END,                                        /* Max threshold buffering size */
    THRESH_BUFFER_MAX,
    THRESH_VALUE,                                                    /* Shared threshold value */
    RHO_MEM_END;
/******************************************************************/

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
