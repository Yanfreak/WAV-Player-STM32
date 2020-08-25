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
#include "stm32f2xx.h"
#include "main.h"
#include <stdio.h>
#include "stm322xg_eval_18b20.h"
#include "stm322xg_eval_lcd.h"
/*变量声明*/
extern uint16_t s_TextColor;/*LCD前景色*/
extern uint16_t s_BackColor;/*LCD背景色*/

/************************************************************************************
*函数名: LCD_Interface
*函数说明:LCD界面
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
void LCD_Interface(void)
{
	char temp[32];
	LCD_Clear(Blue);
  	s_TextColor=Yellow;	  
  	s_BackColor=Blue;
	LCD_DrawRect(0,0,239,319);
	LCD_DrawLine(0,20,320,Horizontal);
	sprintf(temp,"18B20温度采集测试");
	LCD_DisplayStringHZ(80,4,(uint8_t *)temp);	
	sprintf(temp,"温度值:");
	LCD_DisplayStringHZ(50,50,(uint8_t *)temp);
	LCD_DisplayStringHZ(180,135,"℃");
  	LCD_DrawLine(0,220,320,Horizontal);
  	sprintf(temp,"北京亿旗创新科技发展有限公司");
  	LCD_DisplayStringHZ(48,222,(uint8_t *)temp);   
}
/************************************************************************************
*函数名:main
*函数说明:主函数
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
	
int main(void)
{
	char temp[32];
	short temper=0;
	int i;
	STM3220F_LCD_Init(); /*液晶初始化*/
	
    while(DS18B20_Init()) /*18B20初始化*/
    {
   		LCD_DisplayStringHZ(48,222,"18B20错误");	
	}
	LCD_Interface();   /*LCD界面*/
    while (1)
    {
		for(i=0;i<100000;i++);
        temper=DS18B20_Get_Temp();	 /*温度采集*/
		sprintf(temp,"%d",temper);
		LCD_DisplayStringLargeFont(100,100,(uint8_t *)temp);
        
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
