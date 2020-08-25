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
#include "stm32_eval.h"															 	
/* 宏定义-----------------------------------------------------------*/
/***********缓冲区大小***************************************/
#define BUFFER_SIZE         0x1000	/*  K9F11208 */

/*************NAND FLASH ID*************************************/



/* 全局变量定义 ---------------------------------------------------------*/
/**********NAND FLASH ID变量****************************************/
NAND_IDTypeDef NAND_ID;

/************NAND FLASH读写地址结构体变量**************************************/
NAND_ADDRESS WriteReadAddr;

/*************数据缓冲区*************************************/
u8 TxBuffer[BUFFER_SIZE], RxBuffer[BUFFER_SIZE];

/************状态变量，读写页数变量**************************************/
vu32 PageNumber = 2, WriteReadStatus = 0, status= 0;
u32 j = 0;

/* 函数声明 -----------------------------------------------*/
/*******填充数据函数*******************************************/
void Fill_Buffer(u8 *pBuffer, u16 BufferLenght, u32 Offset);
u32 ecc;
/***************************************************
*函数名:main
*输入参数:无
*返回值:无
*函数说明:主函数
*
****************************************************/
int main(void)
{ 
  int i;
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);
  /* NAND FLASH初始化*/
  FSMC_NAND_Init();

  /***NAND FLASH复位***/
//  FSMC_NAND_Reset();
  /* 读取NAND FLASH的ID */
  FSMC_NAND_ReadID(&NAND_ID);

  /* 验证 NAND FLASH ID是否正确 */  
  if((NAND_ID.Maker_ID == NAND_SAMSUNG_MakerID) && (NAND_ID.Device_ID == NAND_SAMSUNG_DeviceID))
  {

    /* 初始化NAND FLASH读写地址 */ 
    WriteReadAddr.Zone = 0x00;
    WriteReadAddr.Block = 0x00;
    WriteReadAddr.Page = 0x00; 

    /* 擦除NAND FLASH */
   // status = FSMC_NAND_EraseBlock(WriteReadAddr);

    /* 生成数据********/
  //  Fill_Buffer(TxBuffer, BUFFER_SIZE , 0x5);
    	//for(i = 0; i < 2048; i ++)
    	//TxBuffer[i] = i / 5 - i;
	/*将生成的数据写入到NAND FLASH中*/
   //ecc = (u32)FSMC_NAND_WriteSmallPage(TxBuffer, WriteReadAddr, PageNumber);
    /*从NAND FLASH中读出数据*/
   ecc = (u32)FSMC_NAND_ReadSmallPage (RxBuffer, WriteReadAddr, PageNumber);

    /* 读出数据与原始数据对比 */
    for(j = 0; j < BUFFER_SIZE; j++)
    {
      if(TxBuffer[j] != RxBuffer[j])
      {     
        WriteReadStatus++;
      } 
    }

    if (WriteReadStatus == 0)
    {
      /* 如果读写无误点亮LED2 */
      STM_EVAL_LEDOn(LED2);
    }
    else
    { 
      /* 如果数据读写有误，点亮LED3 */
      STM_EVAL_LEDOn(LED3);     
    }
  }
  else
  {
    /* 如果NAND FLASH ID不正确，点亮LED4*/
    STM_EVAL_LEDOn(LED4);  
  }

	return (0);
}

 

/***************************************************************
*函数名:Fill_Buffer
*函数说明:初始化pBuffer中的数据，以Offset起始自增1,
*			缓冲区长度为为BufferLenght个short型数据
*输入参数:
*		参数pBuffer:指向生成数据存放的缓冲区
*		参数BufferLenght:缓冲区大小
*		参数Offset：数据自增起始大小
*返回值:无	
*****************************************************************/
void Fill_Buffer(u8 *pBuffer, u16 BufferLenght, u32 Offset)
{
  u16 IndexTmp = 0;

  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}

#ifdef  DEBUG
/***************************************************************
*函数名:assert_failed
*函数说明:报告参数错误的位置
*	
*输入参数:
*		file:错误所在的文件名
*		line:错误所在文件中的行号
*	
*返回值:无	
*****************************************************************/
void assert_failed(u8* file, u32 line)
{ 
 /****此处可以添加用户错误报告方式**********************/
  while (1)
  {
  }
}
#endif
/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
