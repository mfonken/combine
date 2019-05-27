#ifndef global_lib
#define global_lib

#include "resources.h"
#include "global_config.h"
#include "system.h"
#include "printers.h"

#ifdef __OV9712__
#include "OV9712.h"
#endif

#ifdef __RHO__
#include "rho_master.h"
#endif

#ifdef __STM__
#include "stm32l4xx_it.h"
#include "stm32_interface.h"
#endif

void InitPlatform( TIMER_Handle_t *, USART_Handle_t *, I2C_Handle_t * );

typedef struct
{
  bool             Ready;
  USART_Handle_t * Usart;
  TIMER_Handle_t * Timer;
  I2C_Handle_t   * I2C;
} platform_utilities;

static platform_utilities Utilities = { false };

#ifdef __RHO__
/* Application interfaces */
static rho_system_flags_variables * ActiveFlags;
static void InitRhoInterface( TIMER_Handle_t * timer, USART_Handle_t * usart );
static void ActivateFlagsRhoInterface( rho_system_flags_variables * Flags ); 

inline uint8_t   TransmitPacket( packet_t * );
inline uint16_t  ReceivePacket( packet_t * );

typedef struct
{
  uint8_t(*Transmit)( packet_t * );
  uint16_t(*Receive)( packet_t * );
} rho_interface_packet_functions;

typedef struct
{
  void (*Init)( TIMER_Handle_t *, USART_Handle_t * );
  void (*ActivateFlags)( rho_system_flags variables * );
  rho_interface_packet_functions Packet;
} rho_interface_functions;

#endif

/*** Platform interfaces ***/
/* Interrupt */
inline void      _PLATFORM_InterruptHandler( uint16_t );

/* DMA */
inline void      _PLATFORM_InitDMA( void );
inline void      _PLATFORM_PauseDMA( void );
inline void      _PLATFORM_ResumeDMA( void );
inline void      _PLATFORM_ResetDMA( void );

/* UART */
inline uint8_t   _PLATFORM_UartTxDMA( uint8_t *, uint16_t );
inline uint16_t  _PLATFORM_UartRxDMA( uint8_t * );
inline bool      _PLATFORM_UartCompleted( USART_Handle_t * );

/* GPIO */
inline void      _PLATFORM_WritePin(uint32_t *, uint16_t, uint8_t );

/* Time */
inline uint32_t  _PLATFORM_Timestamp(void);

typedef struct
{
  void(*Handler)( uint16_t );
} platform_interface_interrupt_functions;

typedef struct
{
  void(*Init)( void );
  void(*Pause)( void );
  void(*Resume)( void );
  void(*Reset)( void );
} platform_interface_dma_functions;

typedef struct
{
  uint8_t(*Transmit)( uint8_t *, uint16_t );
  uint16_t(*Receive)( uint8_t * );
  bool (*Completed)( USART_Handle_t * );
} platform_interface_uart_functions;

typedef struct
{
  void(*Write)( uint32_t *, uint16_t, uint8_t );
} platform_interface_gpio_functions;

typedef struct
{
  uint32_t(*Now)( void );
} platform_interface_time_functions;

typedef struct
{
  void (*Init)( TIMER_Handle_t *, USART_Handle_t *, I2C_Handle_t * );
#ifdef __RHO__
  rho_interface_functions                Rho;
  platform_interface_packet_functions    Packet;
#endif
  platform_interface_interrupt_functions Interrupt;
  platform_interface_dma_functions       DMA;
  platform_interface_uart_functions      USART;
  platform_interface_gpio_functions      GPIO;
  platform_interface_time_functions      Time;
} platform_interface_functions;


static platform_interface_functions Platform = 
{
  .Init                 = InitPlatform,
  
#ifdef __RHO__
  .Rho.Init             = InitRhoInterface,
  .Rho.ActivateFlags    = ActivateFlagsRhoInterface,
  
  .Rho.Packet.Transmit  = TransmitPacket,
  .Rho.Packet.Receive   = ReceivePacket,
#endif
  .Interrupt.Handler    = _PLATFORM_InterruptHandler,
  
  .DMA.Init             = _PLATFORM_InitDMA,
  .DMA.Pause            = _PLATFORM_PauseDMA,
  .DMA.Resume           = _PLATFORM_ResumeDMA,
  .DMA.Reset            = _PLATFORM_ResetDMA,
  
  .USART.Transmit       = _PLATFORM_UartTxDMA,
  .USART.Receive        = _PLATFORM_UartRxDMA,
  .USART.Completed      = _PLATFORM_UartCompleted,
  
  .GPIO.Write           = _PLATFORM_WritePin,
  
#ifdef __RHO__
  .Flags.Activate       = ActivateClientFlags,
#endif
  
  .Time.Now             = _PLATFORM_Timestamp
};

uint32_t (*timestamp)(void); 

#endif /* global_lib_h */
