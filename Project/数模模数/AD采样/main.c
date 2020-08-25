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
	for(i=0;i<nCount*1000;i++);
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

	/**************GPIO ʱ��ʹ��*****************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);

	/**************ADת������źŹܽ�*****************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 	//��������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//������
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	/*****************ADƬѡ�ܽ�**************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
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

	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_DeInit(SPI2);
	/*************SPI2ʱ��ʹ��******************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    /************SPI��Ӧ�ܽ�����*******************************/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	
	/*SPI SCK �ܽ����� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* SPI MOSI �ܽ�����*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* SPI MISO �ܽ����� */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOC,GPIO_Pin_3);  //ADƬѡ����

	/***********SPI����********************************/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//ȫ˫��ģʽ
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//��ģʽ
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;						//16λ������
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;								//SCK����
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;							//SCK��һ���ز���
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//�������NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//64��Ԥ��Ƶ
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//�ֽ�˳��ΪMSB��ǰ
	SPI_InitStructure.SPI_CRCPolynomial=1;
	SPI_Init(SPI2,&SPI_InitStructure);										//��ʼ��SPI2

	/*********SPI2ʹ��**********************************/
	SPI_Cmd(SPI2,ENABLE);
}
/************************************************************************************
*������:AD_read_data
*����˵��:��ȡADת������ת��ֵ
*�������:
*		 cmd:��ȡת��ͨ����
*����ֵ:��
*
*
**************************************************************************************/
 u16 AD_read_data(u16 cmd)
{
	u16 data;

	/* �ȴ�SPI���ͼĴ���Ϊ��-------------------------------------------------*/
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

	/* ����Ƭѡ--------------------------------------------------------*/
	GPIO_ResetBits(GPIOC, GPIO_Pin_3);

	/* �ȴ�����ʱ�� -----------------------------------------------*/
	DelayMs(5);

	/*���Ͷ�ȡͨ����------------------------------------------------------*/
	SPI_I2S_SendData(SPI2,cmd);

	/* �ȴ����ռĴ���Ϊ�ǿ� ---------------------------------------------*/
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);

	/* ��ȡ���� -----------------------------------------------------*/
	SPI_I2S_ReceiveData(SPI2);

	/* �ȴ�����ʱ�� -----------------------------------------------*/
	DelayMs(5);

	/* �ȴ�ADCת������ź�------------------------------------------*/
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7)==RESET);

	/* �ٴη��Ͷ�ȡͨ���Ŷ�ȡ���� -----------------------------------------*/
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

	SPI_I2S_SendData(SPI2,cmd);

	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);

	data = SPI_I2S_ReceiveData(SPI2);

	/* Ƭѡ���� --------------------------------------------------------*/
	GPIO_SetBits(GPIOC, GPIO_Pin_3);

	DelayMs(5);
	/*�ȴ��´�ת�����------------------------------------------*/
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7)==RESET);

	data=data>>4;

	return data;
}
/************************************************************************************
*������:AD_interface
*����˵��:������ʾ����
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void AD_interface(void)
{
	char temp[32];
	LCD_Clear(Blue);
	LCD_DrawRect(0,0,239,319);
	LCD_DrawLine(0,20,320,Horizontal);
	sprintf(temp,"��ѹ�ɼ�����");
	LCD_DisplayStringHZ(108,4,(uint8_t *)temp);	
	sprintf(temp,"��ѹ�ɼ�ֵ:");
	LCD_DisplayStringHZ(50,50,(uint8_t *)temp);
	LCD_DisplayStringHZ(240,150,"V");
	LCD_DrawLine(0,220,320,Horizontal);
	sprintf(temp,"�������촴�¿Ƽ���չ���޹�˾");
	LCD_DisplayStringHZ(60,222,(uint8_t *)temp);	
}

/************************************************************************************
*������:main
*����˵��:������ �ⲿAD��������
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
int main(void)
{ 
	u16 adc_data;
	float volat; 
	char temp[32];
	GPIO_Configuration();
	SPI_Configuration();
	STM3220F_LCD_Init();
	LCD_SetBackColor(Blue);
	LCD_SetTextColor(Yellow);
	AD_interface();
	while(1)
	{
		adc_data = AD_read_data(0x8c00);
		volat=(float)adc_data/4096*2.5;
		sprintf(temp,"%4.1f",volat);
		LCD_DisplayStringLargeFont(100,100,(uint8_t *)temp);
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
