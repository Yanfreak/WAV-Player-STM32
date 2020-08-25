/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� Systick.h
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
#ifndef __SYSTICK_H
#define __SYSTICK_H

/* �ȴ���ʱ����ʱ�ڼ䣬������CPU����IDLE״̬�� Ŀǰ�ǿ� */
#define CPU_IDLE()

/* ��ʱ���ṹ�壬��Ա���������� volatile, ����C�������Ż�ʱ���������� */
typedef struct
{
	volatile uint32_t count;	/* ������ */
	volatile uint8_t flag;		/* ��ʱ�����־  */
}SOFT_TMR;

static void SoftTimerDec(SOFT_TMR *_tmr);
void SysTick_ISR(void);
void DelayMS(uint32_t n);
void StartTimer(uint8_t _id, uint32_t _period);
uint8_t CheckTimer(uint8_t _id);

#endif

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����**************/
