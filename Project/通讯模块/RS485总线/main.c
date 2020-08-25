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
#include "stm322xg_eval_systick.h"
#include <stdio.h>
#include <string.h>

/* 类型定义  -------------------------------------------------------------------*/

/* 宏定义  ---------------------------------------------------------------------*/

#define SEND_MODE		1	   /* RS485发送模式*/
#define RECV_MODE		2	   /* RS485接收模式*/

//#define CONFIGTYPE	    SEND_MODE  /* 配置工程为 RS485发送数据模式*/
#define CONFIGTYPE		RECV_MODE	   /* 配置工程为 RS485接收数据模式*/

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
void RS485_Control(int mode);
void Display_Init(void);
void Init_GPIO_Config(void);
void Init_Uart_Config(void);
/******************************************************************************
* 函数名：main 
* 功  能：实现RS485的发送和接收功能 
* 输  入：无 
* 输  出：无  
* 备  注：因为RS485为半双工方式，所以通过定义CONFIGTYPE（在第36行定义）来选择发送和接收功能
********************************************************************************/
int main(void)
{
	int ch,line_cnt;
	char buf[50] = {0};
	int cnt = 0;
	int i=0;
	Display_Init();			   /* 配置液晶显示RS485工作界面*/
	Init_GPIO_Config();		   /* 配置RS485使用USART2的GPIO管脚*/
	Init_Uart_Config();		   /* 配置RS485使用USART2*/
 	
#if (CONFIGTYPE == SEND_MODE)  /* 实现RS485发送数据功能*/
	sprintf(buf, "    Send = %d", i); /* 将字符串信息存放在buf中*/
	LCD_DisplayStringLine(LINE(4), (uint8_t *)buf);	   /* 将buf中的字符串信息显示在液晶的第4行*/
	RS485_Control(SEND_MODE);  /* 控制RS485的收发器为 发送数据模式*/
	while (1) {
		printf(" %d   \n", i++);  	  /* 向RS485发送数据*/
		delay_ms(1000);									  /* 延时1秒*/
		sprintf(buf, "    Send = %d", i);				  /* 将字符串信息存放在buf中*/
		LCD_DisplayStringLine(LINE(4), (uint8_t *)buf);	  /* 将buf中的字符串信息显示在液晶的第4行*/
	}
#endif

#if (CONFIGTYPE == RECV_MODE)	/* 实现RS485接收数据功能*/
	ch = -1;
	cnt = 0;
	line_cnt = 2;
	RS485_Control(RECV_MODE);	/* 控制RS485的收发器为 接收数据模式*/
	LCD_DisplayStringLine(LINE(1), "Receive Info :      ");
	while (1) {
		while (1) {
			/* 判断USART2的状态寄存器是否接收到数据*/
			while (USART_GetFlagStatus(USART2, USART_FLAG_RXNE) == RESET){}
			ch = USART_ReceiveData(USART2);	  /* 将接收的数据存放在ch变量中*/

			/* 如果收到'\r'或'\n'或累计连续接收到20个字节就跳出循环*/
			if ((ch == '\r') || (ch == '\n') || (cnt >= 20)) {
				break;
			}
			if (ch != 0) {
				buf[cnt++] = ch;   /* 将ch存放入buf中*/
			}
		}
		buf[cnt] = 0;
		
		/* 显示接收字符信息*/
		LCD_DisplayStringLine(LINE(line_cnt), (uint8_t *)buf);

		line_cnt++;
		if(line_cnt == 10)	/* 如果液晶屏幕显示信息已满，则清空屏幕*/
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
  USART_SendData(USART2, (uint8_t) ch);

  /* 循环等待，直到串口数据发送结束 */
  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

/******************************************************************************
* 函数名：Display_Init 
* 功  能：显示RS485操作界面 
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
  LCD_DisplayStringLine(LINE(0x13), "          USART RS485 example            ");

  /* 设置字体 */
  LCD_SetFont(&Font16x24);

  /* 显示字符串 */
  LCD_DisplayStringLine(LINE(0), "     RS485 9600bps   ");

  /* 设置液晶背景色为白，文本色为蓝色*/
  LCD_SetBackColor(White);
  LCD_SetTextColor(Blue);
}

/******************************************************************************
* 函数名：Init_GPIO_Config 
* 功  能：配置Rs485使用的USART2的GPIO管脚复用，以及收发器的方向控制管脚PF8 
* 输  入：无 
* 输  出：无  
* 备  注：无
********************************************************************************/
void Init_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能串口管脚的时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* 将发送管脚复用到串口*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_USART2);

	/* 将接收管脚复用到串口*/
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3,GPIO_AF_USART2);

	/* 配置串口对应的管脚  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置串口对应的管脚为复用功能  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	/* 配置RS485接收和发送控制管脚 PF8*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/******************************************************************************
* 函数名：RS485_Control 
* 功  能：控制RS485的接收和发送 
* 输  入：int mode，SEND_MODE为发送 ，RECV_MODE为接收
* 输  出：无  
* 备  注：无
********************************************************************************/

void  RS485_Control(int mode)
{
	if(mode == SEND_MODE)	 /* RS485为发送模式 */
	{
	 	GPIO_SetBits(GPIOF, GPIO_Pin_8);	 /* PF8 高电平*/
	}
	else					 /* RS485为接收模式 */
	{
		GPIO_ResetBits(GPIOF, GPIO_Pin_8);	 /* PF8 低电平*/

	}

}
/******************************************************************************
* 函数名：Init_Uart_Config 
* 功  能：初始化RS485使用的USART2的参数 
* 输  入：无 
* 输  出：无  
* 备  注：无
********************************************************************************/
void Init_Uart_Config(void)
{
	USART_InitTypeDef USART_InitStructure;  /*定义串口初始化结构体 */

	/* 使能串口的时钟 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

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

	/* 串口配置 */
	USART_Init(USART2, &USART_InitStructure);

	/* 使能串口 */
	USART_Cmd(USART2, ENABLE);
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
