 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： main.c
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
#include "stm322xg_eval_nand.h"


u32 gecc=0;
//#define ROW_ADDRESS (Address.Page + (Address.Block + (Address.Zone * NAND_ZONE_SIZE)) * NAND_BLOCK_SIZE)
/***************************************************
*函数名:nand_delay
*输入参数:
*		count:延时长度
*返回值:无
*函数说明:延时函数
*
****************************************************/
static void nand_delay(int count)
{
    int i;
    for(i=0;i<count;i++)
    {
        ;;
    }
}
/***************************************************
*函数名:FSMC_NAND_Init
*输入参数:
*		无
*返回值:无
*函数说明:NAND FLASH初始化，包括相关GPIO口，FSMC总线
*
****************************************************/
void FSMC_NAND_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure; 
  FSMC_NANDInitTypeDef FSMC_NANDInitStructure;
  FSMC_NAND_PCCARDTimingInitTypeDef  p;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | 
                         RCC_AHB1Periph_GPIOF | RCC_AHB1Periph_GPIOG, ENABLE);
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
  /*-- GPIO 配置------------------------------------------------------*/
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15 |  
                                 GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5 | 
                                 GPIO_Pin_7;                                  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource0,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource1,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource4,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource7,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource11,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource12,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource14,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource15,GPIO_AF_FSMC);

  /* 数据线 配置 */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);

  /* NWAIT线配置*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   							 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_FSMC);
  /* INT2 线配置 */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   							 
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  /*-- FSMC 配置 ------------------------------------------------------*/
  p.FSMC_SetupTime = 0x1;
  p.FSMC_WaitSetupTime = 0x3;
  p.FSMC_HoldSetupTime = 0x2;
  p.FSMC_HiZSetupTime = 0x1;

  FSMC_NANDInitStructure.FSMC_Bank = FSMC_Bank2_NAND;
  FSMC_NANDInitStructure.FSMC_Waitfeature = FSMC_Waitfeature_Disable;
  FSMC_NANDInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_8b;
  FSMC_NANDInitStructure.FSMC_ECC = FSMC_ECC_Disable;
  FSMC_NANDInitStructure.FSMC_ECCPageSize = FSMC_ECCPageSize_2048Bytes;
  FSMC_NANDInitStructure.FSMC_TCLRSetupTime = 0x00;
  FSMC_NANDInitStructure.FSMC_TARSetupTime = 0x00;
  FSMC_NANDInitStructure.FSMC_CommonSpaceTimingStruct = &p;
  FSMC_NANDInitStructure.FSMC_AttributeSpaceTimingStruct = &p;

  FSMC_NANDInit(&FSMC_NANDInitStructure);

  FSMC_NANDCmd(FSMC_Bank2_NAND, ENABLE);
}

/***************************************************
*函数名:FSMC_NAND_ReadID
*输入参数:
*		 NAND_ID：NAND FLASH ID存放变量
*返回值:无
*函数说明:读取NAND FLASH 的ID号
*
****************************************************/
void FSMC_NAND_ReadID(NAND_IDTypeDef* NAND_ID)
{
   u32 data = 0;

  /* 发送命令 */ 	
  *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = 0x90;
  *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00;
	nand_delay(100);
   /* 读取ID */	
   data = *(vu32 *)(Bank_NAND_ADDR | DATA_AREA);

   NAND_ID->Maker_ID   = ADDR_1st_CYCLE (data);
   NAND_ID->Device_ID  = ADDR_2nd_CYCLE (data);
   NAND_ID->Third_ID   = ADDR_3rd_CYCLE (data);
   NAND_ID->Fourth_ID  = ADDR_4th_CYCLE (data);  
}
/***************************************************
*函数名:FSMC_NAND_WritePage
*输入参数:
*		 pBuffer：数据缓冲区
*        StartPage：NAND FLASH内部起始页地址
*		 NumPageToWrite：写入数据页数
*返回值:
*		u32:NAND FLASH的地址状态
*函数说明:向NAND FLASH的地址Address开始写入NumPageToWrite页的数据
*		  写入的数据放在pBuffer中
****************************************************/
u32 FSMC_NAND_WritePage(u8 *pBuffer, u32 StartPage, u32 NumPageToWrite)
{
  u32 Row_Address;
  NAND_ADDRESS Addresstmp;
  u32 index = 0x00, numpagewritten = 0x00, addressstatus = NAND_VALID_ADDRESS;
  u32 status = NAND_READY, size = 0x00;
  Addresstmp.Block=StartPage/64;
  Addresstmp.Page=StartPage%64;
  if(Addresstmp.Page==0)
  {
    FSMC_NAND_EraseBlock(Addresstmp);
  }
  while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
  while((NumPageToWrite != 0x00) && (addressstatus == NAND_VALID_ADDRESS) && (status == NAND_READY))
  {
    Row_Address=Addresstmp.Page + (Addresstmp.Block )*NAND_BLOCK_SIZE;

    /* 发送页编程命令和地址*/
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM;

    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00;  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00;  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(Row_Address);  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(Row_Address);   
	nand_delay(1000);
    /* 计算数据大小 */
    size = NAND_PAGE_SIZE+ (NAND_PAGE_SIZE * numpagewritten);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,ENABLE);
    /* 写入数据 */
    for(; index < size; index++)
    {
      *(vu8 *)(Bank_NAND_ADDR | DATA_AREA) = pBuffer[index];
    }
    
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM_TRUE;

    while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
    gecc=(u32)FSMC_GetECC(FSMC_Bank2_NAND);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,DISABLE);
    /* 检查数据是否写入完成 */
    status = FSMC_NAND_GetStatus();
    
    if(status == NAND_READY)
    {
      numpagewritten++;

      NumPageToWrite--;

      /* 计算下一个页的地址 */
      addressstatus = FSMC_NAND_AddressIncrement(&Addresstmp);    
    }    
  }
  
  //return (status | addressstatus);
  return gecc;
}
/***************************************************
*函数名:FSMC_NAND_WriteSmallPage
*输入参数:
*		 pBuffer：数据缓冲区
*        Address：NAND FLASH内部地址
*		 NumPageToWrite：写入数据占用页数
*返回值:
*		u32:NAND FLASH的地址状态
*函数说明:向NAND FLASH的地址Address开始写入NumPageToWrite页的数据
*		  写入的数据放在pBuffer中
****************************************************/


u32 FSMC_NAND_WriteSmallPage(u8 *pBuffer, NAND_ADDRESS Address, u32 NumPageToWrite)
{
  u32 Row_Address;
  NAND_ADDRESS Addresstmp=Address;
  u32 index = 0x00, numpagewritten = 0x00, addressstatus = NAND_VALID_ADDRESS;
  u32 status = NAND_READY, size = 0x00;
  while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
  while((NumPageToWrite != 0x00) && (addressstatus == NAND_VALID_ADDRESS) && (status == NAND_READY))
  {
    Row_Address=Addresstmp.Page + (Addresstmp.Block )*NAND_BLOCK_SIZE;

    /* 发送页编程命令和地址*/
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM;

    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00;  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00;  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(Row_Address);  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(Row_Address);   
	nand_delay(1000);
    /* 计算数据大小 */
    size = NAND_PAGE_SIZE+ (NAND_PAGE_SIZE * numpagewritten);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,ENABLE);
    /* 写入数据 */
    for(; index < size; index++)
    {
      *(vu8 *)(Bank_NAND_ADDR | DATA_AREA) = pBuffer[index];
    }
    
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM_TRUE;

    while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
    gecc=(u32)FSMC_GetECC(FSMC_Bank2_NAND);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,DISABLE);
    /* 检查数据是否写入完成 */
    status = FSMC_NAND_GetStatus();
    
    if(status == NAND_READY)
    {
      numpagewritten++;

      NumPageToWrite--;

      /* 计算下一个页的地址 */
      addressstatus = FSMC_NAND_AddressIncrement(&Addresstmp);    
    }    
  }
  
  //return (status | addressstatus);
  return gecc;
}
/***************************************************
*函数名:FSMC_NAND_ReadPage
*输入参数:
*		 StartPage：数据缓冲区
*        Address：NAND FLASH内部地址
*		 NumPageToRead：读取数据占用页数
*返回值:
*		u32:NAND FLASH的地址状态
*函数说明:从NAND FLASH的Address地址开始读取NumPageToRead页数据到
*		 pBuffer
****************************************************/
u32 FSMC_NAND_ReadPage(u8 *pBuffer, u32 StartPage, u32 NumPageToRead)
{
  
  u32 Row_Address;
  NAND_ADDRESS  Addresstmp;
  u32 index = 0x00, numpageread = 0x00, addressstatus = NAND_VALID_ADDRESS;
  u32 status = NAND_READY, size = 0x00;
  Addresstmp.Block=StartPage/64;
  Addresstmp.Page=StartPage%64;
  while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
  while((NumPageToRead != 0x0) && (addressstatus == NAND_VALID_ADDRESS))
  {	 
    Row_Address=Addresstmp.Page + (Addresstmp.Block )*NAND_BLOCK_SIZE;
    
    /* 发送读命令和起始地址*/
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ_1; 
   
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(Row_Address);  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(Row_Address);  
    
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ_TRUE; 
   	 nand_delay(1000);

    while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
    
    /* 计算数据大小 */
    size = NAND_PAGE_SIZE+ (NAND_PAGE_SIZE * numpageread);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,ENABLE);
    /* 读取数据到缓冲区中 */    
    for(; index < size; index++)
    {
      pBuffer[index]= *(vu8 *)(Bank_NAND_ADDR | DATA_AREA);
    }
    gecc=FSMC_GetECC(FSMC_Bank2_NAND);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,DISABLE);
    status = FSMC_NAND_GetStatus();
    numpageread++;
    
    NumPageToRead--;

    /* 计算页地址*/           			 
    addressstatus = FSMC_NAND_AddressIncrement(&Addresstmp);
  }

  status = FSMC_NAND_GetStatus();
  
  return (status | addressstatus);
  //return gecc;
}
/***************************************************
*函数名:FSMC_NAND_ReadSmallPage
*输入参数:
*		 pBuffer：数据缓冲区
*        Address：NAND FLASH内部地址
*		 NumPageToRead：读取数据占用页数
*返回值:
*		u32:NAND FLASH的地址状态
*函数说明:从NAND FLASH的Address地址开始读取NumPageToRead页数据到
*		 pBuffer
****************************************************/
u32 FSMC_NAND_ReadSmallPage(u8 *pBuffer, NAND_ADDRESS Address, u32 NumPageToRead)
{
  
  u32 Row_Address;
  NAND_ADDRESS  Addresstmp=Address;
  u32 index = 0x00, numpageread = 0x00, addressstatus = NAND_VALID_ADDRESS;
  u32 status = NAND_READY, size = 0x00;
  while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
  while((NumPageToRead != 0x0) && (addressstatus == NAND_VALID_ADDRESS))
  {	 
    Row_Address=Addresstmp.Page + (Addresstmp.Block )*NAND_BLOCK_SIZE;
    
    /* 发送读命令和起始地址*/
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ_1; 
   
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(Row_Address);  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(Row_Address);  
    
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ_TRUE; 
   	 nand_delay(1000);

    while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
    
    /* 计算数据大小 */
    size = NAND_PAGE_SIZE+ (NAND_PAGE_SIZE * numpageread);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,ENABLE);
    /* 读取数据到缓冲区中 */    
    for(; index < size; index++)
    {
      pBuffer[index]= *(vu8 *)(Bank_NAND_ADDR | DATA_AREA);
    }
    gecc=FSMC_GetECC(FSMC_Bank2_NAND);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,DISABLE);
    status = FSMC_NAND_GetStatus();
    numpageread++;
    
    NumPageToRead--;

    /* 计算页地址*/           			 
    addressstatus = FSMC_NAND_AddressIncrement(&Addresstmp);
  }

  status = FSMC_NAND_GetStatus();
  
  return (status /*| addressstatus*/);
  //return gecc;
}

/***************************************************
*函数名:FSMC_NAND_WriteSpareArea
*输入参数:
*		 pBuffer：数据缓冲区
*        Address：NAND FLASH内部地址
*		 NumSpareAreaTowrite：写入数据字节数
*返回值:
*		u32:NAND FLASH的地址状态
*函数说明:向NAND FLASH的Address地址的保留区写入NumSpareAreaTowrite字节个数据
*		  原始数据在pBuffer中
****************************************************/
u32 FSMC_NAND_WriteSpareArea(u8 *pBuffer, NAND_ADDRESS Address, u32 NumSpareAreaTowrite)
{
  u32 Row_Address;
  u32 index = 0x00, numsparesreawritten = 0x00, addressstatus = NAND_VALID_ADDRESS;
  u32 status = NAND_READY, size = 0x00; 

  while((NumSpareAreaTowrite != 0x00) && (addressstatus == NAND_VALID_ADDRESS) && (status == NAND_READY))
  {
    Row_Address=Address.Page + (Address.Block )*NAND_BLOCK_SIZE;

    /* 发写保留区的命令和地址 */
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM;

    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x08; 
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(Row_Address);  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(Row_Address);   
	nand_delay(1000);
    /* 计算大小 */ 
    size = NAND_SPARE_AREA_SIZE + (NAND_SPARE_AREA_SIZE * numsparesreawritten);

    /* 写入数据 */ 
    for(; index < size; index++)
    {
      *(vu8 *)(Bank_NAND_ADDR | DATA_AREA) = pBuffer[index];
    }
    
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM_TRUE;

    
    while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );

    /* 检测操作是否完成*/
    status = FSMC_NAND_GetStatus();

    if(status == NAND_READY)
    {
      numsparesreawritten++;      

      NumSpareAreaTowrite--;  
    
      /* 计算下一个地址 */
      addressstatus = FSMC_NAND_AddressIncrement(&Address);
    }       
  }
  
  return (status | addressstatus);
}

/***************************************************
*函数名:FSMC_NAND_ReadSpareArea
*输入参数:
*		 pBuffer：数据缓冲区
*        Address：NAND FLASH内部地址
*		 NumSpareAreaToRead：读出数据字节数
*返回值:
*		u32:NAND FLASH的地址状态
*函数说明:从NAND FLASH的Address地址保留区读取NumSpareAreaToRead个字节数据
*		到pBuffer 
****************************************************/
u32 FSMC_NAND_ReadSpareArea(u8 *pBuffer, NAND_ADDRESS Address, u32 NumSpareAreaToRead)
{
  u32 Row_Address;
  u32 numsparearearead = 0x00, index = 0x00, addressstatus = NAND_VALID_ADDRESS;
  u32 status = NAND_READY, size = 0x00;

  while((NumSpareAreaToRead != 0x0) && (addressstatus == NAND_VALID_ADDRESS))
  {  
    Row_Address=Address.Page + Address.Block* NAND_BLOCK_SIZE;   
    /* 发送读命令和地址 */     
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ_1;

    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x08;     
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(Row_Address);  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(Row_Address);    

    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ_TRUE;
	nand_delay(1000);
    
    while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
    
    /* 数据读取 */
    size = NAND_SPARE_AREA_SIZE +  (NAND_SPARE_AREA_SIZE * numsparearearead);
	
    /* 将数据放到缓存中 */
    for ( ;index < size; index++)
    {
      pBuffer[index] = *(vu8 *)(Bank_NAND_ADDR | DATA_AREA);
    }
    
    numsparearearead++;
    
    NumSpareAreaToRead--;

    /* 计算页地址*/           			 
    addressstatus = FSMC_NAND_AddressIncrement(&Address);
  }

  status = FSMC_NAND_GetStatus();

  return (status | addressstatus);
}

/***************************************************
*函数名:FSMC_NAND_EraseBlock
*输入参数:
*        Address：NAND FLASH内部地址
*返回值:
*		u32:NAND FLASH的地址状态
*函数说明:擦除地址所在的块
*		 
****************************************************/
u32 FSMC_NAND_EraseBlock(NAND_ADDRESS Address)
{
  u32 Row_Address=Address.Page + Address.Block* NAND_BLOCK_SIZE;
  *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_ERASE0;
  
  *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(Row_Address);  
  *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(Row_Address);  
		
  *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_ERASE1; 

  nand_delay(1000);
  while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
  
  return (FSMC_NAND_GetStatus());
}
/***************************************************
*函数名:FSMC_NAND_EraseBlock
*输入参数:
*        Address：NAND FLASH内部地址
*返回值:
*		u32:NAND FLASH的地址状态
*函数说明:擦除地址所在的块
*		 
****************************************************/
u32 FSMC_NAND_EraseBlockID(int block_id)
{
  NAND_ADDRESS Address;
  u32 Row_Address=0;
  Address.Page=0;
  Address.Block=block_id;
  Row_Address=Address.Page + Address.Block* NAND_BLOCK_SIZE;
  *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_ERASE0;
  
  *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(Row_Address);  
  *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(Row_Address);  
		
  *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_ERASE1; 

  nand_delay(1000);
  while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
  
  return (FSMC_NAND_GetStatus());
}
/***************************************************
*函数名:FSMC_NAND_Reset
*输入参数:
*        无
*返回值:
*		u32:NAND FLASH的状态
*函数说明:复位NAND　FLASH
*		 
****************************************************/
u32 FSMC_NAND_Reset(void)
{
  *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_RESET;
   nand_delay(1000);
  return (NAND_READY);
}

/***************************************************
*函数名:FSMC_NAND_GetStatus
*输入参数:
*        无
*返回值:
*		u32:NAND FLASH的状态
*函数说明:获取NAND　FLASH的状态
*		 
****************************************************/
u32 FSMC_NAND_GetStatus(void)
{
  u32 timeout = 0x1000000, status = NAND_READY;

  status = FSMC_NAND_ReadStatus(); 

  /* 等待NAND　FLASH操作完成或者超时*/
  while ((status != NAND_READY) &&( timeout != 0x00))
  {
     status = FSMC_NAND_ReadStatus();
     timeout --;      
  }

  if(timeout == 0x00)
  {          
    status =  NAND_TIMEOUT_ERROR;      
  } 

  /* 返回状态 */
  return (status);      
}
/***************************************************
*函数名:FSMC_NAND_ReadStatus
*输入参数:
*        无
*返回值:
*		u32:NAND FLASH的状态
*函数说明:读取NAND　FLASH的状态
*		 
****************************************************/
u32 FSMC_NAND_ReadStatus(void)
{
  u32 data = 0x00, status = NAND_BUSY;

  /* 读状态操作 ------------------------------------ */
  *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_STATUS;
  data = *(vu8 *)(Bank_NAND_ADDR);

  if((data & NAND_ERROR) == NAND_ERROR)
  {
    status = NAND_ERROR;
  } 
  else if((data & NAND_READY) == NAND_READY)
  {
    status = NAND_READY;
  }
  else
  {
    status = NAND_BUSY; 
  }
  
  return (status);
}

/***************************************************
*函数名:FSMC_NAND_AddressIncrement
*输入参数:
*      Address：NAND FLASH内部页地址
*返回值:
*		u32:NAND FLASH的状态
*函数说明:NAND FLASH页地址自增1
*		 
****************************************************/
u32 FSMC_NAND_AddressIncrement(NAND_ADDRESS* Address)
{
  u32 status = NAND_VALID_ADDRESS;
 
  Address->Page++;

  if(Address->Page == NAND_BLOCK_SIZE)
  {
    Address->Page = 0;
    Address->Block++;
    
    if(Address->Block == NAND_ZONE_SIZE)
    {
      Address->Block = 0;
      Address->Zone++;

      if(Address->Zone == NAND_MAX_ZONE)
      {
        status = NAND_INVALID_ADDRESS;
      }
    }
  } 
  
  return (status);
}

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
