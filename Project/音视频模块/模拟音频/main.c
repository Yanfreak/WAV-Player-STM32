 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： main.c
*   摘 要    ： 
*
*   当前版本 ： 
*   作 者    ： EFLAG
*   完成日期 ： 
*
*   取代版本 ：
*   原作者   ： 
*   修改内容 ：
*   完成日期 ：2012.08.01 
*
**********************************************************/

/* 头文件------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "main.h"
#include <stdio.h>
#include "stm32f2xx_adc.h"
#include "stm322xg_eval_systick.h"
#include "stm322xg_eval_lcd.h"
#include "stm32_eval.h"
#include "stm32f2xx_dac.h"
#define ADC1_DR_ADDRESS    ((uint32_t)0x4001204C)
#define SAMP_COUNT	20	/* 样本个数，表示200ms内的采样数据求平均值 */

uint16_t g_AdcValue;	/* ADC 采样值 */
/*变量引用 LCD背景色和前色*/
extern uint16_t s_TextColor;
extern uint16_t s_BackColor;

/************************************************************************************
*函数名:BoardInit
*函数说明:初始化ADC
*输入参数:无
*
*返回值:无
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

  /*******************配置ADC输入管脚*******************************/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  ADC_DeInit();
  /* ADC通用初始化 */
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; 
  ADC_CommonInit(&ADC_CommonInitStructure);
  
  /* ADC1初始化 */
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = ENABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;   
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  /* ADC1通道配置 */ 
  ADC_RegularChannelConfig(ADC1, ADC_Channel_8, 1, ADC_SampleTime_3Cycles);

  ADC_Cmd(ADC1, ENABLE); /*使能ADC*/
    
  ADC_SoftwareStartConv(ADC1);/*ADC开始转换*/
  SysTick_Config(120000000 / 1000000); /*配置systick没1ms中断一次*/

}
/************************************************************************************
*函数名:AdcPro
*函数说明: 返回ADC采样值
*输入参数:无
*
*返回值:无
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

	/* 下面这段代码采用求平均值的方法进行滤波
		也可以改善下，选择去掉最大和最下2个值，是数据更加精确
	*/
	sum = 0;
	for (i = 0; i < SAMP_COUNT; i++)
	{
		sum += buf[i];
	}
	g_AdcValue = sum / SAMP_COUNT;	/* ADC采样值由若干次采样值平均 */
#endif
	ADC_SoftwareStartConv(ADC1);	/* 软件启动下次ADC转换 */
}

/***********************************************************
函数名：GetADC
输  入:无
输  出:ADC采样值
功能说明：无
*********************************************************/
uint16_t GetADC(void)
{
	uint16_t ret;

	/* 进行临界区保护，关闭中断 */
	__set_PRIMASK(1);  /* 关中断 */

	ret = g_AdcValue;

	__set_PRIMASK(0);  /* 开中断 */

	return ret;
}
/************************************************************************************
*函数名:
*函数说明:
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
void DAC_Config(void)
{
  DAC_InitTypeDef  DAC_InitStructure;
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
  DAC_DeInit();
  DAC_InitStructure.DAC_Trigger=DAC_Trigger_Software;//由软件触发
  DAC_InitStructure.DAC_WaveGeneration=DAC_WaveGeneration_None;//关闭波形生成
  DAC_InitStructure.DAC_LFSRUnmask_TriangleAmplitude=DAC_TriangleAmplitude_4095;
  DAC_InitStructure.DAC_OutputBuffer=DAC_OutputBuffer_Disable;//使能DAC通道缓存   
  DAC_Init(DAC_Channel_2,&DAC_InitStructure);
    /* Enable DAC Channel1 */
  DAC_Cmd(DAC_Channel_2, ENABLE);
  DAC_SetChannel2Data(DAC_Align_12b_R,0x7ff);//输出一半的基准电压
 
  DAC_DualSoftwareTriggerCmd(ENABLE);//使能软件触发,更新DAC的值
}
/************************************************************************************
*函数名: LCD_Interface
*函数说明:LCD界面
*输入参数:无
*
*返回值:无
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
	sprintf(temp,"电压采集测试");
	LCD_DisplayStringHZ(108,4,(uint8_t *)temp);	
	sprintf(temp,"电压采集值:");
	LCD_DisplayStringHZ(50,50,(uint8_t *)temp);
	LCD_DisplayStringHZ(240,140,"V");
  	LCD_DrawLine(0,220,320,Horizontal);
  	sprintf(temp,"北京亿旗创新科技发展有限公司");
  	LCD_DisplayStringHZ(48,222,(uint8_t *)temp);   
}

/************************************************************************************
*函数名:main
*函数说明:主函数
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
int main(void)
{
	uint16_t value;
	float fvalue=0;
	char temp[32];
	BoardInit();	/* 为了是main函数看起来更简洁些，我们将初始化的代码封装到这个函数 */
//	STM3220F_LCD_Init();/*初始化LCD*/
//	LCD_Interface();/*LCD界面*/
	StartTimer(0, 200);	/* 定时器0周期 200毫秒 */
	/* 进入主程序循环体 */
    DAC_Config();
	while (1)
	{
#if 0
		CPU_IDLE();

		if (CheckTimer(0))	/* 定时到 */
		{
			StartTimer(0, 30);	/* 启动下个定时周期 */

			value = GetADC();
			fvalue=(float)value*3.3/4095; /*电压值转换*/
            DAC_SetChannel2Data(DAC_Align_12b_R,value);
//			sprintf(temp,"%4.1f",fvalue);
//			LCD_DisplayStringLargeFont(100,100,(uint8_t *)temp);
		}
#endif
	}
}

#ifdef  USE_FULL_ASSERT

/***************************************************************
*函数名:assert_failed
*函数说明:报告参数错误的位置
*	
*输入参数:
*		file:错误所在的文件名
*		line:错误所在文件中的行号
*	
*返回值:无	
*****************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
 /****此处可以添加用户错误报告方式**********************/
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
