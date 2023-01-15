#include "ds18b20.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F407������
//DS18B20��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2019/9/19
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	
void Hal_DS18B20_IO_OUT()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DS18B20_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	DS18B20_DQ_OUT_SET;
	HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);
}

void Hal_DS18B20_IO_INT()
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	GPIO_InitStruct.Pin = DS18B20_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(DS18B20_GPIO_Port, &GPIO_InitStruct);
}
void delay_us(uint32_t nus)
{		
	uint32_t ticks;
	uint32_t told,tnow,tcnt=0;
	uint32_t reload=SysTick->LOAD;				//LOAD��ֵ	    	 
	ticks=nus*32; 						//��Ҫ�Ľ����� 
	told=SysTick->VAL;        				//�ս���ʱ�ļ�����ֵ
	while(1)
	{
		tnow=SysTick->VAL;	
		if(tnow!=told)
		{	    
			if(tnow<told)tcnt+=told-tnow;	//����ע��һ��SYSTICK��һ���ݼ��ļ������Ϳ�����.
			else tcnt+=reload-tnow+told;	    
			told=tnow;
			if(tcnt>=ticks)break;			//ʱ�䳬��/����Ҫ�ӳٵ�ʱ��,���˳�.
		}  
	};									    
}
//��λDS18B20
void DS18B20_Rst(void)	   
{                 
	DS18B20_IO_OUT();   //����Ϊ���
	DS18B20_DQ_OUT_RESET;  	//����DQ
	delay_us(800);      //����750us
	DS18B20_DQ_OUT_SET;  	//DQ=1 
	//delay_us(15);       //15US
}

//�ȴ�DS18B20�Ļ�Ӧ
//����1:δ��⵽DS18B20�Ĵ���
//����0:����
uint8_t DS18B20_Check(void) 	   
{   
	uint8_t retry=0;
	DS18B20_IO_IN();    //����Ϊ����
    while (DS18B20_DQ_IN&&retry<200)
	{
		retry++;
		delay_us(1);
		//printf("DS18B20_DQ_IN1 = %d\n",DS18B20_DQ_IN);
	};	
//printf("retry1 = %d\n",retry);	
	if(retry>=200)return 1;
	else retry=0;
    while (!DS18B20_DQ_IN&&retry<240)
	{
		retry++;
		delay_us(1);
	};
	//printf("retry2 = %d\n",retry);
	if(retry>=240)return 1;	    
	return 0;
}

//��DS18B20��ȡһ��λ
//����ֵ��1/0
uint8_t DS18B20_Read_Bit(void) 
{
	uint8_t data;
	DS18B20_IO_OUT();   //����Ϊ���
	DS18B20_DQ_OUT_RESET; 
	delay_us(2);
	DS18B20_DQ_OUT_SET; 
	DS18B20_IO_IN();    //����Ϊ����
	delay_us(12);
	if(DS18B20_DQ_IN)data=1;
	else data=0;	 
	delay_us(50);           
	return data;
}

//��DS18B20��ȡһ���ֽ�
//����ֵ������������
uint8_t DS18B20_Read_Byte(void)   
{        
	uint8_t i,j,dat;
	dat=0;
	for (i=1;i<=8;i++) 
	{
        j=DS18B20_Read_Bit();
        dat=(j<<7)|(dat>>1);
    }						    
	return dat;
}

//дһ���ֽڵ�DS18B20
//dat��Ҫд����ֽ�
void DS18B20_Write_Byte(uint8_t dat)     
 {             
    uint8_t j;
    uint8_t testb;
    DS18B20_IO_OUT();     //����Ϊ���
    for (j=1;j<=8;j++) 
	{
        testb=dat&0x01;
        dat=dat>>1;
        if(testb)       // д1
        {
            DS18B20_DQ_OUT_RESET;
            delay_us(2);                            
            DS18B20_DQ_OUT_SET;
            delay_us(60);             
        }
        else            //д0
        {
            DS18B20_DQ_OUT_RESET;
            delay_us(60);             
            DS18B20_DQ_OUT_SET;
            delay_us(2);                          
        }
    }
}
 
//��ʼ�¶�ת��
void DS18B20_Start(void)
{   						               
    DS18B20_Rst();	   
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);// skip rom
    DS18B20_Write_Byte(0x44);// convert
}

//��ʼ��DS18B20��IO�� DQ ͬʱ���DS�Ĵ���
//����1:������
//����0:����    	 
uint8_t DS18B20_Init(void)
{
	//DS18B20_IO_OUT(); 
	//DS18B20_DQ_OUT_SET;  	//����DQ
	DS18B20_Rst();	
	return DS18B20_Check();
}

//��ds18b20�õ��¶�ֵ
//���ȣ�0.1C
//����ֵ���¶�ֵ ��-550~1250�� 
short DS18B20_Get_Temp(void)
{
    uint8_t temp;
    uint8_t TL,TH;
    short tem;
		//vPortEndScheduler();
    DS18B20_Start ();           //��ʼת��
    DS18B20_Rst();
    DS18B20_Check();	 
    DS18B20_Write_Byte(0xcc);   // skip rom
    DS18B20_Write_Byte(0xbe);   // convert	    
    TL=DS18B20_Read_Byte();     // LSB   
    TH=DS18B20_Read_Byte();     // MSB   
    if(TH>7)
    {
        TH=~TH;
        TL=~TL; 
        temp=0;//�¶�Ϊ��  
    }else temp=1;//�¶�Ϊ��	  	  
    tem=TH; //��ø߰�λ
    tem<<=8;    
    tem+=TL;//��õװ�λ
    tem=(double)tem*0.625;//ת�� 
		//xPortStartScheduler();	
	if(temp)return tem; //�����¶�ֵ
	else return -tem;    
}
