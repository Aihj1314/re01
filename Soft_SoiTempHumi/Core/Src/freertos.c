/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "oled.h"
#include "adc.h"
#include "usart.h"
#include "stm32l0xx_it.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
__IO uint16_t adcCovValueBuff[10];
uint8_t Kwh_Percen;
uint8_t OLED_ShowSprintBuff[50];
uint8_t Kwh_ThereShold=20;
float AirTemp;
float AirHumi;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
SemaphoreHandle_t OLED_bUFFxMutex;
/* USER CODE END Variables */
osThreadId UIShowTaskHandle;
osThreadId AdcTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void UI_ShowTask(void const * argument);
void Adc_Task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
    *ppxIdleTaskStackBuffer = &xIdleStack[0];
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
    /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
    /* add mutexes, ... */
    OLED_bUFFxMutex = xSemaphoreCreateMutex( );
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
    /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of UIShowTask */
  osThreadDef(UIShowTask, UI_ShowTask, osPriorityBelowNormal, 0, 128);
  UIShowTaskHandle = osThreadCreate(osThread(UIShowTask), NULL);

  /* definition and creation of AdcTask */
  osThreadDef(AdcTask, Adc_Task, osPriorityNormal, 0, 128);
  AdcTaskHandle = osThreadCreate(osThread(AdcTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_UI_ShowTask */
/**
  * @brief  Function implementing the UIShowTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_UI_ShowTask */
void UI_ShowTask(void const * argument)
{
  /* USER CODE BEGIN UI_ShowTask */
    OLED_Init();
    /* Infinite loop */
    for(;;)
    {
        OLED_Clear();
        OLED_Showstring_CH(0,0,(const uint8_t*)"电量",OLED_SHOW_NOR);
        OLED_Showstring_CH(0,20,(const uint8_t*)"土壤温度",OLED_SHOW_NOR);
        OLED_Showstring_CH(0,40,(const uint8_t*)"土壤湿度",OLED_SHOW_NOR);
        sprintf((char*)OLED_ShowSprintBuff,":%d%%",Kwh_Percen);
        OLED_ShowString(32,0,(const char *)OLED_ShowSprintBuff,16,OLED_SHOW_NOR);
			sprintf((char*)OLED_ShowSprintBuff,":%.1f",AirTemp);
            OLED_ShowString(64,20,(const char *)OLED_ShowSprintBuff,16,OLED_SHOW_NOR);
            sprintf((char*)OLED_ShowSprintBuff,":%.1f%%  ",AirHumi);
            OLED_ShowString(64,40,(const char *)OLED_ShowSprintBuff,16,OLED_SHOW_NOR);
        OLED_Refresh();
        osDelay(1);
    }
  /* USER CODE END UI_ShowTask */
}

/* USER CODE BEGIN Header_Adc_Task */
/**
* @brief Function implementing the AdcTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Adc_Task */
void Adc_Task(void const * argument)
{
  /* USER CODE BEGIN Adc_Task */
    uint32_t sum = 0;
    uint32_t Adc_Value = 0;
    HAL_ADC_Start_DMA(&hadc,(uint32_t *)adcCovValueBuff,10);//
		osDelay(500);
    /* Infinite loop */
    for(;;)
    {
        sum = 0;
        for(uint8_t count = 0; count < 10; count++)
        {
            sum += adcCovValueBuff[count];
        }
        Adc_Value = sum/10;
        Kwh_Percen = (3.3 / 4096 * (Adc_Value))*((100+200)/200.0f) / 4.15 * 100;
        if(Kwh_Percen>100)Kwh_Percen=100;
        printf("电压：%fV\n",(3.3 / 4096 * (Adc_Value))*((100+200)/200.0f));
        printf("Kwh_Percen = %d \n",Kwh_Percen);
        printf("Adc_Value = %d\n",	Adc_Value);
			
				Rs485Rxflag = 1;
				Rs485Len = 0;
				HAL_GPIO_WritePin(GPIOB, RS485RE_Pin, GPIO_PIN_SET);
				HAL_UART_Transmit(&hlpuart1,Rs485Send,8,10);
				Rs485Rxflag = 0;				
				HAL_GPIO_WritePin(GPIOB, RS485RE_Pin, GPIO_PIN_RESET);
        osDelay(1000);
    }
  /* USER CODE END Adc_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

