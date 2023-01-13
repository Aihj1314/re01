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
#define OLED_POWER_OFF				HAL_GPIO_WritePin(POWER_OLED_GPIO_Port, POWER_OLED_Pin, GPIO_PIN_RESET);
#define OLED_POWER_ON					HAL_GPIO_WritePin(POWER_OLED_GPIO_Port, POWER_OLED_Pin, GPIO_PIN_SET);
typedef enum {
    OLED_CMD,
    OLED_DATA,
} Typedef_CmdData;

void OLED_Init();

#endif

