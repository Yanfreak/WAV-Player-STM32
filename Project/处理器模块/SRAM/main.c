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
#include "stm322xg_eval_sram.h"
/* �궨��------------------------------------------------------------*/
/*********��������С*****************************************/
#define BUFFER_SIZE        0x100
/**********SRAM�ڲ���д��ַ****************************************/
#define WRITE_READ_ADDR    0x800
/* ȫ�ֱ������� ---------------------------------------------------------*/
/*********��д������*****************************************/
uint16_t TxBuffer[BUFFER_SIZE], RxBuffer[BUFFER_SIZE];
/************��д״̬**************************************/
__IO uint32_t WriteReadStatus = 0;
uint32_t Index = 0;

/* ��������-----------------------------------------------*/
/********������ݺ���******************************************/
void Fill_Buffer(uint16_t *pBuffer, uint16_t BufferLenght, uint32_t Offset);
/***************************************************
*������:main
*�������:��
*����ֵ:��
*����˵��:������
*
****************************************************/
int main(void)
{ 
  /* LED�Ƴ�ʼ�� */
	STM_EVAL_LEDInit(LED2);
	STM_EVAL_LEDInit(LED3);
	STM_EVAL_LEDOff(LED2);
	STM_EVAL_LEDOff(LED3);
	   
  /* SRAM��ʼ��  */
    SRAM_Init();

  /* �������� */
    Fill_Buffer(TxBuffer, BUFFER_SIZE, 0x320F);

  /*��TxBuffer�е�����д��WRITE_READ_ADDR��ַ�£���СΪBUFFER_SIZE*/
    SRAM_WriteBuffer(TxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);

  /* �� SRAM��WRITE_READ_ADDR��ַ�¶�ȡBUFFER_SIZE�ֽ����ݵ�RxBuffer��*/
    SRAM_ReadBuffer(RxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);  

  /* ��������������ԭʼ���ݶԱ�*/   
	for (Index = 0x00; (Index < BUFFER_SIZE) && (WriteReadStatus == 0); Index++)
    {
     if (RxBuffer[Index] != TxBuffer[Index])
	    {
		  /*����������ݲ�һ��*/
	      WriteReadStatus++;
	    }
    }	

  if (WriteReadStatus == 0)
  {
    /* ȫ������һ�� */
	/* ���� LED2*/
	 STM_EVAL_LEDOn(LED2);
  }
  else
  { 
    /* �����ݲ�һ�� */
    /* ���� LED3*/

    STM_EVAL_LEDOn(LED3);   
  }

  while (1)
  {
  }
}
  /***************************************************************
  *������:Fill_Buffer
  *����˵��:��ʼ��pBuffer�е����ݣ���Offset��ʼ����1,
  *			����������ΪΪBufferLenght��short������
  *�������:
  *		����pBuffer:ָ���������ݴ�ŵĻ�����
  *		����BufferLenght:��������С
  *		����Offset������������ʼ��С
  *����ֵ:��	
  *****************************************************************/
void Fill_Buffer(uint16_t *pBuffer, uint16_t BufferLenght, uint32_t Offset)
{
  uint16_t IndexTmp = 0;
  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
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
