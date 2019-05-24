#ifndef global_config_h
#define global_config_h

#include "global_types.h"

#ifdef __STM32__
#define _PLATFORM_ STM

#include "stm32l4xx_hal.h"

/* DMA Config */
#define TIM2_DMA_ID 	TIM_DMA_ID_CC2
#define TIM2_DMA_CC	TIM_DMA_CC2
#define TIM2_IT_CC	TIM_IT_CC2
#define TIM2_CHANNEL 	TIM_CHANNEL_2

#define CAMERA_PORT     (GPIOA->IDR)
#define UART_TX_PORT    (USART1->TDR)

#define USART_Handle_t  USART_HandleTypeDef
#define TIMER_Handle_t  TIM_HandleTypeDef
#define I2C_Handle_t    I2C_HandleTypeDef

#endif /* __STM32__ */

#define USART_TIMEOUT	100

#endif /* global_config_h */