 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： main.c
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
**********************************************************/
/* 头文件 ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "stm32_eval.h"
#include "stm322xg_eval_lcd.h"
#include <stdio.h>
#include <string.h>

/* 类型定义  -------------------------------------------------------------------*/

/* 宏定义  ---------------------------------------------------------------------*/

#define SEND_MODE		1				  /* 红外IRDA发送模式*/

#define RECV_MODE		2				  /* 红外IRDA接收模式*/

//#define configTYPE	    SEND_MODE	  /* 配置工程为 红外IRDA发送数据模式*/
#define configTYPE		RECV_MODE		  /* 配置工程为 红外IRDA接收数据模式*/


/* 定义 PUTCHAR_PROTOTYPE 函数*/
#ifdef __GNUC__	  
  /* 将 PUTCHAR_PROTOTYPE 定义为 __io_putchar函数 ，在 GCC/RAISONANCE模式下，采用small printf实现方法
  *  可以通过 LD Linker->Libraries->Small printf 选项 设置 'Yes'，来实现printf调用 __io_putchar() 
  */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  /* 默认情况下，printf函数会调用fputc（）函数*/
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* 变量定义 --------------------------------------------------------------------*/

/* 函数定义---------------------------------------------------------------------*/
void Display_Init(void);
void Uart_Irda_Configuration(void);

/******************************************************************************
* 函数名：main 
* 功  能：实现红外IRDA的发送和接收功能 
* 输  入：无 
* 输  出：无  
* 备  注：因为红外IRDA为半双工方式，所以通过定义CONFIGTYPE（在第34行定义）来选择发送和接收功能
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

	Display_Init();					   /* 配置液晶显示红外IRDA工作界面*/
	Uart_Irda_Configuration();		   /* 配置红外IRDA使用USART4*/

 	
#if (configTYPE == SEND_MODE)		   /* 实现红外IRDA发送数据功能*/
	i = 0;
	sprintf(text, "    Send = %d", i);  /* 格式化字符串信息存放在text中*/
	LCD_DisplayStringLine(LINE(4), (uint8_t *)text); /* 将buf中的字符串信息显示在液晶的第4行*/
	while (1) {
		printf(" Send %d   \n\r", i++);	/* 向红外IRDA发送数据*/
		delay = 5000000;
		while (delay--);			    /* 延时 */
		sprintf(text, "    Send = %d", i);	/* 格式化字符串信息存放在text中*/
		LCD_DisplayStringLine(LINE(4), (uint8_t *)text);   /* 将text中的字符串信息显示在液晶的第4行*/
	}
#endif

#if (configTYPE == RECV_MODE)	/* 实现红外IRDA接收数据功能*/
	ch = -1;
	cnt = 0;
	LCD_DisplayStringLine(LINE(3), " Receive Info：");
	while (1) {
		while (1) {
			/* 判断USART4的状态寄存器是否接收到数据*/
			while (USART_GetFlagStatus(UART4, USART_FLAG_RXNE) == RESET){}
			ch = USART_ReceiveData(UART4);	/* 将接收的数据存放在ch变量中*/

			/* 如果收到'\r'或'\n'或累计连续接收到128个字节就跳出循环*/
			if ((ch == '\r') || (ch == '\n') || (cnt >= 128)) {
				break;
			}
			if (ch != 0) {
				buf[cnt++] = ch;   /* 将ch存放入buf中*/
			}
		}
		buf[cnt] = 0;
		LCD_DisplayStringLine(LINE(5), (uint8_t *)buf);	 /* 显示接收字符信息*/
		cnt = 0;
		memset(buf, 0, sizeof(buf));
	}
#endif
}

/******************************************************************************
* 函数名：PUTCHAR_PROTOTYPE  
* 功  能：重定向 C 标准库的printf功能由USART串口输出信息 
* 输  入：int ch ：输出的字符（由库函数调用）
* 输  出：int  ch  
* 备  注：文件开始处条件定义PUTCHAR_PROTOTYPE为__io_putchar(int ch)函数或fputc(int ch, FILE *f)函数
********************************************************************************/
PUTCHAR_PROTOTYPE
{
  /* 此处填写fputc的实现代码 */
  /* 例如： 从串口发送一个字符 */
  USART_SendData(UART4, (uint8_t) ch);

  /* 循环等待，直到串口数据发送结束 */
  while (USART_GetFlagStatus(UART4, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

/******************************************************************************
* 函数名：Display_Init 
* 功  能：显示红外IRDA操作界面 
* 输  入：无
* 输  出：无  
* 备  注：无
********************************************************************************/
void Display_Init(void)
{
  /* 初始化液晶屏幕 */
  STM322xG_LCD_Init();

  /* 清除屏幕为白色 */ 
  LCD_Clear(White);

  /* 选择字体 */
  LCD_SetFont(&Font8x12);

  /* 设置液晶背景色为蓝色，文本色为白色*/
  LCD_SetBackColor(Blue);
  LCD_SetTextColor(White);

  /* 显示字符串 */
  LCD_DisplayStringLine(LINE(0x13), "          USART IRDA example            ");

  /* 设置字体 */
  LCD_SetFont(&Font16x24);

  /* 显示字符串 */
  LCD_DisplayStringLine(LINE(0), "     IRDA 9600bps   ");

  /* 设置液晶背景色为白，文本色为蓝色*/
  LCD_SetBackColor(White);
  LCD_SetTextColor(Blue);
}

/******************************************************************************
* 函数名：Uart_Irda_Configuration 
* 功  能：配置红外使用的USART4 
* 输  入：无 
* 输  出：无  
* 备  注：无
********************************************************************************/

void Uart_Irda_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;	 /*定义串口初始化结构体 */
	/* 使能时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
	/* 复用 PC11 为 USART4_Tx*/
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_UART4);
	
	/* 复用 PC10 为 USART4_Rx*/
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10,GPIO_AF_UART4);
	/* 配置 USART Tx 为复用功能 */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);


	/* 初始化USART_InitStructure结构体参数，USART 配置为以下工作模式:
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
    
	/* 使能UART4 */
	USART_Cmd(UART4, ENABLE);
	/* 配置红外的分频 */
	USART_SetPrescaler(UART4, 10);
	/* 配置红外的工作模式 */
	USART_IrDAConfig(UART4, USART_IrDAMode_Normal);
	/* 使能红外 */	
	USART_IrDACmd(UART4, ENABLE);
}

#ifdef  USE_FULL_ASSERT

/***************************************************************
*函数名:assert_failed
*函数说明:报告参数错误的位置
*	
*输入参数:
*		file:错误所在的文件名
*		line:错误所在文件中的行号
*	
*返回值:无	
*****************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
 /****此处可以添加用户错误报告方式**********************/

  while (1)
  {
  }
}
#endif
/**
  * @}
  */


/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/

