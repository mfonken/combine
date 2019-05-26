//
//  stm32_interface.h
//  rho_client
//
//  Created by Matthew Fonken on 9/22/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifdef __STM32__

#ifndef stm32_interface_h
#define stm32_interface_h

#include "global_config.h"

#define STMInterruptHandler HAL_GPIO_EXTI_Callback
#define STMUartCompleted HAL_UART_TxCpltCallback

void            STMInterruptHandler( uint16_t GPIO_Pin );
void            STMPauseDMA( void );
void            STMResumeDMA( void );
void            STMResetDMA( void );
uint8_t         STMUartTxDMA( uint8_t * buffer, uint16_t length );
uint16_t        STMUartRxDMA( uint8_t * buffer );
void            STMUartCompleted( USART_Handle_t *huart );
void            STMWritePin( uint32_t * port, uint16_t pin, uint8_t state );
uint32_t        STMTimestamp(void);

#endif

#endif /* stm32_interface_h */
