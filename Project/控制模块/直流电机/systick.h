/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： Systick.h
*   摘 要    ： 
*
*   当前版本 ： 
*   作 者    ： EFLAG
*   完成日期 ： 
*
*   取代版本 ：
*   原作者   ： 
*   修改内容 ：
*   完成日期 ：2012.08.01 
*
******************************************************************************/
#ifndef __SYSTICK_H
#define __SYSTICK_H

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
void DelayMS(uint32_t n);
void StartTimer(uint8_t _id, uint32_t _period);
uint8_t CheckTimer(uint8_t _id);

#endif

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束**************/
