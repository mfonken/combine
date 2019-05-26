////
////  stm32_interface.c
////  rho_client
////
////  Created by Matthew Fonken on 9/22/18.
////  Copyright © 2018 Marbl. All rights reserved.
////
//
//#include "stm32_interface.h"
//
//static inline void STMInterruptHandler( uint16_t GPIO_Pin )
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
//static inline void STMInitPCLKDMA( void )
//{
//  if(HAL_DMA_Start_IT(this_timer->hdma[TIM2_DMA_ID], svd.camera_port, (uint32_t)svb.capture, CAPTURE_BUFFER_SIZE) != HAL_OK) 
//      Error_Handler();
//  __HAL_TIM_ENABLE_DMA(this_timer, TIM2_DMA_CC);
//  __HAL_TIM_ENABLE_IT(this_timer, TIM2_IT_CC);
//  TIM_CCxChannelCmd(this_timer->Instance, TIM2_CHANNEL, TIM_CCx_ENABLE);
//}
//static inline void STMPauseDMA( void )
//{
//  TIM_CCxChannelCmd(this_timer->Instance, TIM2_CHANNEL, TIM_CCx_DISABLE);
//}
//static inline void STMResumeDMA( void )
//{
//  TIM_CCxChannelCmd(this_timer->Instance, TIM2_CHANNEL, TIM_CCx_ENABLE);
//}
//static inline void STMResetDMA( void )
//{
//  this_timer->hdma[TIM2_DMA_ID]->Instance->CMAR = (uint32_t)svb.capture;
//}
//
//static inline uint8_t STMUartTxDMA( uint8_t * buffer, uint16_t length )
//{
//  HAL_UART_Transmit( this_uart, &buffer, length, UART_TIMEOUT );
//}
//
//static inline uint16_t STMUartRxDMA( uint8_t * buffer )
//{
//  ///TODO: Actually implement
//  return 1;
//}
//static inline void STMUartCompleted( USART_Handle_t * huart )
//{
//  ActiveFlags.UARTBusy = 0;
//}
//
//static void STMWritePin( uint32_t * port, uint16_t pin, uint8_t state )
//{
//  HAL_GPIO_WritePin((GPIO_Type_Def *)port, pin, GPIO_PinState state);
//}
//
//static inline uint32_t STMTimestamp(void)
//{
//  return HAL_GetTick();
//}