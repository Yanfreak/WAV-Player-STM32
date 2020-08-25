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

/* 头文件------------------------------------------------------------------*/
#include "stm32_eval.h"
#include "stm322xg_eval_sram.h"
/* 宏定义------------------------------------------------------------*/
/*********缓冲区大小*****************************************/
#define BUFFER_SIZE        0x100
/**********SRAM内部读写地址****************************************/
#define WRITE_READ_ADDR    0x800
/* 全局变量定义 ---------------------------------------------------------*/
/*********读写缓冲区*****************************************/
uint16_t TxBuffer[BUFFER_SIZE], RxBuffer[BUFFER_SIZE];
/************读写状态**************************************/
__IO uint32_t WriteReadStatus = 0;
uint32_t Index = 0;

/* 函数声明-----------------------------------------------*/
/********填充数据函数******************************************/
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
  /* LED灯初始化 */
	STM_EVAL_LEDInit(LED2);
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDOff(LED2);
	STM_EVAL_LEDOff(LED3);
	   
  /* SRAM初始化  */
    SRAM_Init();

  /* 产生数据 */
    Fill_Buffer(TxBuffer, BUFFER_SIZE, 0x320F);

  /*将TxBuffer中的数据写到WRITE_READ_ADDR地址下，大小为BUFFER_SIZE*/
    SRAM_WriteBuffer(TxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);

  /* 从 SRAM的WRITE_READ_ADDR地址下读取BUFFER_SIZE字节数据到RxBuffer中*/
    SRAM_ReadBuffer(RxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);  

  /* 将读出的数据与原始数据对比*/   
	for (Index = 0x00; (Index < BUFFER_SIZE) && (WriteReadStatus == 0); Index++)
    {
     if (RxBuffer[Index] != TxBuffer[Index])
	    {
		  /*如果出现数据不一致*/
	      WriteReadStatus++;
	    }
    }	

  if (WriteReadStatus == 0)
  {
    /* 全部数据一致 */
	/* 点亮 LED2*/
	 STM_EVAL_LEDOn(LED2);
  }
  else
  { 
    /* 有数据不一致 */
    /* 点亮 LED3*/

    STM_EVAL_LEDOn(LED3);   
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


/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
