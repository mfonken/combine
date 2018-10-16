//
//  stm32_interface.h
//  rho_client
//
//  Created by Matthew Fonken on 9/22/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef stm32_interface_h
#define stm32_interface_h

#define CAMERA_PORT     &(GPIOA->IDR)
#define UART_TX_PORT    &(USART1->TDR)

#define STMInterruptHandler HAL_GPIO_EXTI_Callback
#define STMUartCompleted HAL_UART_TxCpltCallback

typedef struct
{
    flag_t
    Active,
    IRQ,
    Frame,
    Row,
    Backgrounding,
    UARTBusy;
} rho_system_flags_variables;

static rho_system_flags_variables * ActiveFlags;

static void SetActiveClientFlags( rho_system_flags_variables * Flags )
{
    ActiveFlags = Flags;
}

static inline void STMInterruptHandler( uint16_t GPIO_Pin )
{
    if(!ActiveFlags->IRQ) return;
//    switch(GPIO_Pin)
//    {
//        case VSYNC_Pin:
//            ActiveFlags->Frame = !(flag_t)(VSYNC_GPIO_Port->IDR & VSYNC_Pin);
//            return;
//        case HREF_Pin:
//            ActiveFlags->Row     =  (flag_t)( HREF_GPIO_Port->IDR & HREF_Pin);
//            return;
//        default:
//            return;
//    }
}

static inline void STMInitPCLKDMA( void )
{
  if(HAL_DMA_Start_IT(this_timer->hdma[TIM2_DMA_ID], svd.camera_port, (uint32_t)svb.capture, CAPTURE_BUFFER_SIZE) != HAL_OK) 
      Error_Handler();
  __HAL_TIM_ENABLE_DMA(this_timer, TIM2_DMA_CC);
  __HAL_TIM_ENABLE_IT(this_timer, TIM2_IT_CC);
  TIM_CCxChannelCmd(this_timer->Instance, TIM2_CHANNEL, TIM_CCx_ENABLE);
}
static inline void STMPauseDMA( void )
{
  TIM_CCxChannelCmd(this_timer->Instance, TIM2_CHANNEL, TIM_CCx_DISABLE);
}
static inline void STMResumeDMA( void )
{
  TIM_CCxChannelCmd(this_timer->Instance, TIM2_CHANNEL, TIM_CCx_ENABLE);
}
static inline void STMResetDMA( void )
{
  this_timer->hdma[TIM2_DMA_ID]->Instance->CMAR = (uint32_t)svb.capture;
}

static inline uint8_t STMUartTxDMA( uint8_t * buffer, uint16_t length )
{
  uint8_t ascii_clear = 0x0c;
  HAL_UART_Transmit( this_uart, &ascii_clear, 1, UART_TIMEOUT );
}

static inline uint16_t STMUartRxDMA( uint8_t * buffer )
{
  ///TODO: Actual implement lol
  return 1;
}
static inline void STMUartCompleted( UART_HandleTypeDef *huart )
{
  ActiveFlags.UARTBusy = 0;
}

static inline bool TransmitPacket( packet_t * packet )
{
  return (bool)STMUartTxDMA( (byte_t *)packet, sizeof(packet_t));
}

static inline uint16_t ReceivePacket( packet_t * packet )
{
  return STMUartRxDMA( (byte_t *)packet );
}

#endif /* stm32_interface_h */
