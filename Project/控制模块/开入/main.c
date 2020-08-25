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
*   ������� ��20��12.08.01 
*
******************************************************************************/

/*************ͷ�ļ�**********************************************************/
#include "main.h"

/****************************************************************************
* ������: RCC_Configuration()
* �� ��: ���ò�ͬ��ϵͳʱ��
* �� ��: �� 
* �� ��: ��
****************************************************************************/
static void RCC_Configuration(void)
{
 	/*�ڴ˽׶δ�������ʱ���Ѿ�����ʼ����ɣ�ʱ�ӳ�ʼ��������SystemInit����ʵ�֣�SystemInit���������startup_stm32f2xx.s�Ļ�������ļ��С�
    *��ϵͳ�ϵ��ʼ������ת��main����֮ǰ������������ȵ���SystemInit������	
    *����û�ϣ����������ʱ�ӿ����ڴ˴��ٴε��� SystemInit()�����ĺ���ʵ�ִ����system_stm32f2xx.c�ļ��С�*/
    SystemInit();

    RCC_PCLK2Config(RCC_HCLK_Div2); /* PCLK2 = HCLK/2 */

	/* ʹ��ʱ�� */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC
							|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOG|RCC_AHB1Periph_GPIOF, ENABLE);

}

/****************************************************************************
* ������: GPIO_Configuration()
* �� ��: ���ò�ͬ��GPIO�˿�
* �� ��: �� 
* �� ��: ��
****************************************************************************/
static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	//���룬IN0�����͵���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_NOPULL;  
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;	//���������ͷ�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0;	 //�����OUT0�����͵���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_3;	 //�����OUT1�����͵���	  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_15;	//���룬IN1�����͵���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_6|GPIO_Pin_7;	  //LED��
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/****************************************************************************
* ������: Delay()
* �� ��: ��ʱ
* �� ��: ��ʱʱ�� 
* �� ��: ��
****************************************************************************/
static void Delay(u32 nTime)			//����
{
	u32 i;
	for(i=0;i<nTime*200;i++);
}

/******************************************************************************
* ��������main
* ��  �ܣ�����IN1��GND����ñ�������ѹ�����ͣ������죬����
* ��  �룺��
* ��  ������  
* ��  ע����
********************************************************************************/
int main(void)
{
	RCC_Configuration();
	GPIO_Configuration();

	/*���ߵ�ѹ*/
	GPIO_SetBits(GPIOA,GPIO_Pin_0);	
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	GPIO_SetBits(GPIOC,GPIO_Pin_0);
	GPIO_SetBits(GPIOD,GPIO_Pin_3);
	GPIO_SetBits(GPIOG,GPIO_Pin_15);

	while(1)
	{
		
		/* ���PG15�ܽŵĵ�ѹ����ѹΪ��ʱִ��if����ѹΪ��ʱִ��else */
		if(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_15) == 0)
			{
				GPIO_SetBits(GPIOF,GPIO_Pin_6);	  //���ߣ�����
				GPIO_SetBits(GPIOF,GPIO_Pin_7);
				GPIO_ResetBits(GPIOB,GPIO_Pin_1); //���ͣ�������
				Delay(20000);
				GPIO_SetBits(GPIOB,GPIO_Pin_1);	  //���ߣ�����������
			}
		else
			{
				GPIO_ResetBits(GPIOF,GPIO_Pin_6);
				GPIO_ResetBits(GPIOF,GPIO_Pin_7);
			}
	}
}

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*************/
