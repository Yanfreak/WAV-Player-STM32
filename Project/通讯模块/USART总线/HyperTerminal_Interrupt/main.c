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
/* ͷ�ļ����� ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "stm32_eval.h"
#include "stm322xg_eval_nand.h"
#include "stm322xg_eval_sram.h"
#include "stm32f2xx_it.h"
#define  buffer   	((uint8_t*)0x68000100) //�ⲿsram
/* ���Ͷ���  -------------------------------------------------------------------*/
/* �궨��  ---------------------------------------------------------------------*/
/* �������� --------------------------------------------------------------------*/

USART_InitTypeDef USART_InitStructure;	   /* ���ڳ�ʼ���ṹ��*/
extern uint32_t NbrOfDataToRead;			   /* ��¼����������ݵ�����*/
extern __IO uint32_t RxCounter; 		   /* ��¼�ѽ�������*/
u8 Buffer[255];
NAND_IDTypeDef NAND_ID;
NAND_ADDRESS WriteReadAddr;
vu32 PageNumber = 64;
/* ��������---------------------------------------------------------------------*/
void NVIC_Config(void);

/******************************************************************************
* ��������main 
* ��  �ܣ� 
* ��  �룺 
* ��  ������  
* ��  ע����
********************************************************************************/
int main(void)
{
  /*�ڴ˽׶δ�������ʱ���Ѿ�����ʼ����ɣ�ʱ�ӳ�ʼ��������SystemInit����ʵ�֣�SystemInit����
   *�����startup_stm32f2xx.s�Ļ�������ļ��С�
   *��ϵͳ�ϵ��ʼ������ת��main����֮ǰ������������ȵ���SystemInit������
   *	
   *����û�ϣ����������ʱ�ӿ����ڴ˴��ٴε��� SystemInit()�����ĺ���ʵ�ִ����system_stm32f2xx.c�ļ��С�
   */
    u16 i;
   	u8 j;
  NVIC_Config();	/* NVIC���ã���USART1�ж�*/
	SRAM_Init();
		 /* NAND FLASH��ʼ��*/
 FSMC_NAND_Init();

  /***NAND FLASH��λ***/
	FSMC_NAND_Reset();
  /* ��ȡNAND FLASH��ID */
  FSMC_NAND_ReadID(&NAND_ID);

  /* ��֤ NAND FLASH ID�Ƿ���ȷ */  
 if((NAND_ID.Maker_ID == NAND_SAMSUNG_MakerID) && (NAND_ID.Device_ID == NAND_SAMSUNG_DeviceID))
  {    WriteReadAddr.Zone = 0x00;
    WriteReadAddr.Block = 0x00;
    WriteReadAddr.Page = 0x00; 
		 for(i=0;i<1024;i++)
    {
    	WriteReadAddr.Block=i;
    	WriteReadAddr.Zone=0;
    	WriteReadAddr.Page=0;
    	FSMC_NAND_EraseBlock(WriteReadAddr); 
    }
  }
	WriteReadAddr.Block=0;
/* USART1 ���� ------------------------------------------------------*/
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
	 USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(COM1, &USART_InitStructure);	 /* ����USART_InitStructure�������ã����ô���USART1 */
		for(i=0;i<4096;i++)buffer[i]=0;
	for(j=0;j<31;j++){

  /* ʹ�ܴ���USART1�ķ����жϣ���USART1�Ĵ������ݼĴ���Ϊ��ʱ��������������ж�*/
 // USART_ITConfig(EVAL_COM1, USART_IT_TXE, ENABLE);
  
  /* �ȴ�TxBuffer�е�����ȫ��������ϣ�������ΪNbrOfDataToTransfer */
// while(TxCounter < NbrOfDataToTransfer)
  {}
 // 
  /* �ȴ���ֱ��TC =1Ϊֹ*/
 // while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
 // {}
  /* ʹ�ܴ���USART1�Ľ����жϣ���USART1�Ľ������ݼĴ���Ϊ�ǿ�ʱ��������������ж�*/
  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);
  
  /* �ȴ��������ݣ�������洢��RxBuffer�У�ֱ������������ΪNbrOfDataToReadʱֹͣ��*/
  while(RxCounter < NbrOfDataToRead)
  {}
 FSMC_NAND_WriteSmallPage(buffer, WriteReadAddr, PageNumber);
		WriteReadAddr.Page=WriteReadAddr.Page+PageNumber;
		if(WriteReadAddr.Page>63)
		{
			WriteReadAddr.Page=0x00;
			WriteReadAddr.Block+=1;
		}
		RxCounter=0;
	}
	 WriteReadAddr.Zone = 0x00;
    WriteReadAddr.Block = 0x00;
    WriteReadAddr.Page = 0x00; 	
	FSMC_NAND_ReadSmallPage(Buffer, WriteReadAddr, 1);
  while (1)
  {
  }
}

/******************************************************************************
* �������� NVIC_Config
* ��  �ܣ� ���ô���1���ж����ͣ����򿪴���1�ķ��ͺͽ����жϹ���
* ��  �룺�� 
* ��  ������  
* ��  ע����
********************************************************************************/
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* �� USART1 �ж����� */
	NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM1_IRQn;  /* �趨ΪUSART1�ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  /* �趨�����ȼ�Ϊ0*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		   /* �趨�����ȼ�Ϊ0*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   /* �жϴ�*/
	NVIC_Init(&NVIC_InitStructure);
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
