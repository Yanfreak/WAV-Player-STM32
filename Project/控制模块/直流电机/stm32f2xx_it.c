/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� stm32f2xx_it.c
*   ժ Ҫ    �� �¶ȴ�����ʵ��������
*
*   ��ǰ�汾 �� 
*   �� ��    �� EFLAG
*   ������� �� 
*
*   ȡ���汾 ��
*   ԭ����   �� 
*   �޸����� ��
*   ������� ��2012.08.01 
*
******************************************************************************/

/*************ͷ�ļ�**********************************************************/
#include "main.h"
#include "Systick.h"
#include "stm32f2xx_it.h"
#include "stm32f2xx_tim.h"
#include "stm322xg_eval_keypad.h"

/*************�ض���**********************************************************/
vu32 TimingDelay = 0;
__IO uint16_t IC2Value = 0;
__IO uint16_t DutyCycle = 0;
__IO uint32_t Frequency = 0;

__IO uint16_t IC3ReadValue1 = 0, IC3ReadValue2 = 0;
__IO uint16_t CaptureNumber = 0;
__IO uint32_t Capture = 0;
__IO uint32_t TIM2Freq = 0;

/******************************************************************************/
/*            Cortex-M3  �������쳣�������               */
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
* ������: TIM2_IRQHandler()
* �� ��: �������������������Ƶ��
* �� ��: ��
* �� ��: ��
****************************************************************************/
void TIM2_IRQHandler(void)
{ 

	if(TIM_GetITStatus(TIM2, TIM_IT_CC1) == SET) 
		{
    		TIM_ClearITPendingBit(TIM2, TIM_IT_CC1);/*����ж�λ*/
    		if(CaptureNumber == 0)
    			{	
					/*������������,��TIM_GetCapture1������TIM_GetCapture2*/
     				IC3ReadValue1 = TIM_GetCapture1(TIM2); 
      				CaptureNumber = 1;  /*IC3ReadValuel�Ǽ�ʱ��ʱ��ֵ*/
    			}
    		else if(CaptureNumber == 1)
    			{
      				IC3ReadValue2 = TIM_GetCapture1(TIM2);
     				if (IC3ReadValue2 > IC3ReadValue1)
      					{
						    /*IC3ReadValuel��IC3ReadValue2֮���������*/
        					Capture = (IC3ReadValue2 - IC3ReadValue1);
      					}
      				else
      					{
        					Capture = ((0xffff - IC3ReadValue1) + IC3ReadValue2); 
      					}
 	   				/*Ƶ�ʼ���*/
      				TIM2Freq = 120000000/1000/2/Capture;/*(uint32_t) SystemCoreClock / Capture*/                                                                                                       Capture;//(uint32_t) SystemCoreClock / Capture;
      				CaptureNumber = 0;
				}
  		}
}

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����**************/
