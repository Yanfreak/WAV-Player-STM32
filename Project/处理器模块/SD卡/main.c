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
#include <stdio.h>
#include "stm32f2xx.h"
#include "stm32_eval.h"
#include "stm32_eval_spi_sd.h"
/*************ȫ�ֱ�������***********************************/
uint8_t buff1[512];
uint8_t buff2[512];
u32 Cap=0;

/* ��������-----------------------------------------------*/
void Delay(__IO uint32_t nCount);

/******************************************************************************
* ��������main
* ��  �ܣ�
* ��  �룺��
* ��  ������  
* ��  ע����
********************************************************************************/
int main(void)
{
	int Index,Status=0,i;
	STM_EVAL_LEDInit(LED2); /*LED�Ƴ�ʼ��*/
	STM_EVAL_LEDInit(LED3); /*LED�Ƴ�ʼ��*/
	STM_EVAL_LEDInit(LED4); /*LED�Ƴ�ʼ��*/
	Delay(100);
	STM_EVAL_LEDOff(LED2);
	STM_EVAL_LEDOff(LED3);
	STM_EVAL_LEDOff(LED4);
	for(i=0;i<1024;i++)
	{
		buff1[i]=i;	
	}
	while(SD_Init()!=0)  /*��ⲻ��SD��*/
	{
		STM_EVAL_LEDToggle(LED4);
		Delay(100);	
	}
	  
    Cap=SD_GetCapacity();
    SD_WriteSingleBlock(0,buff1);
    SD_ReadSingleBlock(0,buff2);
    for(Index=0;(Index<512)&&(Status==0);Index++)
    {
    	if (buff1[Index] != buff2[Index])
    	{
        	Status++;
   		}
    }
 	if (Status == 0)
    {
	    /* ���ݶ�д�������LED2 */
	    STM_EVAL_LEDOn(LED2);
    }
    else
    { 
	    /* ���ݶ�д���� */
	    STM_EVAL_LEDOn(LED3);
    }  
	while (1)
    {} 
}

/************************************************************************************
*������:Delay
*����˵��:��ʱ����
*�������:��ʱ����
*
*����ֵ:��
*
*
**************************************************************************************/

void Delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (100000 * nCount); index != 0; index--)
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

/**
  * @}
  */

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
