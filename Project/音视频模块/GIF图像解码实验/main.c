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
#include "stm32_eval_spi_sd.h"
#include "stm322xg_eval_lcd.h"
#include "ff.h"
#include "jpegbmp.h"
#include "stm322xg_eval_keypad.h"
#include "stm322xg_eval_systick.h"
#include "gif.h"
#include "string.h"

FATFS filesystem;		/* 文件系统卷标 */
FIL file;
FRESULT ret;			/* 文件操作返回值 */
u32 cardinfo; /*SD卡容量*/
extern u8 KeyCode;
/***************************************************
*函数名:main
*输入参数:无
*返回值:无
*函数说明:主函数
*
****************************************************/
int main(void)
{ 
	u8 PicNum[10][10]={"1.gif","2.gif","3.gif","4.gif","5.gif",
					   "6.gif","7.gif","8.gif","9.gif","10.gif"};
	int i=0;
	SysTick_Config(SystemCoreClock / 1000);
	STM3220F_LCD_Init();/*LCD初始化*/	
	if(SD_Init()==0)/*SD卡初始化*/
    {
		LCD_Clear(White);
		LCD_SetBackColor(White);
		LCD_SetTextColor(Red);
        LCD_DisplayStringHZ(100,100," SD Init No Err"); 
		LCD_DisplayStringHZ(100,130,"Immediately Play");
    }					
    cardinfo=SD_GetCapacity();
	if (f_mount(0, &filesystem) != FR_OK)/*文件系统挂载*/
	{
		LCD_DisplayStringHZ(100,160,"could not open filesystem\n");
	}

    STM_EVAL_LEDInit(LED1);
    STM_EVAL_LEDOff(LED1);
	STM3220F_LCD_Init();
	Keypad_Interrupt(0,0,0);
    InitKey();
    while (1)
    {
		for(i=0;i<10;i++)
		{
			delay_ms(3000);
			LCD_SetCursor(0,0);
			AI_LoadPicFile_gif(&PicNum[i][0],0,0,320,240);
	    	if(KeyCode==KEY16_DOWN)	//卸载文件，液晶屏停留显示在按下按键时的那张图片
	    	{
	    		KeyCode=0;
	    		f_mount(0,0);
	    		return 0;
	    	}
		}
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
