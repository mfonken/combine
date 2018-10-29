#ifndef global_lib
#define global_lib

#include "stm32l4xx_hal.h"

void Set_GPIO_Type( GPIO_TypeDef *GPIOx, uint32_t Mode );
uint32_t Now(void);

#endif