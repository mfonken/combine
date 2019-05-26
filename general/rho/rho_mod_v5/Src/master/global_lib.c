#include "global_lib.h"

static void InitPlatform( TIMER_Handle_t * timer, USART_Handle_t * usart, I2C_Handle_t * i2c )
{
  Utilities.Timer = timer;
  Utilities.Usart = usart;
  Utilities.I2C = i2c;
  
  Utilities.Ready = true;
}

#ifdef __RHO__
static void SetActiveClientFlags( rho_system_flags_variables * Flags )
{
  ActiveFlags = Flags;
}
static void InitRhoInterface( TIMER_Handle_t * timer, USART_Handle_t * usart )
{
}

static inline bool TransmitPacket( packet_t * packet )
{
  return (bool)Platform.USART.Transmit( (uint8_t *)packet, sizeof(packet_t));
}

static inline uint16_t ReceivePacket( packet_t * packet )
{
  return Platform.USART.Receive( (uint8_t *)packet );
}
#endif
