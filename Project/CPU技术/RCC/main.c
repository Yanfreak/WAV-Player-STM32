/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： RCC时钟控制
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

/*************头文件**********************************************************/
#include "stm32f2xx.h"
#include "stm32_eval.h"

/* 重定义 -------------------------------------------------------------------*/
/* 自定义 -------------------------------------------------------------------*/
/* 私有宏定义 ---------------------------------------------------------------*/
/* 私有变量 -----------------------------------------------------------------*/
void Delay (uint32_t nCount);

/* 私有函数 -----------------------------------------------------------------*/

/****************************************************************************
* 函数名: main()
* 功 能: 主函数
* 输 入: 无
* 输 出: 无
****************************************************************************/
int main(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/*RCC_ClocksTypeDef RCC_ClockFreq;*/

	/*在此阶段处理器的时钟已经被初始化完成，时钟初始化工作由SystemInit函数实现，
	SystemInit函数存放在startup_stm32f2xx.s的汇编启动文件中。在系统上电初始化到
	跳转到main函数之前，启动代码会先调用SystemInit函数。如果用户希望重新配置时钟
	可以在此处再次调用 SystemInit()，它的函数实现存放在system_stm32f2xx.c文件中。*/ 
	  
	
    /* 初始化安装在EITP板上的lED */
    STM_EVAL_LEDInit(LED2);
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED4);

    /* 点亮LED3 */
    STM_EVAL_LEDOn(LED3);
   
    /* 这个函数填补了RCC_ClockFreq结构体，它不同于现在的芯片时钟频率(用于调试目的) */
   /* RCC_GetClocksFreq(&RCC_ClockFreq);*/
  
    /* 使能安全系统时钟(CSS): HSE时钟复位失败将生成一个屏蔽中断异常 */
    RCC_ClockSecuritySystemCmd(ENABLE);
 
    /* 使能和配置RCC全部IRQ通道, 用于管理HSE就绪和PLL中断。这些中断在stm32f2xx_it.c中启用 */
    NVIC_InitStructure.NVIC_IRQChannel = RCC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
   
    /* 选择HSE时钟输出引脚为MCO1(PA8) */
    RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);

    while (1)
    {
        /* 切换LED2和LED4 */
       STM_EVAL_LEDToggle(LED2);
       STM_EVAL_LEDToggle(LED4);

   	 	Delay(0x7FFFF);

    	/* 切换LED3 */
   		//STM_EVAL_LEDToggle(LED3);

    	//Delay(0x7FFFF);    
    }
}

/****************************************************************************
* 函数名: Delay()
* 功 能: 指定延迟时间长度(以毫秒为单位)
* 输 入: 无
* 输 出: 无
****************************************************************************/
void Delay(__IO uint32_t nCount)
{

  while (nCount != 0)
  {
    nCount--;
  }
}
#ifdef  USE_FULL_ASSERT

/******************************************************************************
* 函数名：AssertFailed
* 功  能：报告参数错误的位置
* 输  入：错误所在的文件名和错误所在文件中的行号
* 输  出：无  
* 备  注：无
********************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
	
	/******此处可以添加用户错误报告方式******/

  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
