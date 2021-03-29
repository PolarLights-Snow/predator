/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "mcu_api.h"
#include "protocol.h"
#include "usart.h"
#include "HX711.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
void ws(char p);
extern RTC_HandleTypeDef hrtc;
extern RTC_DateTypeDef DateToUpdate;
extern RTC_TimeTypeDef sTime;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define CZ_IN_Pin GPIO_PIN_0
#define CZ_IN_GPIO_Port GPIOA
#define KEY_Pin GPIO_PIN_1
#define KEY_GPIO_Port GPIOA
#define HX711_DOUT_Pin GPIO_PIN_14
#define HX711_DOUT_GPIO_Port GPIOB
#define HX711_SCK_Pin GPIO_PIN_15
#define HX711_SCK_GPIO_Port GPIOB
#define LED_wifi_Pin GPIO_PIN_8
#define LED_wifi_GPIO_Port GPIOA
#define YY_DN_Pin GPIO_PIN_11
#define YY_DN_GPIO_Port GPIOA
#define YY_UP_Pin GPIO_PIN_12
#define YY_UP_GPIO_Port GPIOA
#define DJ_Pin GPIO_PIN_3
#define DJ_GPIO_Port GPIOB
#define YY_JY_Pin GPIO_PIN_4
#define YY_JY_GPIO_Port GPIOB
#define WS_Pin GPIO_PIN_5
#define WS_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
