 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： stm322xg_eval_18b20.c
*   摘 要    ： 
*
*   当前版本 ： 
*   作 者    ： EFLAG
*   完成日期 ： 
*
*   取代版本 ：
*   原作者   ： 
*   修改内容 ：
*   完成日期 ：2012.08.01 
*
**********************************************************/

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include <stdio.h>

/************************************************************************************
*函数名:delay_Nus
*函数说明:微妙级延时
*输入参数:
*		 Nus：延时微妙数
*返回值:无
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
*函数名:GPIO_DQ_Out_MODULE
*函数说明:数据管脚输出模式
*输入参数:无
*
*返回值:无
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
*函数名:GPIO_DQ_Iput_MODULE
*函数说明:数据管脚输入模式
*输入参数:无
*
*返回值:无
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
*函数名: DS18B20_Rst
*函数说明: 18B20复位函数 与时间有关
*输入参数:无
*
*返回值:无
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
*函数名:DS18B20_Check
*函数说明: 18B20检测函数，
*输入参数:无
*
*返回值:
*		unsigned char: 0,18B20 接入
*					   1,18B20 没有接入
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
*函数名:DS18B20_Read_Bit
*函数说明:从18B20中读取一个位数据
*输入参数:无
*
*返回值:
*		unsigned char: 读取位值，0或者1
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
*函数名: DS18B20_Read_Byte
*函数说明: 从18B20读取一个字节数据
*输入参数:无
*
*返回值:
*		unsigned char：返回读取的字节数据
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
*函数名:DS18B20_Write_Byte
*函数说明:向18B20写入一个字节数据
*输入参数:
*		  dat:写入数据
*返回值:无
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
*函数名:DS18B20_Start
*函数说明:温度转换起始函数
*输入参数:无
*
*返回值:无
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
*函数名:Tx_ResetPulse
*函数说明: 发送复位信号
*输入参数:无
*
*返回值:无
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
*函数名:Rx_PresencePulse
*函数说明: 应答信号
*输入参数:无
*
*返回值:无
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
*函数名:DS18B20_Init
*函数说明:
*输入参数:无
*
*返回值:
*		unsigned char : 返回18B20检测结果 0，初始化成功，1，初始化不成功
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
*函数名: DS18B20_Get_Temp
*函数说明:
*输入参数:无
*
*返回值:
*		short:返回获取到的温度值
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
