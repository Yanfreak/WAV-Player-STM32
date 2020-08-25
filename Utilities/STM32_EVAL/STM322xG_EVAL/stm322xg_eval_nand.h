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
#ifndef __FSMC_NAND_H
#define __FSMC_NAND_H

/****头文件*******************************************************/
#include "stm32f2xx.h"

/* 数据类型定义 ------------------------------------------------------------*/
typedef struct
{
  u8 Maker_ID;
  u8 Device_ID;
  u8 Third_ID;
  u8 Fourth_ID;
}NAND_IDTypeDef;

typedef struct 
{
  u16 Zone;    //区，K9F1G08U0A只有一个区。
  u16 Block;   //块
  u16 Page;    //页
} NAND_ADDRESS;

#define NAND_SAMSUNG_MakerID     0xEC
#define NAND_SAMSUNG_DeviceID    0xF1   /* K9F1G08U0A,  128MB */
/* NAND FLASH 命令宏定义 */
#define CMD_AREA                   (u32)(1<<16)  /* A16 = CLE 高 */
#define ADDR_AREA                  (u32)(1<<17)  /* A17 = ALE 高 */

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


/* NAND FLASH工作状态定义 */
#define NAND_VALID_ADDRESS         ((u32)0x00000100)
#define NAND_INVALID_ADDRESS       ((u32)0x00000200)
#define NAND_TIMEOUT_ERROR         ((u32)0x00000400)
#define NAND_BUSY                  ((u32)0x00000000)
#define NAND_ERROR                 ((u32)0x00000001)
#define NAND_READY                 ((u32)0x00000040)

/* NAND FLASH 相关参数定义 */
#define NAND_SECTION_SIZE           0x0800
#define NAND_PAGE_SIZE             ((u16)0x0800) /* 2K字节每页*/
#define NAND_BLOCK_SIZE            ((u16)0x0040) /* 128K字节每块，64页 */
#define NAND_ZONE_SIZE             ((u16)0x0400) 
#define NAND_SPARE_AREA_SIZE       ((u16)0x0040) /* 64字节空区每页*/
#define NAND_MAX_ZONE              ((u16)0x0001) 
/* 地址宏定义 -----------------------------------------------------------*/
#define FSMC_Bank_NAND     FSMC_Bank2_NAND
#define Bank_NAND_ADDR     Bank2_NAND_ADDR 
#define Bank2_NAND_ADDR    ((u32)0x70000000)
/* NAND FLASH 地址宏定义 */
#define ADDR_1st_CYCLE(ADDR)       (u8)((ADDR)& 0xFF)               /* 第一个地址周期 */
#define ADDR_2nd_CYCLE(ADDR)       (u8)(((ADDR)& 0xFF00) >> 8)      /* 第二个地址周期 */
#define ADDR_3rd_CYCLE(ADDR)       (u8)(((ADDR)& 0xFF0000) >> 16)   /* 第三个地址周期 */
#define ADDR_4th_CYCLE(ADDR)       (u8)(((ADDR)& 0xFF000000) >> 24) /* 第四个地址周期 */

/*函数声明------------------------------------------------------- */
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
