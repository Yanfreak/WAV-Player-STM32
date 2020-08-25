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

/* ͷ�ļ� ------------------------------------------------------------------*/
#include "main.h"
#include "stm322xg_eval_lcd.h"
#include "wave.h"
#include "bmp.h"

/***************************************************
*������:delay_ms
*�������:
*		z����ʱ����
*����ֵ: ��
*����˵��: ��ʱ����
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
*������:lcd_app
*�������:��
*����ֵ:��
*����˵��:Ӧ�ó���
*
****************************************************/
void lcd_app(void)
{
	POINT_T offset;	/* ����ƫ������ */

	LCD_SetBackColor(Blue);		/* �������ֱ�����ɫ */
	LCD_SetTextColor(White);	/* ����������ɫ */

	LCD_SetFont(&Font12x12);
	while(1)
	{
		LCD_Clear(Blue);  			/* ������������ɫ */
		LCD_DrawCharLargeFont(100, 30,"LCD Draw Circle",16);

		LCD_DrawCircle(160,120,45);

		delay_ms(5000);

		LCD_Clear(Blue);  			/* ������������ɫ */
		LCD_DisplayString(100, 30,"LCD Draw Rect");

		LCD_DrawRect(100,80,80,120);

		delay_ms(5000);

		LCD_Clear(Blue);  			/* ������������ɫ */
		LCD_DisplayString(80, 30,"LCD Draw Filled Rect");

		LCD_DrawRectFill(100,80,220,160,Yellow);

		delay_ms(5000);

		LCD_Clear(Blue);  			/* ������������ɫ */
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

		/* װ��ͼƬ���ݵ��Դ� */
		LCD_WriteBMP((const uint16_t *)gImage_bmp);

		delay_ms(20000);
	}
}
/***************************************************
*������:main
*�������:��
*����ֵ:��
*����˵��:������
*
****************************************************/
int main(void)
{

  STM3220F_LCD_Init(); /*LCD��ʼ��*/
  lcd_app();		  /*�������溯��*/
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
