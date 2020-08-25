/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： 调试库函数
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
******************************************************************************/

/*************头文件**********************************************************/
#include "stm32f2xx.h"
#include "stm32f2xx_ip_dbg.h"
#include "stm32_eval.h"
#include <stdio.h>

#pragma import(__use_no_semihosting_swi)

/* 重定义 -------------------------------------------------------------------*/
/* 自定义 -------------------------------------------------------------------*/
/* 私有宏定义 ---------------------------------------------------------------*/
/* 私有变量 -----------------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;

/* 私有函数原型 -----------------------------------------------*/
#ifdef __GNUC__
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif

/****************************************************************************
* 函数名: main()
* 功 能: 
* 输 入: 无
* 输 出: 无
****************************************************************************/
int main(void)
{
	/*在此阶段处理器的时钟已经被初始化完成，时钟初始化工作由SystemInit函数实现，
	SystemInit函数存放在startup_stm32f2xx.s的汇编启动文件中。在系统上电初始化到
	跳转到main函数之前，启动代码会先调用SystemInit函数。如果用户希望重新配置时钟
	可以在此处再次调用 SystemInit()，它的函数实现存放在system_stm32f2xx.c文件中。*/ 
	   
 GPIO_InitTypeDef GPIOA_InitStructure;
 
  /* USARTx 配置如下:
        - BaudRate（波特率） = 115200 波特  
        - Word Length（字长） = 8 位
        - One Stop Bit（一个停止位）
        - No parity（无校验位）
        -硬件流控制不使能 (RTS和CTS信号)
        - 启用接收和发送
  */
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(COM1, &USART_InitStructure);
  printf("\r\n STM32F2xx Firmware Library compiled with FULL ASSERT function... \n\r");
  /* 初始化所有外设指针 */
  IP_Debug();
  
  printf("\r\n STM32F2xx Firmware Library compiled with FULL ASSERT function... \n\r");
  printf("...Run-time checking enabled  \n\r");

  /* 模拟错误的参数传递给库函数 ---------------------*/
  /* 使能SPI1时钟, RCC_APB2PeriphClockCmd()函数必须被使用并且注意不是RCC_APB1PeriphClockCmd() */
  RCC_APB1PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  
  /* GPIOA_InitStructure结构体的 一些成员并没有初始化*/
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
* 函数名：AssertFailed
* 功  能：报告参数错误的位置
* 输  入：错误所在的文件名和错误所在文件中的行号
* 输  出：无  
* 备  注：无
********************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
	/******此处可以添加用户错误报告方式******/
  printf("\n\r Wrong parameter value detected on\r\n");
  printf("       file  %s\r\n", file);
  printf("       line  %d\r\n", line);

}
#endif

/******************************************************************************
* 函数名：PUTCHAR_PROTOTYPE
* 功  能：用于输出C函数库printf不能输出的字符
* 输  入：
* 输  出：无  
********************************************************************************/
PUTCHAR_PROTOTYPE
{
  /* 在此添加要输出的字符 */
  /* 向串口写一个字符 */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* 循环直到传输结束 */
  while(USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {
  }

  return ch;
}

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
