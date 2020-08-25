/****************************************************************************
* Copyright (C), 2011-2012 BeiJing EFLAG Technology Co., LTD.
* All rights reserved.
*
* 文件名: Systick.h
* 内容简述：简易示波器实验
*
* 文件历史：
* 版本号 ：01a
* 日期 ：2011-07-20
* 作者 ：王智博
* 说明 ：
****************************************************************************/
#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "stm32f2xx.h"
#include "systick.h"	

/* 等待定时器超时期间，可以让CPU进入IDLE状态， 目前是空 */
#define CPU_IDLE()

/* 定时器结构体，成员变量必须是 volatile, 否则C编译器优化时可能有问题 */
typedef struct
{
	volatile uint32_t count;	/* 计数器 */
	volatile uint8_t flag;		/* 定时到达标志  */
}SOFT_TMR;

static void SoftTimerDec(SOFT_TMR *_tmr);
void SysTick_ISR(void);
void DelayMs(uint32_t n);
void StartTimer(uint8_t _id, uint32_t _period);
uint8_t CheckTimer(uint8_t _id);

#endif
