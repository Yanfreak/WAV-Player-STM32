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
#ifndef __MAIN_H
#define __MAIN_H

/* ͷ�ļ� ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "stm32_eval.h"

/* �������� ------------------------------------------------------- */
void RTC_Configuration(void);
void RTC_TimeRegulate(void);
void RTC_TimeSchow(void);
uint8_t USART_Scanf(uint32_t value);

#ifdef __GNUC__
/*************����ض���******************************/
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#endif /* __MAIN_H */

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
