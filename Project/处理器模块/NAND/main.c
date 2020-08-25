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
#include "stm322xg_eval_nand.h"
#include "stm32_eval.h"															 	
/* �궨��-----------------------------------------------------------*/
/***********��������С***************************************/
#define BUFFER_SIZE         0x1000	/*  K9F11208 */

/*************NAND FLASH ID*************************************/



/* ȫ�ֱ������� ---------------------------------------------------------*/
/**********NAND FLASH ID����****************************************/
NAND_IDTypeDef NAND_ID;

/************NAND FLASH��д��ַ�ṹ�����**************************************/
NAND_ADDRESS WriteReadAddr;

/*************���ݻ�����*************************************/
u8 TxBuffer[BUFFER_SIZE], RxBuffer[BUFFER_SIZE];

/************״̬��������дҳ������**************************************/
vu32 PageNumber = 2, WriteReadStatus = 0, status= 0;
u32 j = 0;

/* �������� -----------------------------------------------*/
/*******������ݺ���*******************************************/
void Fill_Buffer(u8 *pBuffer, u16 BufferLenght, u32 Offset);
u32 ecc;
/***************************************************
*������:main
*�������:��
*����ֵ:��
*����˵��:������
*
****************************************************/
int main(void)
{ 
  int i;
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);
  /* NAND FLASH��ʼ��*/
  FSMC_NAND_Init();

  /***NAND FLASH��λ***/
//  FSMC_NAND_Reset();
  /* ��ȡNAND FLASH��ID */
  FSMC_NAND_ReadID(&NAND_ID);

  /* ��֤ NAND FLASH ID�Ƿ���ȷ */  
  if((NAND_ID.Maker_ID == NAND_SAMSUNG_MakerID) && (NAND_ID.Device_ID == NAND_SAMSUNG_DeviceID))
  {

    /* ��ʼ��NAND FLASH��д��ַ */ 
    WriteReadAddr.Zone = 0x00;
    WriteReadAddr.Block = 0x00;
    WriteReadAddr.Page = 0x00; 

    /* ����NAND FLASH */
   // status = FSMC_NAND_EraseBlock(WriteReadAddr);

    /* ��������********/
  //  Fill_Buffer(TxBuffer, BUFFER_SIZE , 0x5);
    	//for(i = 0; i < 2048; i ++)
    	//TxBuffer[i] = i / 5 - i;
	/*�����ɵ�����д�뵽NAND FLASH��*/
   //ecc = (u32)FSMC_NAND_WriteSmallPage(TxBuffer, WriteReadAddr, PageNumber);
    /*��NAND FLASH�ж�������*/
   ecc = (u32)FSMC_NAND_ReadSmallPage (RxBuffer, WriteReadAddr, PageNumber);

    /* ����������ԭʼ���ݶԱ� */
    for(j = 0; j < BUFFER_SIZE; j++)
    {
      if(TxBuffer[j] != RxBuffer[j])
      {     
        WriteReadStatus++;
      } 
    }

    if (WriteReadStatus == 0)
    {
      /* �����д�������LED2 */
      STM_EVAL_LEDOn(LED2);
    }
    else
    { 
      /* ������ݶ�д���󣬵���LED3 */
      STM_EVAL_LEDOn(LED3);     
    }
  }
  else
  {
    /* ���NAND FLASH ID����ȷ������LED4*/
    STM_EVAL_LEDOn(LED4);  
  }

	return (0);
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
void Fill_Buffer(u8 *pBuffer, u16 BufferLenght, u32 Offset)
{
  u16 IndexTmp = 0;

  for (IndexTmp = 0; IndexTmp < BufferLenght; IndexTmp++ )
  {
    pBuffer[IndexTmp] = IndexTmp + Offset;
  }
}

#ifdef  DEBUG
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
void assert_failed(u8* file, u32 line)
{ 
 /****�˴���������û����󱨸淽ʽ**********************/
  while (1)
  {
  }
}
#endif
/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
