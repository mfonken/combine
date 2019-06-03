#ifndef global_config_h
#define global_config_h

#include "global_types.h"

/* Global configurations */
#define HOST_COMMUNICATION_PROTOCOL USART

#define USART_TIMEOUT	100

#ifdef __STM32__
#define _PLATFORM_ STM

/* DMA Config */
#define TIM2_DMA_ID 	  TIM_DMA_ID_CC2
#define TIM2_DMA_CC	    TIM_DMA_CC2
#define TIM2_IT_CC	    TIM_IT_CC2
#define TIM2_CHANNEL 	  TIM_CHANNEL_2

#define CAMERA_PORT     (GPIOA->IDR)
#define UART_TX_PORT    (USART1->TDR)

#define TIMER_Handle_t  TIM_HandleTypeDef
#define I2C_Handle_t    I2C_HandleTypeDef
#define SPI_Handle_t    SPI_HandleTypeDef
#define USART_Handle_t  USART_HandleTypeDef
#define USB_Handle_t    USB_HandleTypeDef
#endif /* __STM32__ */

#endif /* global_config_h */
