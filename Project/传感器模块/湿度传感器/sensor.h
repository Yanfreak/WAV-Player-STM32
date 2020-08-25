/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� sessor.h
*   ժ Ҫ    �� �¶ȡ�ʪ�ȴ�����ʵ��������
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

#ifndef SENSOR_HEADER_FILE
#define SENSOR_HEADER_FILE

/*************ͷ�ļ�**********************************************************/
#include "stm322xg_eval_lcd.h"
#include "stm32_eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void SensorDataInit(void);
float NTC3950(float volt,float resi_div,float vcc);
float HumiditySensor(float volt);
int Sessor(void);

#endif
