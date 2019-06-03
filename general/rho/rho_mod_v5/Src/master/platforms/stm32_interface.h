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

#define STM_InterruptHandler HAL_GPIO_EXTI_Callback
#define STM_UartCompleted HAL_UART_TxCpltCallback

void            STM_InterruptHandler( uint16_t GPIO_Pin );
void            STM_InitDMA( void );
void            STM_PauseDMA( void );
void            STM_ResumeDMA( void );
void            STM_ResetDMA( void );
uint8_t         STM_UartTxDMA( USART_Handle_t * huart, uint8_t * buffer, uint16_t length );
uint16_t        STM_UartRxDMA( USART_Handle_t * huart, uint8_t * buffer );
bool            STM_UartCompleted( USART_Handle_t *huart );
void            STM_I2CMasterTx( I2C_Handle_t * hi2c, uint16_t addr, uint8_t * buffer, uint16_t length, uint32_t timeout );
void            STM_SetPortMode( uint32_t * port, uint8_t type );
void            STM_WritePin( uint32_t * port, uint16_t pin, uint8_t state );
uint32_t        STM_Timestamp(void);

#endif

#endif /* stm32_interface_h */
