 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� stm322xg_eval_18b20.c
*   ժ Ҫ    �� 
*
*   ��ǰ�汾 �� 
*   �� ��    �� EFLAG
*   ������� �� 
*
*   ȡ���汾 ��
*   ԭ����   �� 
*   �޸����� ��
*   ������� ��2012.08.01 
*
**********************************************************/

/* ͷ�ļ� ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include <stdio.h>

/************************************************************************************
*������:delay_Nus
*����˵��:΢���ʱ
*�������:
*		 Nus����ʱ΢����
*����ֵ:��
*
*
**************************************************************************************/

static void delay_Nus(unsigned int Nus)
{
	SysTick->LOAD=Nus*15;
	SysTick->CTRL|=0x01;
	while(!(SysTick->CTRL&(1<<16)));
	SysTick->CTRL=0x00000000;
	SysTick->VAL=0x00000000;
}
/************************************************************************************
*������:GPIO_DQ_Out_MODULE
*����˵��:���ݹܽ����ģʽ
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/

static void GPIO_DQ_Out_MODULE(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOF, &GPIO_InitStruct);
}
/************************************************************************************
*������:GPIO_DQ_Iput_MODULE
*����˵��:���ݹܽ�����ģʽ
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/

static void GPIO_DQ_Iput_MODULE(void)
{
   	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}
/************************************************************************************
*������: DS18B20_Rst
*����˵��: 18B20��λ���� ��ʱ���й�
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/

static void DS18B20_Rst(void)
{
	GPIO_DQ_Out_MODULE();
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);
	delay_Nus(750);
	GPIO_SetBits(GPIOF,GPIO_Pin_7);
	delay_Nus(15);
} 
/************************************************************************************
*������:DS18B20_Check
*����˵��: 18B20��⺯����
*�������:��
*
*����ֵ:
*		unsigned char: 0,18B20 ����
*					   1,18B20 û�н���
**************************************************************************************/

static unsigned char DS18B20_Check(void)
{
	unsigned char retry=0;
	GPIO_DQ_Iput_MODULE();
	while(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7))
	{
		retry++;
		delay_Nus(1);
	}
	if(retry>=200) return 1;
	else
		retry=0;
	while(!GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7))
	{
		retry++;
		delay_Nus(1);
	}
	if(retry>=240)return 1;
	return 0;
}
/************************************************************************************
*������:DS18B20_Read_Bit
*����˵��:��18B20�ж�ȡһ��λ����
*�������:��
*
*����ֵ:
*		unsigned char: ��ȡλֵ��0����1
*
**************************************************************************************/

static unsigned char DS18B20_Read_Bit(void)
{
	unsigned char data;
	GPIO_DQ_Out_MODULE();
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);
	delay_Nus(2);
	GPIO_SetBits(GPIOF,GPIO_Pin_7);
	GPIO_DQ_Iput_MODULE();
	delay_Nus(12);
	if(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7)) data=1;
	else
		data=0;
	delay_Nus(50);
	return data;
}
/************************************************************************************
*������: DS18B20_Read_Byte
*����˵��: ��18B20��ȡһ���ֽ�����
*�������:��
*
*����ֵ:
*		unsigned char�����ض�ȡ���ֽ�����
*
**************************************************************************************/

static unsigned char DS18B20_Read_Byte(void)
{
	unsigned char i,j,dat;
	dat=0;
	for(i=1;i<=8;i++)
	{
		j=DS18B20_Read_Bit();
		dat=(j<<7)|(dat>>1);
	}
	return dat;
}
/************************************************************************************
*������:DS18B20_Write_Byte
*����˵��:��18B20д��һ���ֽ�����
*�������:
*		  dat:д������
*����ֵ:��
*
*
**************************************************************************************/

static void DS18B20_Write_Byte(unsigned char dat)
{
	unsigned char j;
	unsigned char testb;
	GPIO_DQ_Out_MODULE();
	for(j=1;j<=8;j++)
	{
		testb=dat&0x01;
		dat=dat>>1;
		if(testb)
		{
			GPIO_ResetBits(GPIOF,GPIO_Pin_7);
			delay_Nus(2);
			GPIO_SetBits(GPIOF, GPIO_Pin_7);
			delay_Nus(60);
		}
		else
		{
			GPIO_ResetBits(GPIOF,GPIO_Pin_7);
			delay_Nus(60);
			GPIO_SetBits(GPIOF, GPIO_Pin_7);
			delay_Nus(2);
		}
	}
}
/************************************************************************************
*������:DS18B20_Start
*����˵��:�¶�ת����ʼ����
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/

static void DS18B20_Start(void)
{
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);
	DS18B20_Write_Byte(0x44);
}
/************************************************************************************
*������:Tx_ResetPulse
*����˵��: ���͸�λ�ź�
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/

static void Tx_ResetPulse(void)
{
	GPIO_DQ_Out_MODULE();
	GPIO_ResetBits(GPIOF,GPIO_Pin_7);
	delay_Nus(500);
	GPIO_SetBits(GPIOF,GPIO_Pin_7);
	delay_Nus(1);
}
/************************************************************************************
*������:Rx_PresencePulse
*����˵��: Ӧ���ź�
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/

static void Rx_PresencePulse(void)
{
	GPIO_DQ_Iput_MODULE();
	while(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7));
	while(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_7)==0);
	delay_Nus(300);
	GPIO_DQ_Out_MODULE();
}
/************************************************************************************
*������:DS18B20_Init
*����˵��:
*�������:��
*
*����ֵ:
*		unsigned char : ����18B20����� 0����ʼ���ɹ���1����ʼ�����ɹ�
*
**************************************************************************************/

unsigned char DS18B20_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	Tx_ResetPulse();
	Rx_PresencePulse();
	DS18B20_Rst();
	return DS18B20_Check();
}
/************************************************************************************
*������: DS18B20_Get_Temp
*����˵��:
*�������:��
*
*����ֵ:
*		short:���ػ�ȡ�����¶�ֵ
*
**************************************************************************************/

short DS18B20_Get_Temp(void)
{
	unsigned char temp;
	unsigned char TL,TH;
	short tem;
	DS18B20_Start();
	DS18B20_Rst();
	DS18B20_Check();
	DS18B20_Write_Byte(0xcc);
	DS18B20_Write_Byte(0xbe);
	TL=DS18B20_Read_Byte();
	TH=DS18B20_Read_Byte();
	if(TH>7)
	{
		TH=~TH;
		TL=~TL;
		temp=0;
	}
	else
		temp=1;
	tem=TH;
	tem<<=8;
	tem+=TL;
	tem=(float)tem*0.0625;
	if(temp)return tem;
	else
		return -tem;
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
