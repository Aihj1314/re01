/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32l0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FreeRTOS.h"
#include "task.h"
#include "cmsis_os.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
extern SemaphoreHandle_t OLED_bUFFxMutex;
extern float AirTemp;
extern float AirHumi;
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
#define POWER_JC_Pin GPIO_PIN_0
#define POWER_JC_GPIO_Port GPIOB
#define VDD_ON_Pin GPIO_PIN_1
#define VDD_ON_GPIO_Port GPIOB
#define POWER_5V_Pin GPIO_PIN_2
#define POWER_5V_GPIO_Port GPIOB
#define BEEP_Pin GPIO_PIN_12
#define BEEP_GPIO_Port GPIOB
#define KEY_OK_Pin GPIO_PIN_13
#define KEY_OK_GPIO_Port GPIOB
#define KEY_DOWN_Pin GPIO_PIN_14
#define KEY_DOWN_GPIO_Port GPIOB
#define KEY_UP_Pin GPIO_PIN_15
#define KEY_UP_GPIO_Port GPIOB
#define SJON_Pin GPIO_PIN_11
#define SJON_GPIO_Port GPIOA
#define POWER_OLED_Pin GPIO_PIN_3
#define POWER_OLED_GPIO_Port GPIOB
#define POWER_4853_3_Pin GPIO_PIN_4
#define POWER_4853_3_GPIO_Port GPIOB
#define POWERKEY_4G_Pin GPIO_PIN_5
#define POWERKEY_4G_GPIO_Port GPIOB
#define RS485RE_Pin GPIO_PIN_9
#define RS485RE_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
