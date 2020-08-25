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
#include "stm32f2xx.h"
#include "main.h"
#include <stdio.h>
#include "stm322xg_eval_18b20.h"
#include "stm322xg_eval_lcd.h"
/*��������*/
extern uint16_t s_TextColor;/*LCDǰ��ɫ*/
extern uint16_t s_BackColor;/*LCD����ɫ*/

/************************************************************************************
*������: LCD_Interface
*����˵��:LCD����
*�������:��
*
*����ֵ:��
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
	sprintf(temp,"18B20�¶Ȳɼ�����");
	LCD_DisplayStringHZ(80,4,(uint8_t *)temp);	
	sprintf(temp,"�¶�ֵ:");
	LCD_DisplayStringHZ(50,50,(uint8_t *)temp);
	LCD_DisplayStringHZ(180,135,"��");
  	LCD_DrawLine(0,220,320,Horizontal);
  	sprintf(temp,"�������촴�¿Ƽ���չ���޹�˾");
  	LCD_DisplayStringHZ(48,222,(uint8_t *)temp);   
}
/************************************************************************************
*������:main
*����˵��:������
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
	
int main(void)
{
	char temp[32];
	short temper=0;
	int i;
	STM3220F_LCD_Init(); /*Һ����ʼ��*/
	
    while(DS18B20_Init()) /*18B20��ʼ��*/
    {
   		LCD_DisplayStringHZ(48,222,"18B20����");	
	}
	LCD_Interface();   /*LCD����*/
    while (1)
    {
		for(i=0;i<100000;i++);
        temper=DS18B20_Get_Temp();	 /*�¶Ȳɼ�*/
		sprintf(temp,"%d",temper);
		LCD_DisplayStringLargeFont(100,100,(uint8_t *)temp);
        
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

/**
  * @}
  */ 

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
