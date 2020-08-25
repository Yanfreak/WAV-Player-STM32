/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： Motor.c
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
#include "stm32f2xx_tim.h"
#include "Motor.h"

/****************************************************************************
* 函数名: Motor_Init()
* 功 能: 设置IO
* 输 入: 无
* 输 出: 无
****************************************************************************/
void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	//电源
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	//电源
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	//Configure PG11 PG pin: L297_RESET pin
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	//TIM2 channel 2 pin (PA0) 速度
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*服用管脚*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
}

/****************************************************************************
* 函数名: 电机的正转、反转和停止
* 功 能:  控制电机的转动
* 输 入: 无
* 输 出: 无
****************************************************************************/
void Motor_Forward(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_0);/*电机顺时针旋转*/
	GPIO_SetBits(GPIOG, GPIO_Pin_15); /*电机顺时针旋转*/
}

void Motor_Back(void)
{
	GPIO_ResetBits(GPIOG, GPIO_Pin_15);/*电机逆时针旋转*/
	GPIO_SetBits(GPIOC, GPIO_Pin_0); /*电机逆时针旋转*/
}

void Motor_Stop(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_0);/*电机停止*/
	GPIO_SetBits(GPIOG, GPIO_Pin_15); /*电机停止*/
}

/****************************************************************************
* 函数名: TIM3_Configuration()
* 功 能:  配置定时器
* 输 入: Pulse的值可以设置为0----100,占空比为Pulse%，比如Pulse=1，那么占空比为1%
* 输 出: 无
****************************************************************************/
void TIM3_Configuration(uint Pulse)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	  TIM_OCInitTypeDef  TIM_OCInitStructure;
    
      SystemInit();
      RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
      RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
  
	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 ;
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
	  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	 
	  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_TIM3);

	  TIM_TimeBaseStructure.TIM_Period =1200-1;//0X0000--0XFFFF决定输出频率
	  TIM_TimeBaseStructure.TIM_Prescaler = 1000;  //预分频
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	  	
	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  
	  /*占空比公式TIM3_CCRx/（TIM3_Period+1），TIM_OCInitStructure.TIM_Pulse设置的是TIM3_CCRx值*/
	  /*占空比1%时，Pulse = 1 时, TIM_Pulse = 120；占空比100%时，Pulse = 100 时, TIM_Pulse = 1200*/
      TIM_OCInitStructure.TIM_Pulse = ((TIM_TimeBaseStructure.TIM_Period +1)* Pulse) / BRIGHT_MAX;	/* 改变占空比 */
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	  TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//设置为第四通道
	  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	  TIM_ARRPreloadConfig(TIM3, ENABLE);
	  TIM_Cmd(TIM3, ENABLE);
}

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*****************/
