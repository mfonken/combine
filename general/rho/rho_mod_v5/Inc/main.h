/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CPI0_Pin GPIO_PIN_0
#define CPI0_GPIO_Port GPIOA
#define CPI1_Pin GPIO_PIN_1
#define CPI1_GPIO_Port GPIOA
#define CPI2_Pin GPIO_PIN_2
#define CPI2_GPIO_Port GPIOA
#define CPI3_Pin GPIO_PIN_3
#define CPI3_GPIO_Port GPIOA
#define CPI4_Pin GPIO_PIN_4
#define CPI4_GPIO_Port GPIOA
#define CPI5_Pin GPIO_PIN_5
#define CPI5_GPIO_Port GPIOA
#define CPI6_Pin GPIO_PIN_6
#define CPI6_GPIO_Port GPIOA
#define CPI7_Pin GPIO_PIN_7
#define CPI7_GPIO_Port GPIOA
#define VSYNC_Pin GPIO_PIN_0
#define VSYNC_GPIO_Port GPIOB
#define VSYNC_EXTI_IRQn EXTI0_IRQn
#define MCLK_Pin GPIO_PIN_8
#define MCLK_GPIO_Port GPIOA
#define SIOC_Pin GPIO_PIN_9
#define SIOC_GPIO_Port GPIOA
#define SIOD_Pin GPIO_PIN_10
#define SIOD_GPIO_Port GPIOA
#define PWDN_Pin GPIO_PIN_11
#define PWDN_GPIO_Port GPIOA
#define EN_1V5_Pin GPIO_PIN_12
#define EN_1V5_GPIO_Port GPIOA
#define CAM_EN_Pin GPIO_PIN_15
#define CAM_EN_GPIO_Port GPIOA
#define PCLK_Pin GPIO_PIN_3
#define PCLK_GPIO_Port GPIOB
#define HREF_Pin GPIO_PIN_4
#define HREF_GPIO_Port GPIOB
#define HREF_EXTI_IRQn EXTI4_IRQn
#define UCK_Pin GPIO_PIN_5
#define UCK_GPIO_Port GPIOB
#define UTX_Pin GPIO_PIN_6
#define UTX_GPIO_Port GPIOB
#define URX_Pin GPIO_PIN_7
#define URX_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
