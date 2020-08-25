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
*   ������� ��2012.08.02 
*
**********************************************************/

/* ͷ�ļ� ------------------------------------------------------------------*/
#include "stm322xg_eval_nor.h"
#include "stm32_eval.h"

/* �궨��------------------------------------------------------------*/
/***********��������С***************************************/
#define BUFFER_SIZE        0x400

/**********NOR FLASH�ڲ���д��ʼ��ַ****************************************/
#define WRITE_READ_ADDR    0x8000

/* ȫ�ֱ������� -------------------------------------------------------------*/
/********GPIO��ʼ���ṹ��******************************************/
GPIO_InitTypeDef GPIO_InitStructure;

/*********�������ݻ�����*****************************************/
uint16_t TxBuffer[BUFFER_SIZE];

/**********�������ݻ�����****************************************/
uint16_t RxBuffer[BUFFER_SIZE];

/*********��д״̬����*****************************************/
uint32_t WriteReadStatus = 0, Index = 0;

/*********NOR FLASH ID ����*****************************************/
NOR_IDTypeDef NOR_ID;

/* �������� -----------------------------------------------*/
/*************������亯��*************************************/
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
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);

  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);     

  /* ʹ��FSMC��ʱ�� */
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);

  /* NOR FLASH��ʼ�� */

  FSMC_NOR_Init();

  /* ��ȡNOR FLASH��ID */
  FSMC_NOR_ReadID(&NOR_ID);

  FSMC_NOR_ReturnToReadMode();

  /* ����NOR FLASH */
  FSMC_NOR_EraseBlock(WRITE_READ_ADDR);

  /* �������� */
  Fill_Buffer(TxBuffer, BUFFER_SIZE, 0x3210);

  /*������������д��NOR FLASH*/
  FSMC_NOR_WriteBuffer(TxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);

  /* ��NOR FLASH�ж�ȡ���� */
  FSMC_NOR_ReadBuffer(RxBuffer, WRITE_READ_ADDR, BUFFER_SIZE);  

  /* ����ȡ��������ԭ���ݽ��жԱ� */   
  for (Index = 0x00; (Index < BUFFER_SIZE) && (WriteReadStatus == 0); Index++)
  {
    if (RxBuffer[Index] != TxBuffer[Index])
    {
      WriteReadStatus = Index + 1;
    }
  }

  if (WriteReadStatus == 0)
  {
    /* ���ݶ�д�������LED2 */
    STM_EVAL_LEDOn(LED2);
  }
  else
  { 
    /* ���ݶ�д���� */
    STM_EVAL_LEDOff(LED3);
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

/**
  * @}
  */ 

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
