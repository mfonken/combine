/************************************************************************
 *  File: global_config.h
 *  Group: Rho Core
 ***********************************************************************/

#ifndef global_config_h
#define global_config_h

#include "global_types.h"

#define THIS_ID           0x01

/* Global configurations */
#define HOST_COMMUNICATION_PROTOCOL     USART_PROTOCOL_TYPE
#define HOST_DEFAULT_CHANNEL            UART_Primary

#define CAMERA_COMMUNICATION_PROTOCOL   I2C_PROTOCOL_TYPE
#define CAMERA_COMMUNICATION_CHANNEL    I2C_Primary

#define USART_TIMEOUT	              100
#define UART_TIMEOUT	              USART_TIMEOUT

#define HOST_ADDRESS                0x01
#define HOST_COMM_RETRIES           3
#define HOST_COMMAND_RETURN_LENGTH  2
#define HOST_COMMAND_WAIT_TIME      5000
#define HOST_COMMAND_RETRY_TIME     1000

#define CAMERA_PORT     	(GPIOA->IDR)
#define UART_TX_PORT    	(USART1->TDR)

#define RHO_TIM_DMA_ID      TIM_DMA_ID_CC2
#define RHO_TIM_DMA_CC	    TIM_DMA_CC2
#define RHO_TIM_IT_CC       TIM_IT_CC2
#define RHO_TIM_CHANNEL     TIM_CHANNEL_2


  /* Application utilities */
#ifdef __RHO__
  static camera_application_flags CameraFlags;
#endif

#endif /* global_config_h */
