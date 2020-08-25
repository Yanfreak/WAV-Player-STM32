/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� Motor.c
*   ժ Ҫ    �� 
*
*   ��ǰ�汾 �� 
*   �� ��    �� EFLAG
*   ������� �� 
*
*   ȡ���汾 ��
*   ԭ����   �� 
*   �޸����� ��
*   ������� ��2012.08.01 
*
******************************************************************************/

/*************ͷ�ļ�**********************************************************/
#include "stm32f2xx.h"
#include "stm32f2xx_tim.h"
#include "Motor.h"

/****************************************************************************
* ������: Motor_Init()
* �� ��: ����IO
* �� ��: ��
* �� ��: ��
****************************************************************************/
void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	

	//��Դ
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	//��Դ
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

	//TIM2 channel 2 pin (PA0) �ٶ�
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; 	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*���ùܽ�*/
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM2);
}

/****************************************************************************
* ������: �������ת����ת��ֹͣ
* �� ��:  ���Ƶ����ת��
* �� ��: ��
* �� ��: ��
****************************************************************************/
void Motor_Forward(void)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_0);/*���˳ʱ����ת*/
	GPIO_SetBits(GPIOG, GPIO_Pin_15); /*���˳ʱ����ת*/
}

void Motor_Back(void)
{
	GPIO_ResetBits(GPIOG, GPIO_Pin_15);/*�����ʱ����ת*/
	GPIO_SetBits(GPIOC, GPIO_Pin_0); /*�����ʱ����ת*/
}

void Motor_Stop(void)
{
    GPIO_SetBits(GPIOC, GPIO_Pin_0);/*���ֹͣ*/
	GPIO_SetBits(GPIOG, GPIO_Pin_15); /*���ֹͣ*/
}

/****************************************************************************
* ������: TIM3_Configuration()
* �� ��:  ���ö�ʱ��
* �� ��: Pulse��ֵ��������Ϊ0----100,ռ�ձ�ΪPulse%������Pulse=1����ôռ�ձ�Ϊ1%
* �� ��: ��
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

	  TIM_TimeBaseStructure.TIM_Period =1200-1;//0X0000--0XFFFF�������Ƶ��
	  TIM_TimeBaseStructure.TIM_Prescaler = 1000;  //Ԥ��Ƶ
	  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	  	
	  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	  
	  /*ռ�ձȹ�ʽTIM3_CCRx/��TIM3_Period+1����TIM_OCInitStructure.TIM_Pulse���õ���TIM3_CCRxֵ*/
	  /*ռ�ձ�1%ʱ��Pulse = 1 ʱ, TIM_Pulse = 120��ռ�ձ�100%ʱ��Pulse = 100 ʱ, TIM_Pulse = 1200*/
      TIM_OCInitStructure.TIM_Pulse = ((TIM_TimeBaseStructure.TIM_Period +1)* Pulse) / BRIGHT_MAX;	/* �ı�ռ�ձ� */
	  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	  TIM_OC4Init(TIM3, &TIM_OCInitStructure);	//����Ϊ����ͨ��
	  TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	  TIM_ARRPreloadConfig(TIM3, ENABLE);
	  TIM_Cmd(TIM3, ENABLE);
}

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*****************/
