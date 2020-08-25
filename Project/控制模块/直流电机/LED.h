/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： led.h
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

/*************宏定义**********************************************************/
#define LED1_0 GPIO_ResetBits(GPIOF,GPIO_Pin_6)	 /*灯亮*/
#define LED1_1 GPIO_SetBits(GPIOF,GPIO_Pin_6)	

#define LED2_0 GPIO_ResetBits(GPIOF,GPIO_Pin_7)  
#define LED2_1 GPIO_SetBits(GPIOF,GPIO_Pin_7)	 

#define LED3_0 GPIO_ResetBits(GPIOF,GPIO_Pin_10)
#define LED3_1 GPIO_SetBits(GPIOF,GPIO_Pin_10)

#define LED4_0 GPIO_ResetBits(GPIOF,GPIO_Pin_9)
#define LED4_1 GPIO_SetBits(GPIOF,GPIO_Pin_9)

/*************函数声明********************************************************/
void LED_Init(void);

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*****************/
