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
#include "stm322xg_eval_touch.h"
#include "stm322xg_eval_lcd.h"
#include "stdio.h"
/************************************************************************************
*������:Delay
*����˵��: ��ʱ����
*�������:
*		 nCount:��ʱ����
*����ֵ:��
*
*
**************************************************************************************/
static void Delay(int nCount)
{
	while(nCount--);
}
/************************************************************************************
*������:Touch_interface
*����˵��:������ʾ����
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void Touch_interface(void)
{
	char temp[32];
	LCD_Clear(Blue);
	LCD_DrawRect(0,0,239,319);
	LCD_DrawLine(0,20,320,Horizontal);
	sprintf(temp,"����������");
	LCD_DisplayStringHZ(108,4,(uint8_t *)temp);	
	sprintf(temp,"���������ֵ:");
	LCD_DisplayStringHZ(50,50,(uint8_t *)temp);
	LCD_DrawLine(0,220,320,Horizontal);
	sprintf(temp,"�������촴�¿Ƽ���չ���޹�˾");
	LCD_DisplayStringHZ(60,222,(uint8_t *)temp);
	LCD_DisplayStringHZ(100,70,"X:");
	LCD_DisplayStringHZ(100,140,"Y:");
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
  int X2,Y2,X1,Y1;
  char Z=0;
  char temp[30];
  STM3220F_LCD_Init();	/*��ʼ��LCD*/
  LCD_SetTextColor(Yellow);/*����ǰ��ɫ*/
  LCD_SetBackColor(Blue);  /*���ñ���ɫ*/
  Touch_interface();	/*������ʾ*/
  Touch_SPI_Configuration(); /*�������ӿ�����*/
  while (1)
  {
		X2 = TOUCH_X_Measure();
		Y2 = TOUCH_Y_Measure();	 /*��һ�ζ�ֵ*/
		Delay(1000);
		X1 = TOUCH_X_Measure();
		Y1 = TOUCH_Y_Measure();	 /*�ڶ��ζ�ֵ*/
		/*********���ζ�����ֵ������Χ��**********************************/
		if(((X1<=X2 && X1>=X2-50)||(X2<=X1 && X2>=X1-50))&&((Y1<=Y2 && Y1>=Y2-50)||(Y2<=Y1 && Y2>=Y1-50)))
		{
		   X2=(X2+X1)/2;
		   Y2=(Y2+Y1)/2;	
		}else
		{
			continue;
		}
		X2=4095-X2;
		if((X2!=0|Y2!=0)&&Z==0)	   /*����ֵ��ʾ��LCD*/
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
