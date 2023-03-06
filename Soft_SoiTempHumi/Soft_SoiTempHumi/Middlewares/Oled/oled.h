/************************************* Copyright(C)******************************
**
**                 (C) Copyright 2022, @mayuxin,China, GCU.
**                            All Rights Reserved
**                     				By( @myx)
**-------------------------------------------------------------------------------
** @FileName   : oled.h
** @Brief	     :
**
**-------------------------------------------------------------------------------
** @Author     : @mayuxin
** @Version    : v1.0
** @Date       : 2023-01-13
**-------------------------------------------------------------------------------
** @Mender	   : Node
** @Version    : Node
** @Date       : Node
** @Brief      : Node
********************************************************************************/

#ifndef __OLED_H__
#define	__OLED_H__
#include "main.h"

#define  OLED_ADDRESS  0x78

#define OLED_POWER_OFF				HAL_GPIO_WritePin(POWER_OLED_GPIO_Port, POWER_OLED_Pin, GPIO_PIN_RESET);
#define OLED_POWER_ON					HAL_GPIO_WritePin(POWER_OLED_GPIO_Port, POWER_OLED_Pin, GPIO_PIN_RESET);

#define OLED_SCL_Clr() 			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);//SCL
#define OLED_SCL_Set() 			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);

#define OLED_SDA_Clr()			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);//SDA
#define OLED_SDA_Set()			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);

typedef enum {
    OLED_CMD,
    OLED_DATA,
} Typedef_CmdData;

typedef enum
{
		OLED_SHOW_NEGATION,
    OLED_SHOW_NOR, // Õý³£ÏÔÊ¾   
} OledShowTypeDef_e;
void OLED_Clear(void);
void OLED_Init(void);
void OLED_ShowString(uint8_t x,uint8_t y,const char *chr,uint8_t size1,uint8_t mode);
void OLED_Showstring_CH(uint8_t usX, uint8_t usY,  const uint8_t* pStr ,uint8_t mode);
void OLED_Refresh(void );

#endif

