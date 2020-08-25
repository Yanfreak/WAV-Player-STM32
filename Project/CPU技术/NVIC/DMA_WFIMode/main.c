/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： NVIC嵌套中断向量中断控制器
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
******************************************************************************/

/*************头文件**********************************************************/
#include "stm32f2xx.h"
#include "stm32f2xx.h"
#include "stm32_eval.h"

/* 重定义 -------------------------------------------------------------------*/
/* 自定义 -------------------------------------------------------------------*/

#define USART3_DR_ADDRESS    ((uint32_t)0x40011004)

/* 私有宏定义 ---------------------------------------------------------------*/
/* 私有变量 -----------------------------------------------------------------*/
__IO uint32_t LowPowerMode = 0;
uint16_t DstBuffer[10]= {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x70, 0x80, 0x90, 0xa0};

/* 私有函数原型 -------------------------------------------------------------*/	
void DMA_Config(void);
void USART_Config(void);
uint8_t Buffercmp16(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);
void Delay(__IO uint32_t nCount);

/* 私有函数 ------------------------------------------------------------------*/

/****************************************************************************
* 函数名: main()
* 功 能: 主函数
* 输 入: 无
* 输 出: 无
****************************************************************************/
int main(void)
{
	/*在此阶段处理器的时钟已经被初始化完成，时钟初始化工作由SystemInit函数实现，
	SystemInit函数存放在startup_stm32f2xx.s的汇编启动文件中。在系统上电初始化到
	跳转到main函数之前，启动代码会先调用SystemInit函数。如果用户希望重新配置时钟
	可以在此处再次调用 SystemInit()，它的函数实现存放在system_stm32f2xx.c文件中。*/ 

  /* 初始化安装在STM322xG-EVAL板上的发光二极管和按钮 */       
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDOff(LED4);
  STM_EVAL_PBInit(BUTTON_USER1, BUTTON_MODE_EXTI); 

  /* 配置DMA1第4通道(连接到USART3_RX) */
  DMA_Config();

  /* 配置EVAL_COM1(USART3) */
  USART_Config();  
    
  while (1)
  {
    if(LowPowerMode == 1) /* 低电量模式该选项用来设置系统休眠或关闭时所用电量 */
    {
      /* 熄灭LED2和LED3 */
      STM_EVAL_LEDOff(LED2);
      STM_EVAL_LEDOff(LED3);

      /* 请求进入WFI模式 */
      __WFI(); //等待中断
      LowPowerMode = 0;
    }

    Delay(0xFFFFF);
    STM_EVAL_LEDToggle(LED4);
  }
}

/****************************************************************************
* 函数名: USART_Config()
* 功 能: 配置EVAL_COM1 (USART3)
* 输 入: 无
* 输 出: 无
****************************************************************************/
void USART_Config(void)
{
    /* EVAL_COM1配置如下:
        - BaudRate（波特率） = 115200 波特  
        - Word Length（字长） = 8 位
        - One Stop Bit（一个停止位）
        - No parity（无校验位）
        -硬件流控制不使能 (RTS和CTS信号)
        - 启用接收和发送
  */
  USART_InitTypeDef  USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(COM1, &USART_InitStructure);

  /* 使能DMA接收请求 */
  USART_DMACmd(EVAL_COM1, USART_DMAReq_Rx, ENABLE);
}

/****************************************************************************
* 函数名: DMA_Config()
* 功 能: 配置DMA1处理USART3_RX DMA请求
* 输 入: 无
* 输 出: 无
****************************************************************************/
void DMA_Config(void)				   
{
  DMA_InitTypeDef  DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* 使能DMA1时钟 */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

  /* 配置DMA1第4通道 */
  DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART3_DR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DstBuffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 10;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream5, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream5, ENABLE);

  /* 使能DMA1_Stream1传递完整的中断 */
  DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE);
  
  /* 使能DMA1_Stream1 */
  DMA_Cmd(DMA2_Stream5, ENABLE);
    
  /* 使能DMA1_Stream1中断请求 */
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* 函数名: Buffercmp16()
* 功 能: 比较两个缓冲区
* 输 入: pBuffer1,pBuffer2:缓冲区相比。BufferLength:缓冲区的长度
* 输 出: 输出0:pBuffer1与pBuffer2相同；输出1:pBuffer1与pBuffer2不同
****************************************************************************/
uint8_t Buffercmp16(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return 0;
}

/****************************************************************************
* 函数名: Delay()
* 功 能: 指定延迟时间长度(以毫秒为单位)
* 输 入: 无
* 输 出: 无
****************************************************************************/
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

#ifdef  USE_FULL_ASSERT

/******************************************************************************
* 函数名：assert_failed()
* 功  能：报告参数错误的位置
* 输  入：错误所在的文件名和错误所在文件中的行号
* 输  出：无  
* 备  注：无
********************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
	
	/******此处可以添加用户错误报告方式******/

  while (1)
  {
  }
}
#endif

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*************/
