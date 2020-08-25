/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： NVIC
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
__IO uint8_t PreemptionOccured = 0; 
__IO uint8_t PreemptionPriorityValue = 0; 

/* 私有函数原型 -------------------------------------------------------------*/
void NVIC_Config(void);
void Delay(__IO uint32_t nCount);

/* 私有函数 ------------------------------------------------------------------*/

/****************************************************************************
* 函数名: main()
* 功 能: 主函数
* 输 入: 无
* 输 出: 无
****************************************************************************/
int main(void)
{
	/*在此阶段处理器的时钟已经被初始化完成，时钟初始化工作由SystemInit函数实现，
	SystemInit函数存放在startup_stm32f2xx.s的汇编启动文件中。在系统上电初始化到
	跳转到main函数之前，启动代码会先调用SystemInit函数。如果用户希望重新配置时钟
	可以在此处再次调用 SystemInit()，它的函数实现存放在system_stm32f2xx.c文件中*/ 

  /* 配置NVIC */
  NVIC_Config();     
  
  /* 初始化安装在STM322xG-EVAL板上的发光二极管和按钮 */       
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_PBInit(BUTTON_USER1, BUTTON_MODE_EXTI);

  /* 配置SysTick处理器优先级: 抢占优先级和子优先级 */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), !PreemptionPriorityValue, 0));

  while (1)
  {
    if(PreemptionOccured != 0)
    {
	    
      /* 时间延时 */
      Delay(0x5FFFF);
      
      STM_EVAL_LEDToggle(LED2);
      
      Delay(0x5FFFF);
      
      STM_EVAL_LEDToggle(LED3);
      
      Delay(0x5FFFF);
      
      STM_EVAL_LEDToggle(LED4);
      
      Delay(0x5FFFF); 
    }
  }
}

/****************************************************************************
* 函数名: NVIC_Config()
* 功 能: 配置中断优先级
* 输 入: 无
* 输 出: 无
****************************************************************************/
void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* 配置抢占优先级和子优先级:
     - 1位抢占优先级: 值为0或1 
     - 3位副优先级: 值为0到7
     - 值越低优先级越高 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* 使能按键中断优先级 */
  NVIC_InitStructure.NVIC_IRQChannel = USER1_BUTTON_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* 函数名: Delay()
* 功 能: 指定延迟时间长度(以毫秒为单位)
* 输 入: 无
* 输 出: 无
****************************************************************************/
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

#ifdef  USE_FULL_ASSERT

/******************************************************************************
* 函数名：assert_failed()
* 功  能：报告参数错误的位置
* 输  入：错误所在的文件名和错误所在文件中的行号
* 输  出：无  
* 备  注：无
********************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* 此处可以添加用户错误报告方式: 输出(“错误参数值: 文件的路径、名文件和行号” */

  /* 无限循环 */
  while (1)
  {
  }
}
#endif

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*************/
