 /****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ：stm32f2xx_it.c
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

/* 函数声明 -------------------------------------------------------------------*/

void TIM_Config(void);

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
	可以在此处再次调用 SystemInit()，它的函数实现存放在system_stm32f2xx.c文件中。*/ 

  /* 初始化安装在STM322xG-EVAL板上的发光二极管和按钮 */  
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* 初始化安装在STM322xG-EVAL板子上的KEY和SEL */
  STM_EVAL_PBInit(BUTTON_USER1, BUTTON_MODE_GPIO);
    
  /* 配置TIM-------------------------------------------------------*/
  TIM_Config();

  while (1)
  { 
    /* 等待按键响应 */
    while(STM_EVAL_PBGetState(BUTTON_USER1) == RESET)
    {
    }
    while(STM_EVAL_PBGetState(BUTTON_USER1) != RESET)
    {
    }

    /* 这个指令的执行优先级为0，这可以防止除了通过上报进制的硬件故障外
	   所有的从激活开始具有可配置的异常 */
    __disable_irq();

    /* 点亮LED4 */
    STM_EVAL_LEDOn(LED4);

    /* 等待按键响应 */
    while(STM_EVAL_PBGetState(BUTTON_USER1) == RESET)
    {
    }
    while(STM_EVAL_PBGetState(BUTTON_USER1) != RESET)
    {
    }

    /* 此指示将允许所有可配置的异常优先级被激活 */
    __enable_irq();

    /* 关闭LED4 */
    STM_EVAL_LEDOff(LED4);
  }
}

/****************************************************************************
* 函数名: TIM_Config()
* 功 能: 配置使用计时器
* 输 入: 无
* 输 出: 无
****************************************************************************/
void TIM_Config(void)
{ 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;

  /* 使能TIM2,TIM3和TIM4时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 |
                         RCC_APB1Periph_TIM4, ENABLE);

  /* 配置TIM2 */
  TIM_TimeBaseStructure.TIM_Period = 0x4AF;          
  TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/1200) - 1);
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);
  
  /* 配置成输出比较定时模式: 通道1（Channel1）*/
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_Pulse = 0x0;  
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  
  /* 配置TIM3 */
  TIM_TimeBaseStructure.TIM_Period = 0x95F;    
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  /* 配置成输出比较定时模式: 通道1（Channel1）*/
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  
  /* 配置TIM4 */
  TIM_TimeBaseStructure.TIM_Period = 0xE0F;  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
  /* 配置成输出比较定时模式: 通道1（Channel1） */
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);

  /* 立即加载TIM2,TIM3和TIM4预分频值 */
  TIM_PrescalerConfig(TIM2, ((SystemCoreClock/1200) - 1), TIM_PSCReloadMode_Immediate);
  TIM_PrescalerConfig(TIM3, ((SystemCoreClock/1200) - 1), TIM_PSCReloadMode_Immediate);
  TIM_PrescalerConfig(TIM4, ((SystemCoreClock/1200) - 1), TIM_PSCReloadMode_Immediate);

  /* 清除TIM2,TIM3和TIM4更新挂起标志寄存器 */
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);

  /* 配置两个位为抢占优先级 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* 使能TIM2中断 */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* 使能TIM3中断 */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_Init(&NVIC_InitStructure);

  /* 使能TIM4中断 */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_Init(&NVIC_InitStructure);

  /* 使能TIM2,TIM3和TIM4更新中断 */
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

  /* 使能TIM2,TIM3和TIM4计数器 */
  TIM_Cmd(TIM2, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
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
