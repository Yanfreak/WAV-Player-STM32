  /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： stm322xg_eval_touch.c
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
/***头文件****************************************/
#include "stm32f2xx.h"
#include "stm32_eval.h"
#include <stdio.h>
/*****宏定义**************************************/
#define TP_CS()  GPIO_ResetBits(GPIOC,GPIO_Pin_13) /*片选*/
#define TP_DCS() GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define WaitTPReady() while(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_11)) /*准备好信号*/
/************************************************************************************
*函数名:Touch_SPI_Configuration
*函数说明:触摸屏SPI管脚及SPI配置
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
void Touch_SPI_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;

	/*使能时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

    /*配置SPI管脚*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   //复用推挽输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1); 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1); 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1); 
	/*片选管脚配置*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 	//推挽输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;     //上拉输入
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOF,&GPIO_InitStructure);

	/* 中断管脚配置*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOE,&GPIO_InitStructure);

	/*SPI配置*/
	SPI_I2S_DeInit(SPI1);
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1,&SPI_InitStructure);

	SPI_Cmd(SPI1,ENABLE);
}
/************************************************************************************
*函数名:SPI_WriteByte
*函数说明:通过SPI总线发送一个字节数据
*输入参数:
*		data：发送数据
*返回值:
*	   接收到数据
*
**************************************************************************************/
static unsigned char SPI_WriteByte(unsigned char data)
{
	unsigned char Data = 0;
	/*等待发送寄存器为空*/
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
	/*发送数据*/
	SPI_I2S_SendData(SPI1,data);
	/*等待接收寄存器为非空*/
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
	/*接收数据*/
	Data = SPI_I2S_ReceiveData(SPI1);
	
	return Data;
}
/************************************************************************************
*函数名: SpiDelay
*函数说明:延时函数
*输入参数:
*		  DelayCnt：延时长度
*返回值:无
*
*
**************************************************************************************/
static void SpiDelay(unsigned int DelayCnt)
{
	unsigned int i;
	for(i=0;i<DelayCnt;i++);
}
/************************************************************************************
*函数名:TPReadX
*函数说明:读取触摸屏输入点的X坐标AD转换值
*输入参数:无
*
*返回值:
*		X坐标转换值
*
**************************************************************************************/
static u16 TPReadX(void)
{
	u16 x=0;
	TP_CS();
	SpiDelay(10000);
	SPI_WriteByte(0x90);
	SpiDelay(10000);
	x=SPI_WriteByte(0x00);
	x<<=8;
	SpiDelay(10000);
	x+=SPI_WriteByte(0x00);
	SpiDelay(10000);
	TP_DCS();
	SpiDelay(10000);
	x = x>>3;
	return (x);
}
/************************************************************************************
*函数名:TPReadY
*函数说明:读取触摸屏输入点的Y坐标AD转换值
*输入参数:无
*
*返回值:
*		Y坐标转换值
*
**************************************************************************************/
static u16 TPReadY(void)
{
	u16 y=0;
	
	TP_CS();
	SpiDelay(10000);
	SPI_WriteByte(0xD0);
	SpiDelay(10000);
	y=SPI_WriteByte(0x00);
	y<<=8;
	SpiDelay(10000);
	y+=SPI_WriteByte(0x00);
	SpiDelay(10000);
	TP_DCS();
	SpiDelay(10000);
	y = y>>3;
	return (y);
}
/************************************************************************************
*函数名:TOUCH_X_Measure
*函数说明:对读取到的X坐标值滤波运算
*输入参数:无
*
*返回值:
*		滤波后的X坐标AD转换值
*
**************************************************************************************/
int TOUCH_X_Measure(void)
{
	unsigned char t=0,t1,count=0;
	unsigned short int databuffer[10]={5,7,9,3,2,6,4,0,3,1};//数据组
	unsigned short temp=0,X=0;

	while(count<10)//循环读数10次
	{
		databuffer[count]=TPReadX();
		count++;
	}
	if(count==10)//一定要读到10次数据,否则丢弃
	{
	    do//将数据X升序排列
		{
			t1=0;
			for(t=0;t<count-1;t++)
			{
				if(databuffer[t]>databuffer[t+1])//升序排列
				{
					temp=databuffer[t+1];
					databuffer[t+1]=databuffer[t];
					databuffer[t]=temp;
					t1=1;
				}
			}
		}while(t1);
		X=(databuffer[3]+databuffer[4]+databuffer[5])/3;
	}
	return(X);
}
/************************************************************************************
*函数名:TOUCH_Y_Measure
*函数说明:对读取到的Y坐标值滤波运算
*输入参数:无
*
*返回值:
*		滤波后的Y坐标AD转换值
*
**************************************************************************************/
 int TOUCH_Y_Measure(void)
 {
  	unsigned char t=0,t1,count=0;
	unsigned short int databuffer[10]={5,7,9,3,2,6,4,0,3,1};//数据组
	unsigned short temp=0,Y=0;

    while(count<10)	//循环读数10次
	{
		databuffer[count]=TPReadY();
		count++;
	}
	if(count==10)//一定要读到10次数据,否则丢弃
	{
	    do//将数据X升序排列
		{
			t1=0;
			for(t=0;t<count-1;t++)
			{
				if(databuffer[t]>databuffer[t+1])//升序排列
				{
					temp=databuffer[t+1];
					databuffer[t+1]=databuffer[t];
					databuffer[t]=temp;
					t1=1;
				}
			}
		}while(t1);
		Y=(databuffer[3]+databuffer[4]+databuffer[5])/3;
	}
	return(Y);
}


/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
