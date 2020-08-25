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
#include "stm322xg_eval_ov9655.h"
#include "stm322xg_eval_lcd.h"
#include "stm32f2xx_dcmi.h"
/***************************************************
*������:main
*�������:��
*����ֵ:��
*����˵��:������
*
****************************************************/
int main(void)
{ 
  STM3220F_LCD_Init();/*LCD��ʼ��*/
  LCD_Clear(White);
  
  Ov9655_I2C_Configuration();/*I2C��ʼ��*/
  Ov9655_Dcmi_GPIO_Configuration();/*DCMI�˿ڳ�ʼ��*/
  Ov9655_DMA_Configuration(); /*DMA��ʼ��*/
  Ov9655_Dcmi_Configuration(); /*DCMI����*/
  DMA_Cmd(DMA2_Stream1, ENABLE); /*DMAʹ��*/	  
  DCMI_Cmd(ENABLE);				 /*DCMIʹ��*/
  DCMI->CR |=0x1; 				 /*ʹ��ͼ����׽*/
  while (1)
  {
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
