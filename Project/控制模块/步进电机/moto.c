/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� moto.c
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
#include "moto.h"

/*************��������********************************************************/
u8 motoflag=0;
unsigned int motofreq=100;

/****************************************************************************
* ������: Moto_GPIO_Configuration()
* �� ��: ����SPI2 ��Ӧ�ܽ�ΪSPI ����ģʽ
* �� ��: ��
* �� ��: ��
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
* ������: Moto_NVIC_Configuration()
* �� ��: ����Ƕ�������жϿ�����
* �� ��: ��
* �� ��: ��
****************************************************************************/
void Moto_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* ʹ��TIM2ȫ���ж� */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* ������: Moto_TIM2_Configuration()
* �� ��: ����Ƕ�������жϿ�����
* �� ��: ��
* �� ��: ��
****************************************************************************/
void Moto_TIM2_Configuration(int freq)
{			 
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	/* ---------------------------------------------------------------
    TIM2����:�����ʱģʽ���бȽ�:
    TIM2������ʱ��Ƶ�� 120 MHz  120000��ƵΪ1kh
    --------------------------------------------------------------- */

    /* ʹ��TIM2ʱ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    /* ����ɨ�跢���� */
    TIM_TimeBaseStructure.TIM_Period =10000/freq;
    TIM_TimeBaseStructure.TIM_Prescaler = 2999;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM3, ENABLE);
}

/****************************************************************************
* ������: moto_test()
* �� ��: �������
* �� ��: ��
* �� ��: ��
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
* ������: moto_lcd_msg()
* �� ��: ������Խ���
* �� ��: ��
* �� ��: ��
****************************************************************************/
void moto_lcd_msg(void)
{
	char buf[0x64];
	LCD_Clear(Blue);  			/* ������������ɫ */
	LCD_SetBackColor(Blue);		/* �������ֱ�����ɫ */
	LCD_SetTextColor(White);	/* ����������ɫ */

	sprintf(buf,"/====================================/");
	LCD_DisplayStringHZ(8, 8,(uint8_t *)buf);

	sprintf(buf,"=             �����������           =");
	LCD_DisplayStringHZ(8, 37,(uint8_t *)buf);

	sprintf(buf,"/====================================/");
	LCD_DisplayStringHZ(8, 66,(uint8_t *)buf);

	LCD_Draw_Eflag_Logo(18,24);
	LCD_DrawRect(8,90,140,300);

	LCD_SetTextColor(Red);
	sprintf(buf,"�������ת��/����:");
	LCD_DisplayStringHZ(40,100,(uint8_t *)buf);
	LCD_SetTextColor(Yellow);
	sprintf(buf,"%4d",(int)1.5*motofreq);
	LCD_DisplayStringLargeFont(60,130,(uint8_t *)buf);
}

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*****************/
