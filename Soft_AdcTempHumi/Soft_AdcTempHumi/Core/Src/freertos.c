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
#include "voltage_adc.h"
#include "stdio.h"
#include "oled.h"
#include "adc.h"
#include "string.h"
#include "ds18b20.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
__IO uint16_t adcCovValueBuff[10][2] = {0};	//存放ADC的值 6通道  每个通道存放30个值，由DMA循环写入
float SoilHumidity;
uint8_t Kwh_Percen;
float Temp_Value;
uint8_t OLED_ShowSprintBuff[50];
uint8_t Ui_ShowIndex;
uint8_t Ui_Set_ThereIndex;
uint8_t Kwh_ThereShold=20;
uint8_t Ui_OK_OFfLAG;
float Temp_ThereSholdValue = 38.0;
float Soi_TherelHumidity = 50.0;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
SemaphoreHandle_t OLED_bUFFxMutex;
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
osThreadId OLEDTaskHandle;
osThreadId AdcTaskHandle;
osThreadId KeyTaskHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
uint16_t adc_value[10];
/* USER CODE END FunctionPrototypes */

void Oled_ShowTask(void const * argument);
void Adc_Task(void const * argument);
void Key_Task(void const * argument);

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
    OLED_bUFFxMutex = xSemaphoreCreateMutex( );
    /* add mutexes, ... */
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
    /* definition and creation of OLEDTask */
    osThreadDef(OLEDTask, Oled_ShowTask, osPriorityBelowNormal, 0, 128);
    OLEDTaskHandle = osThreadCreate(osThread(OLEDTask), NULL);

    /* definition and creation of AdcTask */
    osThreadDef(AdcTask, Adc_Task, osPriorityNormal, 0, 128);
    AdcTaskHandle = osThreadCreate(osThread(AdcTask), NULL);

    /* definition and creation of KeyTask */
    osThreadDef(KeyTask, Key_Task, osPriorityBelowNormal, 0, 128);
    KeyTaskHandle = osThreadCreate(osThread(KeyTask), NULL);

    /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
    /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_Oled_ShowTask */
/**
  * @brief  Function implementing the OLEDTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Oled_ShowTask */
void Oled_ShowTask(void const * argument)
{
    /* USER CODE BEGIN Oled_ShowTask */
    memset(OLED_ShowSprintBuff, '\0', sizeof(OLED_ShowSprintBuff));
    OLED_Init();

    //OLED_Refresh();
    /* Infinite loop */
    for(;;)
    {
        //printf("THIS IS OK\n");
        if(!Ui_ShowIndex)
        {
            OLED_Clear();
            OLED_Showstring_CH(0,0,(const uint8_t*)"电量",OLED_SHOW_NOR);
            OLED_Showstring_CH(0,20,(const uint8_t*)"空气温度",OLED_SHOW_NOR);
            OLED_Showstring_CH(0,40,(const uint8_t*)"土壤湿度",OLED_SHOW_NOR);
            sprintf((char*)OLED_ShowSprintBuff,":%d%%",Kwh_Percen);
            OLED_ShowString(32,0,(const char *)OLED_ShowSprintBuff,16,OLED_SHOW_NOR);
            sprintf((char*)OLED_ShowSprintBuff,":%.1f",Temp_Value);
            OLED_ShowString(64,20,(const char *)OLED_ShowSprintBuff,16,OLED_SHOW_NOR);
            sprintf((char*)OLED_ShowSprintBuff,":%.1f%%  ",SoilHumidity);
            OLED_ShowString(64,40,(const char *)OLED_ShowSprintBuff,16,OLED_SHOW_NOR);
            OLED_Refresh();
        }
        else
        {
            OLED_Clear();
            if(Ui_OK_OFfLAG)
                OLED_ShowString(120,Ui_Set_ThereIndex*20,"<",16,OLED_SHOW_NOR);
            else
                OLED_ShowString(120,Ui_Set_ThereIndex*20,">",16,OLED_SHOW_NOR);
            OLED_Showstring_CH(0,0,(const uint8_t*)"电量阈值",OLED_SHOW_NOR);
            OLED_Showstring_CH(0,20,(const uint8_t*)"温度阈值",OLED_SHOW_NOR);
            OLED_Showstring_CH(0,40,(const uint8_t*)"湿度阈值",OLED_SHOW_NOR);
            sprintf((char*)OLED_ShowSprintBuff,":%d%%",Kwh_ThereShold);
            OLED_ShowString(64,0,(const char *)OLED_ShowSprintBuff,16,OLED_SHOW_NOR);
            sprintf((char*)OLED_ShowSprintBuff,":%.1f",Temp_ThereSholdValue);
            OLED_ShowString(64,20,(const char *)OLED_ShowSprintBuff,16,OLED_SHOW_NOR);
            sprintf((char*)OLED_ShowSprintBuff,":%.1f%%",Soi_TherelHumidity);
            OLED_ShowString(64,40,(const char *)OLED_ShowSprintBuff,16,OLED_SHOW_NOR);
            OLED_Refresh();
        }

        if((Temp_Value>Temp_ThereSholdValue)||(SoilHumidity<Soi_TherelHumidity))
        {
					//printf("Temp_Value = %f SoilHumidity = %f\n",Temp_Value,SoilHumidity);
            HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET);
        }
        else
        {
					//printf("OK\n");
            HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET);
        }
        if(Kwh_Percen<Kwh_ThereShold)
        {
					HAL_GPIO_WritePin(GPIOB, VDD_ON_Pin, GPIO_PIN_RESET);
					osDelay(1000);
        }
        //osDelay(1000);

    }
    /* USER CODE END Oled_ShowTask */
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
    uint16_t Adc_Value[2] ;
    short temperature;

    while(DS18B20_Init())	        //DS18B20初始化
    {

        osDelay(200);
    }

    osDelay(100);
    HAL_ADC_Start_DMA(&hadc,(uint32_t *)adcCovValueBuff,20);
    /* Infinite loop */
    for(;;)
    {
        temperature=DS18B20_Get_Temp();
        if((temperature<1000)&&(temperature>-1000))
        {
            Temp_Value = temperature/10.0f;
        }
        printf("\nTemp_Value = %f\n\n",(float)Temp_Value);

        for(uint8_t i = 0; i < 2; i++)//复制dma 的数据
        {
            sum = 0;
            for(uint8_t count = 0; count < 10; count++)
            {
                sum += adcCovValueBuff[count][i];
            }
            Adc_Value[i] = sum/10;
            if(i==1)
            {
                SoilHumidity = (float)((4096-Adc_Value[i])/(4096.0f-2000))*100;
                printf("SoilHumidity = %f\n",SoilHumidity);
                if(SoilHumidity>100)SoilHumidity=100;
                if(SoilHumidity<40) SoilHumidity = 40;
            }
            else
            {
                Kwh_Percen = (3.3 / 4096 * (Adc_Value[i]+64))*((100+200)/200.0f) / 4.1 * 100;
                if(Kwh_Percen>100)Kwh_Percen=100;
                printf("Kwh_Percen = %d \n",Kwh_Percen);
            }
            printf("Adc_Value[%d] = %d\n",i,	Adc_Value[i]);
        }
        osDelay(1000);
    }
    /* USER CODE END Adc_Task */
}

/* USER CODE BEGIN Header_Key_Task */
/**
* @brief Function implementing the KeyTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Key_Task */
void Key_Task(void const * argument)
{
    /* USER CODE BEGIN Key_Task */
    uint16_t Timerout;
    osDelay(2000);
    /* Infinite loop */
    for(;;)
    {
        if(HAL_GPIO_ReadPin(KEY_OK_GPIO_Port, KEY_OK_Pin) ==RESET)
        {
            osDelay(10);
            if(HAL_GPIO_ReadPin(KEY_OK_GPIO_Port, KEY_OK_Pin) ==RESET)
            {
                if(Ui_ShowIndex )
                {
                    if(Ui_OK_OFfLAG == 0)
                        Ui_OK_OFfLAG = 1;
                    else Ui_OK_OFfLAG = 0;
                }
            }
            while(HAL_GPIO_ReadPin(KEY_OK_GPIO_Port, KEY_OK_Pin) ==RESET);
        }
        if(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) ==RESET)
        {
            osDelay(10);
            if(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) ==RESET)
            {

                if(Ui_OK_OFfLAG)
                {
                    switch(Ui_Set_ThereIndex)
                    {
                    case 0:
                        Kwh_ThereShold++;
                        if(Kwh_ThereShold>100)Kwh_ThereShold = 100;
                        break;

                    case 1:
                        Temp_ThereSholdValue++;
                        if(Temp_ThereSholdValue>100)Temp_ThereSholdValue=100;
                        break;
                    case 2:
                        Soi_TherelHumidity++;
                        if(Soi_TherelHumidity>100)Soi_TherelHumidity=100;
                        break;
                    }
                }
                else
                {
                    Ui_Set_ThereIndex++;
                    if(Ui_Set_ThereIndex>2)Ui_Set_ThereIndex=0;
                }
            }
            while(HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin) ==RESET);
        }
        if(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) ==RESET)
        {
            osDelay(10);
            if(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) ==RESET)
            {
                if(Ui_OK_OFfLAG)
                {
                    switch(Ui_Set_ThereIndex)
                    {
                    case 0:
                        Kwh_ThereShold--;
                        if(Kwh_ThereShold>100)Kwh_ThereShold = 0;
                        break;

                    case 1:
                        Temp_ThereSholdValue--;
                        if(Temp_ThereSholdValue>100)Temp_ThereSholdValue=0;
                        break;
                    case 2:
                        Soi_TherelHumidity--;
                        if(Soi_TherelHumidity>100)Soi_TherelHumidity=0;
                        break;
                    }
                }
                else
                {
                    Ui_Set_ThereIndex--;
                    if(Ui_Set_ThereIndex>2)Ui_Set_ThereIndex=2;
                }

            }
            while(HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin) ==RESET);
        }
        if(HAL_GPIO_ReadPin(POWER_JC_GPIO_Port, POWER_JC_Pin) ==RESET)
        {
            //osDelay(10);
            while(HAL_GPIO_ReadPin(POWER_JC_GPIO_Port, POWER_JC_Pin) ==RESET)
            {
                if(Timerout++ > 300)
                {
                    HAL_GPIO_WritePin(GPIOB, VDD_ON_Pin, GPIO_PIN_RESET);
                    osDelay(1000);
                }
                osDelay(10);
            }
            if(Timerout>1)
            {
                if(!Ui_ShowIndex)Ui_ShowIndex=1;
                else
                {
                    Ui_ShowIndex = 0;
                    Ui_Set_ThereIndex = 0;
                    Ui_OK_OFfLAG = 0;
                }

            }
            Timerout = 0;
        }
        //osDelay(10);
    }
    /* USER CODE END Key_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

