/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： motor.h
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

#ifndef __MOTOR_H
#define __MOTOR_H

/*************宏定义**********************************************************/
#define BRIGHT_MAX		100
#define BRIGHT_MIN		0
#define BRIGHT_DEFAULT	200
#define BRIGHT_STEP		5
#define uint unsigned int

/*************函数声明********************************************************/
void Motor_Init(void);
void Motor_Forward(void);
void Motor_Back(void);
void Motor_Stop(void);
void TIM3_Configuration(uint Pulse);
void Speed(int freq,int Sure_Number);

#endif

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*****************/
