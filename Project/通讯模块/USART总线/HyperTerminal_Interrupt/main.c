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
/* 头文件声明 ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "stm32_eval.h"
#include "stm322xg_eval_nand.h"
#include "stm322xg_eval_sram.h"
#include "stm32f2xx_it.h"
#define  buffer   	((uint8_t*)0x68000100) //外部sram
/* 类型定义  -------------------------------------------------------------------*/
/* 宏定义  ---------------------------------------------------------------------*/
/* 变量定义 --------------------------------------------------------------------*/

USART_InitTypeDef USART_InitStructure;	   /* 串口初始化结构体*/
extern uint32_t NbrOfDataToRead;			   /* 记录允许接收数据的数量*/
extern __IO uint32_t RxCounter; 		   /* 记录已接收数量*/
u8 Buffer[255];
NAND_IDTypeDef NAND_ID;
NAND_ADDRESS WriteReadAddr;
vu32 PageNumber = 64;
/* 函数定义---------------------------------------------------------------------*/
void NVIC_Config(void);

/******************************************************************************
* 函数名：main 
* 功  能： 
* 输  入： 
* 输  出：无  
* 备  注：无
********************************************************************************/
int main(void)
{
  /*在此阶段处理器的时钟已经被初始化完成，时钟初始化工作由SystemInit函数实现，SystemInit函数
   *存放在startup_stm32f2xx.s的汇编启动文件中。
   *在系统上电初始化到跳转到main函数之前，启动代码会先调用SystemInit函数。
   *	
   *如果用户希望重新配置时钟可以在此处再次调用 SystemInit()，它的函数实现存放在system_stm32f2xx.c文件中。
   */
    u16 i;
   	u8 j;
  NVIC_Config();	/* NVIC配置，打开USART1中断*/
	SRAM_Init();
		 /* NAND FLASH初始化*/
 FSMC_NAND_Init();

  /***NAND FLASH复位***/
	FSMC_NAND_Reset();
  /* 读取NAND FLASH的ID */
  FSMC_NAND_ReadID(&NAND_ID);

  /* 验证 NAND FLASH ID是否正确 */  
 if((NAND_ID.Maker_ID == NAND_SAMSUNG_MakerID) && (NAND_ID.Device_ID == NAND_SAMSUNG_DeviceID))
  {    WriteReadAddr.Zone = 0x00;
    WriteReadAddr.Block = 0x00;
    WriteReadAddr.Page = 0x00; 
		 for(i=0;i<1024;i++)
    {
    	WriteReadAddr.Block=i;
    	WriteReadAddr.Zone=0;
    	WriteReadAddr.Page=0;
    	FSMC_NAND_EraseBlock(WriteReadAddr); 
    }
  }
	WriteReadAddr.Block=0;
/* USART1 配置 ------------------------------------------------------*/
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
	 USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(COM1, &USART_InitStructure);	 /* 采用USART_InitStructure参数设置，配置串口USART1 */
		for(i=0;i<4096;i++)buffer[i]=0;
	for(j=0;j<31;j++){

  /* 使能串口USART1的发送中断：当USART1的传输数据寄存器为空时，将会产生发送中断*/
 // USART_ITConfig(EVAL_COM1, USART_IT_TXE, ENABLE);
  
  /* 等待TxBuffer中的数据全部发送完毕，数据量为NbrOfDataToTransfer */
// while(TxCounter < NbrOfDataToTransfer)
  {}
 // 
  /* 等待，直到TC =1为止*/
 // while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
 // {}
  /* 使能串口USART1的接收中断：当USART1的接收数据寄存器为非空时，将会产生发送中断*/
  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);
  
  /* 等待接收数据，并将其存储在RxBuffer中，直到接收数据量为NbrOfDataToRead时停止。*/
  while(RxCounter < NbrOfDataToRead)
  {}
 FSMC_NAND_WriteSmallPage(buffer, WriteReadAddr, PageNumber);
		WriteReadAddr.Page=WriteReadAddr.Page+PageNumber;
		if(WriteReadAddr.Page>63)
		{
			WriteReadAddr.Page=0x00;
			WriteReadAddr.Block+=1;
		}
		RxCounter=0;
	}
	 WriteReadAddr.Zone = 0x00;
    WriteReadAddr.Block = 0x00;
    WriteReadAddr.Page = 0x00; 	
	FSMC_NAND_ReadSmallPage(Buffer, WriteReadAddr, 1);
  while (1)
  {
  }
}

/******************************************************************************
* 函数名： NVIC_Config
* 功  能： 配置串口1的中断类型，并打开串口1的发送和接收中断功能
* 输  入：无 
* 输  出：无  
* 备  注：无
********************************************************************************/
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 打开 USART1 中断配置 */
	NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM1_IRQn;  /* 设定为USART1中断 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  /* 设定主优先级为0*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		   /* 设定次优先级为0*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   /* 中断打开*/
	NVIC_Init(&NVIC_InitStructure);
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
