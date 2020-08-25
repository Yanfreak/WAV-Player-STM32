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

/* ͷ�ļ�------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "main.h"
#include <stdio.h>
#include "stm32f2xx_adc.h"
#include "stm322xg_eval_systick.h"
#include "stm322xg_eval_lcd.h"
#include "stm32_eval.h"
#include "stm32f2xx_dac.h"
#define ADC1_DR_ADDRESS    ((uint32_t)0x4001204C)
#define SAMP_COUNT	20	/* ������������ʾ200ms�ڵĲ���������ƽ��ֵ */

uint16_t g_AdcValue;	/* ADC ����ֵ */
/*�������� LCD����ɫ��ǰɫ*/
extern uint16_t s_TextColor;
extern uint16_t s_BackColor;

/************************************************************************************
*������:BoardInit
*����˵��:��ʼ��ADC
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/

static void BoardInit(void)
{
  ADC_InitTypeDef       ADC_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  GPIO_InitTypeDef      GPIO_InitStructure;
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOA, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  /*******************����ADC����ܽ�*******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  ADC_DeInit();
  /* ADCͨ�ó�ʼ�� */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; 
  ADC_CommonInit(&ADC_CommonInitStructure);
  
  /* ADC1��ʼ�� */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;   
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1ͨ������ */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_3Cycles);

  ADC_Cmd(ADC1, ENABLE); /*ʹ��ADC*/
    
  ADC_SoftwareStartConv(ADC1);/*ADC��ʼת��*/
  SysTick_Config(120000000 / 1000000); /*����systickû1ms�ж�һ��*/

}
/************************************************************************************
*������:AdcPro
*����˵��: ����ADC����ֵ
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/

void AdcPro(void)
{
	static uint16_t buf[SAMP_COUNT];
	static uint8_t write;
	uint32_t sum;
	uint8_t i;

	g_AdcValue= ADC_GetConversionValue(ADC1);
#if 0
	if (++write >= SAMP_COUNT)
	{
		write = 0;
	}

	/* ������δ��������ƽ��ֵ�ķ��������˲�
		Ҳ���Ը����£�ѡ��ȥ����������2��ֵ�������ݸ��Ӿ�ȷ
	*/
	sum = 0;
	for (i = 0; i < SAMP_COUNT; i++)
	{
		sum += buf[i];
	}
	g_AdcValue = sum / SAMP_COUNT;	/* ADC����ֵ�����ɴβ���ֵƽ�� */
#endif
	ADC_SoftwareStartConv(ADC1);	/* ��������´�ADCת�� */
}

/***********************************************************
��������GetADC
��  ��:��
��  ��:ADC����ֵ
����˵������
*********************************************************/
uint16_t GetADC(void)
{
	uint16_t ret;

	/* �����ٽ����������ر��ж� */
	__set_PRIMASK(1);  /* ���ж� */

	ret = g_AdcValue;

	__set_PRIMASK(0);  /* ���ж� */

	return ret;
}
/************************************************************************************
*������:
*����˵��:
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
void DAC_Config(void)
{
  DAC_InitTypeDef  DAC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  DAC_DeInit();
  DAC_InitStructure.DAC_Trigger=DAC_Trigger_Software;//���������
  DAC_InitStructure.DAC_WaveGeneration=DAC_WaveGeneration_None;//�رղ�������
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_TriangleAmplitude_4095;
  DAC_InitStructure.DAC_OutputBuffer=DAC_OutputBuffer_Disable;//ʹ��DACͨ������   
  DAC_Init(DAC_Channel_2,&DAC_InitStructure);
    /* Enable DAC Channel1 */
  DAC_Cmd(DAC_Channel_2, ENABLE);
  DAC_SetChannel2Data(DAC_Align_12b_R,0x7ff);//���һ��Ļ�׼��ѹ
 
  DAC_DualSoftwareTriggerCmd(ENABLE);//ʹ���������,����DAC��ֵ
}
/************************************************************************************
*������: LCD_Interface
*����˵��:LCD����
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
void LCD_Interface(void)
{
	char temp[32];
	LCD_Clear(Blue);
  	s_TextColor=Yellow;
  	s_BackColor=Blue;
	LCD_DrawRect(0,0,239,319);
	LCD_DrawLine(0,20,320,Horizontal);
	sprintf(temp,"��ѹ�ɼ�����");
	LCD_DisplayStringHZ(108,4,(uint8_t *)temp);	
	sprintf(temp,"��ѹ�ɼ�ֵ:");
	LCD_DisplayStringHZ(50,50,(uint8_t *)temp);
	LCD_DisplayStringHZ(240,140,"V");
  	LCD_DrawLine(0,220,320,Horizontal);
  	sprintf(temp,"�������촴�¿Ƽ���չ���޹�˾");
  	LCD_DisplayStringHZ(48,222,(uint8_t *)temp);   
}

/************************************************************************************
*������:main
*����˵��:������
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
int main(void)
{
	uint16_t value;
	float fvalue=0;
	char temp[32];
	BoardInit();	/* Ϊ����main���������������Щ�����ǽ���ʼ���Ĵ����װ��������� */
//	STM3220F_LCD_Init();/*��ʼ��LCD*/
//	LCD_Interface();/*LCD����*/
	StartTimer(0, 200);	/* ��ʱ��0���� 200���� */
	/* ����������ѭ���� */
    DAC_Config();
	while (1)
	{
#if 0
		CPU_IDLE();

		if (CheckTimer(0))	/* ��ʱ�� */
		{
			StartTimer(0, 30);	/* �����¸���ʱ���� */

			value = GetADC();
			fvalue=(float)value*3.3/4095; /*��ѹֵת��*/
            DAC_SetChannel2Data(DAC_Align_12b_R,value);
//			sprintf(temp,"%4.1f",fvalue);
//			LCD_DisplayStringLargeFont(100,100,(uint8_t *)temp);
		}
#endif
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
