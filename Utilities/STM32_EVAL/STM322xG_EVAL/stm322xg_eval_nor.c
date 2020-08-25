 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： stm322xg_eval_nor.c
*   摘 要    ： 
*
*   当前版本 ： 
*   作 者    ： EFLAG
*   完成日期 ： 
*
*   取代版本 ：
*   原作者   ： 
*   修改内容 ：
*   完成日期 ：2012.08.02 
*
**********************************************************/

/* 头文件------------------------------------------------------------------*/
#include "stm322xg_eval_nor.h"

/* 宏定义 ------------------------------------------------------------*/
#define Bank1_NOR2_ADDR       ((uint32_t)0x64000000) 
#define BlockErase_Timeout    ((uint32_t)0x00A00000)
#define ChipErase_Timeout     ((uint32_t)0x30000000) 
#define Program_Timeout       ((uint32_t)0x00001400)

#define ADDR_SHIFT(A) (Bank1_NOR2_ADDR +  (A<<1))
#define NOR_WRITE(Address, Data)  (*(__IO uint16_t *)(Address) = (Data))

/***************************************************
*函数名:FSMC_NOR_Init
*输入参数:无
*返回值:无
*函数说明:NOR FLASH初始化，包括GPIO以及FSMC总线的配置
*
****************************************************/
void FSMC_NOR_Init(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;
  GPIO_InitTypeDef GPIO_InitStructure;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | 
                         RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);

  /*-- GPIO配置 -------------*/
  /* NOR 数据线GPIO配置**** */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource8,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource9,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource10,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 |
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource11,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource12,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource13,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource14,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource15,GPIO_AF_FSMC);

  /* NOR 地址线GPIO配置 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 |
                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_12 | GPIO_Pin_13 |
                                GPIO_Pin_14 | GPIO_Pin_15;
  GPIO_Init(GPIOF, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource0,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource1,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource2,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource3,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource4,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource5,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource12,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource13,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource14,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOF,GPIO_PinSource15,GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 |
                                GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource0,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource1,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource2,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource3,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource4,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource5,GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource11,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource3,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource4,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource5,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource6,GPIO_AF_FSMC);

  /* NOE 和 NWE 配置 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);

  /* NE2 配置 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_12;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource9,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource10,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOG,GPIO_PinSource12,GPIO_AF_FSMC);

  /*-- FSMC 总线配置 ----------------------------------------------------*/
  p.FSMC_AddressSetupTime = 0x02;
  p.FSMC_AddressHoldTime = 0x00;
  p.FSMC_DataSetupTime = 0x05;
  p.FSMC_BusTurnAroundDuration = 0x00;
  p.FSMC_CLKDivision = 0x00;
  p.FSMC_DataLatency = 0x00;
  p.FSMC_AccessMode = FSMC_AccessMode_B;

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM2;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_NOR;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

  /* 使能FSMC总线 */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE);
}

/***************************************************
*函数名:FSMC_NOR_ReadID
*输入参数:
*     	NOR_ID:存放NOR FLASH ID的结构体
*返回值:无
*函数说明:读取NOR FLASH的ID号
*
****************************************************/
void FSMC_NOR_ReadID(NOR_IDTypeDef* NOR_ID)
{
  NOR_WRITE(ADDR_SHIFT(0x5555), 0xAAAA);
  NOR_WRITE(ADDR_SHIFT(0x2AAA), 0x5555);
  NOR_WRITE(ADDR_SHIFT(0x5555), 0x9090);

  NOR_ID->Manufacturer_Code = *(__IO uint16_t *) ADDR_SHIFT(0x0000);
  NOR_ID->Device_Code1 = *(__IO uint16_t *) ADDR_SHIFT(0x0001);
  NOR_ID->Device_Code2 = *(__IO uint16_t *) ADDR_SHIFT(0x000E);
  NOR_ID->Device_Code3 = *(__IO uint16_t *) ADDR_SHIFT(0x000F);
}

/***************************************************
*函数名:FSMC_NOR_EraseBlock
*输入参数:
*     	BlockAddr：待擦除NOR FLASH地址
*返回值:
*	   NOR_Status：NOR FLASH的工作状态
*函数说明:擦除NOR FLASH指定的块
*
****************************************************/
NOR_Status FSMC_NOR_EraseBlock(uint32_t BlockAddr)
{
  NOR_WRITE(ADDR_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR_SHIFT(0x5555), 0x0080);
  NOR_WRITE(ADDR_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE((Bank1_NOR2_ADDR + BlockAddr), 0x50);

  return (FSMC_NOR_GetStatus(BlockErase_Timeout));
}

/***************************************************
*函数名:FSMC_NOR_EraseChip
*输入参数:无
*返回值:
*      NOR_Status：NOR FLASH的工作状态
*函数说明:擦除NOR FLASH芯片，整片擦除
*
****************************************************/
NOR_Status FSMC_NOR_EraseChip(void)
{
  NOR_WRITE(ADDR_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR_SHIFT(0x5555), 0x0080);
  NOR_WRITE(ADDR_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR_SHIFT(0x5555), 0x0010);

  return (FSMC_NOR_GetStatus(ChipErase_Timeout));
}
/***************************************************
*函数名:FSMC_NOR_WriteHalfWord
*输入参数:
*		 WriteAddr：NOR FLASH内部地址
*		 Data: 待写入的数据
*返回值:
*      NOR_Status：NOR FLASH的工作状态
*函数说明:向指定NOR FLASH的地址下写入半个字数据
*
****************************************************/
NOR_Status FSMC_NOR_WriteHalfWord(uint32_t WriteAddr, uint16_t Data)
{
  NOR_WRITE(ADDR_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE(ADDR_SHIFT(0x5555), 0x00A0);
  NOR_WRITE((Bank1_NOR2_ADDR + WriteAddr), Data);

  return (FSMC_NOR_GetStatus(Program_Timeout));
}

/***************************************************
*函数名:FSMC_NOR_WriteBuffer
*输入参数:
*		 pBuffer：数据缓冲区
*		 WriteAddr：NOR FLASH内部地址
*		 NumHalfwordToWrite: 写入数据长度，单位为半字
*返回值:
*      NOR_Status：NOR FLASH的工作状态
*函数说明:将缓冲区的数据写入指定NOR FLASH的地址下，
*		  写入数据长度为NumHalfwordToWrite
****************************************************/
NOR_Status FSMC_NOR_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite)
{
  NOR_Status status = NOR_ONGOING; 

  do
  {
    status = FSMC_NOR_WriteHalfWord(WriteAddr, *pBuffer++);
    WriteAddr = WriteAddr + 2;
    NumHalfwordToWrite--;
  }
  while((status == NOR_SUCCESS) && (NumHalfwordToWrite != 0));
  
  return (status); 
}
/***************************************************
*函数名:FSMC_NOR_ReadHalfWord
*输入参数:
*		 ReadAddr：数据缓冲区
*返回值:
*      	uint16_t：返回读取到的数据
*函数说明:将指定NOR FLASH内部地址下的数据读出
*		 
****************************************************/
uint16_t FSMC_NOR_ReadHalfWord(uint32_t ReadAddr)
{
  NOR_WRITE(ADDR_SHIFT(0x5555), 0x00AA); 
  NOR_WRITE(ADDR_SHIFT(0x2AAA), 0x0055);  
  NOR_WRITE((Bank1_NOR2_ADDR + ReadAddr), 0x00F0 );

  return (*(__IO uint16_t *)((Bank1_NOR2_ADDR + ReadAddr)));
}

/***************************************************
*函数名:FSMC_NOR_ReadBuffer
*输入参数:
*		 pBuffer：数据缓冲区
*		 ReadAddr：NOR FLASH内部地址
*		 NumHalfwordToRead: 读出数据长度，单位为半字
*返回值:
*      无
*函数说明:将缓冲区的数据写入指定NOR FLASH的地址下，
*		  写入数据长度为NumHalfwordToWrite
****************************************************/
void FSMC_NOR_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead)
{
  NOR_WRITE(ADDR_SHIFT(0x5555), 0x00AA);
  NOR_WRITE(ADDR_SHIFT(0x2AAA), 0x0055);
  NOR_WRITE((Bank1_NOR2_ADDR + ReadAddr), 0x00F0);

  for(; NumHalfwordToRead != 0x00; NumHalfwordToRead--) 
  {

    *pBuffer++ = *(__IO uint16_t *)((Bank1_NOR2_ADDR + ReadAddr));
    ReadAddr = ReadAddr + 2; 
  }  
}

/***************************************************
*函数名:FSMC_NOR_ReturnToReadMode
*输入参数:
*		 无
*返回值:
*      	NOR_Status:返回NOR FLASH的状态
*函数说明:使NOR FLASH返回到读模式
*		 
****************************************************/
NOR_Status FSMC_NOR_ReturnToReadMode(void)
{
  NOR_WRITE(Bank1_NOR2_ADDR, 0x00F0);

  return (NOR_SUCCESS);
}
/***************************************************
*函数名:FSMC_NOR_Reset
*输入参数:
*		 无
*返回值:
*      	NOR_Status:返回NOR FLASH的状态
*函数说明:复位NOR FLASH
*		 
****************************************************/
NOR_Status FSMC_NOR_Reset(void)
{
  NOR_WRITE(ADDR_SHIFT(0x5555), 0x00AA); 
  NOR_WRITE(ADDR_SHIFT(0x2AAA), 0x0055); 
  NOR_WRITE(Bank1_NOR2_ADDR, 0x00F0); 

  return (NOR_SUCCESS);
}

/***************************************************
*函数名:FSMC_NOR_GetStatus
*输入参数:
*		 Timeout：超时时间
*返回值:
*      	NOR_Status:返回NOR FLASH的状态
*函数说明:读取NOR FLASH 的状态
*		 
****************************************************/
NOR_Status FSMC_NOR_GetStatus(uint32_t Timeout)
{ 
  uint16_t val1 = 0x00, val2 = 0x00;
  NOR_Status status = NOR_ONGOING; 
  uint32_t timeout = Timeout;

  /* 轮询NOR FLASH的状态管脚------------------------------------*/
  while((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) != RESET) && (timeout > 0)) 
  {
    timeout--;
  }

  timeout = Timeout;
  
  while((GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_6) == RESET) && (timeout > 0))   
  {
    timeout--;
  }
  
  /* 获取NOR FLASH的状态-------------------------------------*/
  while((Timeout != 0x00) && (status != NOR_SUCCESS))
  {
    Timeout--;

    val1 = *(__IO uint16_t *)(Bank1_NOR2_ADDR);
    val2 = *(__IO uint16_t *)(Bank1_NOR2_ADDR);
    if((val1 & 0x0040) == (val2 & 0x0040)) 
    {
      return NOR_SUCCESS;
    }

    if((val1 & 0x0020) != 0x0020)
    {
      status = NOR_ONGOING;
    }

    val1 = *(__IO uint16_t *)(Bank1_NOR2_ADDR);
    val2 = *(__IO uint16_t *)(Bank1_NOR2_ADDR);
    
    if((val1 & 0x0040) == (val2 & 0x0040)) 
    {
      return NOR_SUCCESS;
    }
    else if((val1 & 0x0020) == 0x0020)
    {
      return NOR_ERROR;
    }
  }

  if(Timeout == 0x00)
  {
    status = NOR_TIMEOUT;
  } 

  /* 返回状态 */
  return (status);
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
