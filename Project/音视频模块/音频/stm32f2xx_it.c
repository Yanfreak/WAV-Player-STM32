/**
  ******************************************************************************
  * @file    Project/STM32F2xx_StdPeriph_Template/stm32f2xx_it.c 
  * @author  MCD Application Team
  * @version V0.0.3
  * @date    10/15/2010
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @copy
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_it.h"
#include "main.h"
#include "stm322xg_eval_keypad.h"
/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define TXBUFFERSIZE   (countof(TxBuffer) - 1)	/* 定义发送缓冲区大小 */
#define RXBUFFERSIZE   0x20000		/* 定义接收缓冲区大小为32*/
#define  buffer   	((uint8_t*)0x68000100) //外部sram
/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint8_t TxBuffer[] = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal\
 communication using Interrupt\n\r";			 /* 串口发送缓冲区 */
uint8_t RxBuffer;					 /* 串口接收缓冲区*/
uint8_t NbrOfDataToTransfer = TXBUFFERSIZE;		 /* 记录允许发送数据的数量*/
uint32_t NbrOfDataToRead = RXBUFFERSIZE;			 /* 记录允许接收数据的数量*/
__IO uint8_t TxCounter = 0; 					 /* 记录已发送数量*/
__IO uint32_t RxCounter = 0; 					 /*RxBuffer[i]* 记录已接收数量*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
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

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

}

void EXTI15_10_IRQHandler(void)
{

	if(EXTI_GetITStatus(EXTI_Line11) != RESET) 
    {
	     //添加中断处理程序
		 i2cRead_ch452(0x6F,&KeyCode);
		 KeyCode=KeyCode&0xff;
	     EXTI_ClearFlag(EXTI_Line11);			       //清除中断标志（必须）
	     EXTI_ClearITPendingBit(EXTI_Line11);
     }

}

/******************************************************************************/
/*                 STM32F2xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f2xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */ 
/******************************************************************************
* 函数名： USART1_IRQHandler中断服务程序
* 功  能： 在中断服务程序中接收数据将其存放在RxBuffer中，将TxBuffer中的数据发送出去
* 输  入： 
* 输  出：无  
* 备  注：USART1_IRQHandler 在startup_stm32f2xx.s中声明
********************************************************************************/
void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(EVAL_COM1, USART_IT_RXNE) != RESET)
  {
    /* 从接收数据寄存器中读取一个字节数据 */
    RxBuffer= (USART_ReceiveData(EVAL_COM1) & 0xFF);
		    buffer[RxCounter++]=RxBuffer;
    if(RxCounter == NbrOfDataToRead)
    {
      /* 关闭 EVAL_COM1 接收中断 */
      USART_ITConfig(EVAL_COM1, USART_IT_RXNE, DISABLE);
    }
  }

  if(USART_GetITStatus(EVAL_COM1, USART_IT_TXE) != RESET)
  {   
    /* 写一个字节到发送数据寄存器*/
   USART_SendData(EVAL_COM1,TxBuffer[TxCounter++] );

    if(TxCounter == NbrOfDataToTransfer)
    {
      /* 关闭 EVAL_COM1 发送中断  */
      USART_ITConfig(EVAL_COM1, USART_IT_TXE, DISABLE);
    }
  }
}


/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
