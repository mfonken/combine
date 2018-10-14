//
//  stm32_interface.h
//  rho_client
//
//  Created by Matthew Fonken on 9/22/18.
//  Copyright © 2018 Marbl. All rights reserved.
//

#ifndef stm32_interface_h
#define stm32_interface_h

#define CAMERA_PORT     0//&(GPIOA->IDR)
#define UART_TX_PORT    0//&(USART1->TDR)

#define STMInterruptHandler HAL_GPIO_EXTI_Callback

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
    
}
static inline void STMPauseDMA( void )
{
    
}
static inline void STMResumeDMA( void )
{
    
}
static inline void STMResetDMA( void )
{
    
}

static inline uint8_t STMUartTxDMA( uint8_t * buffer, uint16_t length )
{
    return 1;
}

static inline uint16_t STMUartRxDMA( uint8_t * buffer )
{
    return 1;
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
