 /****************************************************************************
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
******************************************************************************/

/*************头文件**********************************************************/
#include "main.h"

/****************************************************************************
* 函数名: RCC_Configuration()
* 功 能: 配置不同的系统时钟
* 输 入: 无 
* 输 出: 无
****************************************************************************/
void RCC_Configuration(void)
{
 	/*在此阶段处理器的时钟已经被初始化完成，时钟初始化工作由SystemInit函数实现，SystemInit函数存放在startup_stm32f2xx.s的汇编启动文件中。
    *在系统上电初始化到跳转到main函数之前，启动代码会先调用SystemInit函数。	
    *如果用户希望重新配置时钟可以在此处再次调用 SystemInit()，它的函数实现存放在system_stm32f2xx.c文件中。*/
    SystemInit();

    RCC_PCLK2Config(RCC_HCLK_Div2); /* PCLK2 = HCLK/2 */

	/* 使能时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC
							|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOG, ENABLE);
}

/****************************************************************************
* 函数名: GPIO_Configuration()
* 功 能: 配置不同的GPIO端口
* 输 入: 无 
* 输 出: 无
****************************************************************************/
void GPIO_Configuration(void)
{ 
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	//IN0，拉低灯亮 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;	 //蜂鸣，拉低蜂鸣响
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	//OUT0，拉低灯亮  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3;	 //OUT1，拉低灯亮	   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15;	 //IN1，拉低灯亮 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

}

/******************************************************************************
* 函数名：main
* 功  能：
* 输  入：无
* 输  出：无  
* 备  注：无
********************************************************************************/
int main(void)
{ 
	RCC_Configuration();
	GPIO_Configuration();

	GPIO_ResetBits(GPIOA,GPIO_Pin_0);	//拉低
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	GPIO_ResetBits(GPIOD,GPIO_Pin_3);
	GPIO_ResetBits(GPIOG,GPIO_Pin_15);
}

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*************/
