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
#include "main.h"
#include "stm322xg_eval_i2c_ee.h"
#include "stdlib.h"

/*宏定义 ------------------------------------------------------------*/
/***********写起始地址***************************************/
#define EEPROM_WriteAddress1    0x05

/************读起始地址**************************************/
#define EEPROM_ReadAddress1     0x05

/************缓冲区大小**************************************/
#define BufferSize1             (countof(Tx1_Buffer)-1)

/************计算数据大小宏**************************************/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* 全局变量定义 -----------------------------------------------------------*/
/***********状态枚举变量***************************************/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/***********发送数据数组***************************************/
u8 Tx1_Buffer[] = "/* STM32F10x I2C Firmware ";

/***********接收数据数组***************************************/
u8 Rx1_Buffer[BufferSize1];

/***********状态变量***************************************/
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;
    
/* 函数声明 ---------------------------------------------------------*/
/*********数据比较函数*****************************************/
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength);

/*********延时函数*****************************************/
void Delay(vu32 nCount);
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
  /* I2C EEPROM初始化 ----------------------------------------*/
  sEE_Init();  
 
  /* 向eeprom写入数据 */
  sEE_WriteBuffer(Tx1_Buffer, (uint16_t )EEPROM_WriteAddress1, (uint16_t)BufferSize1); 

  /* 从eeprom中读出数据*/
  sEE_ReadBuffer(Rx1_Buffer, (uint16_t )EEPROM_ReadAddress1, (uint16_t*)BufferSize1); 
  Delay(0x100000);

  /* 比较写入与读出的数据是否一致 一致返回PASSED */
  TransferStatus1 = Buffercmp(Tx1_Buffer, Rx1_Buffer, BufferSize1);

  while (1)
  {
	if(TransferStatus1 == PASSED)
	{
		/* 如果数据读写无误LED2亮 */
		STM_EVAL_LEDOn(LED2);
	    Delay(0x3AFFFF);
	
	}
	else
	{
	    /* 点亮LED3亮 */
	    STM_EVAL_LEDOn(LED3);
		Delay(0x3AFFFF);
	}
  }
}

/***************************************************************
*函数名:Buffercmp
*函数说明:比较两个缓冲区的数据是否一致，一致返回PASSED
*		  不一致返回FAILED
*输入参数:
*		参数pBuffer1:缓冲区1
*		参数pBuffer2:缓冲区2
*		参数BufferLength：要比较的数据大小
*返回值:
*	PASSED：数据比较一致
*	FAILED：数据比较不一致	
*****************************************************************/
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;  
}
/***************************************************************
*函数名:Delay
*函数说明:延时函数
*	
*输入参数:
*		nCount：延时大小
*返回值:无	
*****************************************************************/
void Delay(vu32 nCount)
{
  int i;
  for(i=0;i<nCount;i++)
  {
  	;;
  }
}
/**
  * @}
  */


/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
