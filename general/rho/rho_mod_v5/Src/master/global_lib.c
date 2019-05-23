#include "global_lib.h"

//typedef struct
//{
//  
//} global_lib_functions;

#ifdef __RHO__
static void SetActiveClientFlags( rho_system_flags_variables * Flags )
{
    ActiveFlags = Flags;
}
static void InitRhoInterface(TIM_HandleTypeDef * timer, USART_HandleTypeDef * usart )
{
  Utilities.Timer = timer;
  Utilities.Usart = usart;
  
  RhoSystem.Functions.Perform.ConnectToInterface( &PlatformInterface );
}
#endif

static inline uint32_t Timestamp( void )
{
  return PlatformInterface.Time.Now();
}