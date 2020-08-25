/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� motor.h
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

#ifndef __MOTOR_H
#define __MOTOR_H

/*************�궨��**********************************************************/
#define BRIGHT_MAX		100
#define BRIGHT_MIN		0
#define BRIGHT_DEFAULT	200
#define BRIGHT_STEP		5
#define uint unsigned int

/*************��������********************************************************/
void Motor_Init(void);
void Motor_Forward(void);
void Motor_Back(void);
void Motor_Stop(void);
void TIM3_Configuration(uint Pulse);
void Speed(int freq,int Sure_Number);

#endif

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*****************/
