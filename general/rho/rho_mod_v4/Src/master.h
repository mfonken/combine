#ifndef master_h
#define master_h

#include "OV9712.h"
#include "stm32l4xx_hal.h"
#include "stm32l4xx_it.h"

#include "stm32_rho_interface.h"
#include "printers.h"

void master_init( I2C_HandleTypeDef *, TIM_HandleTypeDef *, USART_HandleTypeDef * );
void master_run( void );



#endif
