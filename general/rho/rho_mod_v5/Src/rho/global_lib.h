#ifndef global_lib
#define global_lib

#include "resources.h"
#include "global_config.h"
#include "system.h"
#include "printers.h"

#include "_CAMERA_.h"

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
static rho_system_flags_variables * ActiveFlags;

/* System interfaces */
static void ActivateClientFlags( rho_system_flags_variables * Flags ); 

/* Application interfaces */
static void InitRhoInterface( TIMER_Handle_t * timer, USART_Handle_t * usart );

/* Communication interfaces */
static inline uint8_t   TransmitPacket( packet_t * packet );
static inline uint16_t  ReceivePacket( packet_t * packet );

/* Platform interfaces */
static inline void      _PLATFORM_InterruptHandler( uint16_t GPIO_Pin );
static inline void      _PLATFORM_InitDMA( void );
static inline void      _PLATFORM_PauseDMA( void );
static inline void      _PLATFORM_ResumeDMA( void );
static inline void      _PLATFORM_ResetDMA( void );
static inline uint8_t   _PLATFORM_UartTxDMA( uint8_t * buffer, uint16_t length );
static inline uint16_t  _PLATFORM_UartRxDMA( uint8_t * buffer );
static inline void      _PLATFORM_UartCompleted( USART_Handle_t *huart );

static platform_interface_functions PlatformInterface = 
{
  .DMA.Init             = _PLATFORM_InitDMA,
  .DMA.Pause            = _PLATFORM_PauseDMA,
  .DMA.Resume           = _PLATFORM_ResumeDMA,
  .DMA.Reset            = _PLATFORM_ResetDMA,
  
  .Usart.Transmit       = _PLATFORM_UartTxDMA,
  
  .Flags.Activate       = ActivateClientFlags,
    
  .Time.Now             = Now
};

#endif
