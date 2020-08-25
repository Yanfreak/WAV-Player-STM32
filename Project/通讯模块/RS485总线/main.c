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
#include "stm322xg_eval_systick.h"
#include <stdio.h>
#include <string.h>

/* ���Ͷ���  -------------------------------------------------------------------*/

/* �궨��  ---------------------------------------------------------------------*/

#define SEND_MODE		1	   /* RS485����ģʽ*/
#define RECV_MODE		2	   /* RS485����ģʽ*/

//#define CONFIGTYPE	    SEND_MODE  /* ���ù���Ϊ RS485��������ģʽ*/
#define CONFIGTYPE		RECV_MODE	   /* ���ù���Ϊ RS485��������ģʽ*/

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
void RS485_Control(int mode);
void Display_Init(void);
void Init_GPIO_Config(void);
void Init_Uart_Config(void);
/******************************************************************************
* ��������main 
* ��  �ܣ�ʵ��RS485�ķ��ͺͽ��չ��� 
* ��  �룺�� 
* ��  ������  
* ��  ע����ΪRS485Ϊ��˫����ʽ������ͨ������CONFIGTYPE���ڵ�36�ж��壩��ѡ���ͺͽ��չ���
********************************************************************************/
int main(void)
{
	int ch,line_cnt;
	char buf[50] = {0};
	int cnt = 0;
	int i=0;
	Display_Init();			   /* ����Һ����ʾRS485��������*/
	Init_GPIO_Config();		   /* ����RS485ʹ��USART2��GPIO�ܽ�*/
	Init_Uart_Config();		   /* ����RS485ʹ��USART2*/
 	
#if (CONFIGTYPE == SEND_MODE)  /* ʵ��RS485�������ݹ���*/
	sprintf(buf, "    Send = %d", i); /* ���ַ�����Ϣ�����buf��*/
	LCD_DisplayStringLine(LINE(4), (uint8_t *)buf);	   /* ��buf�е��ַ�����Ϣ��ʾ��Һ���ĵ�4��*/
	RS485_Control(SEND_MODE);  /* ����RS485���շ���Ϊ ��������ģʽ*/
	while (1) {
		printf(" %d   \n", i++);  	  /* ��RS485��������*/
		delay_ms(1000);									  /* ��ʱ1��*/
		sprintf(buf, "    Send = %d", i);				  /* ���ַ�����Ϣ�����buf��*/
		LCD_DisplayStringLine(LINE(4), (uint8_t *)buf);	  /* ��buf�е��ַ�����Ϣ��ʾ��Һ���ĵ�4��*/
	}
#endif

#if (CONFIGTYPE == RECV_MODE)	/* ʵ��RS485�������ݹ���*/
	ch = -1;
	cnt = 0;
	line_cnt = 2;
	RS485_Control(RECV_MODE);	/* ����RS485���շ���Ϊ ��������ģʽ*/
	LCD_DisplayStringLine(LINE(1), "Receive Info :      ");
	while (1) {
		while (1) {
			/* �ж�USART2��״̬�Ĵ����Ƿ���յ�����*/
			while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET){}
			ch = USART_ReceiveData(USART2);	  /* �����յ����ݴ����ch������*/

			/* ����յ�'\r'��'\n'���ۼ��������յ�20���ֽھ�����ѭ��*/
			if ((ch == '\r') || (ch == '\n') || (cnt >= 20)) {
				break;
			}
			if (ch != 0) {
				buf[cnt++] = ch;   /* ��ch�����buf��*/
			}
		}
		buf[cnt] = 0;
		
		/* ��ʾ�����ַ���Ϣ*/
		LCD_DisplayStringLine(LINE(line_cnt), (uint8_t *)buf);

		line_cnt++;
		if(line_cnt == 10)	/* ���Һ����Ļ��ʾ��Ϣ�������������Ļ*/
		{
			Display_Init();
			LCD_DisplayStringLine(LINE(1), "Receive Info :      ");
			line_cnt = 2;
		}
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
  USART_SendData(USART2, (uint8_t) ch);

  /* ѭ���ȴ���ֱ���������ݷ��ͽ��� */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

/******************************************************************************
* ��������Display_Init 
* ��  �ܣ���ʾRS485�������� 
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
  LCD_DisplayStringLine(LINE(0x13), "          USART RS485 example            ");

  /* �������� */
  LCD_SetFont(&Font16x24);

  /* ��ʾ�ַ��� */
  LCD_DisplayStringLine(LINE(0), "     RS485 9600bps   ");

  /* ����Һ������ɫΪ�ף��ı�ɫΪ��ɫ*/
  LCD_SetBackColor(White);
  LCD_SetTextColor(Blue);
}

/******************************************************************************
* ��������Init_GPIO_Config 
* ��  �ܣ�����Rs485ʹ�õ�USART2��GPIO�ܽŸ��ã��Լ��շ����ķ�����ƹܽ�PF8 
* ��  �룺�� 
* ��  ������  
* ��  ע����
********************************************************************************/
void Init_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ�ܴ��ڹܽŵ�ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* �����͹ܽŸ��õ�����*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);

	/* �����չܽŸ��õ�����*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

	/* ���ô��ڶ�Ӧ�Ĺܽ�  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ���ô��ڶ�Ӧ�Ĺܽ�Ϊ���ù���  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	/* ����RS485���պͷ��Ϳ��ƹܽ� PF8*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/******************************************************************************
* ��������RS485_Control 
* ��  �ܣ�����RS485�Ľ��պͷ��� 
* ��  �룺int mode��SEND_MODEΪ���� ��RECV_MODEΪ����
* ��  ������  
* ��  ע����
********************************************************************************/

void  RS485_Control(int mode)
{
	if(mode == SEND_MODE)	 /* RS485Ϊ����ģʽ */
	{
	 	GPIO_SetBits(GPIOF, GPIO_Pin_8);	 /* PF8 �ߵ�ƽ*/
	}
	else					 /* RS485Ϊ����ģʽ */
	{
		GPIO_ResetBits(GPIOF, GPIO_Pin_8);	 /* PF8 �͵�ƽ*/

	}

}
/******************************************************************************
* ��������Init_Uart_Config 
* ��  �ܣ���ʼ��RS485ʹ�õ�USART2�Ĳ��� 
* ��  �룺�� 
* ��  ������  
* ��  ע����
********************************************************************************/
void Init_Uart_Config(void)
{
	USART_InitTypeDef USART_InitStructure;  /*���崮�ڳ�ʼ���ṹ�� */

	/* ʹ�ܴ��ڵ�ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

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

	/* �������� */
	USART_Init(USART2, &USART_InitStructure);

	/* ʹ�ܴ��� */
	USART_Cmd(USART2, ENABLE);
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
