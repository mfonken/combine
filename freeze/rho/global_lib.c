#include "global_lib.h"

void Set_GPIO_Type( GPIO_TypeDef *GPIOx, uint32_t Mode )
{
  GPIO_InitTypeDef GPIO_InitStruct;
  
  /*Configure GPIO pin : MCLK_Pin */
  GPIO_InitStruct.Pin = MCLK_Pin;
  GPIO_InitStruct.Mode = Mode;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF0_MCO;
  HAL_GPIO_Init(MCLK_GPIO_Port, &GPIO_InitStruct);
}

inline uint32_t Now(void)
{
  return HAL_GetTick();
}