 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� CAN/Networking/main.c 
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
/* �궨��  ---------------------------------------------------------------------*/
/* �������� --------------------------------------------------------------------*/
CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
CanTxMsg TxMessage;
uint8_t KeyNumber = 0x0;

/* ��������---------------------------------------------------------------------*/
void NVIC_Config(void);
void CAN_Config(void);
void Init_RxMes(CanRxMsg *RxMessage);
void Delay(void);
void Display_Init(void);


/******************************************************************************
* ��������main 
* ��  �ܣ�ͨ��������ʽ����CAN�����������ݷ��ͺͽ��չ��� 
* ��  �룺 
* ��  ������  
* ��  ע����
********************************************************************************/

int main(void)
{
 u8  SendNum=0;
 char buf[50] = {0};
  /*�ڴ˽׶δ�������ʱ���Ѿ�����ʼ����ɣ�ʱ�ӳ�ʼ��������SystemInit����ʵ�֣�SystemInit����
   *�����startup_stm32f2xx.s�Ļ�������ļ��С�
   *��ϵͳ�ϵ��ʼ������ת��main����֮ǰ������������ȵ���SystemInit������
   *	
   *����û�ϣ����������ʱ�ӿ����ڴ˴��ٴε��� SystemInit()�����ĺ���ʵ�ִ����system_stm32f2xx.c�ļ��С�
   */
           
  /* NVIC���ã�����CAN�ж� */
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

  /* �����˵���ʾ */
  Display_Init();
  
  /* ����CAN  */
  CAN_Config();
  while(1)
  {
        TxMessage.Data[0] = SendNum;
        CAN_Transmit(CANx, &TxMessage);
		SendNum++;
		sprintf(buf, " Send Data = %d", SendNum);				  /* ���ַ�����Ϣ�����buf��*/
        LCD_DisplayStringLine(LINE(3),"                          ");
		LCD_DisplayStringLine(LINE(3), (uint8_t *)buf);	  /* ��buf�е��ַ�����Ϣ��ʾ��Һ���ĵ�4��*/
        delay_ms(1000);
  }
  
}

/******************************************************************************
* ��������CAN_Config 
* ��  �ܣ�����CAN ���������� 
* ��  �룺 
* ��  ������  
* ��  ע����
********************************************************************************/
void CAN_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* CAN ��GPIOs �ܽ����� **************************************************/

  /* �� GPIO ʱ�� */
  RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);

  /* ���� CAN pins �� AF9 */
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 
  
  /* ���� CAN RX �� TX �ܽ� */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

  /* �� CAN ʱ�� */
  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
  
  /* ��ʼ�� CAN �Ĵ��� */
  CAN_DeInit(CANx);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN ��ʼ���ṹ��������� */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
  /* CAN ����������Ϊ 1MBps (CAN �ڲ�ʱ�ӹ����� 30 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 2;
  CAN_Init(CANx, &CAN_InitStructure);

  /* ��ʼ��CAN ���˺� */
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
  
  /* ��д ���ͷ������� */
  TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x01;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 1;
  
  /* �� FIFO 0 �����ж� */
  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
}

/******************************************************************************
* ��������NVIC_Config 
* ��  �ܣ�����CAN��RX0�ж� 
* ��  �룺 
* ��  ������  
* ��  ע����
********************************************************************************/
void NVIC_Config(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;

#ifdef  USE_CAN1 
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
#else  /* USE_CAN2 */
  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
#endif /* USE_CAN1 */

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Initializes the Rx Message.
  * @param  RxMessage: pointer to the message to initialize
  * @retval None
  */
/******************************************************************************
* ��������Init_RxMes 
* ��  �ܣ���ʼ���������ݵ���Ϣ�ṹ�� 
* ��  �룺RxMessage �� ָ��������ݵ���Ϣ�ṹ��
* ��  ������  
* ��  ע����
********************************************************************************/

void Init_RxMes(CanRxMsg *RxMessage)
{
  uint8_t i = 0;

  RxMessage->StdId = 0x00;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (i = 0;i < 8;i++)
  {
    RxMessage->Data[i] = 0x00;
  }
}

/**
  * @brief  Turn ON/OFF the dedicated led
  * @param  Ledstatus: Led number from 0 to 3.
  * @retval None
  */
/******************************************************************************
* ��������LED_Display 
* ��  �ܣ�LED ָʾ����ʾ��ͨ�����������ʾ��Ӧ��LED 
* ��  �룺Ledstatus : 0~4 ��ָ����ӦLED���� 
* ��  ������  
* ��  ע����
********************************************************************************/

void LED_Display(uint8_t Ledstatus)
{
  /* Turn off all leds */
  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);
  
  switch(Ledstatus)
  {
    case(1): 
      STM_EVAL_LEDOn(LED1);
      break;
   
    case(2): 
      STM_EVAL_LEDOn(LED2);
      break;
 
    case(3): 
      STM_EVAL_LEDOn(LED3);
      break;

    case(4): 
      STM_EVAL_LEDOn(LED4);
      break;
    default:
      break;
  }
}

/******************************************************************************
* ��������Delay 
* ��  �ܣ���ʱ���� 
* ��  �룺 
* ��  ������  
* ��  ע����
********************************************************************************/

void Delay(void)
{
  uint16_t nTime = 0x0000;

  for(nTime = 0; nTime <0xFFF; nTime++)
  {
  }
}

/******************************************************************************
* ��������Display_Init 
* ��  �ܣ���ʾRS485�������� 
* ��  �룺��
* ��  ������  
* ��  ע����
********************************************************************************/

void Display_Init(void)
{
  /* ��ʼ��Һ����Ļ */
  STM322xG_LCD_Init();

  /* �����ĻΪ��ɫ */ 
  LCD_Clear(White);

  /* ѡ������ */
  LCD_SetFont(&Font8x12);

  /* ����Һ������ɫΪ��ɫ���ı�ɫΪ��ɫ*/
  LCD_SetBackColor(Blue);
  LCD_SetTextColor(White);

  /* ��ʾ�ַ��� */
  LCD_DisplayStringLine(LINE(0x13), "          CAN bus example               ");

  /* �������� */
  LCD_SetFont(&Font16x24);

  /* ��ʾ�ַ��� */
  LCD_DisplayStringLine(LINE(0), "   CAN bus example  ");

  /* ����Һ������ɫΪ�ף��ı�ɫΪ��ɫ*/
  LCD_SetBackColor(White);
  LCD_SetTextColor(Blue);
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
