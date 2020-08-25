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
*   完成日期 ：20·12.08.01 
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
static void RCC_Configuration(void)
{
 	/*在此阶段处理器的时钟已经被初始化完成，时钟初始化工作由SystemInit函数实现，SystemInit函数存放在startup_stm32f2xx.s的汇编启动文件中。
    *在系统上电初始化到跳转到main函数之前，启动代码会先调用SystemInit函数。	
    *如果用户希望重新配置时钟可以在此处再次调用 SystemInit()，它的函数实现存放在system_stm32f2xx.c文件中。*/
    SystemInit();

    RCC_PCLK2Config(RCC_HCLK_Div2); /* PCLK2 = HCLK/2 */

	/* 使能时钟 */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC
							|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOF, ENABLE);

}

/****************************************************************************
* 函数名: GPIO_Configuration()
* 功 能: 配置不同的GPIO端口
* 输 入: 无 
* 输 出: 无
****************************************************************************/
static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	//输入，IN0，拉低灯亮
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;	//蜂鸣，拉低蜂鸣响
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	 //输出，OUT0，拉低灯亮
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3;	 //输出，OUT1，拉低灯亮	  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15;	//输入，IN1，拉低灯亮
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6|GPIO_Pin_7;	  //LED灯
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/****************************************************************************
* 函数名: Delay()
* 功 能: 延时
* 输 入: 延时时间 
* 输 出: 无
****************************************************************************/
static void Delay(u32 nTime)			//毫秒
{
	u32 i;
	for(i=0;i<nTime*200;i++);
}

/******************************************************************************
* 函数名：main
* 功  能：插入IN1和GND跳线帽，输入电压被拉低，蜂鸣响，灯灭
* 输  入：无
* 输  出：无  
* 备  注：无
********************************************************************************/
int main(void)
{
	RCC_Configuration();
	GPIO_Configuration();

	/*拉高电压*/
	GPIO_SetBits(GPIOA,GPIO_Pin_0);	
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	GPIO_SetBits(GPIOD,GPIO_Pin_3);
	GPIO_SetBits(GPIOG,GPIO_Pin_15);

	while(1)
	{
		
		/* 检测PG15管脚的电压，电压为低时执行if，电压为高时执行else */
		if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15) == 0)
			{
				GPIO_SetBits(GPIOF,GPIO_Pin_6);	  //拉高，灯灭
				GPIO_SetBits(GPIOF,GPIO_Pin_7);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1); //拉低，蜂鸣响
				Delay(20000);
				GPIO_SetBits(GPIOB,GPIO_Pin_1);	  //拉高，蜂鸣不再响
			}
		else
			{
				GPIO_ResetBits(GPIOF,GPIO_Pin_6);
				GPIO_ResetBits(GPIOF,GPIO_Pin_7);
			}
	}
}

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*************/
