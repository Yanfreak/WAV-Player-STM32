 /**********************************************************
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
**********************************************************/

/* ͷ�ļ�------------------------------------------------------------------*/
#include "stm32_eval.h"
#include "stm322xg_eval_lcd.h"
#include "stdio.h"
/************************************************************************************
*������: DelayMs
*����˵��:��ʱ����
*�������:
*		 nCount����ʱ����
*����ֵ:��
*
*
**************************************************************************************/
void DelayMs(uint32_t nCount)
{
	int i=0;
	for(i=0;i<nCount*10000;i++);
}
/************************************************************************************
*������:GPIO_Configuration
*����˵��:SPI��Ӧ�ܽ��Լ�Ƭѡ�ܽų�ʼ��
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*************ʹ�ܹܽ�ʱ��******************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG,ENABLE);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_UP;	 
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	/**************���ܽŸ��õ�SPI2*****************************/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);
	/**************����Ƭѡ�ܽ�*****************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_UP; 
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	/************����֡ͬ���ܽ�*******************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_DOWN; 	
	GPIO_Init(GPIOF,&GPIO_InitStructure);	
}
/************************************************************************************
*������:SPI_Configuration
*����˵��:SPI��������
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void SPI_Configuration(void)
{
	SPI_InitTypeDef   SPI_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);/*ʹ��SPI2ʱ��*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; /*SPI��ģʽ*/
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;/*���ݳ���16λ*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;	/*����ʱ����*/
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;/*���ݲ�����ѡ��*/
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;	/*ʱ�ӷ�Ƶ*/
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; /*���ݸ�λ��ǰ*/
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2,&SPI_InitStructure);

	SPI_Cmd(SPI2,ENABLE);/*ʹ��SPI2*/
}
/************************************************************************************
*������: tlv5616
*����˵��:TLV5616���������
*�������:
*		 DAT��������
*����ֵ:��
*
*
**************************************************************************************/
static void tlv5616(u16 DAT)
{	

	GPIO_ResetBits(GPIOG, GPIO_Pin_8); /*Ƭѡ����*/
	DelayMs(5);
	GPIO_ResetBits(GPIOF, GPIO_Pin_10); /*֡ͬ������*/
	DelayMs(3);
	SPI_I2S_SendData(SPI2,DAT);	/*SPI��������*/
	DelayMs(3);
	GPIO_SetBits(GPIOF, GPIO_Pin_10);  /*Ƭѡ����*/
	DelayMs(5);
	GPIO_SetBits(GPIOG, GPIO_Pin_8);  /*֡ͬ������*/

}
/************************************************************************************
*������:Breath_interface
*����˵��:������ʾ����
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void Breath_interface(void)
{
	char temp[32];
	LCD_Clear(Blue);
	LCD_DrawRect(0,0,239,319);
	LCD_DrawLine(0,20,320,Horizontal);
	sprintf(temp,"�����Ʋ���");
	LCD_DisplayStringHZ(108,4,(uint8_t *)temp);	
	sprintf(temp,"DA����ֵ:");
	LCD_DisplayStringHZ(50,50,(uint8_t *)temp);
	LCD_DrawLine(0,220,320,Horizontal);
	sprintf(temp,"�������촴�¿Ƽ���չ���޹�˾");
	LCD_DisplayStringHZ(60,222,(uint8_t *)temp);
}

/************************************************************************************
*������:main
*����˵��:������
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
int main(void)
{ 
	int i;
	char temp[32]; /*�ַ�������*/
	GPIO_Configuration();/*GPIO�ܽ�����*/
	SPI_Configuration();/*SPI����*/
	STM3220F_LCD_Init();/*LCD��ʼ��*/
	LCD_SetBackColor(Blue);/*���ñ���Ϊ��ɫ*/
	LCD_SetTextColor(Yellow);/*����ǰ��ɫΪ��ɫ*/
	GPIO_SetBits(GPIOG, GPIO_Pin_8);/*Ƭѡ����*/  
	GPIO_SetBits(GPIOF, GPIO_Pin_10); /*֡ͬ������*/
	Breath_interface();
	while(1)
	{
		for(i = 0x600 ; i < 0xc00 ; i++)
		{
			tlv5616(0x4000|i); 
			DelayMs(3);				  /*DAд��ֵ������*/
			sprintf(temp,"%d",i);
			LCD_DisplayStringLargeFont(100,100,(uint8_t *)temp);
		}
		for(i = 0xc00 ; i >= 0x700 ; i--)
		{
			tlv5616(0x4000|i); 
			DelayMs(3);			  /*д��ֵ�𽥼�С*/
			sprintf(temp,"%d",i);
			LCD_DisplayStringLargeFont(100,100,(uint8_t *)temp);
		}
		DelayMs(1000);
	}
}

#ifdef  USE_FULL_ASSERT

/***************************************************************
*������:assert_failed
*����˵��:������������λ��
*	
*�������:
*		file:�������ڵ��ļ���
*		line:���������ļ��е��к�
*	
*����ֵ:��	
*****************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
 /****�˴���������û����󱨸淽ʽ**********************/

  while (1)
  {
  }
}
#endif
/**
  * @}
  */


/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
