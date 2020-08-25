/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� NVICǶ���ж������жϿ�����
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
******************************************************************************/

/*************ͷ�ļ�**********************************************************/
#include "stm32f2xx.h"
#include "stm32f2xx.h"
#include "stm32_eval.h"

/* �ض��� -------------------------------------------------------------------*/
/* �Զ��� -------------------------------------------------------------------*/

#define USART3_DR_ADDRESS    ((uint32_t)0x40011004)

/* ˽�к궨�� ---------------------------------------------------------------*/
/* ˽�б��� -----------------------------------------------------------------*/
__IO uint32_t LowPowerMode = 0;
uint16_t DstBuffer[10]= {0x1, 0x2, 0x3, 0x4, 0x5, 0x6, 0x70, 0x80, 0x90, 0xa0};

/* ˽�к���ԭ�� -------------------------------------------------------------*/	
void DMA_Config(void);
void USART_Config(void);
uint8_t Buffercmp16(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength);
void Delay(__IO uint32_t nCount);

/* ˽�к��� ------------------------------------------------------------------*/

/****************************************************************************
* ������: main()
* �� ��: ������
* �� ��: ��
* �� ��: ��
****************************************************************************/
int main(void)
{
	/*�ڴ˽׶δ�������ʱ���Ѿ�����ʼ����ɣ�ʱ�ӳ�ʼ��������SystemInit����ʵ�֣�
	SystemInit���������startup_stm32f2xx.s�Ļ�������ļ��С���ϵͳ�ϵ��ʼ����
	��ת��main����֮ǰ������������ȵ���SystemInit����������û�ϣ����������ʱ��
	�����ڴ˴��ٴε��� SystemInit()�����ĺ���ʵ�ִ����system_stm32f2xx.c�ļ��С�*/ 

  /* ��ʼ����װ��STM322xG-EVAL���ϵķ�������ܺͰ�ť */       
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDOff(LED4);
  STM_EVAL_PBInit(BUTTON_USER1, BUTTON_MODE_EXTI); 

  /* ����DMA1��4ͨ��(���ӵ�USART3_RX) */
  DMA_Config();

  /* ����EVAL_COM1(USART3) */
  USART_Config();  
    
  while (1)
  {
    if(LowPowerMode == 1) /* �͵���ģʽ��ѡ����������ϵͳ���߻�ر�ʱ���õ��� */
    {
      /* Ϩ��LED2��LED3 */
      STM_EVAL_LEDOff(LED2);
      STM_EVAL_LEDOff(LED3);

      /* �������WFIģʽ */
      __WFI(); //�ȴ��ж�
      LowPowerMode = 0;
    }

    Delay(0xFFFFF);
    STM_EVAL_LEDToggle(LED4);
  }
}

/****************************************************************************
* ������: USART_Config()
* �� ��: ����EVAL_COM1 (USART3)
* �� ��: ��
* �� ��: ��
****************************************************************************/
void USART_Config(void)
{
    /* EVAL_COM1��������:
        - BaudRate�������ʣ� = 115200 ����  
        - Word Length���ֳ��� = 8 λ
        - One Stop Bit��һ��ֹͣλ��
        - No parity����У��λ��
        -Ӳ�������Ʋ�ʹ�� (RTS��CTS�ź�)
        - ���ý��պͷ���
  */
  USART_InitTypeDef  USART_InitStructure;
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(COM1, &USART_InitStructure);

  /* ʹ��DMA�������� */
  USART_DMACmd(EVAL_COM1, USART_DMAReq_Rx, ENABLE);
}

/****************************************************************************
* ������: DMA_Config()
* �� ��: ����DMA1����USART3_RX DMA����
* �� ��: ��
* �� ��: ��
****************************************************************************/
void DMA_Config(void)				   
{
  DMA_InitTypeDef  DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* ʹ��DMA1ʱ�� */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

  /* ����DMA1��4ͨ�� */
  DMA_InitStructure.DMA_Channel = DMA_Channel_4;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)USART3_DR_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)DstBuffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize = 10;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream5, &DMA_InitStructure);
  DMA_Cmd(DMA2_Stream5, ENABLE);

  /* ʹ��DMA1_Stream1�����������ж� */
  DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE);
  
  /* ʹ��DMA1_Stream1 */
  DMA_Cmd(DMA2_Stream5, ENABLE);
    
  /* ʹ��DMA1_Stream1�ж����� */
  NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* ������: Buffercmp16()
* �� ��: �Ƚ�����������
* �� ��: pBuffer1,pBuffer2:��������ȡ�BufferLength:�������ĳ���
* �� ��: ���0:pBuffer1��pBuffer2��ͬ�����1:pBuffer1��pBuffer2��ͬ
****************************************************************************/
uint8_t Buffercmp16(uint16_t* pBuffer1, uint16_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return 1;
    }

    pBuffer1++;
    pBuffer2++;
  }
  return 0;
}

/****************************************************************************
* ������: Delay()
* �� ��: ָ���ӳ�ʱ�䳤��(�Ժ���Ϊ��λ)
* �� ��: ��
* �� ��: ��
****************************************************************************/
void Delay(__IO uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

#ifdef  USE_FULL_ASSERT

/******************************************************************************
* ��������assert_failed()
* ��  �ܣ�������������λ��
* ��  �룺�������ڵ��ļ����ʹ��������ļ��е��к�
* ��  ������  
* ��  ע����
********************************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
	
	/******�˴���������û����󱨸淽ʽ******/

  while (1)
  {
  }
}
#endif

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*************/
