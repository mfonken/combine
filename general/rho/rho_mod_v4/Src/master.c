#include "master.h"

/***************************************************************************************/
/*                                      Core                                           */
/***************************************************************************************/
void master_init( I2C_HandleTypeDef * i2c, TIM_HandleTypeDef * timer, USART_HandleTypeDef * usart )
{
  Camera.init(i2c);
  InitRhoInterface( timer, usart );
}

void master_run( void )
{
  
}
