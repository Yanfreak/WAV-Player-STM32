/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� �������Ź�
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
__IO uint32_t TimingDelay = 0;
__IO uint32_t LsiFreq = 0;
__IO uint32_t CaptureNumber = 0, PeriodValue = 0;

/* ˽��ԭ�ͺ��� -------------------------------------------------------------*/
void Delay(__IO uint32_t nTime);
uint32_t GetLSIFrequency(void);

/* ˽�к��� -----------------------------------------------------------------*/

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
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDOff(LED2);

   /* ����Ϊ1����SysTick��ʱ���ж� */
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* ������� */ 
    while (1);
  }

  /* ���ϵͳ�Ѿ���IWDG��λ */
  if (RCC_GetFlagStatus(RCC_FLAG_IWDGRST) != RESET)
  {
    /* IWDGRST������� */
    /* ����LED2 */
    STM_EVAL_LEDOn(LED2);

    /* �����λ��ǼĴ��� */
    RCC_ClearFlag();
  }
  else
  {
    /* IWDGRST��ǼĴ���û�б����� */
    /* �ر�LED2 */
    STM_EVAL_LEDOff(LED2);
  }
 
  /* �õ�LSIƵ��:TIM5����������LSIƵ��*/
  LsiFreq = GetLSIFrequency();
   
  /* IWDG��ʱ����250 ms (��ʱ��������LSI��Ƶ�ʱ仯 */
  /* ����IWDG_PR��IWDG_RLR�Ĵ���*/
  IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);

  /* IWDG����ʱ��: LSI/32 */
  IWDG_SetPrescaler(IWDG_Prescaler_32);

  /* ���ü���������ֵ�����250ms IWDG��ʱTimeOut.
     ��������װ��ֵ = 250ms/IWDG��������ʱ������
                          = 250ms / (LSI/32)
                          = 0.25s / (LsiFreq/32)
                          = LsiFreq/(32 * 4)
                          = LsiFreq/128
   */
  IWDG_SetReload(LsiFreq/128);

  /* ���¼���IWDG������ */
  IWDG_ReloadCounter();

  /* ʹ��IWDG(LSI������ͨ��Ӳ��) */
  IWDG_Enable();

  while (1)
  {
    /* �л� LED3 */
    STM_EVAL_LEDToggle(LED3);

    /* ����240ms���ӳ� */
    Delay(240);

    /* ���¼���IWDG������ */
    IWDG_ReloadCounter();  
  }
}

/****************************************************************************
* ������: GetLSIFrequency()
* �� ��: ����TIM5����LSI����Ƶ��
* �� ��: ��
* �� ��: ��
****************************************************************************/
uint32_t GetLSIFrequency(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;
  TIM_ICInitTypeDef  TIM_ICInitStructure;
  RCC_ClocksTypeDef  RCC_ClockFreq;

  /* ����LSI���� ************************************************/
  RCC_LSICmd(ENABLE);
  
  /* �ȴ�LSI������λ*/
  while (RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {}

  /* ����TIM5 *******************************************************/ 
  /* ʹ��TIM5ʱ�� */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
  
  /* �����ڲ�TIM5_CH4���룬����LSIʱ����� */
  TIM_RemapConfig(TIM5, TIM5_LSI);

  /* ����TIM5Ԥ��Ƶ�� */
  TIM_PrescalerConfig(TIM5, 0, TIM_PSCReloadMode_Immediate);
  
  /* TIM5����:���벶��ģʽ ---------------------
     LSI�������ӵ�TIM5 CH4�������ش�����CCR4��TIM5���ڼ���Ƶ��ֵ */
  TIM_ICInitStructure.TIM_Channel = TIM_Channel_4;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV8;
  TIM_ICInitStructure.TIM_ICFilter = 0;
  TIM_ICInit(TIM5, &TIM_ICInitStructure);
  
  /* ʹ��TIM5�ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /*ʹ��TIM5������ */
  TIM_Cmd(TIM5, ENABLE);

  /* ��λ��־�Ĵ��� */
  TIM5->SR = 0;
    
  /* ʹ��CC4�ж����� */  
  TIM_ITConfig(TIM5, TIM_IT_CC4, ENABLE);


  /* �ȵ�TIM5�õ�2 LSI��Ե(������stm32f2xx_it.c�ļ��е�TIM5_IRQHandler() */
  while(CaptureNumber != 2)
  {
  }
  /* ��һ�γ�ʼ��TIM5��Χ�Ĵ���ΪĬ������ֵ */
  TIM_DeInit(TIM5);


  /* ����LSIƵ��,����TIM5����ʱ��Ƶ��(PCLK1)*/
  /* ���SYSCLK, HCLK �� PCLKx Ƶ�� */
  RCC_GetClocksFreq(&RCC_ClockFreq);

  /* �õ�PCLK1Ԥ��Ƶֵ */
  if ((RCC->CFGR & RCC_CFGR_PPRE1) == 0)
  { 
    /* PCLK1Ԥ��Ƶֵ����1 => TIMCLK = PCLK1 */
    return ((RCC_ClockFreq.PCLK1_Frequency / PeriodValue) * 8);
  }
  else
  { /* PCLK1Ԥ��Ƶֵ������1 => TIMCLK = 2 * PCLK1 */
    return (((2 * RCC_ClockFreq.PCLK1_Frequency) / PeriodValue) * 8) ;
  }
}

/****************************************************************************
* ������: delay_ms()
* �� ��: ָ���ӳ�ʱ�䳤��(�Ժ���Ϊ��λ)
* �� ��: ��
* �� ��: ��
****************************************************************************/
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

#ifdef  USE_FULL_ASSERT

/******************************************************************************
* ��������AssertFailed
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
