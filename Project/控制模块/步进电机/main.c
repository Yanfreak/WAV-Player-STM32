/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� main.c
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
#include "main.h"

/******************************************************************************
* ��������main
* ��  �ܣ�������
* ��  �룺��
* ��  ������  
* ��  ע����
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
* ��������main_lcd_msg
* ��  �ܣ���ʼ����
* ��  �룺��
* ��  ������  
********************************************************************************/
static void main_lcd_msg(void)
{
	char buf[0x64];
	LCD_Clear(Blue);  			/* ������������ɫ */
	LCD_SetBackColor(Blue);		/* �������ֱ�����ɫ */
	LCD_SetTextColor(White);	/* ����������ɫ */

	sprintf(buf,"/====================================/");
	LCD_DisplayStringHZ(8, 8,(uint8_t *)buf);

	sprintf(buf,"=               ��������������     =");
	LCD_DisplayStringHZ(8, 37,(uint8_t *)buf);

	sprintf(buf,"/====================================/");
	LCD_DisplayStringHZ(8, 66,(uint8_t *)buf);

	LCD_Draw_Eflag_Logo(18,24);
	LCD_DrawRect(8,90,140,300);

	LCD_SetTextColor(Yellow);
	sprintf(buf,"����1:�������");
	LCD_DisplayStringHZ(100,130,(uint8_t *)buf);
	LCD_SetTextColor(Yellow);
	sprintf(buf,"����8:�������˵�");
	LCD_DisplayStringHZ(100,160,(uint8_t *)buf);	
}

/****************************************************************************
* ������: RCC_Configuration()
* �� ��: ����ϵͳʱ��
* �� ��: ��
* �� ��: ��
****************************************************************************/
static void RCC_Configuration(void)
{
	/*�ڴ˽׶δ�������ʱ���Ѿ�����ʼ����ɣ�ʱ�ӳ�ʼ��������SystemInit����ʵ�֣�
	 SystemInit���������startup_stm32f2xx.s�Ļ�������ļ��С���ϵͳ�ϵ��ʼ��
	 ����ת��main����֮ǰ������������ȵ���SystemInit����������û�ϣ����������
	 ʱ�ӿ����ڴ˴��ٴε��� SystemInit()�����ĺ���ʵ�ִ����system_stm32f2xx.c��
	 ���С�*/

	SystemInit();

	/* PCLK2 = HCLK/2 */
	RCC_PCLK2Config(RCC_HCLK_Div2);

	/* ʹ��Χʱ�� */
    /* ʹ��GPIOA,GPIOB��SPI2ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
}

/****************************************************************************
* ������: EXTI_Configuration()
* �� ��: �жϺ���
* �� ��: ��
* �� ��: ��
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
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;// �����ж�
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
* ������: NVIC_Configuration()
* �� ��: ��ʼ���ж�������
* �� ��: ��
* �� ��: ��
****************************************************************************/
 static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;  

#ifdef  VECT_TAB_RAM  
	/* ����ʸ�������ַ 0x20000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else 
	/* ����ʸ�������ַ 0x08000000 */ 
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  /* ��ռʽ���ȼ�������Ϊ����ռ���ȼ� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  /* ����TIM2ͨ�������ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; /* �������ȼ��� */
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;   /* ����TIM2ȫ���ж� */
	NVIC_Init(&NVIC_InitStructure);
}

#ifdef  USE_FULL_ASSERT

/******************************************************************************
* ��������assert_failed()
* ��  �ܣ�������������λ��
* ��  �룺�������ڵ��ļ����ʹ��������ļ��е��к�
* ��  ������  
* ��  ע����
********************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{

    /******�˴���������û����󱨸淽ʽ******/
	while (1)
	{}
}
#endif

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*****************/
