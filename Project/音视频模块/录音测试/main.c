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
/* ͷ�ļ� ------------------------------------------------------------------*/
#include "stm322xg_eval_nand.h"
#include "stm32_eval.h"
#include "stm322xg_eval_systick.h" 
#include "stm322xg_eval_lcd.h"
#include "main.h"
/* �궨��-----------------------------------------------------------*/
/***********��������С***************************************/


/*************NAND FLASH ID*************************************/
#define NAND_SAMSUNG_MakerID     0xEC
#define NAND_SAMSUNG_DeviceID    0xF1   /* K9F1G08U0A,  128MB */
#define SalveAddress  0x1A<<1
#define ClockSpeed              100000
u32 ecc;
/* ȫ�ֱ������� ---------------------------------------------------------*/
/**********NAND FLASH ID����****************************************/
NAND_IDTypeDef NAND_ID;

/************NAND FLASH��д��ַ�ṹ�����**************************************/
NAND_ADDRESS WriteReadAddr;
volatile u8 WRflag=0;
/*************���ݻ�����*************************************/
u16 RxBuffer[BUFFER_SIZE];
/*************��������******************************/
void AIC_I2C_Init(void);
void I2S_GPIO_Config(void);
void NVIC_Configuration(void);
void AIC23B_Init(void);
void I2S_ConfigRx(void);
/***************************************************
*������:main
*�������:��
*����ֵ:��
*����˵��:������
*
****************************************************/
int main(void)
{ 
    int i;
    char temp[32];
    STM_EVAL_LEDInit(LED2);
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED4);
    STM_EVAL_LEDOff(LED2);
    STM_EVAL_LEDOff(LED3);
    STM_EVAL_LEDOff(LED4);
    NVIC_Configuration();
    STM3220F_LCD_Init();	   /*LCD��ʼ��*/
    LCD_Clear(Blue);  	   /*����*/
    LCD_SetBackColor(Blue);  /*���ñ���ɫ*/
    LCD_SetTextColor(Yellow);/*����ǰ��ɫ*/
    LCD_DrawRect(0,0,239,319);
    LCD_DrawLine(0,20,320,Horizontal);
    sprintf(temp,"¼������");
    LCD_DisplayStringHZ(130,4,(uint8_t *)temp);
    LCD_DrawLine(0,220,320,Horizontal);
    sprintf(temp,"�������촴�¿Ƽ���չ���޹�˾");
    LCD_DisplayStringHZ(60,222,(uint8_t *)temp);
    /* NAND FLASH��ʼ��*/
    FSMC_NAND_Init();
    
    /***NAND FLASH��λ***/
    FSMC_NAND_Reset();
    /* ��ȡNAND FLASH��ID */
    FSMC_NAND_ReadID(&NAND_ID);

    /* ��֤ NAND FLASH ID�Ƿ���ȷ */  
    if(!((NAND_ID.Maker_ID == NAND_SAMSUNG_MakerID) && (NAND_ID.Device_ID == NAND_SAMSUNG_DeviceID)))
    {
        sprintf(temp,"NAND FLASH ERROR");
        LCD_DisplayStringHZ(60,80,(uint8_t *)temp);
        while(1)
        {
            delay_ms(1000);
            STM_EVAL_LEDToggle(LED4);    /*NAND FLASH�쳣*/
        }
    }
    I2S_GPIO_Config();
    AIC_I2C_Init();
    AIC23B_Init();
    for(i=0;i<1024;i++)
    {
    	WriteReadAddr.Block=i;
    	WriteReadAddr.Zone=0;
    	WriteReadAddr.Page=0;
    	FSMC_NAND_EraseBlock(WriteReadAddr); 
    	delay_ms(1);
    }
    sprintf(temp,"NAND FLASH OK");
    LCD_DisplayStringHZ(50,50,(uint8_t *)temp);
    STM_EVAL_LEDOn(LED2);
    sprintf(temp,"¼��������...");
    LCD_DisplayStringHZ(50,80,(uint8_t *)temp);
    WriteReadAddr.Block=0;
    I2S_ConfigRx();
    while(1)
    {
    	if(WRflag)        /*������д��NAND FLASH*/
    	{
    		WRflag=0;
    		ecc=FSMC_NAND_WriteSmallPage((u8 *)RxBuffer,WriteReadAddr,1);
    	}
    }
}
/************************************************************************************
*������:I2S_GPIO_Config
*����˵��: I2S��GPIO����
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
void I2S_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOG, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC,GPIO_Pin_2);
    GPIO_SetBits(GPIOC,GPIO_Pin_3);
    GPIO_SetBits(GPIOG,GPIO_Pin_8);
}
/************************************************************************************
*������: AIC_I2C_Init
*����˵��:��ʼ��I2C��GPIO�ܽ��Լ�����I2C
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
void AIC_I2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef   I2C_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_I2C2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_I2C2);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = SalveAddress;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;
    I2C_Init(I2C2, &I2C_InitStructure);
    I2C_Cmd(I2C2, ENABLE);
}
/************************************************************************************
*������:NVIC_Configuration
*����˵��: �жϳ�ʼ��
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM  
  /* ����ж���������SRAM�� */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* ����ж���������Flash���� */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif


  /* SPI2�ж�����*/
  NVIC_InitStructure.NVIC_IRQChannel =SPI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/************************************************************************************
*������:I2S_ConfigRx
*����˵��:����I2SΪ���չ���
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
void I2S_ConfigRx(void)
{
    I2S_InitTypeDef I2S_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    SPI_I2S_DeInit(SPI2);
    I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
    I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
    I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
    I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
    I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_48k;
    I2S_InitStructure.I2S_Mode =I2S_Mode_MasterRx;
    I2S_Init(SPI2, &I2S_InitStructure);
    SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE); 
    I2S_Cmd(SPI2,ENABLE);     
}

/************************************************************************************
*������:AIC_ByteWrite
*����˵��: ��AIC23B��������
*�������:
*        data��д�Ĵ�������
*        WriteAddr:�Ĵ�����ַ
*����ֵ:��
*
*
**************************************************************************************/
void AIC_ByteWrite(u16 data,u8 WriteAddr)
{
	u8 dataH,dataL;
	u16 datatmp=0;
	datatmp=WriteAddr;
	data|=datatmp<<9;
	dataH=(data&0xFF00)>>8;
	dataL=data&0x00FF;
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	/***������ʼ�ź�****/
	I2C_GenerateSTART(I2C2,ENABLE);
	/***���EV5λ����� */
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));
	/***����AIC23B����ַ***/
	I2C_Send7bitAddress(I2C2,SalveAddress,I2C_Direction_Transmitter);
	/****���EV6λ����� ***/
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	/*���͸�λ����*/
	I2C_SendData(I2C2,dataH);
	/**���EV8λ�����*/
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    /*�������ݵ�λ*/
	I2C_SendData(I2C2,dataL);
	/*���EV8λ����� */
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	/***������ֹ�ź�*/
	I2C_GenerateSTOP(I2C2,ENABLE);
}
/************************************************************************************
*������:AIC23B_Init
*����˵��: AIC23B��ʼ��
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
void AIC23B_Init(void)
{
  AIC_ByteWrite(0x00FF,0x06);  /*�رյ�Դ*/
  AIC_ByteWrite(0x009f,0x00); /*��������������������1f*/
  AIC_ByteWrite(0x009f,0x01); /*��������������������1f*/
  AIC_ByteWrite(0x0070,0x02); /*��ͨ��������������70*/
  AIC_ByteWrite(0x0070,0x03); /*��ͨ��������������70*/
  AIC_ByteWrite(0x0014,0x04);  /*ģ��ͨ������14*/
  AIC_ByteWrite(0x0004,0x05);  /*����ͨ������4*/
  AIC_ByteWrite(0x0012,0x07);	/*���ֽӿڿ���52*/
  AIC_ByteWrite(0x0000,0x08);  /*�����ʿ���20*/
  AIC_ByteWrite(0x00c1,0x09);  /*���ֽӿڼ���c1*/
  AIC_ByteWrite(0x0019,0x06);  /*�ϵ�*/
}

#ifdef  DEBUG
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
void assert_failed(u8* file, u32 line)
{ 
 /****�˴���������û����󱨸淽ʽ**********************/
  while (1)
  {
  }
}
#endif
/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
