 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� USART/USART_Printf/main.c
*   ժ Ҫ    �� ʵ��printf�����ض������������USART1
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
**********************************************************/
/* ͷ�ļ����� ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "stm32_eval.h"
#include <stdio.h>

/* ���Ͷ���  -------------------------------------------------------------------*/

/* �궨��  ---------------------------------------------------------------------*/

/* ���� PUTCHAR_PROTOTYPE ����*/
#ifdef __GNUC__	  
  /* �� PUTCHAR_PROTOTYPE ����Ϊ __io_putchar���� ���� GCC/RAISONANCEģʽ�£�����small printfʵ�ַ���
  *  ����ͨ�� LD Linker->Libraries->Small printf ѡ�� ���� 'Yes'����ʵ��printf���� __io_putchar() 
  */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  /* Ĭ������£�printf���������fputc��������*/
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/* �������� --------------------------------------------------------------------*/

USART_InitTypeDef USART_InitStructure;  /*���崮�ڳ�ʼ���ṹ�� */

/* ��������---------------------------------------------------------------------*/

void YangHui(void);	/* ʹ��printf��ʽ�����������ν�� */

/* Private functions ---------------------------------------------------------*/

/******************************************************************************
* �������� main
* ��  �ܣ� 
* ��  �룺�� 
* ��  ������  
* ��  ע����
********************************************************************************/

int main(void)
{
       
  /* ��ʼ��USART_InitStructure�ṹ�������USART ����Ϊ���¹���ģʽ:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(COM1, &USART_InitStructure);	 /*��USART_InitStructure�ṹ��������õ�������USART1������USART1�ķ��ͺͽ��ܹ��� */

  /* ʹ��printf�����Ϣ */
  printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");
  
  YangHui();  /* ������������μ��㺯�� */
  while (1)
  {
  }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
/******************************************************************************
* ��������PUTCHAR_PROTOTYPE  
* ��  �ܣ��ض��� C ��׼���printf������USART���������Ϣ 
* ��  �룺int ch ��������ַ����ɿ⺯�����ã�
* ��  ����int  ch  
* ��  ע���ļ���ʼ����������PUTCHAR_PROTOTYPEΪ__io_putchar(int ch)������fputc(int ch, FILE *f)����
********************************************************************************/
PUTCHAR_PROTOTYPE
{
  /* �˴���дfputc��ʵ�ִ��� */
  /* ���磺 �Ӵ��ڷ���һ���ַ� */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* ѭ���ȴ���ֱ���������ݷ��ͽ��� */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

/******************************************************************************
* ��������YangHui 
* ��  �ܣ���������μ��㷽��������printf���������
* ��  �룺�� 
* ��  ������  
* ��  ע��NUM ����Ϊ 10 �� 
********************************************************************************/

#define  NUM  10
void YangHui(void)
{
       int a[NUM][NUM],i,j;
	printf("\n\r ����10����������Σ�\n\r\n\r");
       for (i=0;i<NUM;i++)
       {
              a[i][0]=1;
              a[i][i]=1;
       }
       for (i=2;i<NUM;i++)
       {
              for (j=1;j<i;j++)
              a[i][j]=a[i-1][j-1]+a[i-1][j];
       }
       for (i=0;i<NUM;i++)
       {
              for (j=0;j<i+1;j++) printf("%3d  ",a[i][j]);
              printf("\n\r");
       }
       printf("\n\r");
}


#ifdef  USE_FULL_ASSERT

/***************************************************************
*������:assert_failed
*����˵��:������������λ��
*	
*�������:
*		file:�������ڵ��ļ���
*		line:���������ļ��е��к�
*	
*����ֵ:��	
*****************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
 /****�˴���������û����󱨸淽ʽ**********************/

  while (1)
  {
  }
}
#endif
/**
  * @}
  */


/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
