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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stm32f1xx_it.h"
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
uint32_t BsmIOStatus(uint8_t checkNum);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RF_C5_Pin GPIO_PIN_6
#define RF_C5_GPIO_Port GPIOA
#define RF_C6_Pin GPIO_PIN_7
#define RF_C6_GPIO_Port GPIOA
#define RF_C7_Pin GPIO_PIN_0
#define RF_C7_GPIO_Port GPIOB
#define RF_C8_Pin GPIO_PIN_1
#define RF_C8_GPIO_Port GPIOB
#define RF_C1_Pin GPIO_PIN_6
#define RF_C1_GPIO_Port GPIOC
#define RF_C2_Pin GPIO_PIN_7
#define RF_C2_GPIO_Port GPIOC
#define RF_C3_Pin GPIO_PIN_8
#define RF_C3_GPIO_Port GPIOC
#define RF_C4_Pin GPIO_PIN_9
#define RF_C4_GPIO_Port GPIOC

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
