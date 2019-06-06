#include "platform.h"

void InitPlatform( platform_t * platform, protocol_t host_communication_protocol, generic_handle_t host_communication_handle )
{
  *platform = (platform_t){ host_communication_protocol, host_communication_handle };

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

uint8_t TransmitToHost( uint8_t * buffer, uint16_t length )
{
#if HOST_COMMUNICATION_PROTOCOL == USART
  return PlatformFunctions.USART.Transmit( (USART_Handle_t *)Platform.host_communication_handle, buffer, length );
#elif HOST_COMMUNICATION_PROTOCOL == I2C
  PlatformFunction.I2C.Transmit( (I2C_Handle_t *)Platform.host_communication_handle, HOST_ADDRESS, buffer, length );
  return 1;
#else
#error "Invalid host communication protocol!"
#endif
}
uint16_t ReceiveFromHost( uint8_t * buffer )
{
  #if HOST_COMMUNICATION_PROTOCOL == USART
    return PlatformFunctions.USART.Receive( (USART_Handle_t *)Platform.host_communication_handle, buffer );
  #elif HOST_COMMUNICATION_PROTOCOL == I2C
    PlatformFunction.I2C.Receive( (I2C_Handle_t *)Platform.host_communication_handle, HOST_ADDRESS, data, len );
    return 1;
  #else
  #error "Invalid host communication protocol!"
  #endif
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
