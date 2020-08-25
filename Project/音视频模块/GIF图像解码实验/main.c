 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� main.c
*   ժ Ҫ    �� 
*
*   ��ǰ�汾 �� 
*   �� ��    �� EFLAG
*   ������� �� 
*
*   ȡ���汾 ��
*   ԭ����   �� 
*   �޸����� ��
*   ������� ��2012.08.01 
*
**********************************************************/

/* ͷ�ļ�------------------------------------------------------------------*/
#include "stm32_eval.h"
#include "stm32_eval_spi_sd.h"
#include "stm322xg_eval_lcd.h"
#include "ff.h"
#include "jpegbmp.h"
#include "stm322xg_eval_keypad.h"
#include "stm322xg_eval_systick.h"
#include "gif.h"
#include "string.h"

FATFS filesystem;		/* �ļ�ϵͳ��� */
FIL file;
FRESULT ret;			/* �ļ���������ֵ */
u32 cardinfo; /*SD������*/
extern u8 KeyCode;
/***************************************************
*������:main
*�������:��
*����ֵ:��
*����˵��:������
*
****************************************************/
int main(void)
{ 
	u8 PicNum[10][10]={"1.gif","2.gif","3.gif","4.gif","5.gif",
					   "6.gif","7.gif","8.gif","9.gif","10.gif"};
	int i=0;
	SysTick_Config(SystemCoreClock / 1000);
	STM3220F_LCD_Init();/*LCD��ʼ��*/	
	if(SD_Init()==0)/*SD����ʼ��*/
    {
		LCD_Clear(White);
		LCD_SetBackColor(White);
		LCD_SetTextColor(Red);
        LCD_DisplayStringHZ(100,100," SD Init No Err"); 
		LCD_DisplayStringHZ(100,130,"Immediately Play");
    }					
    cardinfo=SD_GetCapacity();
	if (f_mount(0, &filesystem) != FR_OK)/*�ļ�ϵͳ����*/
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
	    	if(KeyCode==KEY16_DOWN)	//ж���ļ���Һ����ͣ����ʾ�ڰ��°���ʱ������ͼƬ
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
*������:assert_failed
*����˵��:������������λ��
*	
*�������:
*		file:�������ڵ��ļ���
*		line:���������ļ��е��к�
*	
*����ֵ:��	
*****************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
 /****�˴���������û����󱨸淽ʽ**********************/

  while (1)
  {
  }
}
#endif
/**
  * @}
  */


/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
