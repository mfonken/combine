#include "master.h"

/***************************************************************************************/
/*                                      Core                                           */
/***************************************************************************************/

void Master_Init( I2C_Handle_t * i2c, TIMER_Handle_t * timer, USART_Handle_t * usart, bool run_after_init )
{
  SystemFunctions.Init( &System );
  PlatformFunctions.Init( &Platform );

  if( run_after_init )
  {
    Master.Run();
  }
}

void Master_Run( void )
{
#ifdef __RHO__

#endif
}
