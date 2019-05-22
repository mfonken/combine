#include "master.h"

/***************************************************************************************/
/*                                      Core                                           */
/***************************************************************************************/

void master_init( I2C_Handle_t * i2c, TIM_Handle_t * timer, USART_Handle_t * usart, bool run_after_init )
{
  _CAMERA_.Init(i2c);
  InitRhoInterface( timer, usart );
  
  if( run_after_init )
  {
    Master.Run();
  }
}

void master_run( void )
{
  
}
