/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� RCCʱ�ӿ���
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
void Delay (uint32_t nCount);

/* ˽�к��� -----------------------------------------------------------------*/

/****************************************************************************
* ������: main()
* �� ��: ������
* �� ��: ��
* �� ��: ��
****************************************************************************/
int main(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	/*RCC_ClocksTypeDef RCC_ClockFreq;*/

	/*�ڴ˽׶δ�������ʱ���Ѿ�����ʼ����ɣ�ʱ�ӳ�ʼ��������SystemInit����ʵ�֣�
	SystemInit���������startup_stm32f2xx.s�Ļ�������ļ��С���ϵͳ�ϵ��ʼ����
	��ת��main����֮ǰ������������ȵ���SystemInit����������û�ϣ����������ʱ��
	�����ڴ˴��ٴε��� SystemInit()�����ĺ���ʵ�ִ����system_stm32f2xx.c�ļ��С�*/ 
	  
	
    /* ��ʼ����װ��EITP���ϵ�lED */
    STM_EVAL_LEDInit(LED2);
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED4);

    /* ����LED3 */
    STM_EVAL_LEDOn(LED3);
   
    /* ����������RCC_ClockFreq�ṹ�壬����ͬ�����ڵ�оƬʱ��Ƶ��(���ڵ���Ŀ��) */
   /* RCC_GetClocksFreq(&RCC_ClockFreq);*/
  
    /* ʹ�ܰ�ȫϵͳʱ��(CSS): HSEʱ�Ӹ�λʧ�ܽ�����һ�������ж��쳣 */
    RCC_ClockSecuritySystemCmd(ENABLE);
 
    /* ʹ�ܺ�����RCCȫ��IRQͨ��, ���ڹ���HSE������PLL�жϡ���Щ�ж���stm32f2xx_it.c������ */
    NVIC_InitStructure.NVIC_IRQChannel = RCC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
   
    /* ѡ��HSEʱ���������ΪMCO1(PA8) */
    RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);

    while (1)
    {
        /* �л�LED2��LED4 */
       STM_EVAL_LEDToggle(LED2);
       STM_EVAL_LEDToggle(LED4);

   	 	Delay(0x7FFFF);

    	/* �л�LED3 */
   		//STM_EVAL_LEDToggle(LED3);

    	//Delay(0x7FFFF);    
    }
}

/****************************************************************************
* ������: Delay()
* �� ��: ָ���ӳ�ʱ�䳤��(�Ժ���Ϊ��λ)
* �� ��: ��
* �� ��: ��
****************************************************************************/
void Delay(__IO uint32_t nCount)
{

  while (nCount != 0)
  {
    nCount--;
  }
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
