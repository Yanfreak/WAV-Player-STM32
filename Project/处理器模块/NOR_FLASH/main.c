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
*   完成日期 ：2012.08.02 
*
**********************************************************/

/* 头文件 ------------------------------------------------------------------*/
#include "stm322xg_eval_nor.h"
#include "stm32_eval.h"

/* 宏定义------------------------------------------------------------*/
/***********缓冲区大小***************************************/
#define BUFFER_SIZE        0x400

/**********NOR FLASH内部读写起始地址****************************************/
#define WRITE_READ_ADDR    0x8000

/* 全局变量定义 -------------------------------------------------------------*/
/********GPIO初始化结构体******************************************/
GPIO_InitTypeDef GPIO_InitStructure;

/*********发送数据缓冲区*****************************************/
uint16_t TxBuffer[BUFFER_SIZE];

/**********接收数据缓冲区****************************************/
uint16_t RxBuffer[BUFFER_SIZE];

/*********读写状态变量*****************************************/
uint32_t WriteReadStatus = 0, Index = 0;

/*********NOR FLASH ID 变量*****************************************/
NOR_IDTypeDef NOR_ID;

/* 函数声明 -----------------------------------------------*/
/*************数据填充函数*************************************/
void Fill_Buffer(uint16_t *pBuffer, uint16_t BufferLenght, uint32_t Offset);

/***************************************************
*函数名:main
*输入参数:无
*返回值:无
*函数说明:主函数
*
****************************************************/

int main(void)
{
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);

  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);     

  /* 使能FSMC的时钟 */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

  /* NOR FLASH初始化 */

  FSMC_NOR_Init();

  /* 读取NOR FLASH的ID */
  FSMC_NOR_ReadID(&NOR_ID);

  FSMC_NOR_ReturnToReadMode();

  /* 擦除NOR FLASH */
  FSMC_NOR_EraseBlock(WRITE_READ_ADDR);

  /* 产生数据 */
  Fill_Buffer(TxBuffer, BUFFER_SIZE, 0x3210);

  /*将产生的数据写入NOR FLASH*/
  FSMC_NOR_WriteBuffer(TxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);

  /* 从NOR FLASH中读取数据 */
  FSMC_NOR_ReadBuffer(RxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);  

  /* 将读取的数据与原数据进行对比 */   
  for (Index = 0x00; (Index < BUFFER_SIZE) && (WriteReadStatus == 0); Index++)
  {
    if (RxBuffer[Index] != TxBuffer[Index])
    {
      WriteReadStatus = Index + 1;
    }
  }

  if (WriteReadStatus == 0)
  {
    /* 数据读写无误点亮LED2 */
    STM_EVAL_LEDOn(LED2);
  }
  else
  { 
    /* 数据读写有误 */
    STM_EVAL_LEDOff(LED3);
  } 
      
  while (1)
  {
  }
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
void Fill_Buffer(uint16_t *pBuffer, uint16_t BufferLenght, uint32_t Offset)
{
  uint16_t IndexTmp = 0;

  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}

#ifdef  USE_FULL_ASSERT

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
void assert_failed(uint8_t* file, uint32_t line)
{ 
 /****此处可以添加用户错误报告方式**********************/
  while (1)
  {
  }
}
#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
