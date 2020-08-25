 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� main.c
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
**********************************************************/
/* ͷ�ļ� ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "stm32_eval.h"
#include "stm322xg_eval_lcd.h"
#include <stdio.h>
#include <string.h>

/* ���Ͷ���  -------------------------------------------------------------------*/

/* �궨��  ---------------------------------------------------------------------*/

#define SEND_MODE		1				  /* ����IRDA����ģʽ*/

#define RECV_MODE		2				  /* ����IRDA����ģʽ*/

//#define configTYPE	    SEND_MODE	  /* ���ù���Ϊ ����IRDA��������ģʽ*/
#define configTYPE		RECV_MODE		  /* ���ù���Ϊ ����IRDA��������ģʽ*/


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

/* ��������---------------------------------------------------------------------*/
void Display_Init(void);
void Uart_Irda_Configuration(void);

/******************************************************************************
* ��������main 
* ��  �ܣ�ʵ�ֺ���IRDA�ķ��ͺͽ��չ��� 
* ��  �룺�� 
* ��  ������  
* ��  ע����Ϊ����IRDAΪ��˫����ʽ������ͨ������CONFIGTYPE���ڵ�34�ж��壩��ѡ���ͺͽ��չ���
********************************************************************************/
int main(void)
{
#if (configTYPE == SEND_MODE)
	unsigned int i = 0;
	volatile int delay;
	char text[50] = {0};
#endif

#if (configTYPE == RECV_MODE)
	int ch;
	char buf[128] = {0};
	int cnt = 0;
#endif

	Display_Init();					   /* ����Һ����ʾ����IRDA��������*/
	Uart_Irda_Configuration();		   /* ���ú���IRDAʹ��USART4*/

 	
#if (configTYPE == SEND_MODE)		   /* ʵ�ֺ���IRDA�������ݹ���*/
	i = 0;
	sprintf(text, "    Send = %d", i);  /* ��ʽ���ַ�����Ϣ�����text��*/
	LCD_DisplayStringLine(LINE(4), (uint8_t *)text); /* ��buf�е��ַ�����Ϣ��ʾ��Һ���ĵ�4��*/
	while (1) {
		printf(" Send %d   \n\r", i++);	/* �����IRDA��������*/
		delay = 5000000;
		while (delay--);			    /* ��ʱ */
		sprintf(text, "    Send = %d", i);	/* ��ʽ���ַ�����Ϣ�����text��*/
		LCD_DisplayStringLine(LINE(4), (uint8_t *)text);   /* ��text�е��ַ�����Ϣ��ʾ��Һ���ĵ�4��*/
	}
#endif

#if (configTYPE == RECV_MODE)	/* ʵ�ֺ���IRDA�������ݹ���*/
	ch = -1;
	cnt = 0;
	LCD_DisplayStringLine(LINE(3), " Receive Info��");
	while (1) {
		while (1) {
			/* �ж�USART4��״̬�Ĵ����Ƿ���յ�����*/
			while (USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == RESET){}
			ch = USART_ReceiveData(UART4);	/* �����յ����ݴ����ch������*/

			/* ����յ�'\r'��'\n'���ۼ��������յ�128���ֽھ�����ѭ��*/
			if ((ch == '\r') || (ch == '\n') || (cnt >= 128)) {
				break;
			}
			if (ch != 0) {
				buf[cnt++] = ch;   /* ��ch�����buf��*/
			}
		}
		buf[cnt] = 0;
		LCD_DisplayStringLine(LINE(5), (uint8_t *)buf);	 /* ��ʾ�����ַ���Ϣ*/
		cnt = 0;
		memset(buf, 0, sizeof(buf));
	}
#endif
}

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
  USART_SendData(UART4, (uint8_t) ch);

  /* ѭ���ȴ���ֱ���������ݷ��ͽ��� */
  while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

/******************************************************************************
* ��������Display_Init 
* ��  �ܣ���ʾ����IRDA�������� 
* ��  �룺��
* ��  ������  
* ��  ע����
********************************************************************************/
void Display_Init(void)
{
  /* ��ʼ��Һ����Ļ */
  STM322xG_LCD_Init();

  /* �����ĻΪ��ɫ */ 
  LCD_Clear(White);

  /* ѡ������ */
  LCD_SetFont(&Font8x12);

  /* ����Һ������ɫΪ��ɫ���ı�ɫΪ��ɫ*/
  LCD_SetBackColor(Blue);
  LCD_SetTextColor(White);

  /* ��ʾ�ַ��� */
  LCD_DisplayStringLine(LINE(0x13), "          USART IRDA example            ");

  /* �������� */
  LCD_SetFont(&Font16x24);

  /* ��ʾ�ַ��� */
  LCD_DisplayStringLine(LINE(0), "     IRDA 9600bps   ");

  /* ����Һ������ɫΪ�ף��ı�ɫΪ��ɫ*/
  LCD_SetBackColor(White);
  LCD_SetTextColor(Blue);
}

/******************************************************************************
* ��������Uart_Irda_Configuration 
* ��  �ܣ����ú���ʹ�õ�USART4 
* ��  �룺�� 
* ��  ������  
* ��  ע����
********************************************************************************/

void Uart_Irda_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;	 /*���崮�ڳ�ʼ���ṹ�� */
	/* ʹ��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	/* ���� PC11 Ϊ USART4_Tx*/
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
	
	/* ���� PC10 Ϊ USART4_Rx*/
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10,GPIO_AF_UART4);
	/* ���� USART Tx Ϊ���ù��� */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	/* ��ʼ��USART_InitStructure�ṹ�������USART ����Ϊ���¹���ģʽ:
	    - BaudRate = 9600 baud  
	    - Word Length = 8 Bits
	    - One Stop Bit
	    - No parity
	    - Hardware flow control disabled (RTS and CTS signals)
	    - Receive and transmit enabled
	*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(UART4, &USART_InitStructure);
    
	/* ʹ��UART4 */
	USART_Cmd(UART4, ENABLE);
	/* ���ú���ķ�Ƶ */
	USART_SetPrescaler(UART4, 10);
	/* ���ú���Ĺ���ģʽ */
	USART_IrDAConfig(UART4, USART_IrDAMode_Normal);
	/* ʹ�ܺ��� */	
	USART_IrDACmd(UART4, ENABLE);
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

