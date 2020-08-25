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
#include "stm322xg_eval_touch.h"
#include "stm322xg_eval_lcd.h"
#include "stdio.h"
/************************************************************************************
*函数名:Delay
*函数说明: 延时函数
*输入参数:
*		 nCount:延时长度
*返回值:无
*
*
**************************************************************************************/
static void Delay(int nCount)
{
	while(nCount--);
}
/************************************************************************************
*函数名:Touch_interface
*函数说明:界面显示程序
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
static void Touch_interface(void)
{
	char temp[32];
	LCD_Clear(Blue);
	LCD_DrawRect(0,0,239,319);
	LCD_DrawLine(0,20,320,Horizontal);
	sprintf(temp,"触摸屏测试");
	LCD_DisplayStringHZ(108,4,(uint8_t *)temp);	
	sprintf(temp,"触摸屏输出值:");
	LCD_DisplayStringHZ(50,50,(uint8_t *)temp);
	LCD_DrawLine(0,220,320,Horizontal);
	sprintf(temp,"北京亿旗创新科技发展有限公司");
	LCD_DisplayStringHZ(60,222,(uint8_t *)temp);
	LCD_DisplayStringHZ(100,70,"X:");
	LCD_DisplayStringHZ(100,140,"Y:");
}
/***************************************************
*函数名:main
*输入参数:无
*返回值:无
*函数说明:主函数
*
****************************************************/
int main(void)
{ 
  int X2,Y2,X1,Y1;
  char Z=0;
  char temp[30];
  STM3220F_LCD_Init();	/*初始化LCD*/
  LCD_SetTextColor(Yellow);/*设置前景色*/
  LCD_SetBackColor(Blue);  /*设置背景色*/
  Touch_interface();	/*界面显示*/
  Touch_SPI_Configuration(); /*触摸屏接口配置*/
  while (1)
  {
		X2 = TOUCH_X_Measure();
		Y2 = TOUCH_Y_Measure();	 /*第一次读值*/
		Delay(1000);
		X1 = TOUCH_X_Measure();
		Y1 = TOUCH_Y_Measure();	 /*第二次读值*/
		/*********两次读数差值在允许范围内**********************************/
		if(((X1<=X2 && X1>=X2-50)||(X2<=X1 && X2>=X1-50))&&((Y1<=Y2 && Y1>=Y2-50)||(Y2<=Y1 && Y2>=Y1-50)))
		{
		   X2=(X2+X1)/2;
		   Y2=(Y2+Y1)/2;	
		}else
		{
			continue;
		}
		X2=4095-X2;
		if((X2!=0|Y2!=0)&&Z==0)	   /*将数值显示到LCD*/
		{		 
		   sprintf(temp,"%4d",X2);
		   LCD_DisplayStringHZ(150,70,"          ");
		   LCD_DisplayStringHZ(150,86,"          ");
		   LCD_DisplayStringHZ(150,102,"          ");
		   LCD_DisplayStringHZ(150,118,"           ");
		   LCD_DisplayStringLargeFont(150,70,(uint8_t *)temp);
		   sprintf(temp,"%4d",Y2);
		   LCD_DisplayStringHZ(150,140,"             ");
		   LCD_DisplayStringHZ(150,156,"             ");
		   LCD_DisplayStringHZ(150,172,"             ");
		   LCD_DisplayStringHZ(150,188,"             ");
		   LCD_DisplayStringLargeFont(150,140,(uint8_t *)temp);
		}
		Delay(1000000);
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
