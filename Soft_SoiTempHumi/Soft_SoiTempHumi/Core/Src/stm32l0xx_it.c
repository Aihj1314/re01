/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32l0xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "main.h"
#include "stm32l0xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
uint8_t Rs485Buff[9];
uint16_t Rs485Len ;
uint8_t Rs485Rxflag;
uint8_t Rs485Send[8] = {0x01,0x03,0x00,0x00,0x00,0x02,0xC4,0x0B};
uint16_t BuffTTem = 0;
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/
extern DMA_HandleTypeDef hdma_adc;
extern ADC_HandleTypeDef hadc;
extern UART_HandleTypeDef hlpuart1;
extern UART_HandleTypeDef huart1;
extern UART_HandleTypeDef huart2;
extern TIM_HandleTypeDef htim22;

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M0+ Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
    while (1)
    {
    }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/******************************************************************************/
/* STM32L0xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32l0xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel 1 interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */
  HAL_DMA_IRQHandler(&hdma_adc);
  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles ADC, COMP1 and COMP2 interrupts (COMP interrupts through EXTI lines 21 and 22).
  */
void ADC1_COMP_IRQHandler(void)
{
  /* USER CODE BEGIN ADC1_COMP_IRQn 0 */

  /* USER CODE END ADC1_COMP_IRQn 0 */
  HAL_ADC_IRQHandler(&hadc);
  /* USER CODE BEGIN ADC1_COMP_IRQn 1 */

  /* USER CODE END ADC1_COMP_IRQn 1 */
}

/**
  * @brief This function handles TIM22 global interrupt.
  */
void TIM22_IRQHandler(void)
{
  /* USER CODE BEGIN TIM22_IRQn 0 */

  /* USER CODE END TIM22_IRQn 0 */
  HAL_TIM_IRQHandler(&htim22);
  /* USER CODE BEGIN TIM22_IRQn 1 */

  /* USER CODE END TIM22_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt / USART1 wake-up interrupt through EXTI line 25.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */
	uint8_t Csar;
	 if(__HAL_UART_GET_FLAG(&huart1,UART_FLAG_RXNE) == SET) {
            //��ȡ���ռĴ���
           Csar = huart1.Instance->RDR;
					 while((USART2->ISR&0X40)==0) {}; //ѭ������,ֱ���������
						USART2->TDR = (uint8_t) Csar;
						 while((USART1->ISR&0X40)==0) {}; //ѭ������,ֱ���������
						USART1->TDR = (uint8_t) Csar;
					//HAL_UART_Transmit(&huart1, &Csar,1,10);
		 	//HAL_UART_Transmit(&huart2, &Csar,1,10);
        }
  /* USER CODE END USART1_IRQn 0 */
  HAL_UART_IRQHandler(&huart1);
  /* USER CODE BEGIN USART1_IRQn 1 */

  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART2 global interrupt / USART2 wake-up interrupt through EXTI line 26.
  */
void USART2_IRQHandler(void)
{
  /* USER CODE BEGIN USART2_IRQn 0 */
	uint8_t Csar;
	 if(__HAL_UART_GET_FLAG(&huart2,UART_FLAG_RXNE) == SET) {
            //��ȡ���ռĴ���
           Csar = huart2.Instance->RDR;
		  while((USART1->ISR&0X40)==0) {}; //ѭ������,ֱ���������
						USART1->TDR = (uint8_t) Csar;
					//HAL_UART_Transmit(&huart2, &Csar,1,10);
					//HAL_UART_Transmit(&huart1, &Csar,1,10);
		// HAL_UART_Transmit(&huart1, (uint8_t*)"2",1,10);
        }
  /* USER CODE END USART2_IRQn 0 */
  HAL_UART_IRQHandler(&huart2);
  /* USER CODE BEGIN USART2_IRQn 1 */

  /* USER CODE END USART2_IRQn 1 */
}

/**
  * @brief This function handles LPUART1 global interrupt / LPUART1 wake-up interrupt through EXTI line 28.
  */
void LPUART1_IRQHandler(void)
{
  /* USER CODE BEGIN LPUART1_IRQn 0 */
    //�жϽ��ձ�־��λ

    if(Rs485Rxflag!=1)
    {
        //Rs485Rxflag = 3;
        if(__HAL_UART_GET_FLAG(&hlpuart1,UART_FLAG_RXNE) == SET) {
            //��ȡ���ռĴ���
            Rs485Buff[Rs485Len++] = hlpuart1.Instance->RDR;
            if(Rs485Len>=9)
            {
                Rs485Len = 0;
                AirTemp = (float)((Rs485Buff[3]<<8)|(Rs485Buff[4])) /10.0f;
                AirHumi = (float)((Rs485Buff[5]<<8)|(Rs485Buff[6])) /10.0f;
                Rs485Rxflag = 0;
            }
        }
    }
  /* USER CODE END LPUART1_IRQn 0 */
  HAL_UART_IRQHandler(&hlpuart1);
  /* USER CODE BEGIN LPUART1_IRQn 1 */

  /* USER CODE END LPUART1_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
