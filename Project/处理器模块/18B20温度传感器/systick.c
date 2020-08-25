/****************************************************************************
*
* 文件名: sysytick.c
* 内容简述: 本模块包含systick中断的设置，以及软件定时器的实现。
*
*
*/

#include "stm32f2xx.h"
#include <stdio.h>

#include "systick.h"

/* 在此定义若干个软件定时器全局变量
注意，必须增加volatile，因为这个变量在中断和主程序中同时
被访问，有可能造成编译器错误优化
*/
#define TMR_COUNT	4		/* 软件定时器的个数 */
SOFT_TMR g_Tmr[TMR_COUNT];

/*******************************************************************************
	函数名：SoftTimerDec
	输  入: 定时器变量指针,每隔1ms减1
	输  出:
	功能说明：
*/
static void SoftTimerDec(SOFT_TMR *_tmr)
{
	if (_tmr->flag == 0)
	{
		if (_tmr->count > 0)
		{
			if (--_tmr->count == 0)
			{
				_tmr->flag = 1;
			}
		}
	}
}

/*******************************************************************************
	函数名：SysTick_ISR
	输  入:
	输  出:
	功能说明：SysTick中断服务程序，每隔1ms进入1次
*/
extern void AdcPro(void);
void SysTick_ISR(void)
{
	static uint8_t s_count = 0;
	uint8_t i;

	for (i = 0; i < TMR_COUNT; i++)
	{
		SoftTimerDec(&g_Tmr[i]);
	}

	if (++s_count >= 10)
	{
		s_count = 0;

		AdcPro();		/* 该函数在 main.c */
	}
}

/*******************************************************************************
	函数名：StartTimer
	输  入: 定时器ID (0 - 3)
	输  出:
	功能说明：
*/
void StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		return;
	}

	g_Tmr[_id].count = _period;
	g_Tmr[_id].flag = 0;
}

/*******************************************************************************
	函数名：StartTimer
	输  入: 定时器ID (0 - 3)
	输  出: 返回 0 表示定时未到， 1表示定时到
	功能说明：
*/
uint8_t CheckTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	return g_Tmr[_id].flag;
}

