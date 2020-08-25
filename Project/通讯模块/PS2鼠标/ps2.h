/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� ps.h
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

#ifndef __PS2_H
#define __PS2_H	

/*************ͷ�ļ�**********************************************************/
#include "stm32f2xx.h"

/*************�궨��**********************************************************/
#define PS2_SCL_PORT              GPIOC
#define PS2_SCL_CLK               RCC_AHB1Periph_GPIOC  
#define PS2_SCL_PIN               GPIO_Pin_12
#define PS2_SCL    (GPIO_ReadInputDataBit(PS2_SCL_PORT, PS2_SCL_PIN))

#define PS2_SDA_PORT              GPIOF
#define PS2_SDA_CLK               RCC_AHB1Periph_GPIOF  
#define PS2_SDA_PIN               GPIO_Pin_8
#define PS2_SDA    (GPIO_ReadInputDataBit(PS2_SDA_PORT, PS2_SDA_PIN))

#define PS2_SCL_OUT_PORT              GPIOC
#define PS2_SCL_OUT_CLK               RCC_AHB1Periph_GPIOC  
#define PS2_SCL_OUT_PIN               GPIO_Pin_12
#define Set_PS2_SCL_OUT  {GPIO_SetBits(PS2_SCL_OUT_PORT,PS2_SCL_OUT_PIN);}
#define Clr_PS2_SCL_OUT  {GPIO_ResetBits(PS2_SCL_OUT_PORT,PS2_SCL_OUT_PIN);} 

#define PS2_SDA_OUT_PORT              GPIOF
#define PS2_SDA_OUT_CLK               RCC_AHB1Periph_GPIOF 
#define PS2_SDA_OUT_PIN               GPIO_Pin_8
#define Set_PS2_SDA_OUT  {GPIO_SetBits(PS2_SDA_OUT_PORT,PS2_SDA_OUT_PIN);}
#define Clr_PS2_SDA_OUT  {GPIO_ResetBits(PS2_SDA_OUT_PORT,PS2_SDA_OUT_PIN);} 

#define MOUSE    0X20 //���ģʽ
#define KEYBOARD 0X10 //����ģʽ
#define CMDMODE  0X00 //��������

/*************�ⲿ����**********************************************************/
extern u8 PS2_Status;  //����Ϊ����ģʽ
extern u8 PS2_DATA_BUF[16]; //ps2���ݻ�����
extern u8 MOUSE_ID;

/*************��������**********************************************************/
void PS2_Init(void);
u8 PS2_Send_Cmd(u8 cmd);
void PS2_Set_Int(u8 en);
u8 Wait_PS2_Scl(u8 sta);
u8 PS2_Get_Byte(void);
void PS2_En_Data_Report(void);  
void PS2_Dis_Data_Report(void);
		  				    
#endif

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*****************/
