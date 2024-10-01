/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f7xx_hal.h"

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
#define LED_PE2_Pin GPIO_PIN_2
#define LED_PE2_GPIO_Port GPIOE
#define AIRPlus_Control_Pin GPIO_PIN_1
#define AIRPlus_Control_GPIO_Port GPIOC
#define AIRMinus_Control_Pin GPIO_PIN_0
#define AIRMinus_Control_GPIO_Port GPIOA
#define Precharge_Control_Pin GPIO_PIN_3
#define Precharge_Control_GPIO_Port GPIOA
#define SDC_End_Monitoring_Pin GPIO_PIN_4
#define SDC_End_Monitoring_GPIO_Port GPIOA
#define IMD_Monitoring_Pin GPIO_PIN_10
#define IMD_Monitoring_GPIO_Port GPIOA
#define LTC6820_CS_Pin GPIO_PIN_6
#define LTC6820_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
