 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� stm322xg_eval.c
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
  
/* ͷ�ļ� ------------------------------------------------------------------*/
#include "stm322xg_eval.h"
#include "stm32f2xx_spi.h"
#include "stm32f2xx_dma.h"
#include "stm32f2xx_i2c.h"
#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_exti.h"
#include "misc.h"
#include "stm32f2xx_syscfg.h"
#include "stm32_eval_spi_sd.h"
/*ȫ�ֱ�������-----------------------------------------------------------------------*/ 
GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT, LED3_GPIO_PORT,
                                 LED4_GPIO_PORT};
const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, LED3_PIN,
                                 LED4_PIN};
const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, LED3_GPIO_CLK,
                                 LED4_GPIO_CLK};

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = { USER1_BUTTON_GPIO_PORT, USER2_BUTTON_GPIO_PORT}; 

const uint16_t BUTTON_PIN[BUTTONn] = {USER1_BUTTON_PIN, USER2_BUTTON_PIN}; 

const uint32_t BUTTON_CLK[BUTTONn] = {USER1_BUTTON_GPIO_CLK, USER2_BUTTON_GPIO_CLK};

const uint16_t BUTTON_EXTI_LINE[BUTTONn] = {USER1_BUTTON_EXTI_LINE, 
                                            USER2_BUTTON_EXTI_LINE};

const uint8_t BUTTON_PORT_SOURCE[BUTTONn] = { USER1_BUTTON_EXTI_PORT_SOURCE, 
                                             USER2_BUTTON_EXTI_PORT_SOURCE};
								 
const uint8_t BUTTON_PIN_SOURCE[BUTTONn] = {USER1_BUTTON_EXTI_PIN_SOURCE, 
                                            USER2_BUTTON_EXTI_PIN_SOURCE}; 

const uint8_t BUTTON_IRQn[BUTTONn] = {USER1_BUTTON_EXTI_IRQn,
                                      USER2_BUTTON_EXTI_IRQn};

USART_TypeDef* COM_USART[COMn] = {EVAL_COM1}; 

GPIO_TypeDef* COM_TX_PORT[COMn] = {EVAL_COM1_TX_GPIO_PORT};
 
GPIO_TypeDef* COM_RX_PORT[COMn] = {EVAL_COM1_RX_GPIO_PORT};

const uint32_t COM_USART_CLK[COMn] = {EVAL_COM1_CLK};

const uint32_t COM_TX_PORT_CLK[COMn] = {EVAL_COM1_TX_GPIO_CLK};
 
const uint32_t COM_RX_PORT_CLK[COMn] = {EVAL_COM1_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COMn] = {EVAL_COM1_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {EVAL_COM1_RX_PIN};
 
const uint8_t COM_TX_PIN_SOURCE[COMn] = {EVAL_COM1_TX_SOURCE};

const uint8_t COM_RX_PIN_SOURCE[COMn] = {EVAL_COM1_RX_SOURCE};
 
const uint8_t COM_TX_AF[COMn] = {EVAL_COM1_TX_AF};
 
const uint8_t COM_RX_AF[COMn] = {EVAL_COM1_RX_AF};

DMA_InitTypeDef    sEEDMA_InitStructure; 
NVIC_InitTypeDef   NVIC_InitStructure;
/**************************************************************
*������:STM_EVAL_LEDInit
*����˵��:LED�Ƴ�ʼ�������������ѡLED1,LED2,LED3,LED4
*�������:
*		   Led��Led�Ƶı��
*����ֵ:��
*
*
**************************************************************/
void STM_EVAL_LEDInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* ʹ��LED�ƶ˿ڵ�ʱ�� */
  RCC_AHB1PeriphClockCmd(GPIO_CLK[Led], ENABLE);


  /* ����LED�ƹܽ� */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}
/******************************************************
*������:STM_EVAL_LEDOn
*����˵��:����ָ��LED
*�������:
*     LEDn:Led�Ƶı��
*����ֵ:��
*
*
*******************************************************/
void STM_EVAL_LEDOn(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRRH = GPIO_PIN[Led];
}

/**������:STM_EVAL_LEDOff
  *�������:LEDn
  *����ֵ:��
  *����˵��:Ϩ��ָ��LED
  *
  **/
void STM_EVAL_LEDOff(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRRL = GPIO_PIN[Led];  
}
/******************************************************
*������:STM_EVAL_LEDToggle
*����˵��:��תָ��LED��״̬����--->�� ��--->��
*�������:
*      LEDn��Led�Ƶı��
*����ֵ:��
*
*
**/
void STM_EVAL_LEDToggle(Led_TypeDef Led)
{
		GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
}
/****************************************************
*������:STM_EVAL_PBInit
*����˵��:������ʼ����ButtonΪָ���İ�����Button_ModeΪ��������ģʽ���Ƿ�����ж�
*�������:
*      Button��ָ���������
*      Button_Mode����������ģʽ
*����ֵ:��
*
*
**/
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;


  /* ʹ�ܰ����˿�ʱ��*/
  RCC_AHB1PeriphClockCmd(BUTTON_CLK[Button], ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* ���ð����ܽ�Ϊ����ģʽ */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
  GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);


  if (Button_Mode == BUTTON_MODE_EXTI)
  {
    /* �������ܽ����ӵ��ж��� */
    SYSCFG_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

    /* �����ж���*/
    EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;

    if(Button != BUTTON_USER1)
    {
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
    }
    else
    {
      EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  
    }
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* �����ж����ȼ�*/
    NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure); 
  }
}
/*******************************************************
*������:STM_EVAL_PBGetState
*����˵��:��ȡ������״̬
*�������:
*      Button��ָ���������
*����ֵ:����״̬
*       RESET:����û����
*		  SET:��������
**/
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button)
{
	return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/******************************************************************
*������:STM_EVAL_COMInit
*����˵��:��ʼ��ָ���Ĵ��ڣ�COMΪ���ںţ�USART_InitStructΪ���ڳ�ʼ���ṹ��
*�������:
*		COM�����ڱ�ţ�COM1������COM2
*       USART_InitStruct�����ڳ�ʼ���ṹ��
*����ֵ:��
*
*
**/
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* ʹ�ܴ��ڹܽŵ�ʱ�� */
  RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

  if (COM == COM1)
  {
    /* ʹ�ܴ��ڵ�ʱ�� */
    RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
  }

  /* �����͹ܽŸ��õ�����*/
  GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

  /* �����չܽŸ��õ�����*/
  GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);

  /* ���ô��ڶ�Ӧ�Ĺܽ�  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

  /* ���ô��ڶ�Ӧ�Ĺܽ�Ϊ���ù���  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

  /* �������� */
  USART_Init(COM_USART[COM], USART_InitStruct);
    
  /* ʹ�ܴ��� */
  USART_Cmd(COM_USART[COM], ENABLE);
}
/****************************************************
*������:SD_LowLevel_DeInit
*����˵��:��
*�������:��
*����ֵ:��
*
*
*******************************************************/
void SD_LowLevel_DeInit(void)
{

  SPI_Cmd(SD_SPI, DISABLE); /*!< SD_SPI disable */
  SPI_I2S_DeInit(SD_SPI);   /*!< DeInitializes the SD_SPI */

}
/********************************************************
*������:SD_LowLevel_Init
*����˵��:SD����عܽż�spi��ʼ��
*�������:��
*����ֵ:��
*
*
**/
void SD_LowLevel_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	SPI_InitTypeDef   SPI_InitStructure;

	/*ʹ��SPI�ܽŵĶ�Ӧ�˿�ʱ��*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(SD_SPI_CLK, ENABLE);

	
	GPIO_InitStruct.GPIO_Pin=SD_SPI_SCK_PIN|SD_SPI_MISO_PIN|SD_SPI_MOSI_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;  
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
  /*�ܽŸ��� */	  
	GPIO_PinAFConfig(GPIOB, SD_SPI_SCK_PIN_SOURCE, GPIO_AF_SPI3);  
	GPIO_PinAFConfig(GPIOB, SD_SPI_MOSI_PIN_SOURCE, GPIO_AF_SPI3);	
	GPIO_PinAFConfig(GPIOB, SD_SPI_MISO_PIN_SOURCE, GPIO_AF_SPI3);

	 
 /*SPI���� */											   
	SPI_I2S_DeInit(SD_SPI);	    
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; 
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b; 
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;//;Low//; 
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge; 
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;   //SPI_NSS_Hard
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16; 
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; 
	SPI_InitStructure.SPI_CRCPolynomial = 7; 
	SPI_Init(SD_SPI,&SPI_InitStructure); 
	SPI_Cmd(SD_SPI,ENABLE);   

	//SD��Ƭѡ�ܽ�
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;  

	GPIO_InitStruct.GPIO_Pin=SD_CS_PIN;
	GPIO_Init(GPIOD,&GPIO_InitStruct);     	    			
	//SD_CS_HIGH();

}
/******************************************************
*������:sEE_LowLevel_DeInit
*����˵��:��
*�������:��
*����ֵ:��
*
*
*******************************************************/
void sEE_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
   
  /* �ر�I2C */
  I2C_Cmd(sEE_I2C, DISABLE);
 
  I2C_DeInit(sEE_I2C);

  /*�ر�I2Cʱ�� */
  RCC_APB1PeriphClockCmd(sEE_I2C_CLK, DISABLE);
    
  /*I2C��Ӧ�ܽ����� */  
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(sEE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
  GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

  /* �ر�I2C���ж����� */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_RX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_Init(&NVIC_InitStructure);   
  
  /*��I2C��DMA�ر� */
  DMA_Cmd(sEE_I2C_DMA_STREAM_TX, DISABLE);
  DMA_Cmd(sEE_I2C_DMA_STREAM_RX, DISABLE);
  DMA_DeInit(sEE_I2C_DMA_STREAM_TX);
  DMA_DeInit(sEE_I2C_DMA_STREAM_RX);
}
/************************************************************
*������:sEE_LowLevel_Init
*����˵��:I2C��Ӧ�ܽż��ж�����
*�������:��
*����ֵ:��
*
*
************************************************************/
void sEE_LowLevel_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
   
  /*I2Cʱ��ʹ�� */
  RCC_APB1PeriphClockCmd(sEE_I2C_CLK, ENABLE);
  
  /*I2C��Ӧ�ܽ�ʱ��ʹ�� */
  RCC_AHB1PeriphClockCmd(sEE_I2C_SCL_GPIO_CLK | sEE_I2C_SDA_GPIO_CLK, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* I2C��λʹ�� */
  RCC_APB1PeriphResetCmd(sEE_I2C_CLK, ENABLE);
  
  /* I2C��λ�ͷ� */
  RCC_APB1PeriphResetCmd(sEE_I2C_CLK, DISABLE);
    
  /*I2C��SCK��Ӧ�ܽ�����*/    
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(sEE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*I2C��SDA��Ӧ�ܽ�����*/
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
  GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

  /* ��SCK�ܽŸ��õ�I2C*/
  GPIO_PinAFConfig(sEE_I2C_SCL_GPIO_PORT, sEE_I2C_SCL_SOURCE, sEE_I2C_SCL_AF);

  /*��SDA�ܽŸ��õ�I2C*/
  GPIO_PinAFConfig(sEE_I2C_SDA_GPIO_PORT, sEE_I2C_SDA_SOURCE, sEE_I2C_SDA_AF);  
  
  /* ����I2C�ķ����ж� */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* ����I2C�Ľ����ж� */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_RX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_Init(&NVIC_InitStructure);  
 
  /*ʹ��I2C��DMAʱ�� */
  RCC_AHB1PeriphClockCmd(sEE_I2C_DMA_CLK, ENABLE);
  
  /* ���I2C����DMA�Ĺ����־  */
  DMA_ClearFlag(sEE_I2C_DMA_STREAM_TX, sEE_TX_DMA_FLAG_FEIF | sEE_TX_DMA_FLAG_DMEIF | sEE_TX_DMA_FLAG_TEIF | \
                                       sEE_TX_DMA_FLAG_HTIF | sEE_TX_DMA_FLAG_TCIF);
  /* �ر�I2C��DMA */
  DMA_Cmd(sEE_I2C_DMA_STREAM_TX, DISABLE);

  /*����I2C��DMA */
  DMA_DeInit(sEE_I2C_DMA_STREAM_TX);
  sEEDMA_InitStructure.DMA_Channel = sEE_I2C_DMA_CHANNEL;
  sEEDMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)sEE_I2C_DR_Address;
  sEEDMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)0; 
  sEEDMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; 
  sEEDMA_InitStructure.DMA_BufferSize = 0xFFFF;             
  sEEDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  sEEDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  sEEDMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  sEEDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  sEEDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  sEEDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  sEEDMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
  sEEDMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  sEEDMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  sEEDMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(sEE_I2C_DMA_STREAM_TX, &sEEDMA_InitStructure);

  /* �ر�I2C����DMA */
  DMA_Cmd(sEE_I2C_DMA_STREAM_RX, DISABLE);
  DMA_DeInit(sEE_I2C_DMA_STREAM_RX);
  DMA_Init(sEE_I2C_DMA_STREAM_RX, &sEEDMA_InitStructure);
  
  /* ʹ��I2C��DMA�ж� */
  DMA_ITConfig(sEE_I2C_DMA_STREAM_TX, DMA_IT_TC, ENABLE);
  DMA_ITConfig(sEE_I2C_DMA_STREAM_RX, DMA_IT_TC, ENABLE);      
}
/****************************************************************
*������:sEE_LowLevel_DMAConfig
*����˵��:����I2C��DMA���䷽��
*�������:
*     pBuffer��DMA���ݻ�����
*     BufferSize��DMA���ݻ�������С
*     Direction��DMA�Ĵ��䷽��sEE_DIRECTION_TX����sEE_DIRECTION_RX
*����ֵ:��
*
*
**/
void sEE_LowLevel_DMAConfig(uint32_t pBuffer, uint32_t BufferSize, uint32_t Direction)
{ 
  /* ���I2C��� */
  if (Direction == sEE_DIRECTION_TX)
  {
    /* ����I2C��DMA���Դ�����ݴ�С�� */
    sEEDMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)pBuffer;
    sEEDMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;    
    sEEDMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;  
    DMA_Init(sEE_I2C_DMA_STREAM_TX, &sEEDMA_InitStructure);  
  }
  else
  { 
    /* ����I2C��DMA�����ַ���������ݴ�С */
    sEEDMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)pBuffer;
    sEEDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    sEEDMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;      
    DMA_Init(sEE_I2C_DMA_STREAM_RX, &sEEDMA_InitStructure);    
  }
}

/**
  * @}
  */ 


/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */    

/**
  * @}
  */ 
    
/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD. *****END OF FILE****/
