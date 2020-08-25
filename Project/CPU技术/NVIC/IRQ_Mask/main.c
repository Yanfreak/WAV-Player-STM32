 /****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� ��stm32f2xx_it.c
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

/* �������� -------------------------------------------------------------------*/

void TIM_Config(void);

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
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);

  /* ��ʼ����װ��STM322xG-EVAL�����ϵ�KEY��SEL */
  STM_EVAL_PBInit(BUTTON_USER1, BUTTON_MODE_GPIO);
    
  /* ����TIM-------------------------------------------------------*/
  TIM_Config();

  while (1)
  { 
    /* �ȴ�������Ӧ */
    while(STM_EVAL_PBGetState(BUTTON_USER1) == RESET)
    {
    }
    while(STM_EVAL_PBGetState(BUTTON_USER1) != RESET)
    {
    }

    /* ���ָ���ִ�����ȼ�Ϊ0������Է�ֹ����ͨ���ϱ����Ƶ�Ӳ��������
	   ���еĴӼ��ʼ���п����õ��쳣 */
    __disable_irq();

    /* ����LED4 */
    STM_EVAL_LEDOn(LED4);

    /* �ȴ�������Ӧ */
    while(STM_EVAL_PBGetState(BUTTON_USER1) == RESET)
    {
    }
    while(STM_EVAL_PBGetState(BUTTON_USER1) != RESET)
    {
    }

    /* ��ָʾ���������п����õ��쳣���ȼ������� */
    __enable_irq();

    /* �ر�LED4 */
    STM_EVAL_LEDOff(LED4);
  }
}

/****************************************************************************
* ������: TIM_Config()
* �� ��: ����ʹ�ü�ʱ��
* �� ��: ��
* �� ��: ��
****************************************************************************/
void TIM_Config(void)
{ 
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
  TIM_OCInitTypeDef  TIM_OCInitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;

  /* ʹ��TIM2,TIM3��TIM4ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 | RCC_APB1Periph_TIM3 |
                         RCC_APB1Periph_TIM4, ENABLE);

  /* ����TIM2 */
  TIM_TimeBaseStructure.TIM_Period = 0x4AF;          
  TIM_TimeBaseStructure.TIM_Prescaler = ((SystemCoreClock/1200) - 1);
  TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;    
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
  TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  TIM_OCStructInit(&TIM_OCInitStructure);
  
  /* ���ó�����Ƚ϶�ʱģʽ: ͨ��1��Channel1��*/
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStructure.TIM_Pulse = 0x0;  
  TIM_OC1Init(TIM2, &TIM_OCInitStructure);
  
  /* ����TIM3 */
  TIM_TimeBaseStructure.TIM_Period = 0x95F;    
  TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
  
  /* ���ó�����Ƚ϶�ʱģʽ: ͨ��1��Channel1��*/
  TIM_OC1Init(TIM3, &TIM_OCInitStructure);
  
  /* ����TIM4 */
  TIM_TimeBaseStructure.TIM_Period = 0xE0F;  
  TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
  
  /* ���ó�����Ƚ϶�ʱģʽ: ͨ��1��Channel1�� */
  TIM_OC1Init(TIM4, &TIM_OCInitStructure);

  /* ��������TIM2,TIM3��TIM4Ԥ��Ƶֵ */
  TIM_PrescalerConfig(TIM2, ((SystemCoreClock/1200) - 1), TIM_PSCReloadMode_Immediate);
  TIM_PrescalerConfig(TIM3, ((SystemCoreClock/1200) - 1), TIM_PSCReloadMode_Immediate);
  TIM_PrescalerConfig(TIM4, ((SystemCoreClock/1200) - 1), TIM_PSCReloadMode_Immediate);

  /* ���TIM2,TIM3��TIM4���¹����־�Ĵ��� */
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  TIM_ClearFlag(TIM3, TIM_FLAG_Update);
  TIM_ClearFlag(TIM4, TIM_FLAG_Update);

  /* ��������λΪ��ռ���ȼ� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* ʹ��TIM2�ж� */
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* ʹ��TIM3�ж� */
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_Init(&NVIC_InitStructure);

  /* ʹ��TIM4�ж� */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_Init(&NVIC_InitStructure);

  /* ʹ��TIM2,TIM3��TIM4�����ж� */
  TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
  TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
  TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

  /* ʹ��TIM2,TIM3��TIM4������ */
  TIM_Cmd(TIM2, ENABLE);
  TIM_Cmd(TIM3, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
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
