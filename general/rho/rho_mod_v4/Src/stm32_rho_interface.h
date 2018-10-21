//
//  stm32_interface.h
//  rho_client
//
//  Created by Matthew Fonken on 9/22/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef stm32_interface_h
#define stm32_interface_h

#include "rho_master.h"

#define CAMERA_PORT     (GPIOA->IDR)
#define UART_TX_PORT    (USART1->TDR)

#define UART_TIMEOUT	100

/* DMA Config */
#define TIM2_DMA_ID 	TIM_DMA_ID_CC2
#define TIM2_DMA_CC	TIM_DMA_CC2
#define TIM2_IT_CC	TIM_IT_CC2
#define TIM2_CHANNEL 	TIM_CHANNEL_2

#define STMInterruptHandler HAL_GPIO_EXTI_Callback
#define STMUartCompleted HAL_UART_TxCpltCallback

typedef struct
{
  USART_HandleTypeDef * Usart;
  TIM_HandleTypeDef *   Timer;
} platform_utilities;

static platform_utilities Utilities;
static rho_system_flags_variables * ActiveFlags;

static void InitRhoInterface( TIM_HandleTypeDef * timer, USART_HandleTypeDef * usart );
static void ActivateClientFlags( rho_system_flags_variables * Flags ); 
inline void STMInterruptHandler( uint16_t GPIO_Pin );
static inline void STMInitDMA( void );
static inline void STMPauseDMA( void );
static inline void STMResumeDMA( void );
static inline void STMResetDMA( void );
static inline uint8_t STMUartTxDMA( uint8_t * buffer, uint16_t length );
static inline uint16_t STMUartRxDMA( uint8_t * buffer );
static inline void STMUartCompleted( USART_HandleTypeDef *huart );
static inline uint8_t TransmitPacket( packet_t * packet );
static inline uint16_t ReceivePacket( packet_t * packet );

static platform_interface_functions PlatformInterface = 
{
  .DMA.Init = STMInitDMA,
  .DMA.Pause = STMPauseDMA,
  .DMA.Resume = STMResumeDMA,
  .DMA.Reset = STMResetDMA,
  
  .Usart.Transmit = STMUartTxDMA,
  
  .Flags.Activate = ActivateClientFlags
};

static void InitRhoInterface(TIM_HandleTypeDef * timer, USART_HandleTypeDef * usart )
{
  Utilities.Timer = timer;
  Utilities.Usart = usart;
  
  RhoSystem.Functions.Perform.ConnectToInterface( &PlatformInterface );
}

static void ActivateClientFlags( rho_system_flags_variables * Flags )
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

static inline void STMInitDMA( void )
{
  if(HAL_DMA_Start_IT(Utilities.Timer->hdma[TIM2_DMA_ID], 
                      CAMERA_PORT, 
                      (uint32_t)RhoSystem.Variables.Buffers.Capture, 
                      CAPTURE_BUFFER_SIZE) != HAL_OK) 
      Error_Handler();
  __HAL_TIM_ENABLE_DMA(Utilities.Timer, TIM2_DMA_CC);
  __HAL_TIM_ENABLE_IT(Utilities.Timer, TIM2_IT_CC);
  TIM_CCxChannelCmd(Utilities.Timer->Instance, TIM2_CHANNEL, TIM_CCx_ENABLE);
}
static inline void STMPauseDMA( void )
{
  TIM_CCxChannelCmd(Utilities.Timer->Instance, TIM2_CHANNEL, TIM_CCx_DISABLE);
}
static inline void STMResumeDMA( void )
{
  TIM_CCxChannelCmd(Utilities.Timer->Instance, TIM2_CHANNEL, TIM_CCx_ENABLE);
}
static inline void STMResetDMA( void )
{
  Utilities.Timer->hdma[TIM2_DMA_ID]->Instance->CMAR = (uint32_t)RhoSystem.Variables.Buffers.Capture;
}

static inline uint8_t STMUartTxDMA( uint8_t * buffer, uint16_t length )
{
  uint8_t ascii_clear = 0x0c;
  return (uint8_t)HAL_USART_Transmit( Utilities.Usart, &ascii_clear, 1, UART_TIMEOUT );
}

static inline uint16_t STMUartRxDMA( uint8_t * buffer )
{
  ///TODO: Actually implement lol
  return 1;
}
static inline void STMUartCompleted( USART_HandleTypeDef *huart )
{
  ActiveFlags->UARTBusy = 0;
}

static inline uint8_t TransmitPacket( packet_t * packet )
{
  return STMUartTxDMA( (byte_t *)packet, sizeof(packet_t));
}

static inline uint16_t ReceivePacket( packet_t * packet )
{
  return STMUartRxDMA( (byte_t *)packet );
}

#endif /* stm32_interface_h */
