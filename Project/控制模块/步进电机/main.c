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

/******************************************************************************
* 函数名：main
* 功  能：主函数
* 输  入：无
* 输  出：无  
* 备  注：无
********************************************************************************/
int main(void)
{
	RCC_Configuration();
	EXTI_Configuration();
	NVIC_Configuration();

	STM3220F_LCD_Init();
	SysTick_Config(SystemCoreClock / 1000);
	main_lcd_msg();
	InitKey();
	while(1)
	{
		switch(KeyCode)
		{
			case 0x40:
				GPIO_SetBits(GPIOA, GPIO_Pin_15);
				moto_run=1;
				moto_test();
				GPIO_ResetBits(GPIOA, GPIO_Pin_15);
				main_lcd_msg();
				break;
			default:
				break;
		}
	}
}

/******************************************************************************
* 函数名：main_lcd_msg
* 功  能：开始界面
* 输  入：无
* 输  出：无  
********************************************************************************/
static void main_lcd_msg(void)
{
	char buf[0x64];
	LCD_Clear(Blue);  			/* 清屏，背景蓝色 */
	LCD_SetBackColor(Blue);		/* 设置文字背景颜色 */
	LCD_SetTextColor(White);	/* 设置文字颜色 */

	sprintf(buf,"/====================================/");
	LCD_DisplayStringHZ(8, 8,(uint8_t *)buf);

	sprintf(buf,"=               步进电机电机测试     =");
	LCD_DisplayStringHZ(8, 37,(uint8_t *)buf);

	sprintf(buf,"/====================================/");
	LCD_DisplayStringHZ(8, 66,(uint8_t *)buf);

	LCD_Draw_Eflag_Logo(18,24);
	LCD_DrawRect(8,90,140,300);

	LCD_SetTextColor(Yellow);
	sprintf(buf,"按键1:电机测试");
	LCD_DisplayStringHZ(100,130,(uint8_t *)buf);
	LCD_SetTextColor(Yellow);
	sprintf(buf,"按键8:返回主菜单");
	LCD_DisplayStringHZ(100,160,(uint8_t *)buf);	
}

/****************************************************************************
* 函数名: RCC_Configuration()
* 功 能: 设置系统时钟
* 输 入: 无
* 输 出: 无
****************************************************************************/
static void RCC_Configuration(void)
{
	/*在此阶段处理器的时钟已经被初始化完成，时钟初始化工作由SystemInit函数实现，
	 SystemInit函数存放在startup_stm32f2xx.s的汇编启动文件中。在系统上电初始化
	 到跳转到main函数之前，启动代码会先调用SystemInit函数。如果用户希望重新配置
	 时钟可以在此处再次调用 SystemInit()，它的函数实现存放在system_stm32f2xx.c文
	 件中。*/

	SystemInit();

	/* PCLK2 = HCLK/2 */
	RCC_PCLK2Config(RCC_HCLK_Div2);

	/* 使外围时钟 */
    /* 使能GPIOA,GPIOB和SPI2时钟 */
    RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
}

/****************************************************************************
* 函数名: EXTI_Configuration()
* 功 能: 中断函数
* 输 入: 无
* 输 出: 无
****************************************************************************/
static void EXTI_Configuration(void)
{
    EXTI_InitTypeDef   EXTI_InitStructure;
    GPIO_InitTypeDef   GPIO_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;// 按键中断
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_Init(GPIOF, &GPIO_InitStructure);

    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource11);

    EXTI_InitStructure.EXTI_Line = EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);								
}	

/****************************************************************************
* 函数名: NVIC_Configuration()
* 功 能: 初始化中断向量表
* 输 入: 无
* 输 出: 无
****************************************************************************/
 static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;  

#ifdef  VECT_TAB_RAM  
	/* 设置矢量表基地址 0x20000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else 
	/* 设置矢量表基地址 0x08000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  /* 抢占式优先级别设置为无抢占优先级 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  /* 设置TIM2通道输入中断 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; /* 配置优先级组 */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   /* 允许TIM2全局中断 */
	NVIC_Init(&NVIC_InitStructure);
}

#ifdef  USE_FULL_ASSERT

/******************************************************************************
* 函数名：assert_failed()
* 功  能：报告参数错误的位置
* 输  入：错误所在的文件名和错误所在文件中的行号
* 输  出：无  
* 备  注：无
********************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{

    /******此处可以添加用户错误报告方式******/
	while (1)
	{}
}
#endif

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*****************/
