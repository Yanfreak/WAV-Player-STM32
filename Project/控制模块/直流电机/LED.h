/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� led.h
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

/*************�궨��**********************************************************/
#define LED1_0 GPIO_ResetBits(GPIOF,GPIO_Pin_6)	 /*����*/
#define LED1_1 GPIO_SetBits(GPIOF,GPIO_Pin_6)	

#define LED2_0 GPIO_ResetBits(GPIOF,GPIO_Pin_7)  
#define LED2_1 GPIO_SetBits(GPIOF,GPIO_Pin_7)	 

#define LED3_0 GPIO_ResetBits(GPIOF,GPIO_Pin_10)
#define LED3_1 GPIO_SetBits(GPIOF,GPIO_Pin_10)

#define LED4_0 GPIO_ResetBits(GPIOF,GPIO_Pin_9)
#define LED4_1 GPIO_SetBits(GPIOF,GPIO_Pin_9)

/*************��������********************************************************/
void LED_Init(void);

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*****************/
