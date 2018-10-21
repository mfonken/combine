#include "master.h"

/*
#include "master.h"
master_init( &hi2c1, &htim2, &husart1 );
*/

/***************************************************************************************/
/*                                      Core                                           */
/***************************************************************************************/
void master_init( I2C_HandleTypeDef * i2c, TIM_HandleTypeDef * timer, USART_HandleTypeDef * usart )
{
  InitRhoInterface( timer, usart );
  
}

void master_run( void )
{
  
}
