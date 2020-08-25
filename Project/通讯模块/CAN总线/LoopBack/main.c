 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� CAN/LoopBack/main.c
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
#include "main.h"

/* ���Ͷ���  -------------------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* �궨��  ---------------------------------------------------------------------*/
/* �������� --------------------------------------------------------------------*/
__IO uint32_t ret = 0; /* �жϷ�����򷵻�״̬�ı��� */
volatile TestStatus TestRx;


/* ��������---------------------------------------------------------------------*/
void NVIC_Config(void);
TestStatus CAN_Polling(void);
TestStatus CAN_Interrupt(void);

/******************************************************************************
* ��������main.c 
* ��  �ܣ�CAN������ LOOPBACK���ܲ��� 
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

#ifdef USE_CAN1
  /* ��CANx ����ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
#else /* USE_CAN2 */
  /* ��CAN1 & 2  ����ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);
#endif  /* USE_CAN1 */

  /* NVIC �ж����� */
  NVIC_Config();
  
  /* ���� LED 1..4 */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);

  /* ��loopbackģʽ�£�����polling��ѯ��ʽ ����CAN���ߵķ��ͺͽ��ղ��ԣ���������Ϊ125Kb/s */
  TestRx = CAN_Polling();

  if (TestRx !=  FAILED)
  { /* ���ղ��Գɹ� */

    /* ����LED1 */
    STM_EVAL_LEDOn(LED1);
  }
  else
  { /* ���ղ���ʧ�� */

    /* ����LED3 */
    STM_EVAL_LEDOn(LED3);
  }

  /* ��loopbackģʽ�£������жϷ�ʽ ����CAN���ߵķ��ͺͽ��ղ��ԣ���������Ϊ500Kb/s */
  TestRx = CAN_Interrupt();

  if (TestRx !=  FAILED)
  { /* ���ղ��Գɹ� */

    /* ����LED4 */
    STM_EVAL_LEDOn(LED4);
  }
  else
  { /* ���ղ���ʧ�� */

    /* ����LED2 */
    STM_EVAL_LEDOn(LED2);
  }

  /* ����ѭ�� */
  while (1)
  {
  }
}

/******************************************************************************
* ��������CAN_Polling 
* ��  �ܣ�����CAN��������ͨ����ѯ��ʽʵ�ַ��ͺͽ��չ��� 
* ��  �룺 
* ��  ���� PASSED : ���ͺͽ��ճɹ�
*          FAILED : ���ͺͽ��ճ��� 
* ��  ע����
********************************************************************************/

TestStatus CAN_Polling(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  CanTxMsg TxMessage;
  CanRxMsg RxMessage;
  uint32_t i = 0;
  uint8_t TransmitMailbox = 0;

  /* CAN �Ĵ�����ʼ�� */
  CAN_DeInit(CANx);

  CAN_StructInit(&CAN_InitStructure);

  /* CAN ��ʼ���ṹ�����ò��� */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

  /* CAN ����������Ϊ 125kbps (CAN�ڲ�ʱ�ӹ����� 30 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 16;
  CAN_Init(CANx, &CAN_InitStructure);

  /* CAN �˲���ʼ�� */
#ifdef  USE_CAN1
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
#else /* USE_CAN2 */
  CAN_FilterInitStructure.CAN_FilterNumber = 14;
#endif  /* USE_CAN1 */
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;  
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;

  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* ���� */
  TxMessage.StdId = 0x11;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 2;
  TxMessage.Data[0] = 0xCA;
  TxMessage.Data[1] = 0xFE;

  TransmitMailbox = CAN_Transmit(CANx, &TxMessage);
  i = 0;
  while((CAN_TransmitStatus(CANx, TransmitMailbox)  !=  CANTXOK) && (i  !=  0xFFFF))
  {
    i++;
  }

  i = 0;
  while((CAN_MessagePending(CANx, CAN_FIFO0) < 1) && (i  !=  0xFFFF))
  {
    i++;
  }

  /* ���� */
  RxMessage.StdId = 0x00;
  RxMessage.IDE = CAN_ID_STD;
  RxMessage.DLC = 0;
  RxMessage.Data[0] = 0x00;
  RxMessage.Data[1] = 0x00;
  CAN_Receive(CANx, CAN_FIFO0, &RxMessage);

  if (RxMessage.StdId != 0x11)
  {
    return FAILED;  
  }

  if (RxMessage.IDE != CAN_ID_STD)
  {
    return FAILED;
  }

  if (RxMessage.DLC != 2)
  {
    return FAILED;  
  }

  if ((RxMessage.Data[0]<<8|RxMessage.Data[1]) != 0xCAFE)
  {
    return FAILED;
  }
  
  return PASSED; /* ����ͨ�� */
}

/******************************************************************************
* ��������CAN_Polling 
* ��  �ܣ�����CAN��������ͨ���жϷ�ʽʵ�ַ��ͺͽ��չ��� 
* ��  �룺 
* ��  ���� PASSED : ���ͺͽ��ճɹ�
*          FAILED : ���ͺͽ��ճ��� 
* ��  ע����
********************************************************************************/
TestStatus CAN_Interrupt(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  CanTxMsg TxMessage;
  uint32_t i = 0;

  /* CAN �Ĵ�����ʼ�� */
  CAN_DeInit(CANx);

  CAN_StructInit(&CAN_InitStructure);

  /* CAN �Ĵ�����ʼ�� */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  
  /* CAN ����������Ϊ 500kbps (CAN�ڲ�ʱ�ӹ����� 30 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 4;
  CAN_Init(CANx, &CAN_InitStructure);

  /* CAN �˲� ��ʼ�� */
#ifdef  USE_CAN1
  CAN_FilterInitStructure.CAN_FilterNumber = 1;
#else /* USE_CAN2 */
  CAN_FilterInitStructure.CAN_FilterNumber = 15;
#endif  /* USE_CAN1 */
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* ��CAN FIFO0 �ж� */ 
  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);

  /*����1������ */
  TxMessage.StdId = 0;
  TxMessage.ExtId = 0x1234;
  TxMessage.IDE = CAN_ID_EXT;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.DLC = 2;
  TxMessage.Data[0] = 0xDE;
  TxMessage.Data[1] = 0xCA;
  CAN_Transmit(CANx, &TxMessage);

  /* ��ʼ���ж�״̬����ֵ */
  ret = 0xFF;
       
  /*�ȴ��жϷ���������������� */
  i = 0;
  while((ret ==  0xFF) && (i < 0xFFF))
  {
    i++;
  }
  
  if (i ==  0xFFF)
  {
    ret = 0;  
  }

  /* �ر�CAN�ж� */
  CAN_ITConfig(CANx, CAN_IT_FMP0, DISABLE);

  return (TestStatus)ret;
}

/******************************************************************************
* ��������NVIC_Config 
* ��  �ܣ�����CAN RX0�ж� 
* ��  �룺 
* ��  ������  
* ��  ע����
********************************************************************************/
void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* �� CANx RX0 �ж� */
#ifdef  USE_CAN1 
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
#else  /* USE_CAN2 */
  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
#endif /* USE_CAN1 */

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
