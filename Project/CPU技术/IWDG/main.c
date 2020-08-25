/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： 独立看门狗
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
__IO uint32_t TimingDelay = 0;
__IO uint32_t LsiFreq = 0;
__IO uint32_t CaptureNumber = 0, PeriodValue = 0;

/* 私有原型函数 -------------------------------------------------------------*/
void Delay(__IO uint32_t nTime);
uint32_t GetLSIFrequency(void);

/* 私有函数 -----------------------------------------------------------------*/

/****************************************************************************
* 函数名: main()
* 功 能: 主程序
* 输 入: 无
* 输 出: 无
****************************************************************************/
int main(void)
{
	/*在此阶段处理器的时钟已经被初始化完成，时钟初始化工作由SystemInit函数实现，
	SystemInit函数存放在startup_stm32f2xx.s的汇编启动文件中。在系统上电初始化到
	跳转到main函数之前，启动代码会先调用SystemInit函数。如果用户希望重新配置时钟
	可以在此处再次调用 SystemInit()，它的函数实现存放在system_stm32f2xx.c文件中。*/     
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDOff(LED2);

   /* 设置为1毫秒SysTick定时器中断 */
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* 捕获错误 */ 
    while (1);
  }

  /* 检查系统已经从IWDG复位 */
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    /* IWDGRST标记设置 */
    /* 启动LED2 */
    STM_EVAL_LEDOn(LED2);

    /* 清除复位标记寄存器 */
    RCC_ClearFlag();
  }
  else
  {
    /* IWDGRST标记寄存器没有被设置 */
    /* 关闭LED2 */
    STM_EVAL_LEDOff(LED2);
  }
 
  /* 得到LSI频率:TIM5是用来测量LSI频率*/
  LsiFreq = GetLSIFrequency();
   
  /* IWDG超时等于250 ms (超时可能由于LSI的频率变化 */
  /* 启用IWDG_PR和IWDG_RLR寄存器*/
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  /* IWDG计数时钟: LSI/32 */
  IWDG_SetPrescaler(IWDG_Prescaler_32);

  /* 设置计数器重载值来获得250ms IWDG超时TimeOut.
     计数器重装载值 = 250ms/IWDG计数器的时钟周期
                          = 250ms / (LSI/32)
                          = 0.25s / (LsiFreq/32)
                          = LsiFreq/(32 * 4)
                          = LsiFreq/128
   */
  IWDG_SetReload(LsiFreq/128);

  /* 重新加载IWDG计数器 */
  IWDG_ReloadCounter();

  /* 使能IWDG(LSI振荡器将通过硬件) */
  IWDG_Enable();

  while (1)
  {
    /* 切换 LED3 */
    STM_EVAL_LEDToggle(LED3);

    /* 插入240ms的延迟 */
    Delay(240);

    /* 重新加载IWDG计数器 */
    IWDG_ReloadCounter();  
  }
}

/****************************************************************************
* 函数名: GetLSIFrequency()
* 功 能: 配置TIM5测量LSI振荡器频率
* 输 入: 无
* 输 出: 无
****************************************************************************/
uint32_t GetLSIFrequency(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  RCC_ClocksTypeDef  RCC_ClockFreq;

  /* 启用LSI振荡器 ************************************************/
  RCC_LSICmd(ENABLE);
  
  /* 等待LSI振荡器复位*/
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}

  /* 配置TIM5 *******************************************************/ 
  /* 使能TIM5时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  
  /* 连接内部TIM5_CH4输入，捕获LSI时钟输出 */
  TIM_RemapConfig(TIM5, TIM5_LSI);

  /* 配置TIM5预分频器 */
  TIM_PrescalerConfig(TIM5, 0, TIM_PSCReloadMode_Immediate);
  
  /* TIM5配置:输入捕获模式 ---------------------
     LSI振荡器连接到TIM5 CH4，上升沿触发，CCR4的TIM5用于计算频率值 */
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM5, &TIM_ICInitStructure);
  
  /* 使能TIM5中断通道 */
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /*使能TIM5计数器 */
  TIM_Cmd(TIM5, ENABLE);

  /* 复位标志寄存器 */
  TIM5->SR = 0;
    
  /* 使能CC4中断请求 */  
  TIM_ITConfig(TIM5, TIM_IT_CC4, ENABLE);


  /* 等到TIM5得到2 LSI边缘(参照在stm32f2xx_it.c文件中的TIM5_IRQHandler() */
  while(CaptureNumber != 2)
  {
  }
  /* 再一次初始化TIM5外围寄存器为默认重置值 */
  TIM_DeInit(TIM5);


  /* 捕获LSI频率,依靠TIM5输入时钟频率(PCLK1)*/
  /* 获得SYSCLK, HCLK 和 PCLKx 频率 */
  RCC_GetClocksFreq(&RCC_ClockFreq);

  /* 得到PCLK1预分频值 */
  if ((RCC->CFGR & RCC_CFGR_PPRE1) == 0)
  { 
    /* PCLK1预分频值等于1 => TIMCLK = PCLK1 */
    return ((RCC_ClockFreq.PCLK1_Frequency / PeriodValue) * 8);
  }
  else
  { /* PCLK1预分频值不等于1 => TIMCLK = 2 * PCLK1 */
    return (((2 * RCC_ClockFreq.PCLK1_Frequency) / PeriodValue) * 8) ;
  }
}

/****************************************************************************
* 函数名: delay_ms()
* 功 能: 指定延迟时间长度(以毫秒为单位)
* 输 入: 无
* 输 出: 无
****************************************************************************/
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
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
