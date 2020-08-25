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
 void Delay(u32 nTime)			//毫秒
{
	u32 i;
	for(i=0;i<nTime*200;i++);
}

/****************************************************************************
* 函数名: SW_EXTI()
* 功 能: 中断
* 输 入: 无
* 输 出: 无
****************************************************************************/
void SW_EXTI(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;	//中断
	EXTI_InitTypeDef EXTI_InitStructure;	//外部中断线

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);	//系统时钟必须使能
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOG, EXTI_PinSource15); //选择PG15所在的GPIO管脚用作外部中断线路EXIT15		   
	 			
	EXTI_InitStructure.EXTI_Line = EXTI_Line15;	//外部线路EXIT15
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//设外外部中断模式,设置 EXTI线路为事件请求 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  //外部中断触发沿选择:设置输入线路下降沿为中断请求
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能外部中断新状态
	EXTI_Init(&EXTI_InitStructure);	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器		  

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //使能按键所在的外部中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级1级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
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
	SW_EXTI();

	/*拉高电压*/
	GPIO_SetBits(GPIOA,GPIO_Pin_0);	
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	GPIO_SetBits(GPIOD,GPIO_Pin_3);
	GPIO_SetBits(GPIOG,GPIO_Pin_15);

	while(1)
	{	
	 
	}
}

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*************/
