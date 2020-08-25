/**
  ******************************************************************************
  * @file    USART/HyperTerminal_Interrupt/stm32f2xx_it.c 
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    18-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx_it.h"
#include "stm32_eval.h"

/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup USART_HyperTerminal_Interrupt
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define TXBUFFERSIZE   (countof(TxBuffer) - 1)	/* ���巢�ͻ�������С */
#define RXBUFFERSIZE   0x20000		/* ������ջ�������СΪ32*/
#define  buffer   	((uint8_t*)0x68000100) //�ⲿsram
/* Private macro -------------------------------------------------------------*/
#define countof(a)   (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/
uint8_t TxBuffer[] = "\n\rUSART Hyperterminal Interrupts Example: USART-Hyperterminal\
 communication using Interrupt\n\r";			 /* ���ڷ��ͻ����� */
uint8_t RxBuffer;					 /* ���ڽ��ջ�����*/
uint8_t NbrOfDataToTransfer = TXBUFFERSIZE;		 /* ��¼���������ݵ�����*/
uint32_t NbrOfDataToRead = RXBUFFERSIZE;			 /* ��¼����������ݵ�����*/
__IO uint8_t TxCounter = 0; 					 /* ��¼�ѷ�������*/
__IO uint32_t RxCounter = 0; 					 /*RxBuffer[i]* ��¼�ѽ�������*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
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
  * @brief  This function handles PendSV_Handler exception.
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

/******************************************************************************/
/*            STM32F2xx Peripherals Interrupt Handlers                        */
/******************************************************************************/


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

/**
  * @}
  */ 


/******************************************************************************
* �������� USART1_IRQHandler�жϷ������
* ��  �ܣ� ���жϷ�������н������ݽ�������RxBuffer�У���TxBuffer�е����ݷ��ͳ�ȥ
* ��  �룺 
* ��  ������  
* ��  ע��USART1_IRQHandler ��startup_stm32f2xx.s������
********************************************************************************/
void USART1_IRQHandler(void)
{
  if(USART_GetITStatus(EVAL_COM1, USART_IT_RXNE) != RESET)
  {
    /* �ӽ������ݼĴ����ж�ȡһ���ֽ����� */
    RxBuffer= (USART_ReceiveData(EVAL_COM1) & 0xFF);
		    buffer[RxCounter++]=RxBuffer;
    if(RxCounter == NbrOfDataToRead)
    {
      /* �ر� EVAL_COM1 �����ж� */
      USART_ITConfig(EVAL_COM1, USART_IT_RXNE, DISABLE);
    }
  }

  if(USART_GetITStatus(EVAL_COM1, USART_IT_TXE) != RESET)
  {   
    /* дһ���ֽڵ��������ݼĴ���*/
   USART_SendData(EVAL_COM1,TxBuffer[TxCounter++] );

    if(TxCounter == NbrOfDataToTransfer)
    {
      /* �ر� EVAL_COM1 �����ж�  */
      USART_ITConfig(EVAL_COM1, USART_IT_TXE, DISABLE);
    }
  }
}


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
