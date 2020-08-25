 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： USART/USART_Printf/main.c
*   摘 要    ： 实现printf函数重定向输出到串口USART1
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
#include <stdio.h>

/* 类型定义  -------------------------------------------------------------------*/

/* 宏定义  ---------------------------------------------------------------------*/

/* 定义 PUTCHAR_PROTOTYPE 函数*/
#ifdef __GNUC__	  
  /* 将 PUTCHAR_PROTOTYPE 定义为 __io_putchar函数 ，在 GCC/RAISONANCE模式下，采用small printf实现方法
  *  可以通过 LD Linker->Libraries->Small printf 选项 设置 'Yes'，来实现printf调用 __io_putchar() 
  */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  /* 默认情况下，printf函数会调用fputc（）函数*/
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */


/* 变量定义 --------------------------------------------------------------------*/

USART_InitTypeDef USART_InitStructure;  /*定义串口初始化结构体 */

/* 函数定义---------------------------------------------------------------------*/

void YangHui(void);	/* 使用printf方式输出杨辉三角形结果 */

/* Private functions ---------------------------------------------------------*/

/******************************************************************************
* 函数名： main
* 功  能： 
* 输  入：无 
* 输  出：无  
* 备  注：无
********************************************************************************/

int main(void)
{
       
  /* 初始化USART_InitStructure结构体参数，USART 配置为以下工作模式:
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

  STM_EVAL_COMInit(COM1, &USART_InitStructure);	 /*将USART_InitStructure结构体参数配置到处理器USART1，并打开USART1的发送和接受功能 */

  /* 使用printf输出信息 */
  printf("\n\rUSART Printf Example: retarget the C library printf function to the USART\n\r");
  
  YangHui();  /* 调用杨辉三角形计算函数 */
  while (1)
  {
  }
}

/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
/******************************************************************************
* 函数名：PUTCHAR_PROTOTYPE  
* 功  能：重定向 C 标准库的printf功能由USART串口输出信息 
* 输  入：int ch ：输出的字符（由库函数调用）
* 输  出：int  ch  
* 备  注：文件开始处条件定义PUTCHAR_PROTOTYPE为__io_putchar(int ch)函数或fputc(int ch, FILE *f)函数
********************************************************************************/
PUTCHAR_PROTOTYPE
{
  /* 此处填写fputc的实现代码 */
  /* 例如： 从串口发送一个字符 */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* 循环等待，直到串口数据发送结束 */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

/******************************************************************************
* 函数名：YangHui 
* 功  能：杨辉三角形计算方法，采用printf输出计算结果
* 输  入：无 
* 输  出：无  
* 备  注：NUM 定义为 10 阶 
********************************************************************************/

#define  NUM  10
void YangHui(void)
{
       int a[NUM][NUM],i,j;
	printf("\n\r 计算10阶杨辉三角形：\n\r\n\r");
       for (i=0;i<NUM;i++)
       {
              a[i][0]=1;
              a[i][i]=1;
       }
       for (i=2;i<NUM;i++)
       {
              for (j=1;j<i;j++)
              a[i][j]=a[i-1][j-1]+a[i-1][j];
       }
       for (i=0;i<NUM;i++)
       {
              for (j=0;j<i+1;j++) printf("%3d  ",a[i][j]);
              printf("\n\r");
       }
       printf("\n\r");
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
