/************************************************************************
 *  File: stm32_interface.h
 *  Group: Rho Hardware Interface
 ***********************************************************************/

#ifdef __STM32__

/***************************************************************************************/
/*                                    Includes                                         */
/***************************************************************************************/
#ifndef stm32_interface_h
#define stm32_interface_h

#include "master_interface.h"

/***************************************************************************************/
/*                                  Macro Definitions                                  */
/***************************************************************************************/
#define STM_InterruptHandler HAL_GPIO_EXTI_Callback
#define STM_UartCompleted HAL_UART_TxCpltCallback

/***************************************************************************************/
/*                                  Global Variables                                   */
/***************************************************************************************/
static uint32_t _dma_destination = NULL, _dma_size = 0;

/***************************************************************************************/
/*                               Handler Definitions                                   */
/***************************************************************************************/
void            STM_InterruptHandler( uint16_t GPIO_Pin );
void            STM_InterruptEnable( void );
void            STM_InterruptDisable( void );
void            STM_InitDMA( uint32_t, uint32_t, uint16_t, bool );
void            STM_PauseDMA( void );
void            STM_ResumeDMA( void );
void            STM_ResetDMA( void );
uint32_t        STM_GetDMAFillAddress( void );
uint8_t         STM_UartTxDMA( UART_Handle_t * huart, uint8_t * buffer, uint16_t length );
uint16_t        STM_UartRxDMA( UART_Handle_t * huart, uint8_t * buffer );
//bool            STM_UartCompleted( UART_Handle_t *huart );
void            STM_I2CMasterTx( I2C_Handle_t * hi2c, uint16_t addr, uint8_t * buffer, uint16_t length, uint32_t timeout );
void            STM_SetPortMode( GPIO_TypeDef * port, uint16_t pin, uint8_t type );
uint8_t         STM_ReadPort(GPIO_TypeDef * port );
void            STM_WritePin( GPIO_TypeDef * port, uint16_t pin, uint8_t state );
uint32_t        STM_Timestamp( void );
void            STM_Wait( uint32_t );
uint32_t        STM_SysClockFreq(void);
void            STM_Reset( void );

#endif

#endif /* stm32_interface_h */
