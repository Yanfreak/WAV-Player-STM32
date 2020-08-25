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

#include "stm32f2xx.h"
#include "stm322xg_eval_keypad.h"
#include "stm322xg_eval_lcd.h"
#include "systick.h"
#include "stm32f2xx_exti.h"
#include "moto.h"
#include <stdio.h>

extern unsigned char KeyCode;
uint8_t moto_run;

static void RCC_Configuration(void);
static void EXTI_Configuration(void);
static void NVIC_Configuration(void);
static void main_lcd_msg(void);

#endif

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*****************/
