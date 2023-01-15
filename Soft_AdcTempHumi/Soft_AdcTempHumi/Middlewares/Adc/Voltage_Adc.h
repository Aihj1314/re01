#ifndef __VOLTAGE_ADC_H__
#define	__VOLTAGE_ADC_H__
#include "main.h"

typedef struct{
	uint16_t Adc_Value;
	float Voltage;
}VoltV_Typedef;

VoltV_Typedef Adc_Buff1[2];

#endif
