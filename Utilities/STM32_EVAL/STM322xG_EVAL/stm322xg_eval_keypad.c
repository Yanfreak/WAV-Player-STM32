 /****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� stm322xg_eval_keypad.c
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
******************************************************************************/
/*************ͷ�ļ�**********************************************************/
#include "stm32f2xx.h"
#include "stm322xg_eval_keypad.h"
#include <stdio.h>
 u8 KeyCode;

/******************************************************************************
* ��������GPIO_Configuration
* ��  �ܣ�GPIO���ã���©���
* ��  �룺�ӳ�ʱ��
* ��  ������  
* ��  ע����
********************************************************************************/
static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 	
}

/******************************************************************************
* ��������GPIO_SDA_Out_MODULE
* ��  �ܣ�����DQ����Ϊ��©���ģʽ
* ��  �룺��
* ��  ������  
* ��  ע����
********************************************************************************/
static void GPIO_SDA_Out_MODULE(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/******************************************************************************
* ��������GPIO_SDA_Iput_MODULE
* ��  �ܣ�����DQ����Ϊ��������ģʽ
* ��  �룺��
* ��  ������  
* ��  ע����
********************************************************************************/
static void GPIO_SDA_Iput_MODULE(void)
{
   	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/******************************************************************************
* ��������Delay
* ��  �ܣ��ӳٺ���
* ��  �룺�ӳ�ʱ��
* ��  ������  
* ��  ע����
********************************************************************************/
static void Delay(u32 nTime)			
{
	int i;
	for(i=0;i<nTime*200;i++);
}

/******************************************************************************
* ��������I2C_Start
* ��  �ܣ�IOģ��IIC��ʼ�ź�
* ��  �룺��
* ��  ������  
* ��  ע����
********************************************************************************/
static void I2C_Start(void)
{
	SDA_SET;
	SCL_SET;
	Delay(1);
	SDA_CLR; /* ʱ��Ϊ�ߣ����������ߵ�ƽ */
	Delay(1);
	SCL_CLR; /* ǯסI2C���ߣ�׼�����ջ������� */
	Delay(1);
}

/******************************************************************************
* ��������I2C_Stop
* ��  �ܣ�IOģ��IIC��ֹ�ź�
* ��  �룺��
* ��  ������  
* ��  ע����
********************************************************************************/
static void I2C_Stop(void)
{
	SDA_CLR; /* ʱ��Ϊ�ߣ����������ߵ�ƽ */
	SCL_CLR;										  
	Delay(1);
	SCL_SET;
	Delay(1);
	SDA_SET; /* ����I2C���߽����ź� */
	Delay(1);
}

/******************************************************************************
* ��������WriteI2CByte
* ��  �ܣ�IOģ��IICд8λ����
* ��  �룺8λ����
* ��  ������  
* ��  ע����
********************************************************************************/
static void WriteI2CByte(unsigned char b)
{
    /*��I2C����дһ���ֽ�*/
	char i;
	for(i=0;i<8;i++)
	{
		if((b<<i)&0x80)
		{	
		   	SDA_SET;
			Delay(1);
			SCL_CLR;
			Delay(1);
			SCL_SET;
			Delay(1);
			SCL_CLR;
			Delay(1);
		}
		else
		{
			/*����0����SCLΪ�ߵ�ƽʱʹSDA�ź�Ϊ��*/
			SDA_CLR;
			Delay(1);
			SCL_CLR;
			Delay(1);      
			SCL_SET;
			Delay(1);
			SCL_CLR;
			Delay(1);
		}
	}	
}

/******************************************************************************
* ��������i2cRead_ch452
* ��  �ܣ�IOģ��IIC������
* ��  �룺��ȡ���ݵ�Ŀ�ĵ�ַ�����������׵�ַ
* ��  ����1  
* ��  ע����
********************************************************************************/
unsigned char i2cRead_ch452(unsigned char slaveAddr,unsigned char *data)
{
    /*��I2C���߶�һ���ֽ�*/
	int key_num=0,i;
	I2C_Start();
	WriteI2CByte(slaveAddr);
			Delay(1);
           	SDA_SET;
			Delay(1); 
			SCL_CLR;
			Delay(1);
			SCL_SET;
			Delay(1);
			SCL_CLR;
			Delay(1);     /*Ӧ��*/
	GPIO_SDA_Iput_MODULE();
	for(i=0;i<8;i++)
	{
	  Delay(1);
	  SCL_SET;
	  key_num=((key_num)<<1)|GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	  Delay(2);
	  SCL_CLR;
	  Delay(1);
	}
	I2C_Stop();
	GPIO_SDA_Out_MODULE();
	*data = (char)(key_num&0xff);
	return 1;	
}

/******************************************************************************
* ��������i2cWrite_ch452
* ��  �ܣ�IOģ��IICд����
* ��  �룺д�����ݵ�Ŀ�ĵ�ַ��Ҫд�������
* ��  ����1  
* ��  ע����
********************************************************************************/
unsigned char i2cWrite_ch452(unsigned char slaveAddr,unsigned char data)
{
	I2C_Start();
	WriteI2CByte(slaveAddr);
	SDA_SET;
	Delay(1);
	SCL_CLR;
	Delay(1);
	SCL_SET;
	Delay(1);
	SCL_CLR;
	Delay(1);
	WriteI2CByte(data);
	Delay(1);
	SDA_SET;
	Delay(1);
	SCL_CLR;
	Delay(1);
	SCL_SET;
	Delay(1);
	SCL_CLR;
	Delay(1); 
	I2C_Stop();
	return 1;
}

/******************************************************************************
* ��������unsigned char GetKeyNum(void)
* ��  �ܣ���ȡ���̼�ֵ
* ��  �룺��
* ��  ������ֵ  
* ��  ע����
********************************************************************************/
unsigned char GetKeyNum(void)
{
	unsigned char tmp;
	i2cRead_ch452(0x6F,&tmp);
	return(tmp);
}

/******************************************************************************
* ��������int I2C_ByteWrite(unsigned short data,unsigned char slaveAddr)
* ��  �ܣ�дAIC23���ƼĴ���
* ��  �룺AIC23���ƼĴ�����ַ�Ϳ�����
* ��  ����1  
* ��  ע����
********************************************************************************/
int I2C_ByteWrite(unsigned short data,unsigned char slaveAddr)
{
	unsigned short tmp;
	unsigned char dataH,dataL;
	tmp=(slaveAddr<<9)| data;
	dataH=(tmp>>8)&0x00ff;
	dataL=tmp&0x00ff;
	I2C_Start();
	WriteI2CByte((0x1A<<1)& 0xfe );
	        SDA_CLR;
			Delay(1);
			SCL_CLR;
			Delay(1);
			SCL_SET;
			Delay(1);
			SCL_CLR;
			Delay(1);
	WriteI2CByte(dataH);
			Delay(1);
         	SDA_CLR;
			Delay(1);
			SCL_CLR;
			Delay(1);
			SCL_SET;
			Delay(1);
			SCL_CLR;
			Delay(1);     /*Ӧ��*/
	WriteI2CByte(dataL);
			Delay(1);
         	SDA_CLR;
			Delay(1);
			SCL_CLR;
			Delay(1);
			SCL_SET;
			Delay(1);
			SCL_CLR;
			Delay(1);
	I2C_Stop();
	return 1;
}

/******************************************************************************
* ��������WriteI2CByte
* ��  �ܣ����̳�ʼ������
* ��  �룺��
* ��  ������  
* ��  ע����
********************************************************************************/
void InitKey(void)
{
	GPIO_Configuration();
	Delay(5);
	i2cWrite_ch452(0x64,0x01);  /*оƬ��λ*/
	Delay(100);
	i2cWrite_ch452(0x68,CH452SYS_SSPD|CH452SYS_DISP);	   
	i2cWrite_ch452(0x62,0x40);  /*ȫ��*/
	Delay(100000);
	i2cWrite_ch452(0x64,0x01);  /*ȫ��*/
	Delay(5000);
	i2cWrite_ch452(0x68,CH452SYS_SSPD|CH452SYS_DISP|CH452SYS_KEYB);/*��������*/
}
/************************************************************************************
*������:
*����˵��:
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
 void Keypad_Interrupt(u32 group,u32 PreemptionPriority,u32 SubPriority)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
	//�����������λ�ú�ƫ��
	#ifdef  VECT_TAB_RAM  
		/* Set the Vector Table base location at 0x20000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 		//������λ��RAM
	#else  /* VECT_TAB_FLASH  */
		/* Set the Vector Table base location at 0x08000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   //������λ��FLASH
	#endif
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure); 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource11);
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	   //�½��ش���EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	NVIC_PriorityGroupConfig(group);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}
/************************************************************************************
*������:
*����˵��:
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*************/
