 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： main.c
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
**********************************************************/
#include "stm32f2xx.h"
#include <stdio.h>

#include "stm322xg_eval_systick.h"

/* 在此定义若干个软件定时器全局变量
注意，必须增加volatile，因为这个变量在中断和主程序中同时
被访问，有可能造成编译器错误优化
*/

SOFT_TMR g_Tmr[TMR_COUNT];

/******************************************************
函数名：SoftTimerDec
输  入: 
*    _tmr:  定时器变量指针,
输  出:	无
功能说明：每调用一次本函数，定时器变量结构体中的count减一
*		当减到0时定时器变量结构体中的flag置1
********************************************************/
 void SoftTimerDec(SOFT_TMR *_tmr)
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

/*****************************************************
函数名：SysTick_ISR
输  入:无
输  出:无
功能说明：全部设定定时器，本函数在systick中断子程序中调用
********************************************************/
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

	}
}
/*******************************************************************************
函数名：DelayMS
输  入: 延迟长度，单位1 ms. 延迟精度为正负1ms
输  出:
功能说明：延时函数。占用软件定时器0
*******************************************************************************/
void DelayMs(uint32_t n)
{
	/* 避免 n = 1 出现主程序死锁 */
	if (n == 1)
	{
		n = 2;
	}
	g_Tmr[0].count = n;
	g_Tmr[0].flag = 0;

	/* while 循环体最好让CPU进入IDLE状态，已降低功耗 */
	while (1)
	{
		CPU_IDLE();

		if (g_Tmr[0].flag == 1)
		{
			break;
		}
	}
}
/*****************************************************
函数名：StartTimer
输  入: 定时器ID (0 - 3)
输  出:无
功能说明：设定定时器时长
******************************************************/
void StartTimer(uint8_t _id, uint32_t _period)
{
	if (_id >= TMR_COUNT)
	{
		return;
	}

	g_Tmr[_id].count = _period;
	g_Tmr[_id].flag = 0;
}

/**************************************************
函数名：CheckTimer
输  入: 定时器ID (0 - 3)
输  出: 返回 0 表示定时未到， 1表示定时到
功能说明：定时器检查函数
***************************************************/
uint8_t CheckTimer(uint8_t _id)
{
	if (_id >= TMR_COUNT)
	{
		return 0;
	}

	return g_Tmr[_id].flag;
}


  /****************************************************************************
* 函数名: delay_ms()
* 功 能: 延迟多少纳秒
* 输 入: nus为要延时的ms数
* 输 出: 无
* 备注: 注意nms的范围, SysTick->LOAD为24位寄存器,所以,最大延时为:nms<=0xffffff*8*1000/SYSCLK
*       SYSCLK单位为Hz,nms单位为ms,对120M条件下,nms<=1118
***************************************************************************/
void delay_ms(u16 nms)
{         
	u32 temp;     
	SysTick->LOAD=(u32)nms*15000;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL=0x01 ;          //开始倒数  
	do
		{
  			temp=SysTick->CTRL;
		}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器        
} 
/****************************************************************************
* 函数名: delay_us()
* 功 能: 延迟函数
* 输 入: nus为要延时的us数
* 输 出:注意nus的范围, SysTick->LOAD为24位寄存器,所以,最大延时为:nus<=0xffffff*8/SYSCLK
*       SYSCLK单位为Hz,nus单位为ms,对120M条件下,nus<=1118000
****************************************************************************/               
void delay_us(u32 nus)
{  
	u32 temp;       
	SysTick->LOAD=nus*15; //时间加载      
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL=0x01 ;      //开始倒数   
	do
		{
  			temp=SysTick->CTRL;
		}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL=0x00;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器  
} 
/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
