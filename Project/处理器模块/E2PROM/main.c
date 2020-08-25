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
#include "stm322xg_eval_i2c_ee.h"
#include "stdlib.h"

/*�궨�� ------------------------------------------------------------*/
/***********д��ʼ��ַ***************************************/
#define EEPROM_WriteAddress1    0x05

/************����ʼ��ַ**************************************/
#define EEPROM_ReadAddress1     0x05

/************��������С**************************************/
#define BufferSize1             (countof(Tx1_Buffer)-1)

/************�������ݴ�С��**************************************/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* ȫ�ֱ������� -----------------------------------------------------------*/
/***********״̬ö�ٱ���***************************************/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/***********������������***************************************/
u8 Tx1_Buffer[] = "/* STM32F10x I2C Firmware ";

/***********������������***************************************/
u8 Rx1_Buffer[BufferSize1];

/***********״̬����***************************************/
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED;
    
/* �������� ---------------------------------------------------------*/
/*********���ݱȽϺ���*****************************************/
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength);

/*********��ʱ����*****************************************/
void Delay(vu32 nCount);
/***************************************************
*������:main
*�������:��
*����ֵ:��
*����˵��:������
*
****************************************************/
int main(void)
{
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  /* I2C EEPROM��ʼ�� ----------------------------------------*/
  sEE_Init();  
 
  /* ��eepromд������ */
  sEE_WriteBuffer(Tx1_Buffer, (uint16_t )EEPROM_WriteAddress1, (uint16_t)BufferSize1); 

  /* ��eeprom�ж�������*/
  sEE_ReadBuffer(Rx1_Buffer, (uint16_t )EEPROM_ReadAddress1, (uint16_t*)BufferSize1); 
  Delay(0x100000);

  /* �Ƚ�д��������������Ƿ�һ�� һ�·���PASSED */
  TransferStatus1 = Buffercmp(Tx1_Buffer, Rx1_Buffer, BufferSize1);

  while (1)
  {
	if(TransferStatus1 == PASSED)
	{
		/* ������ݶ�д����LED2�� */
		STM_EVAL_LEDOn(LED2);
	    Delay(0x3AFFFF);
	
	}
	else
	{
	    /* ����LED3�� */
	    STM_EVAL_LEDOn(LED3);
		Delay(0x3AFFFF);
	}
  }
}

/***************************************************************
*������:Buffercmp
*����˵��:�Ƚ������������������Ƿ�һ�£�һ�·���PASSED
*		  ��һ�·���FAILED
*�������:
*		����pBuffer1:������1
*		����pBuffer2:������2
*		����BufferLength��Ҫ�Ƚϵ����ݴ�С
*����ֵ:
*	PASSED�����ݱȽ�һ��
*	FAILED�����ݱȽϲ�һ��	
*****************************************************************/
TestStatus Buffercmp(u8* pBuffer1, u8* pBuffer2, u16 BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;  
}
/***************************************************************
*������:Delay
*����˵��:��ʱ����
*	
*�������:
*		nCount����ʱ��С
*����ֵ:��	
*****************************************************************/
void Delay(vu32 nCount)
{
  int i;
  for(i=0;i<nCount;i++)
  {
  	;;
  }
}
/**
  * @}
  */


/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
