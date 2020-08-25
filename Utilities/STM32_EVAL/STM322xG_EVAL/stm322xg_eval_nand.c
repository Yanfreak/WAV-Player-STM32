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


u32 gecc=0;
//#define ROW_ADDRESS (Address.Page + (Address.Block + (Address.Zone * NAND_ZONE_SIZE)) * NAND_BLOCK_SIZE)
/***************************************************
*������:nand_delay
*�������:
*		count:��ʱ����
*����ֵ:��
*����˵��:��ʱ����
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
*������:FSMC_NAND_Init
*�������:
*		��
*����ֵ:��
*����˵��:NAND FLASH��ʼ�����������GPIO�ڣ�FSMC����
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
  /*-- GPIO ����------------------------------------------------------*/
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

  /* ������ ���� */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource7,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource8,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE,GPIO_PinSource10,GPIO_AF_FSMC);

  /* NWAIT������*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   							 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_UP;
  GPIO_Init(GPIOD, &GPIO_InitStructure); 
  GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_FSMC);
  /* INT2 ������ */  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;   							 
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  /*-- FSMC ���� ------------------------------------------------------*/
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
*������:FSMC_NAND_ReadID
*�������:
*		 NAND_ID��NAND FLASH ID��ű���
*����ֵ:��
*����˵��:��ȡNAND FLASH ��ID��
*
****************************************************/
void FSMC_NAND_ReadID(NAND_IDTypeDef* NAND_ID)
{
   u32 data = 0;

  /* �������� */ 	
  *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = 0x90;
  *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00;
	nand_delay(100);
   /* ��ȡID */	
   data = *(vu32 *)(Bank_NAND_ADDR | DATA_AREA);

   NAND_ID->Maker_ID   = ADDR_1st_CYCLE (data);
   NAND_ID->Device_ID  = ADDR_2nd_CYCLE (data);
   NAND_ID->Third_ID   = ADDR_3rd_CYCLE (data);
   NAND_ID->Fourth_ID  = ADDR_4th_CYCLE (data);  
}
/***************************************************
*������:FSMC_NAND_WritePage
*�������:
*		 pBuffer�����ݻ�����
*        StartPage��NAND FLASH�ڲ���ʼҳ��ַ
*		 NumPageToWrite��д������ҳ��
*����ֵ:
*		u32:NAND FLASH�ĵ�ַ״̬
*����˵��:��NAND FLASH�ĵ�ַAddress��ʼд��NumPageToWriteҳ������
*		  д������ݷ���pBuffer��
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

    /* ����ҳ�������͵�ַ*/
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM;

    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00;  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00;  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(Row_Address);  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(Row_Address);   
	nand_delay(1000);
    /* �������ݴ�С */
    size = NAND_PAGE_SIZE+ (NAND_PAGE_SIZE * numpagewritten);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,ENABLE);
    /* д������ */
    for(; index < size; index++)
    {
      *(vu8 *)(Bank_NAND_ADDR | DATA_AREA) = pBuffer[index];
    }
    
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM_TRUE;

    while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
    gecc=(u32)FSMC_GetECC(FSMC_Bank2_NAND);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,DISABLE);
    /* ��������Ƿ�д����� */
    status = FSMC_NAND_GetStatus();
    
    if(status == NAND_READY)
    {
      numpagewritten++;

      NumPageToWrite--;

      /* ������һ��ҳ�ĵ�ַ */
      addressstatus = FSMC_NAND_AddressIncrement(&Addresstmp);    
    }    
  }
  
  //return (status | addressstatus);
  return gecc;
}
/***************************************************
*������:FSMC_NAND_WriteSmallPage
*�������:
*		 pBuffer�����ݻ�����
*        Address��NAND FLASH�ڲ���ַ
*		 NumPageToWrite��д������ռ��ҳ��
*����ֵ:
*		u32:NAND FLASH�ĵ�ַ״̬
*����˵��:��NAND FLASH�ĵ�ַAddress��ʼд��NumPageToWriteҳ������
*		  д������ݷ���pBuffer��
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

    /* ����ҳ�������͵�ַ*/
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM;

    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00;  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00;  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(Row_Address);  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(Row_Address);   
	nand_delay(1000);
    /* �������ݴ�С */
    size = NAND_PAGE_SIZE+ (NAND_PAGE_SIZE * numpagewritten);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,ENABLE);
    /* д������ */
    for(; index < size; index++)
    {
      *(vu8 *)(Bank_NAND_ADDR | DATA_AREA) = pBuffer[index];
    }
    
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM_TRUE;

    while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
    gecc=(u32)FSMC_GetECC(FSMC_Bank2_NAND);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,DISABLE);
    /* ��������Ƿ�д����� */
    status = FSMC_NAND_GetStatus();
    
    if(status == NAND_READY)
    {
      numpagewritten++;

      NumPageToWrite--;

      /* ������һ��ҳ�ĵ�ַ */
      addressstatus = FSMC_NAND_AddressIncrement(&Addresstmp);    
    }    
  }
  
  //return (status | addressstatus);
  return gecc;
}
/***************************************************
*������:FSMC_NAND_ReadPage
*�������:
*		 StartPage�����ݻ�����
*        Address��NAND FLASH�ڲ���ַ
*		 NumPageToRead����ȡ����ռ��ҳ��
*����ֵ:
*		u32:NAND FLASH�ĵ�ַ״̬
*����˵��:��NAND FLASH��Address��ַ��ʼ��ȡNumPageToReadҳ���ݵ�
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
    
    /* ���Ͷ��������ʼ��ַ*/
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ_1; 
   
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(Row_Address);  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(Row_Address);  
    
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ_TRUE; 
   	 nand_delay(1000);

    while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
    
    /* �������ݴ�С */
    size = NAND_PAGE_SIZE+ (NAND_PAGE_SIZE * numpageread);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,ENABLE);
    /* ��ȡ���ݵ��������� */    
    for(; index < size; index++)
    {
      pBuffer[index]= *(vu8 *)(Bank_NAND_ADDR | DATA_AREA);
    }
    gecc=FSMC_GetECC(FSMC_Bank2_NAND);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,DISABLE);
    status = FSMC_NAND_GetStatus();
    numpageread++;
    
    NumPageToRead--;

    /* ����ҳ��ַ*/           			 
    addressstatus = FSMC_NAND_AddressIncrement(&Addresstmp);
  }

  status = FSMC_NAND_GetStatus();
  
  return (status | addressstatus);
  //return gecc;
}
/***************************************************
*������:FSMC_NAND_ReadSmallPage
*�������:
*		 pBuffer�����ݻ�����
*        Address��NAND FLASH�ڲ���ַ
*		 NumPageToRead����ȡ����ռ��ҳ��
*����ֵ:
*		u32:NAND FLASH�ĵ�ַ״̬
*����˵��:��NAND FLASH��Address��ַ��ʼ��ȡNumPageToReadҳ���ݵ�
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
    
    /* ���Ͷ��������ʼ��ַ*/
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ_1; 
   
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(Row_Address);  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(Row_Address);  
    
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ_TRUE; 
   	 nand_delay(1000);

    while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
    
    /* �������ݴ�С */
    size = NAND_PAGE_SIZE+ (NAND_PAGE_SIZE * numpageread);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,ENABLE);
    /* ��ȡ���ݵ��������� */    
    for(; index < size; index++)
    {
      pBuffer[index]= *(vu8 *)(Bank_NAND_ADDR | DATA_AREA);
    }
    gecc=FSMC_GetECC(FSMC_Bank2_NAND);
    FSMC_NANDECCCmd(FSMC_Bank2_NAND,DISABLE);
    status = FSMC_NAND_GetStatus();
    numpageread++;
    
    NumPageToRead--;

    /* ����ҳ��ַ*/           			 
    addressstatus = FSMC_NAND_AddressIncrement(&Addresstmp);
  }

  status = FSMC_NAND_GetStatus();
  
  return (status /*| addressstatus*/);
  //return gecc;
}

/***************************************************
*������:FSMC_NAND_WriteSpareArea
*�������:
*		 pBuffer�����ݻ�����
*        Address��NAND FLASH�ڲ���ַ
*		 NumSpareAreaTowrite��д�������ֽ���
*����ֵ:
*		u32:NAND FLASH�ĵ�ַ״̬
*����˵��:��NAND FLASH��Address��ַ�ı�����д��NumSpareAreaTowrite�ֽڸ�����
*		  ԭʼ������pBuffer��
****************************************************/
u32 FSMC_NAND_WriteSpareArea(u8 *pBuffer, NAND_ADDRESS Address, u32 NumSpareAreaTowrite)
{
  u32 Row_Address;
  u32 index = 0x00, numsparesreawritten = 0x00, addressstatus = NAND_VALID_ADDRESS;
  u32 status = NAND_READY, size = 0x00; 

  while((NumSpareAreaTowrite != 0x00) && (addressstatus == NAND_VALID_ADDRESS) && (status == NAND_READY))
  {
    Row_Address=Address.Page + (Address.Block )*NAND_BLOCK_SIZE;

    /* ��д������������͵�ַ */
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM;

    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x08; 
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(Row_Address);  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(Row_Address);   
	nand_delay(1000);
    /* �����С */ 
    size = NAND_SPARE_AREA_SIZE + (NAND_SPARE_AREA_SIZE * numsparesreawritten);

    /* д������ */ 
    for(; index < size; index++)
    {
      *(vu8 *)(Bank_NAND_ADDR | DATA_AREA) = pBuffer[index];
    }
    
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_PAGEPROGRAM_TRUE;

    
    while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );

    /* �������Ƿ����*/
    status = FSMC_NAND_GetStatus();

    if(status == NAND_READY)
    {
      numsparesreawritten++;      

      NumSpareAreaTowrite--;  
    
      /* ������һ����ַ */
      addressstatus = FSMC_NAND_AddressIncrement(&Address);
    }       
  }
  
  return (status | addressstatus);
}

/***************************************************
*������:FSMC_NAND_ReadSpareArea
*�������:
*		 pBuffer�����ݻ�����
*        Address��NAND FLASH�ڲ���ַ
*		 NumSpareAreaToRead�����������ֽ���
*����ֵ:
*		u32:NAND FLASH�ĵ�ַ״̬
*����˵��:��NAND FLASH��Address��ַ��������ȡNumSpareAreaToRead���ֽ�����
*		��pBuffer 
****************************************************/
u32 FSMC_NAND_ReadSpareArea(u8 *pBuffer, NAND_ADDRESS Address, u32 NumSpareAreaToRead)
{
  u32 Row_Address;
  u32 numsparearearead = 0x00, index = 0x00, addressstatus = NAND_VALID_ADDRESS;
  u32 status = NAND_READY, size = 0x00;

  while((NumSpareAreaToRead != 0x0) && (addressstatus == NAND_VALID_ADDRESS))
  {  
    Row_Address=Address.Page + Address.Block* NAND_BLOCK_SIZE;   
    /* ���Ͷ�����͵�ַ */     
    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ_1;

    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x00; 
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = 0x08;     
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_1st_CYCLE(Row_Address);  
    *(vu8 *)(Bank_NAND_ADDR | ADDR_AREA) = ADDR_2nd_CYCLE(Row_Address);    

    *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_READ_TRUE;
	nand_delay(1000);
    
    while( GPIO_ReadInputDataBit(GPIOG, GPIO_Pin_6) == 0 );
    
    /* ���ݶ�ȡ */
    size = NAND_SPARE_AREA_SIZE +  (NAND_SPARE_AREA_SIZE * numsparearearead);
	
    /* �����ݷŵ������� */
    for ( ;index < size; index++)
    {
      pBuffer[index] = *(vu8 *)(Bank_NAND_ADDR | DATA_AREA);
    }
    
    numsparearearead++;
    
    NumSpareAreaToRead--;

    /* ����ҳ��ַ*/           			 
    addressstatus = FSMC_NAND_AddressIncrement(&Address);
  }

  status = FSMC_NAND_GetStatus();

  return (status | addressstatus);
}

/***************************************************
*������:FSMC_NAND_EraseBlock
*�������:
*        Address��NAND FLASH�ڲ���ַ
*����ֵ:
*		u32:NAND FLASH�ĵ�ַ״̬
*����˵��:������ַ���ڵĿ�
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
*������:FSMC_NAND_EraseBlock
*�������:
*        Address��NAND FLASH�ڲ���ַ
*����ֵ:
*		u32:NAND FLASH�ĵ�ַ״̬
*����˵��:������ַ���ڵĿ�
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
*������:FSMC_NAND_Reset
*�������:
*        ��
*����ֵ:
*		u32:NAND FLASH��״̬
*����˵��:��λNAND��FLASH
*		 
****************************************************/
u32 FSMC_NAND_Reset(void)
{
  *(vu8 *)(Bank_NAND_ADDR | CMD_AREA) = NAND_CMD_RESET;
   nand_delay(1000);
  return (NAND_READY);
}

/***************************************************
*������:FSMC_NAND_GetStatus
*�������:
*        ��
*����ֵ:
*		u32:NAND FLASH��״̬
*����˵��:��ȡNAND��FLASH��״̬
*		 
****************************************************/
u32 FSMC_NAND_GetStatus(void)
{
  u32 timeout = 0x1000000, status = NAND_READY;

  status = FSMC_NAND_ReadStatus(); 

  /* �ȴ�NAND��FLASH������ɻ��߳�ʱ*/
  while ((status != NAND_READY) &&( timeout != 0x00))
  {
     status = FSMC_NAND_ReadStatus();
     timeout --;      
  }

  if(timeout == 0x00)
  {          
    status =  NAND_TIMEOUT_ERROR;      
  } 

  /* ����״̬ */
  return (status);      
}
/***************************************************
*������:FSMC_NAND_ReadStatus
*�������:
*        ��
*����ֵ:
*		u32:NAND FLASH��״̬
*����˵��:��ȡNAND��FLASH��״̬
*		 
****************************************************/
u32 FSMC_NAND_ReadStatus(void)
{
  u32 data = 0x00, status = NAND_BUSY;

  /* ��״̬���� ------------------------------------ */
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
*������:FSMC_NAND_AddressIncrement
*�������:
*      Address��NAND FLASH�ڲ�ҳ��ַ
*����ֵ:
*		u32:NAND FLASH��״̬
*����˵��:NAND FLASHҳ��ַ����1
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
