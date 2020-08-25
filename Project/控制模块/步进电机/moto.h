/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� moto.h
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
#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f2xx.h"
#include "systick.h"
#include "stm322xg_eval_lcd.h"
#include "moto.h"
#include "stm32f2xx_tim.h"
#include <stdio.h>
#include "stm32_eval.h"
/*************�ⲿ����******************************************************/	
extern unsigned char KeyCode;
extern u8 moto_run;

/*************��������******************************************************/	
void Moto_GPIO_Configuration(void); 
void Moto_NVIC_Configuration(void);
void Moto_TIM2_Configuration(int freq);
void moto_test(void);
void moto_lcd_msg(void);
	   	 			    
#endif

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*****************/
