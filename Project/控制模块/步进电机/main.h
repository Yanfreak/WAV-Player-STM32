 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� main.c
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

#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f2xx.h"
#include "stm322xg_eval_keypad.h"
#include "stm322xg_eval_lcd.h"
#include "systick.h"
#include "stm32f2xx_exti.h"
#include "moto.h"
#include <stdio.h>

extern unsigned char KeyCode;
uint8_t moto_run;

static void RCC_Configuration(void);
static void EXTI_Configuration(void);
static void NVIC_Configuration(void);
static void main_lcd_msg(void);

#endif

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*****************/
