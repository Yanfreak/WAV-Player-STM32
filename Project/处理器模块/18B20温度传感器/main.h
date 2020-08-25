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
#ifndef __MAIN_H
#define __MAIN_H

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "stm32_eval.h"

/* 函数声明 ------------------------------------------------------- */
void RTC_Configuration(void);
void RTC_TimeRegulate(void);
void RTC_TimeSchow(void);
uint8_t USART_Scanf(uint32_t value);

#ifdef __GNUC__
/*************输出重定向******************************/
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#endif /* __MAIN_H */

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
