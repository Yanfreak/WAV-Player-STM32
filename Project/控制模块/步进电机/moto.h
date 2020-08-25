/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： moto.h
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
#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f2xx.h"
#include "systick.h"
#include "stm322xg_eval_lcd.h"
#include "moto.h"
#include "stm32f2xx_tim.h"
#include <stdio.h>
#include "stm32_eval.h"
/*************外部变量******************************************************/	
extern unsigned char KeyCode;
extern u8 moto_run;

/*************函数声明******************************************************/	
void Moto_GPIO_Configuration(void); 
void Moto_NVIC_Configuration(void);
void Moto_TIM2_Configuration(int freq);
void moto_test(void);
void moto_lcd_msg(void);
	   	 			    
#endif

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*****************/
