/****************************************************************************
* Copyright (C), 2011-2012 BeiJing EFLAG Technology Co., LTD.
* All rights reserved.
*
* �ļ���: Systick.h
* ���ݼ���������ʾ����ʵ��
*
* �ļ���ʷ��
* �汾�� ��01a
* ���� ��2011-07-20
* ���� �����ǲ�
* ˵�� ��
****************************************************************************/
#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f2xx.h"
#include "systick.h"	

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
void DelayMs(uint32_t n);
void StartTimer(uint8_t _id, uint32_t _period);
uint8_t CheckTimer(uint8_t _id);

#endif
