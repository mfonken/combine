#include "master.h"

/***************************************************************************************/
/*                                      Core                                           */
/***************************************************************************************/

void Master_Init( I2C_Handle_t * i2c, TIMER_Handle_t * timer, USART_Handle_t * usart, bool run_after_init )
{
  Platform.Init( timer, usart, i2c );
  
#ifdef __OV9712__
  OV9712.Init(i2c);
#endif
  
#ifdef __RHO__
  RhoSystem.Functions.Perform.ConnectToInterface( &Platform );
#endif
  
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
