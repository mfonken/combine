#include "global_lib.h"

void InitPlatform( TIMER_Handle_t * timer, USART_Handle_t * usart, I2C_Handle_t * i2c )
{
  Utilities.Timer = timer;
  Utilities.Usart = usart;
  Utilities.I2C = i2c;
  
  Utilities.Ready = true;
  
  timestamp = PLATFORM_SPECIFIC(Timestamp);
}

void WritePin( gpio_t * gpio, uint8_t val )
{
  PLATFORM_SPECIFIC(WritePin)( gpio->port, gpio->pin, val );
}

void SetPortMode(gpio_t * gpio, uint8_t val )
{
  PLATFORM_SPECIFIC(SetPortMode)( gpio->port, val );
}

#ifdef __RHO__
void SetActiveClientFlags( rho_system_flags_variables * Flags )
{
  ActiveFlags = Flags;
}
void InitRhoInterface( TIMER_Handle_t * timer, USART_Handle_t * usart )
{
}

inline bool TransmitPacket( packet_t * packet )
{
  return (bool)Platform.USART.Transmit( (uint8_t *)packet, sizeof(packet_t));
}

inline uint16_t ReceivePacket( packet_t * packet )
{
  return Platform.USART.Receive( (uint8_t *)packet );
}
#endif
