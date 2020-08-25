/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� NVIC
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
#include "stm32_eval.h"

/* �ض��� -------------------------------------------------------------------*/
/* �Զ��� -------------------------------------------------------------------*/
/* ˽�к궨�� ---------------------------------------------------------------*/
/* ˽�б��� -----------------------------------------------------------------*/
__IO uint8_t PreemptionOccured = 0; 
__IO uint8_t PreemptionPriorityValue = 0; 

/* ˽�к���ԭ�� -------------------------------------------------------------*/
void NVIC_Config(void);
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
	�����ڴ˴��ٴε��� SystemInit()�����ĺ���ʵ�ִ����system_stm32f2xx.c�ļ���*/ 

  /* ����NVIC */
  NVIC_Config();     
  
  /* ��ʼ����װ��STM322xG-EVAL���ϵķ�������ܺͰ�ť */       
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_PBInit(BUTTON_USER1, BUTTON_MODE_EXTI);

  /* ����SysTick���������ȼ�: ��ռ���ȼ��������ȼ� */
  NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), !PreemptionPriorityValue, 0));

  while (1)
  {
    if(PreemptionOccured != 0)
    {
	    
      /* ʱ����ʱ */
      Delay(0x5FFFF);
      
      STM_EVAL_LEDToggle(LED2);
      
      Delay(0x5FFFF);
      
      STM_EVAL_LEDToggle(LED3);
      
      Delay(0x5FFFF);
      
      STM_EVAL_LEDToggle(LED4);
      
      Delay(0x5FFFF); 
    }
  }
}

/****************************************************************************
* ������: NVIC_Config()
* �� ��: �����ж����ȼ�
* �� ��: ��
* �� ��: ��
****************************************************************************/
void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* ������ռ���ȼ��������ȼ�:
     - 1λ��ռ���ȼ�: ֵΪ0��1 
     - 3λ�����ȼ�: ֵΪ0��7
     - ֵԽ�����ȼ�Խ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

  /* ʹ�ܰ����ж����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannel = USER1_BUTTON_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
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
  /* �˴���������û����󱨸淽ʽ: ���(���������ֵ: �ļ���·�������ļ����кš� */

  /* ����ѭ�� */
  while (1)
  {
  }
}
#endif

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*************/
