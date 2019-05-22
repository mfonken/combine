#include "global_lib.h"

static void SetActiveClientFlags( rho_system_flags_variables * Flags )
{
    ActiveFlags = Flags;
}

typedef struct
{
  
} global_lib_functions;

static void InitRhoInterface(TIM_HandleTypeDef * timer, USART_HandleTypeDef * usart )
{
  Utilities.Timer = timer;
  Utilities.Usart = usart;
  
  RhoSystem.Functions.Perform.ConnectToInterface( &PlatformInterface );
}
