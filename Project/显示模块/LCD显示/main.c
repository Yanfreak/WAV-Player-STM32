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
#include "stm322xg_eval_lcd.h"
#include "wave.h"
#include "bmp.h"

/***************************************************
*函数名:delay_ms
*输入参数:
*		z：延时长度
*返回值: 无
*函数说明: 延时函数
*
****************************************************/
void delay_ms(int z)
{
	int j,k;
	for(j=0;j<z;j++)
	{
		for(k=0;k<10000;k++);
	}
}
/***************************************************
*函数名:lcd_app
*输入参数:无
*返回值:无
*函数说明:应用程序
*
****************************************************/
void lcd_app(void)
{
	POINT_T offset;	/* 波形偏移坐标 */

	LCD_SetBackColor(Blue);		/* 设置文字背景颜色 */
	LCD_SetTextColor(White);	/* 设置文字颜色 */

	LCD_SetFont(&Font12x12);
	while(1)
	{
		LCD_Clear(Blue);  			/* 清屏，背景蓝色 */
		LCD_DrawCharLargeFont(100, 30,"LCD Draw Circle",16);

		LCD_DrawCircle(160,120,45);

		delay_ms(5000);

		LCD_Clear(Blue);  			/* 清屏，背景蓝色 */
		LCD_DisplayString(100, 30,"LCD Draw Rect");

		LCD_DrawRect(100,80,80,120);

		delay_ms(5000);

		LCD_Clear(Blue);  			/* 清屏，背景蓝色 */
		LCD_DisplayString(80, 30,"LCD Draw Filled Rect");

		LCD_DrawRectFill(100,80,220,160,Yellow);

		delay_ms(5000);

		LCD_Clear(Blue);  			/* 清屏，背景蓝色 */
		LCD_DisplayString(100, 30, "LCD Draw Wave");

		offset.x = 0;
		offset.y = 100;
		LCD_DrawPoints(offset, (POINT_T *)g_SineWave, sizeof(g_SineWave) / sizeof(POINT_T), Yellow);

		offset.x = 128;
		offset.y = 100;
		LCD_DrawPoints(offset, (POINT_T *)g_SineWave, sizeof(g_SineWave) / sizeof(POINT_T), Yellow);

		offset.x = 128 + 128;
		offset.y = 100;
		LCD_DrawPoints(offset, (POINT_T *)g_SineWave, sizeof(g_SineWave) / sizeof(POINT_T), Yellow);
		
		delay_ms(5000);

		/* 装载图片数据到显存 */
		LCD_WriteBMP((const uint16_t *)gImage_bmp);

		delay_ms(20000);
	}
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

  STM3220F_LCD_Init(); /*LCD初始化*/
  lcd_app();		  /*画点线面函数*/
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
