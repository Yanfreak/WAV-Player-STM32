/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� ���Կ⺯��
*   ժ Ҫ    �� 
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
#include "stm32f2xx.h"
#include "stm32f2xx_ip_dbg.h"
#include "stm32_eval.h"
#include <stdio.h>

#pragma import(__use_no_semihosting_swi)

/* �ض��� -------------------------------------------------------------------*/
/* �Զ��� -------------------------------------------------------------------*/
/* ˽�к궨�� ---------------------------------------------------------------*/
/* ˽�б��� -----------------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;

/* ˽�к���ԭ�� -----------------------------------------------*/
#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

/****************************************************************************
* ������: main()
* �� ��: 
* �� ��: ��
* �� ��: ��
****************************************************************************/
int main(void)
{
	/*�ڴ˽׶δ�������ʱ���Ѿ�����ʼ����ɣ�ʱ�ӳ�ʼ��������SystemInit����ʵ�֣�
	SystemInit���������startup_stm32f2xx.s�Ļ�������ļ��С���ϵͳ�ϵ��ʼ����
	��ת��main����֮ǰ������������ȵ���SystemInit����������û�ϣ����������ʱ��
	�����ڴ˴��ٴε��� SystemInit()�����ĺ���ʵ�ִ����system_stm32f2xx.c�ļ��С�*/ 
	   
 GPIO_InitTypeDef GPIOA_InitStructure;
 
  /* USARTx ��������:
        - BaudRate�������ʣ� = 115200 ����  
        - Word Length���ֳ��� = 8 λ
        - One Stop Bit��һ��ֹͣλ��
        - No parity����У��λ��
        -Ӳ�������Ʋ�ʹ�� (RTS��CTS�ź�)
        - ���ý��պͷ���
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(COM1, &USART_InitStructure);
  printf("\r\n STM32F2xx Firmware Library compiled with FULL ASSERT function... \n\r");
  /* ��ʼ����������ָ�� */
  IP_Debug();
  
  printf("\r\n STM32F2xx Firmware Library compiled with FULL ASSERT function... \n\r");
  printf("...Run-time checking enabled  \n\r");

  /* ģ�����Ĳ������ݸ��⺯�� ---------------------*/
  /* ʹ��SPI1ʱ��, RCC_APB2PeriphClockCmd()�������뱻ʹ�ò���ע�ⲻ��RCC_APB1PeriphClockCmd() */
  RCC_APB1PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  
  /* GPIOA_InitStructure�ṹ��� һЩ��Ա��û�г�ʼ��*/
  GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIOA_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  /*GPIOA_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;*/
  GPIOA_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIOA_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 
  GPIO_Init(GPIOA, &GPIOA_InitStructure);
  
  while (1)
  {
  }
}

#ifdef  USE_FULL_ASSERT

/******************************************************************************
* ��������AssertFailed
* ��  �ܣ�������������λ��
* ��  �룺�������ڵ��ļ����ʹ��������ļ��е��к�
* ��  ������  
* ��  ע����
********************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/******�˴���������û����󱨸淽ʽ******/
  printf("\n\r Wrong parameter value detected on\r\n");
  printf("       file  %s\r\n", file);
  printf("       line  %d\r\n", line);

}
#endif

/******************************************************************************
* ��������PUTCHAR_PROTOTYPE
* ��  �ܣ��������C������printf����������ַ�
* ��  �룺
* ��  ������  
********************************************************************************/
PUTCHAR_PROTOTYPE
{
  /* �ڴ����Ҫ������ַ� */
  /* �򴮿�дһ���ַ� */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* ѭ��ֱ��������� */
  while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {
  }

  return ch;
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
