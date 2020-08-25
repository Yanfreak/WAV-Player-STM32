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
#include <stdio.h>
#include "stm32f2xx.h"
#include "stm32_eval.h"
#include "stm32_eval_spi_sd.h"
/*************全局变量定义***********************************/
uint8_t buff1[512];
uint8_t buff2[512];
u32 Cap=0;

/* 函数声明-----------------------------------------------*/
void Delay(__IO uint32_t nCount);

/******************************************************************************
* 函数名：main
* 功  能：
* 输  入：无
* 输  出：无  
* 备  注：无
********************************************************************************/
int main(void)
{
	int Index,Status=0,i;
	STM_EVAL_LEDInit(LED2); /*LED灯初始化*/
	STM_EVAL_LEDInit(LED3); /*LED灯初始化*/
	STM_EVAL_LEDInit(LED4); /*LED灯初始化*/
	Delay(100);
	STM_EVAL_LEDOff(LED2);
	STM_EVAL_LEDOff(LED3);
	STM_EVAL_LEDOff(LED4);
	for(i=0;i<1024;i++)
	{
		buff1[i]=i;	
	}
	while(SD_Init()!=0)  /*检测不到SD卡*/
	{
		STM_EVAL_LEDToggle(LED4);
		Delay(100);	
	}
	  
    Cap=SD_GetCapacity();
    SD_WriteSingleBlock(0,buff1);
    SD_ReadSingleBlock(0,buff2);
    for(Index=0;(Index<512)&&(Status==0);Index++)
    {
    	if (buff1[Index] != buff2[Index])
    	{
        	Status++;
   		}
    }
 	if (Status == 0)
    {
	    /* 数据读写无误点亮LED2 */
	    STM_EVAL_LEDOn(LED2);
    }
    else
    { 
	    /* 数据读写有误 */
	    STM_EVAL_LEDOn(LED3);
    }  
	while (1)
    {} 
}

/************************************************************************************
*函数名:Delay
*函数说明:延时函数
*输入参数:延时长度
*
*返回值:无
*
*
**************************************************************************************/

void Delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
  {
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
