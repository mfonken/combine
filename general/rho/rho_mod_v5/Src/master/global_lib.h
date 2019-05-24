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

typedef struct
{
  USART_Handle_t * Usart;
  TIMER_Handle_t * Timer;
} platform_utilities;

static platform_utilities Utilities;

#ifdef __RHO__
/* Application interfaces */
static void InitRhoInterface( TIMER_Handle_t * timer, USART_Handle_t * usart );
static rho_system_flags_variables * ActiveFlags;
static void ActivateClientFlags( rho_system_flags_variables * Flags ); 
#endif

/* Communication interfaces */
inline uint8_t   TransmitPacket( packet_t * packet );
inline uint16_t  ReceivePacket( packet_t * packet );
inline uint32_t  Timestamp( void );

/* Platform interfaces */
inline void      _PLATFORM_InterruptHandler( uint16_t GPIO_Pin );
inline void      _PLATFORM_InitDMA( void );
inline void      _PLATFORM_PauseDMA( void );
inline void      _PLATFORM_ResumeDMA( void );
inline void      _PLATFORM_ResetDMA( void );
inline uint8_t   _PLATFORM_UartTxDMA( uint8_t * buffer, uint16_t length );
inline uint16_t  _PLATFORM_UartRxDMA( uint8_t * buffer );
inline void      _PLATFORM_UartCompleted( USART_Handle_t *huart );

static platform_interface_functions PlatformInterface = 
{
  .DMA.Init             = _PLATFORM_InitDMA,
  .DMA.Pause            = _PLATFORM_PauseDMA,
  .DMA.Resume           = _PLATFORM_ResumeDMA,
  .DMA.Reset            = _PLATFORM_ResetDMA,
  
  .Usart.Transmit       = _PLATFORM_UartTxDMA,
  
#ifdef __RHO__
  .Flags.Activate       = ActivateClientFlags,
#endif
  
  .Time.Now             = Timestamp
};

#endif /* global_lib_h */
