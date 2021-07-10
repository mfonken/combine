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

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "stm32_config.h"

/***************************************************************************************/
/*                                  Macro Definitions                                  */
/***************************************************************************************/
#define STM_InterruptHandler HAL_GPIO_EXTI_Callback
#define STM_UartCompleted HAL_UART_TxCpltCallback

/***************************************************************************************/
/*                                  Global Variables                                   */
/***************************************************************************************/
static uint32_t _dma_destination = (uint32_t)NULL, _dma_size = 0;

///***************************************************************************************/
///*                                  Master Definitions                                   */
///***************************************************************************************/
typedef struct
{
	I2C_Handle_t * I2C_Primary;
	UART_Handle_t * UART_Primary;
//  USB_Handle_t * USB_Primary;
} master_ios_t;

typedef struct
{
	TIMER_Handle_t * Timer_Primary;
} master_utilities_t;

typedef struct
{
  master_ios_t IOs;
  master_utilities_t Utilities;
} master_t;

extern master_t Master;

/***************************************************************************************/
/*                               Handler Definitions                                   */
/***************************************************************************************/
void            STM_InterruptHandler( uint16_t GPIO_Pin );
void            STM_InterruptEnable( void );
void            STM_InterruptDisable( void );
void            STM_InitDMA( uint32_t, uint32_t, uint16_t, bool );
void            STM_PauseDMA( uint32_t, uint32_t );
void            STM_ResumeDMA( uint32_t, uint32_t );
void            STM_ResetDMA( void *, uint8_t );
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
