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
/****�궨��***************************************/
#define SQUARE   0x2028	  /*����*/
#define SIN		 0x2000	  /*���Ҳ�*/
#define TRIANGULAR   0x2002	/*���ǲ�*/
/******ȫ�ֱ���*************************************/
int gFrequence;	   /*DDS���Ƶ��*/
unsigned short CMD9833;	/*DDS�������*/
/*******��������************************************/
static void SPI_GPIO_Config(void);
void output(void);
/***************************************************
*������:main
*�������:��
*����ֵ:��
*����˵��:������
*
****************************************************/
int main(void)
{ 
  gFrequence=3000;	/*����Ƶ��Ƶ�ʲ�����3M*/
  CMD9833=SQUARE; /*���ò���*/
  SPI_GPIO_Config();
  output();			/*���*/
  while (1)
  {
  }
}
/************************************************************************************
*������: SPI_GPIO_Config
*����˵��: ��ʼ��SPI�ӿڼ�SPI����
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void SPI_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	   /*����SPI�˿ڳ�ʼ���ṹ��*/

  SPI_InitTypeDef  SPI_InitStructure;

  /* ʹ��SPI2ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  /* ʹ�����GPIO�ܽ�ʱ��*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  /*ʹ��SPI GPIOʱ��*/	 

  /* SPI�ĸ��ù���*/  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);     /*����GPIO���õ�SPIʱ������ */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2); /*����GPIO���õ�SPI�����*/
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		/*����SPI GPIOģʽ*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz; /*����SPI GPIO�ٶ�*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    /*����SPI GPIO IOģʽ*/
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;   


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* SPI ���� -------------------------------------------------------*/
  SPI_I2S_DeInit(SPI2);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   /*˫��ȫ˫��*/
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;  /*����SPI���ݳ���*/
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;	/*SPI_CPOL_High; //SPI_CPOL_Low;   //DDS Ϊ�ߣ�DAC Ϊ��*/
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	 /*SPI �������*/
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  /*SPIʱ�ӷ�Ƶ�����Ƶ��Ϊ���ߵ�1/4������ƵΪ16��Ƶ��*/
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  /*����SPI��һ������λ�������λ�������λ��MSBΪ���λ��LSBΪ���λ*/
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2,&SPI_InitStructure);
  SPI_Cmd(SPI2,ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

  /*����DDSƬѡ PC2*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/************************************************************************************
*������:Delay
*����˵��:��ʱ����
*�������:
*		nCount����ʱ����
*����ֵ:��
*
*
**************************************************************************************/
void Delay(__IO uint32_t nCount)
{
	  unsigned int count=2*nCount;
	  while(count--)
	  {
	  }
}
/************************************************************************************
*������:Write9833
*����˵��:��DDS���������
*�������:
*		dat������
*����ֵ:��
*
*
**************************************************************************************/
void Write9833(unsigned short dat)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_2); /*Ƭѡ����*/
	Delay(1000);
	SPI_I2S_SendData(SPI2, dat);
	Delay(1000);
	GPIO_SetBits(GPIOC, GPIO_Pin_2);    /*Ƭѡ����*/
}

/************************************************************************************
*������:output
*����˵��:����DDS��Ƶ�ʺͲ���
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
void output(void)		  
{			
	int dds;
	short int dds1,dds2;

	if(gFrequence < 0)
	{
		gFrequence = 0;
	}

	if(gFrequence > 12000000)//12M
	{
		gFrequence = 12000000;
		gFrequence = 12000000;
	}

	dds=gFrequence*10.73741824;
	dds=dds<<2;
	dds1=dds;
	dds2=dds>>16;
	dds1=dds1>>2;
	dds2=dds2&0x7FFF;
	dds2=dds2|0x4000;
	dds1=dds1&0x7FFF;
	dds1=dds1|0x4000;
	Write9833(CMD9833|0x100);
	Write9833(dds1);
	Write9833(dds2);
	Write9833(0xc000);
	Write9833(CMD9833);
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
