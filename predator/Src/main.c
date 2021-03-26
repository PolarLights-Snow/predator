/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "mcu_api.h"
#include "protocol.h"
#include "HX711.h"
//#include "core_cm3.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
int YY_YL=5;			//定义语音音量


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);

void key_xz()
{
	if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin)==1)					//WiFi配网
		{
			printf("WIFI  RST");
			HAL_Delay(300);
			if (HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin)==1)
			{
				mcu_reset_wifi();			//WIFI配网复位，ap   和  自动   切换
				while(HAL_GPIO_ReadPin(KEY_GPIO_Port, KEY_Pin)==1);
			}
		}
		if (HAL_GPIO_ReadPin(YY_JY_GPIO_Port, YY_JY_Pin)==1)			//mic静音
		{
			printf("YY_JY");
			HAL_Delay(9);
			if (HAL_GPIO_ReadPin(YY_JY_GPIO_Port, YY_JY_Pin)==1)
			{
				while(HAL_GPIO_ReadPin(YY_JY_GPIO_Port, YY_JY_Pin)==1);
			}
		}
		if (HAL_GPIO_ReadPin(YY_DN_GPIO_Port, YY_DN_Pin)==1)			//声音减小
		{
			printf("YY_DN");
			HAL_Delay(9);
			if (HAL_GPIO_ReadPin(YY_DN_GPIO_Port, YY_DN_Pin)==1)
			{
				if(YY_YL>0)	YY_YL--;
				set_speaker_voice(YY_YL);					//设置语音-
				while(HAL_GPIO_ReadPin(YY_DN_GPIO_Port, YY_DN_Pin)==1);
			}
		}
		if (HAL_GPIO_ReadPin(YY_UP_GPIO_Port, YY_UP_Pin)==1)			//声音增大
		{
			printf("YY_UP");
			HAL_Delay(9);
			if (HAL_GPIO_ReadPin(YY_UP_GPIO_Port, YY_UP_Pin)==1)
			{
				if(YY_YL<10)	YY_YL++;
				set_speaker_voice(YY_YL);					//设置语音+
				while(HAL_GPIO_ReadPin(YY_UP_GPIO_Port, YY_UP_Pin)==1);
			}
		}
		if (HAL_GPIO_ReadPin(WS_GPIO_Port, WS_Pin)==1)						//快速喂食
		{
			printf("WS_K");
			HAL_Delay(9);
			if (HAL_GPIO_ReadPin(WS_GPIO_Port, WS_Pin)==1)
			{
					
				mcu_dp_enum_update(DPID_FEED_STATE,1); //当前喂食状态  枚举型数据上报;		0:准备     1：喂食			2：完成
					HAL_GPIO_WritePin(DJ_GPIO_Port,DJ_Pin,0);
					HAL_Delay(300);
					HAL_GPIO_WritePin(DJ_GPIO_Port,DJ_Pin,1);
					mcu_dp_value_update(DPID_MANUAL_FEED,1);
				while(HAL_GPIO_ReadPin(WS_GPIO_Port, WS_Pin)==1);
			}
			mcu_dp_enum_update(DPID_FEED_STATE,2); //当前喂食状态  枚举型数据上报;		0:准备     1：喂食			2：完成
		}

}
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/*	LED:PA1
		KEY:PC13
		
		UART1:与soc通讯				RX:PA10	TX:PA9
		UART2:debug						RX:PA3	TX:PA2
*/
#ifdef __GUNC_				//转移串口   打印
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch,FILE *f)
#endif		/* __GUNC_ */

PUTCHAR_PROTOTYPE {
		HAL_UART_Transmit(&huart2, (uint8_t*)&ch, 1, 0xffff);
		return ch;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
	
	__HAL_UART_ENABLE_IT(&huart1,UART_IT_RXNE);
	
	wifi_protocol_init();		//协议串口初始化函数
	
	HAL_GPIO_WritePin(LED_wifi_GPIO_Port, LED_wifi_Pin, GPIO_PIN_SET);
	

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		wifi_uart_service();		//wifi串口数据处理服务
		
		key_xz();				//按键选择    
		if(Flag_YL_SW !=get_HX711data())
		{
			Flag_YL_SW = get_HX711data();
			mcu_dp_value_update(DPID_SURPLUS_GRAIN,Flag_YL_SW); //当前食物  VALUE型数据上报;
		
		}			
		
		
		get_wifi_status(mcu_get_wifi_work_state());
		

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
