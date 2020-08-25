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
#include "stm322xg_eval_ov9655.h"
#include "stm322xg_eval_lcd.h"
#include "stm32f2xx_dcmi.h"
/***************************************************
*函数名:main
*输入参数:无
*返回值:无
*函数说明:主函数
*
****************************************************/
int main(void)
{ 
  STM3220F_LCD_Init();/*LCD初始化*/
  LCD_Clear(White);
  
  Ov9655_I2C_Configuration();/*I2C初始化*/
  Ov9655_Dcmi_GPIO_Configuration();/*DCMI端口初始化*/
  Ov9655_DMA_Configuration(); /*DMA初始化*/
  Ov9655_Dcmi_Configuration(); /*DCMI配置*/
  DMA_Cmd(DMA2_Stream1, ENABLE); /*DMA使能*/	  
  DCMI_Cmd(ENABLE);				 /*DCMI使能*/
  DCMI->CR |=0x1; 				 /*使能图像扑捉*/
  while (1)
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


/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
