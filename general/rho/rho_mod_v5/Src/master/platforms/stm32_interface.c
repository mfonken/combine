//
//  stm32_interface.c
//  rho_client
//
//  created by matthew fonken on 9/22/18.
//  copyright © 2018 marbl. all rights reserved.
//

#include "stm32_interface.h"

//static inline void STM_InterruptHandler( uint16_t GPIO_Pin )
//{
//    if(!ActiveFlags->IRQ) return;
////    switch(GPIO_Pin)
////    {
////        case VSYNC_Pin:
////            ActiveFlags->Frame = !(flag_t)(VSYNC_GPIO_Port->IDR & VSYNC_Pin);
////            return;
////        case HREF_Pin:
////            ActiveFlags->Row     =  (flag_t)( HREF_GPIO_Port->IDR & HREF_Pin);
////            return;
////        default:
////            return;
////    }
//}
//
//static inline void STM_InitPCLKDMA( void )
//{
//  if(HAL_DMA_Start_IT(this_timer->hdma[TIM2_DMA_ID], svd.camera_port, (uint32_t)svb.capture, CAPTURE_BUFFER_SIZE) != HAL_OK)
//      Error_Handler();
//  __HAL_TIM_ENABLE_DMA(this_timer, TIM2_DMA_CC);
//  __HAL_TIM_ENABLE_IT(this_timer, TIM2_IT_CC);
//  TIM_CCxChannelCmd(this_timer->Instance, TIM2_CHANNEL, TIM_CCx_ENABLE);
//}
//static inline void STM_PauseDMA( void )
//{
//  TIM_CCxChannelCmd(this_timer->Instance, TIM2_CHANNEL, TIM_CCx_DISABLE);
//}
//static inline void STM_ResumeDMA( void )
//{
//  TIM_CCxChannelCmd(this_timer->Instance, TIM2_CHANNEL, TIM_CCx_ENABLE);
//}
//static inline void STM_ResetDMA( void )
//{
//  this_timer->hdma[TIM2_DMA_ID]->Instance->CMAR = (uint32_t)svb.capture;
//}
//
//static inline uint8_t STM_UartTxDMA( USART_Handle_t * huart, uint8_t * buffer, uint16_t length )
//{
//  HAL_UART_Transmit( this_uart, &buffer, length, UART_TIMEOUT );
//}
//
//static inline uint16_t STM_UartRxDMA( USART_Handle_t * huart, uint8_t * buffer )
//{
//  ///TODO: Actually implement
//  return 1;
//}
//static inline bool STM_UartCompleted( USART_Handle_t * huart )
//{
//  ActiveFlags.UARTBusy = 0;
//}
inline void STM_I2CMasterTx( I2C_Handle_t * hi2c, uint16_t addr, uint8_t * buffer, uint16_t length, uint32_t timeout )
{
  HAL_I2C_Master_Transmit( hi2c, addr, buffer, length, timeout);
}

inline void STM_SetPortMode( uint32_t * port, uint8_t type )
{
  //Set_GPIO_Type( port, type );
}

inline void STM_WritePin( uint32_t * port, uint16_t pin, uint8_t state )
{
  HAL_GPIO_WritePin( (GPIO_TypeDef *)port, pin, (GPIO_PinState)state);
}
//
//static inline uint32_t STM_Timestamp(void)
//{
//  return HAL_GetTick();
//}

inline void STM_Wait( uint32_t nTime );
{
  DelayMs( nTime );
}
