/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： moto.c
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
#include "moto.h"

/*************变量定义********************************************************/
u8 motoflag=0;
unsigned int motofreq=100;

/****************************************************************************
* 函数名: Moto_GPIO_Configuration()
* 功 能: 配置SPI2 相应管脚为SPI 功能模式
* 输 入: 无
* 输 出: 无
****************************************************************************/
void Moto_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOA|
						   RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOD, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	
	GPIO_Init(GPIOF,&GPIO_InitStructure);
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(GPIOC,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(GPIOB,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

	GPIO_Init(GPIOG,&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_Init(GPIOD,&GPIO_InitStructure);

}

/****************************************************************************
* 函数名: Moto_NVIC_Configuration()
* 功 能: 配置嵌套向量中断控制器
* 输 入: 无
* 输 出: 无
****************************************************************************/
void Moto_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* 使能TIM2全部中断 */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* 函数名: Moto_TIM2_Configuration()
* 功 能: 配置嵌套向量中断控制器
* 输 入: 无
* 输 出: 无
****************************************************************************/
void Moto_TIM2_Configuration(int freq)
{			 
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* ---------------------------------------------------------------
    TIM2配置:输出计时模式进行比较:
    TIM2计数器时钟频率 120 MHz  120000分频为1kh
    --------------------------------------------------------------- */

    /* 使能TIM2时钟 */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* 配置扫描发生器 */
    TIM_TimeBaseStructure.TIM_Period =10000/freq;
    TIM_TimeBaseStructure.TIM_Prescaler = 2999;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

/****************************************************************************
* 函数名: moto_test()
* 功 能: 电机测试
* 输 入: 无
* 输 出: 无
****************************************************************************/
void moto_test(void)
{
	unsigned int motofreqtemp;
	Moto_GPIO_Configuration();
	Moto_NVIC_Configuration();
	moto_lcd_msg();
	DelayMs(1000);
	GPIO_ResetBits(GPIOF, GPIO_Pin_0);
	DelayMs(200);
	GPIO_SetBits(GPIOF, GPIO_Pin_0);
    GPIO_SetBits(GPIOD, GPIO_Pin_3);
	GPIO_SetBits(GPIOG, GPIO_Pin_15);
	DelayMs(600);
	Moto_TIM2_Configuration(motofreq);
	motofreqtemp=motofreq;
	while(moto_run)
	{
		if(motofreqtemp!=motofreq)
		{
			motofreqtemp=motofreq;
			Moto_TIM2_Configuration(motofreq);
			LCD_Clear(Blue);
			moto_lcd_msg();	
		}
		if(motoflag==0)
		{		
			GPIO_ResetBits(GPIOC, GPIO_Pin_0);
		}
		else if(motoflag==1)
		{
			GPIO_SetBits(GPIOC, GPIO_Pin_0);
		}
		if(KeyCode==0x5a)
		{
			KeyCode=0;
			if(motofreq>10)
			{
				motofreq-=10;
			}else
			{
				motofreq=10;
			}

		}
		if(KeyCode==0x5b)
		{
			KeyCode=0;
			if(motofreq<1000)
			{
				motofreq+=10;
			}else
			{
				motofreq=1000;
			}
		}
	}
}
/****************************************************************************
* 函数名: moto_lcd_msg()
* 功 能: 电机测试界面
* 输 入: 无
* 输 出: 无
****************************************************************************/
void moto_lcd_msg(void)
{
	char buf[0x64];
	LCD_Clear(Blue);  			/* 清屏，背景蓝色 */
	LCD_SetBackColor(Blue);		/* 设置文字背景颜色 */
	LCD_SetTextColor(White);	/* 设置文字颜色 */

	sprintf(buf,"/====================================/");
	LCD_DisplayStringHZ(8, 8,(uint8_t *)buf);

	sprintf(buf,"=             步进电机测试           =");
	LCD_DisplayStringHZ(8, 37,(uint8_t *)buf);

	sprintf(buf,"/====================================/");
	LCD_DisplayStringHZ(8, 66,(uint8_t *)buf);

	LCD_Draw_Eflag_Logo(18,24);
	LCD_DrawRect(8,90,140,300);

	LCD_SetTextColor(Red);
	sprintf(buf,"步进电机转速/分钟:");
	LCD_DisplayStringHZ(40,100,(uint8_t *)buf);
	LCD_SetTextColor(Yellow);
	sprintf(buf,"%4d",(int)1.5*motofreq);
	LCD_DisplayStringLargeFont(60,130,(uint8_t *)buf);
}

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*****************/
