/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
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
#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal.h"

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
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOA
#define SPI1_DC_Pin GPIO_PIN_6
#define SPI1_DC_GPIO_Port GPIOA
#define SPI1_RST_Pin GPIO_PIN_0
#define SPI1_RST_GPIO_Port GPIOB
#define Y0_Pin GPIO_PIN_12
#define Y0_GPIO_Port GPIOB
#define Y1_Pin GPIO_PIN_13
#define Y1_GPIO_Port GPIOB
#define Y2_Pin GPIO_PIN_14
#define Y2_GPIO_Port GPIOB
#define Y3_Pin GPIO_PIN_15
#define Y3_GPIO_Port GPIOB
#define Y4_Pin GPIO_PIN_8
#define Y4_GPIO_Port GPIOA
#define Y5_Pin GPIO_PIN_9
#define Y5_GPIO_Port GPIOA
#define Y6_Pin GPIO_PIN_10
#define Y6_GPIO_Port GPIOA
#define Y7_Pin GPIO_PIN_11
#define Y7_GPIO_Port GPIOA
#define RESET_Pin GPIO_PIN_12
#define RESET_GPIO_Port GPIOA
#define HREF_Pin GPIO_PIN_15
#define HREF_GPIO_Port GPIOA
#define VSYNC_Pin GPIO_PIN_3
#define VSYNC_GPIO_Port GPIOB
#define PCLK_Pin GPIO_PIN_4
#define PCLK_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
