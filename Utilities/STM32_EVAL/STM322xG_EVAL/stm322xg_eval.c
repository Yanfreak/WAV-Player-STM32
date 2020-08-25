 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： stm322xg_eval.c
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
**********************************************************/
  
/* 头文件 ------------------------------------------------------------------*/
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
/*全局变量定义-----------------------------------------------------------------------*/ 
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
*函数名:STM_EVAL_LEDInit
*函数说明:LED灯初始化，输入参数可选LED1,LED2,LED3,LED4
*输入参数:
*		   Led：Led灯的编号
*返回值:无
*
*
**************************************************************/
void STM_EVAL_LEDInit(Led_TypeDef Led)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* 使能LED灯端口的时钟 */
  RCC_AHB1PeriphClockCmd(GPIO_CLK[Led], ENABLE);


  /* 配置LED灯管脚 */
  GPIO_InitStructure.GPIO_Pin = GPIO_PIN[Led];
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIO_PORT[Led], &GPIO_InitStructure);
}
/******************************************************
*函数名:STM_EVAL_LEDOn
*函数说明:点亮指定LED
*输入参数:
*     LEDn:Led灯的编号
*返回值:无
*
*
*******************************************************/
void STM_EVAL_LEDOn(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRRH = GPIO_PIN[Led];
}

/**函数名:STM_EVAL_LEDOff
  *输入参数:LEDn
  *返回值:无
  *函数说明:熄灭指定LED
  *
  **/
void STM_EVAL_LEDOff(Led_TypeDef Led)
{
  GPIO_PORT[Led]->BSRRL = GPIO_PIN[Led];  
}
/******************************************************
*函数名:STM_EVAL_LEDToggle
*函数说明:反转指定LED的状态，亮--->灭， 灭--->亮
*输入参数:
*      LEDn：Led灯的编号
*返回值:无
*
*
**/
void STM_EVAL_LEDToggle(Led_TypeDef Led)
{
		GPIO_PORT[Led]->ODR ^= GPIO_PIN[Led];
}
/****************************************************
*函数名:STM_EVAL_PBInit
*函数说明:按键初始化，Button为指定的按键，Button_Mode为按键输入模式，是否采用中断
*输入参数:
*      Button：指定按键编号
*      Button_Mode：按键工作模式
*返回值:无
*
*
**/
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;


  /* 使能按键端口时钟*/
  RCC_AHB1PeriphClockCmd(BUTTON_CLK[Button], ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* 设置按键管脚为输入模式 */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
  GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);


  if (Button_Mode == BUTTON_MODE_EXTI)
  {
    /* 将按键管脚连接到中断线 */
    SYSCFG_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

    /* 配置中断线*/
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

    /* 配置中断优先级*/
    NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure); 
  }
}
/*******************************************************
*函数名:STM_EVAL_PBGetState
*函数说明:获取按键的状态
*输入参数:
*      Button：指定按键编号
*返回值:按键状态
*       RESET:按键没按下
*		  SET:按键按下
**/
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button)
{
	return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}

/******************************************************************
*函数名:STM_EVAL_COMInit
*函数说明:初始化指定的串口，COM为串口号，USART_InitStruct为串口初始化结构体
*输入参数:
*		COM：串口编号，COM1，或者COM2
*       USART_InitStruct：串口初始化结构体
*返回值:无
*
*
**/
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* 使能串口管脚的时钟 */
  RCC_AHB1PeriphClockCmd(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM], ENABLE);

  if (COM == COM1)
  {
    /* 使能串口的时钟 */
    RCC_APB2PeriphClockCmd(COM_USART_CLK[COM], ENABLE);
  }

  /* 将发送管脚复用到串口*/
  GPIO_PinAFConfig(COM_TX_PORT[COM], COM_TX_PIN_SOURCE[COM], COM_TX_AF[COM]);

  /* 将接收管脚复用到串口*/
  GPIO_PinAFConfig(COM_RX_PORT[COM], COM_RX_PIN_SOURCE[COM], COM_RX_AF[COM]);

  /* 配置串口对应的管脚  */
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

  GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

  /* 配置串口对应的管脚为复用功能  */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
  GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

  /* 串口配置 */
  USART_Init(COM_USART[COM], USART_InitStruct);
    
  /* 使能串口 */
  USART_Cmd(COM_USART[COM], ENABLE);
}
/****************************************************
*函数名:SD_LowLevel_DeInit
*函数说明:无
*输入参数:无
*返回值:无
*
*
*******************************************************/
void SD_LowLevel_DeInit(void)
{

  SPI_Cmd(SD_SPI, DISABLE); /*!< SD_SPI disable */
  SPI_I2S_DeInit(SD_SPI);   /*!< DeInitializes the SD_SPI */

}
/********************************************************
*函数名:SD_LowLevel_Init
*函数说明:SD卡相关管脚及spi初始化
*输入参数:无
*返回值:无
*
*
**/
void SD_LowLevel_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	SPI_InitTypeDef   SPI_InitStructure;

	/*使能SPI管脚的对应端口时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(SD_SPI_CLK, ENABLE);

	
	GPIO_InitStruct.GPIO_Pin=SD_SPI_SCK_PIN|SD_SPI_MISO_PIN|SD_SPI_MOSI_PIN;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
//	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;  
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
  /*管脚复用 */	  
	GPIO_PinAFConfig(GPIOB, SD_SPI_SCK_PIN_SOURCE, GPIO_AF_SPI3);  
	GPIO_PinAFConfig(GPIOB, SD_SPI_MOSI_PIN_SOURCE, GPIO_AF_SPI3);	
	GPIO_PinAFConfig(GPIOB, SD_SPI_MISO_PIN_SOURCE, GPIO_AF_SPI3);

	 
 /*SPI配置 */											   
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

	//SD卡片选管脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_25MHz;
	GPIO_InitStruct.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd=GPIO_PuPd_UP;  

	GPIO_InitStruct.GPIO_Pin=SD_CS_PIN;
	GPIO_Init(GPIOD,&GPIO_InitStruct);     	    			
	//SD_CS_HIGH();

}
/******************************************************
*函数名:sEE_LowLevel_DeInit
*函数说明:无
*输入参数:无
*返回值:无
*
*
*******************************************************/
void sEE_LowLevel_DeInit(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
   
  /* 关闭I2C */
  I2C_Cmd(sEE_I2C, DISABLE);
 
  I2C_DeInit(sEE_I2C);

  /*关闭I2C时钟 */
  RCC_APB1PeriphClockCmd(sEE_I2C_CLK, DISABLE);
    
  /*I2C对应管脚配置 */  
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(sEE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
  GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

  /* 关闭I2C的中断配置 */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);

  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_RX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_Init(&NVIC_InitStructure);   
  
  /*将I2C的DMA关闭 */
  DMA_Cmd(sEE_I2C_DMA_STREAM_TX, DISABLE);
  DMA_Cmd(sEE_I2C_DMA_STREAM_RX, DISABLE);
  DMA_DeInit(sEE_I2C_DMA_STREAM_TX);
  DMA_DeInit(sEE_I2C_DMA_STREAM_RX);
}
/************************************************************
*函数名:sEE_LowLevel_Init
*函数说明:I2C对应管脚及中断配置
*输入参数:无
*返回值:无
*
*
************************************************************/
void sEE_LowLevel_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure; 
   
  /*I2C时钟使能 */
  RCC_APB1PeriphClockCmd(sEE_I2C_CLK, ENABLE);
  
  /*I2C对应管脚时钟使能 */
  RCC_AHB1PeriphClockCmd(sEE_I2C_SCL_GPIO_CLK | sEE_I2C_SDA_GPIO_CLK, ENABLE);

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
  
  /* I2C复位使能 */
  RCC_APB1PeriphResetCmd(sEE_I2C_CLK, ENABLE);
  
  /* I2C复位释放 */
  RCC_APB1PeriphResetCmd(sEE_I2C_CLK, DISABLE);
    
  /*I2C的SCK对应管脚配置*/    
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(sEE_I2C_SCL_GPIO_PORT, &GPIO_InitStructure);

  /*I2C的SDA对应管脚配置*/
  GPIO_InitStructure.GPIO_Pin = sEE_I2C_SDA_PIN;
  GPIO_Init(sEE_I2C_SDA_GPIO_PORT, &GPIO_InitStructure);

  /* 将SCK管脚复用到I2C*/
  GPIO_PinAFConfig(sEE_I2C_SCL_GPIO_PORT, sEE_I2C_SCL_SOURCE, sEE_I2C_SCL_AF);

  /*将SDA管脚复用到I2C*/
  GPIO_PinAFConfig(sEE_I2C_SDA_GPIO_PORT, sEE_I2C_SDA_SOURCE, sEE_I2C_SDA_AF);  
  
  /* 配置I2C的发送中断 */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_TX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* 配置I2C的接收中断 */
  NVIC_InitStructure.NVIC_IRQChannel = sEE_I2C_DMA_RX_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = sEE_I2C_DMA_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = sEE_I2C_DMA_SUBPRIO;
  NVIC_Init(&NVIC_InitStructure);  
 
  /*使能I2C的DMA时钟 */
  RCC_AHB1PeriphClockCmd(sEE_I2C_DMA_CLK, ENABLE);
  
  /* 清除I2C接收DMA的挂起标志  */
  DMA_ClearFlag(sEE_I2C_DMA_STREAM_TX, sEE_TX_DMA_FLAG_FEIF | sEE_TX_DMA_FLAG_DMEIF | sEE_TX_DMA_FLAG_TEIF | \
                                       sEE_TX_DMA_FLAG_HTIF | sEE_TX_DMA_FLAG_TCIF);
  /* 关闭I2C的DMA */
  DMA_Cmd(sEE_I2C_DMA_STREAM_TX, DISABLE);

  /*配置I2C的DMA */
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

  /* 关闭I2C接收DMA */
  DMA_Cmd(sEE_I2C_DMA_STREAM_RX, DISABLE);
  DMA_DeInit(sEE_I2C_DMA_STREAM_RX);
  DMA_Init(sEE_I2C_DMA_STREAM_RX, &sEEDMA_InitStructure);
  
  /* 使能I2C的DMA中断 */
  DMA_ITConfig(sEE_I2C_DMA_STREAM_TX, DMA_IT_TC, ENABLE);
  DMA_ITConfig(sEE_I2C_DMA_STREAM_RX, DMA_IT_TC, ENABLE);      
}
/****************************************************************
*函数名:sEE_LowLevel_DMAConfig
*函数说明:配置I2C的DMA传输方向
*输入参数:
*     pBuffer：DMA数据缓冲区
*     BufferSize：DMA数据缓冲区大小
*     Direction：DMA的传输方向，sEE_DIRECTION_TX或者sEE_DIRECTION_RX
*返回值:无
*
*
**/
void sEE_LowLevel_DMAConfig(uint32_t pBuffer, uint32_t BufferSize, uint32_t Direction)
{ 
  /* 如果I2C输出 */
  if (Direction == sEE_DIRECTION_TX)
  {
    /* 配置I2C的DMA输出源，数据大小， */
    sEEDMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)pBuffer;
    sEEDMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;    
    sEEDMA_InitStructure.DMA_BufferSize = (uint32_t)BufferSize;  
    DMA_Init(sEE_I2C_DMA_STREAM_TX, &sEEDMA_InitStructure);  
  }
  else
  { 
    /* 配置I2C的DMA缓存地址，接收数据大小 */
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
