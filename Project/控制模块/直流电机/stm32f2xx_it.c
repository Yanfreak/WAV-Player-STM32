/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： stm32f2xx_it.c
*   摘 要    ： 温度传感器实验主程序
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
#include "main.h"
#include "Systick.h"
#include "stm32f2xx_it.h"
#include "stm32f2xx_tim.h"
#include "stm322xg_eval_keypad.h"

/*************重定义**********************************************************/
vu32 TimingDelay = 0;
__IO uint16_t IC2Value = 0;
__IO uint16_t DutyCycle = 0;
__IO uint32_t Frequency = 0;

__IO uint16_t IC3ReadValue1 = 0, IC3ReadValue2 = 0;
__IO uint16_t CaptureNumber = 0;
__IO uint32_t Capture = 0;
__IO uint32_t TIM2Freq = 0;

/******************************************************************************/
/*            Cortex-M3  处理器异常处理程序               */
/******************************************************************************/

/**
  * @brief   This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

void SysTick_Handler(void)
{		
	SysTick_ISR();
}

/****************************************************************************
* 函数名: TIM2_IRQHandler()
* 功 能: 捕获脉冲个数，并计算频率
* 输 入: 无
* 输 出: 无
****************************************************************************/
void TIM2_IRQHandler(void)
{ 

	if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET) 
		{
    		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);/*清除中断位*/
    		if(CaptureNumber == 0)
    			{	
					/*捕获输入脉冲,是TIM_GetCapture1！不是TIM_GetCapture2*/
     				IC3ReadValue1 = TIM_GetCapture1(TIM2); 
      				CaptureNumber = 1;  /*IC3ReadValuel是计时器时间值*/
    			}
    		else if(CaptureNumber == 1)
    			{
      				IC3ReadValue2 = TIM_GetCapture1(TIM2);
     				if (IC3ReadValue2 > IC3ReadValue1)
      					{
						    /*IC3ReadValuel与IC3ReadValue2之差就是周期*/
        					Capture = (IC3ReadValue2 - IC3ReadValue1);
      					}
      				else
      					{
        					Capture = ((0xffff - IC3ReadValue1) + IC3ReadValue2); 
      					}
 	   				/*频率计算*/
      				TIM2Freq = 120000000/1000/2/Capture;/*(uint32_t) SystemCoreClock / Capture*/                                                                                                       Capture;//(uint32_t) SystemCoreClock / Capture;
      				CaptureNumber = 0;
				}
  		}
}

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束**************/
