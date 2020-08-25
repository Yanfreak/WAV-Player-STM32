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
#ifndef __FSMC_NAND_H
#define __FSMC_NAND_H

/****ͷ�ļ�*******************************************************/
#include "stm32f2xx.h"

/* �������Ͷ��� ------------------------------------------------------------*/
typedef struct
{
  u8 Maker_ID;
  u8 Device_ID;
  u8 Third_ID;
  u8 Fourth_ID;
}NAND_IDTypeDef;

typedef struct 
{
  u16 Zone;    //����K9F1G08U0Aֻ��һ������
  u16 Block;   //��
  u16 Page;    //ҳ
} NAND_ADDRESS;

#define NAND_SAMSUNG_MakerID     0xEC
#define NAND_SAMSUNG_DeviceID    0xF1   /* K9F1G08U0A,  128MB */
/* NAND FLASH ����궨�� */
#define CMD_AREA                   (u32)(1<<16)  /* A16 = CLE �� */
#define ADDR_AREA                  (u32)(1<<17)  /* A17 = ALE �� */

#define DATA_AREA                  ((u32)0x00000000) 

#define	NAND_CMD_READ_1             ((u8)0x00)
#define	NAND_CMD_READ_TRUE          ((u8)0x30)

#define	NAND_CMD_RDCOPYBACK         ((u8)0x00)
#define	NAND_CMD_RDCOPYBACK_TRUE    ((u8)0x35)

#define NAND_CMD_PAGEPROGRAM        ((u8)0x80)
#define NAND_CMD_PAGEPROGRAM_TRUE   ((u8)0x10)

#define NAND_CMD_COPYBACKPGM        ((u8)0x85)
#define NAND_CMD_COPYBACKPGM_TRUE   ((u8)0x10)
	
#define NAND_CMD_ERASE0             ((u8)0x60)
#define NAND_CMD_ERASE1             ((u8)0xD0)  

#define NAND_CMD_READID             ((u8)0x90)	
#define NAND_CMD_STATUS             ((u8)0x70)
#define NAND_CMD_RESET              ((u8)0xFF)

#define NAND_CMD_CACHEPGM           ((u8)0x80)
#define NAND_CMD_CACHEPGM_TRUE      ((u8)0x15)

#define NAND_CMD_RANDOMIN           ((u8)0x85)
#define NAND_CMD_RANDOMOUT          ((u8)0x05)
#define NAND_CMD_RANDOMOUT_TRUE     ((u8)0xE0)

#define NAND_CMD_CACHERD_START      ((u8)0x00)
#define NAND_CMD_CACHERD_START2     ((u8)0x31)
#define NAND_CMD_CACHERD_EXIT       ((u8)0x34)


/* NAND FLASH����״̬���� */
#define NAND_VALID_ADDRESS         ((u32)0x00000100)
#define NAND_INVALID_ADDRESS       ((u32)0x00000200)
#define NAND_TIMEOUT_ERROR         ((u32)0x00000400)
#define NAND_BUSY                  ((u32)0x00000000)
#define NAND_ERROR                 ((u32)0x00000001)
#define NAND_READY                 ((u32)0x00000040)

/* NAND FLASH ��ز������� */
#define NAND_SECTION_SIZE           0x0800
#define NAND_PAGE_SIZE             ((u16)0x0800) /* 2K�ֽ�ÿҳ*/
#define NAND_BLOCK_SIZE            ((u16)0x0040) /* 128K�ֽ�ÿ�飬64ҳ */
#define NAND_ZONE_SIZE             ((u16)0x0400) 
#define NAND_SPARE_AREA_SIZE       ((u16)0x0040) /* 64�ֽڿ���ÿҳ*/
#define NAND_MAX_ZONE              ((u16)0x0001) 
/* ��ַ�궨�� -----------------------------------------------------------*/
#define FSMC_Bank_NAND     FSMC_Bank2_NAND
#define Bank_NAND_ADDR     Bank2_NAND_ADDR 
#define Bank2_NAND_ADDR    ((u32)0x70000000)
/* NAND FLASH ��ַ�궨�� */
#define ADDR_1st_CYCLE(ADDR)       (u8)((ADDR)& 0xFF)               /* ��һ����ַ���� */
#define ADDR_2nd_CYCLE(ADDR)       (u8)(((ADDR)& 0xFF00) >> 8)      /* �ڶ�����ַ���� */
#define ADDR_3rd_CYCLE(ADDR)       (u8)(((ADDR)& 0xFF0000) >> 16)   /* ��������ַ���� */
#define ADDR_4th_CYCLE(ADDR)       (u8)(((ADDR)& 0xFF000000) >> 24) /* ���ĸ���ַ���� */

/*��������------------------------------------------------------- */
void FSMC_NAND_Init(void);
void FSMC_NAND_ReadID(NAND_IDTypeDef* NAND_ID);
u32 FSMC_NAND_WriteSmallPage(u8 *pBuffer, NAND_ADDRESS Address, u32 NumPageToWrite);
u32 FSMC_NAND_ReadSmallPage (u8 *pBuffer, NAND_ADDRESS Address, u32 NumPageToRead);
u32 FSMC_NAND_WriteSpareArea(u8 *pBuffer, NAND_ADDRESS Address, u32 NumSpareAreaTowrite);
u32 FSMC_NAND_ReadSpareArea(u8 *pBuffer, NAND_ADDRESS Address, u32 NumSpareAreaToRead);
u32 FSMC_NAND_EraseBlock(NAND_ADDRESS Address);
u32 FSMC_NAND_Reset(void);
u32 FSMC_NAND_GetStatus(void);
u32 FSMC_NAND_ReadStatus(void);
u32 FSMC_NAND_AddressIncrement(NAND_ADDRESS* Address);
u32 FSMC_NAND_ReadPage(u8 *pBuffer, u32 StartPage, u32 NumPageToRead);
u32 FSMC_NAND_WritePage(u8 *pBuffer, u32 StartPage, u32 NumPageToWrite);
u32 FSMC_NAND_EraseBlockID(int block_id);
#endif 

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
