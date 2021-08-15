#ifndef PLATFORM_CONFIG_H_
#define PLATFORM_CONFIG_H_

#include "main.h"

#define _PLATFORM_ STM

#define GPIO_Port_t     GPIO_TypeDef
#define TIMER_Handle_t  TIM_HandleTypeDef
#define I2C_Handle_t    I2C_HandleTypeDef
#define SPI_Handle_t    SPI_HandleTypeDef
#define UART_Handle_t   UART_HandleTypeDef
#define USB_Handle_t    PCD_HandleTypeDef

#include "global_config.h"

#endif /* PLATFORM_CONFIG_H_ */
