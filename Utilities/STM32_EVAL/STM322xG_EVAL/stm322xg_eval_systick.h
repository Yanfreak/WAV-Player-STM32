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
#ifndef __SYSTICK_H
#define __SYSTICK_H
#include "stm32f2xx.h"
/* �ȴ���ʱ����ʱ�ڼ䣬������CPU����IDLE״̬�� Ŀǰ�ǿ� */
#define CPU_IDLE()
#define TMR_COUNT	4		/* �����ʱ���ĸ��� */
/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
	volatile uint32_t count;	/* ������ */
	volatile uint8_t flag;		/* ��ʱ�����־  */
}SOFT_TMR;

void SysTick_Configuration(void);

void StartTimer(uint8_t _id, uint32_t _period);
uint8_t CheckTimer(uint8_t _id);
 void SoftTimerDec(SOFT_TMR *_tmr);
 void delay_ms(u16 nms);
 void delay_us(u32 nus);
 void DelayMs(uint32_t n);
#endif
/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
