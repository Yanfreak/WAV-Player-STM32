  /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� stm322xg_eval_touch.c
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
/***ͷ�ļ�****************************************/
#include "stm32f2xx.h"
#include "stm32_eval.h"
#include <stdio.h>
/*****�궨��**************************************/
#define TP_CS()  GPIO_ResetBits(GPIOC,GPIO_Pin_13) /*Ƭѡ*/
#define TP_DCS() GPIO_SetBits(GPIOC,GPIO_Pin_13)
#define WaitTPReady() while(GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_11)) /*׼�����ź�*/
/************************************************************************************
*������:Touch_SPI_Configuration
*����˵��:������SPI�ܽż�SPI����
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
void Touch_SPI_Configuration(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	SPI_InitTypeDef   SPI_InitStructure;

	/*ʹ��ʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);

    /*����SPI�ܽ�*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;   //�����������
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1); 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1); 
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1); 
	/*Ƭѡ�ܽ�����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 	//�������
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;     //��������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_Init(GPIOF,&GPIO_InitStructure);

	/* �жϹܽ�����*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(GPIOE,&GPIO_InitStructure);

	/*SPI����*/
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
*������:SPI_WriteByte
*����˵��:ͨ��SPI���߷���һ���ֽ�����
*�������:
*		data����������
*����ֵ:
*	   ���յ�����
*
**************************************************************************************/
static unsigned char SPI_WriteByte(unsigned char data)
{
	unsigned char Data = 0;
	/*�ȴ����ͼĴ���Ϊ��*/
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);
	/*��������*/
	SPI_I2S_SendData(SPI1,data);
	/*�ȴ����ռĴ���Ϊ�ǿ�*/
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
	/*��������*/
	Data = SPI_I2S_ReceiveData(SPI1);
	
	return Data;
}
/************************************************************************************
*������: SpiDelay
*����˵��:��ʱ����
*�������:
*		  DelayCnt����ʱ����
*����ֵ:��
*
*
**************************************************************************************/
static void SpiDelay(unsigned int DelayCnt)
{
	unsigned int i;
	for(i=0;i<DelayCnt;i++);
}
/************************************************************************************
*������:TPReadX
*����˵��:��ȡ������������X����ADת��ֵ
*�������:��
*
*����ֵ:
*		X����ת��ֵ
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
*������:TPReadY
*����˵��:��ȡ������������Y����ADת��ֵ
*�������:��
*
*����ֵ:
*		Y����ת��ֵ
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
*������:TOUCH_X_Measure
*����˵��:�Զ�ȡ����X����ֵ�˲�����
*�������:��
*
*����ֵ:
*		�˲����X����ADת��ֵ
*
**************************************************************************************/
int TOUCH_X_Measure(void)
{
	unsigned char t=0,t1,count=0;
	unsigned short int databuffer[10]={5,7,9,3,2,6,4,0,3,1};//������
	unsigned short temp=0,X=0;

	while(count<10)//ѭ������10��
	{
		databuffer[count]=TPReadX();
		count++;
	}
	if(count==10)//һ��Ҫ����10������,������
	{
	    do//������X��������
		{
			t1=0;
			for(t=0;t<count-1;t++)
			{
				if(databuffer[t]>databuffer[t+1])//��������
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
*������:TOUCH_Y_Measure
*����˵��:�Զ�ȡ����Y����ֵ�˲�����
*�������:��
*
*����ֵ:
*		�˲����Y����ADת��ֵ
*
**************************************************************************************/
 int TOUCH_Y_Measure(void)
 {
  	unsigned char t=0,t1,count=0;
	unsigned short int databuffer[10]={5,7,9,3,2,6,4,0,3,1};//������
	unsigned short temp=0,Y=0;

    while(count<10)	//ѭ������10��
	{
		databuffer[count]=TPReadY();
		count++;
	}
	if(count==10)//һ��Ҫ����10������,������
	{
	    do//������X��������
		{
			t1=0;
			for(t=0;t<count-1;t++)
			{
				if(databuffer[t]>databuffer[t+1])//��������
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
