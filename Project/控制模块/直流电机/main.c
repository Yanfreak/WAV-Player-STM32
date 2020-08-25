/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� ֱ�����
*   ժ Ҫ    �� 
*
*   ��ǰ�汾 �� 
*   �� ��    �� EFLAG
*   ������� �� 
*
*   ȡ���汾 ��
*   ԭ����   ����ΰ 
*   �޸����� ��
*   ������� ��2012.08.01 
*
******************************************************************************/

/*************ͷ�ļ�**********************************************************/
#include <stdio.h>
#include "main.h"
#include "stm322xg_eval_lcd.h"
#include "stm322xg_eval_keypad.h"
#include "stm32f2xx.h"
#include "stm32f2xx_tim.h"
#include "stm32f2xx_gpio.h"
#include "stm32f2xx_rcc.h"
#include "stm32f2xx_conf.h"
#include "Motor.h"
#include "LED.h"

/*************�궨��*********************************************************/
#define SYSCLK_FREQ_120MHz 120000000  //ϵͳƵ��
#define HZ_MAX	0    //���Ƶ��50Hz
#define HZ_MIN	99	 //��������СƵ��1HZ�����ǵ����ת����Ϊ��Ħ��������
#define HZ_MIDDLE  71  //30Hz

/*************�ض���*********************************************************/
extern void STM3220F_LCD_Init(void);
extern uint32_t TIM2Freq;
uint16_t Pulse=0;
uint32_t Speed_uint32;
static __IO uint32_t TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/****************************************************************************
* ������: delay_ms()
* �� ��: ��ʱ
* �� ��: ��
* �� ��: ��
****************************************************************************/
static void delay_ms(u32 nTime)			//����
{
	u32 i;
	for(i=0;i<nTime*200;i++);
}

/****************************************************************************
* ������: RCC_Configuration()
* �� ��: ����ϵͳʱ��
* �� ��: ��
* �� ��: ��
****************************************************************************/
void RCC_Configuration(void)
{
	SystemInit();

	RCC_PCLK2Config(RCC_HCLK_Div2);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC
							|RCC_AHB1Periph_GPIOD|RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG, ENABLE);
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_SYSCFG, ENABLE );
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
}

/****************************************************************************
* ������: disp_lcd_msg()
* �� ��: ��lcd��ʾ������Ϣ
* �� ��: ��
* �� ��: ��
****************************************************************************/
void disp_lcd_msg(void)
{
	char buf[0x64];
	LCD_Clear(Blue); /* ������������ɫ */
	LCD_SetBackColor(Blue); /* �������ֱ�����ɫ */
	LCD_SetTextColor(White); /* ����������ɫ */

	sprintf(buf,"/************************************/");
	LCD_DisplayStringHZ(8, 8,(uint8_t *)buf);

	sprintf(buf,"*             ֱ�����ת��ʵʱ���   *");
	LCD_DisplayStringHZ(8, 37,(uint8_t *)buf);

	sprintf(buf,"/************************************/");
	LCD_DisplayStringHZ(8, 66,(uint8_t *)buf);
	LCD_Draw_Eflag_Logo(18,24);
}

/****************************************************************************
* ������: lcd_app()
* �� ��: lcd����
* �� ��: ��
* �� ��: ��
****************************************************************************/
void lcd_app( uint8_t *adc_data1,uint8_t *temperature)
{
	#define Large_Font_Hight	64
	#define Font_16_Hight		16
	#define Large_Font_Width	32
	#define Font_16_Width		16

	#define Large_Font_Interval	16
	#define	Interval_Hight		8

	char buf[0x20];
	int start_x,start_y;
	int start_x_2;
	int x,y;

	LCD_SetBackColor(Blue);	/* �������ֱ�����ɫ */
	LCD_SetTextColor(White); /* ����������ɫ */

	start_x = 12;
	start_y = 133;

	x = start_x;
	y = start_y+12;
	sprintf(buf,"Ƶ�� ����ֵ");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x+5*Font_16_Width/2;
	y += Interval_Hight+Font_16_Hight;
	LCD_DisplayStringHZ(x, y,adc_data1);

	start_x_2 = 144;
	LCD_DrawLine(122,85,Large_Font_Hight*2+Large_Font_Interval,Vertical);

	x = start_x_2;
	y = start_y+12;
	sprintf(buf,"ת");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x_2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"��");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);		/* �������ֱ�����ɫ */
	LCD_SetTextColor(Yellow);	/* ����������ɫ */

	x = start_x_2+Font_16_Width+8;
	y = start_y;

	/* ������岿������ */
	LCD_DrawRectFill(x,y,x+Large_Font_Width*5,y+Large_Font_Hight,Blue);

	LCD_DisplayStringLargeFont(x+20, y,temperature);

	LCD_SetBackColor(Blue);	/* �������ֱ�����ɫ */
	LCD_SetTextColor(White); /* ����������ɫ */

	x = start_x_2+Large_Font_Width*4+Font_16_Width;
	y = start_y+12+Interval_Hight+Font_16_Hight;
	sprintf(buf,"R/S");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);
}


/****************************************************************************
* ������: lcd_app_void()
* �� ��: lcd����
* �� ��: ��
* �� ��: ��
****************************************************************************/
void lcd_app_void(void)
{
	#define Large_Font_Hight	64
	#define Font_16_Hight		16
	#define Large_Font_Width	32
	#define Font_16_Width		16

	#define Large_Font_Interval	16
	#define	Interval_Hight		8

	char buf[0x20];
	int start_x,start_y;
	int start_x_2;
	int x,y;

	LCD_SetBackColor(Blue);	/* �������ֱ�����ɫ */
	LCD_SetTextColor(White); /* ����������ɫ */

	start_x = 12;
	start_y = 133;

	x = start_x;
	y = start_y+12;
	sprintf(buf,"Ƶ�� ����ֵ");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x+5*Font_16_Width/2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"0x%04x",0);
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	start_x_2 = 144;
	LCD_DrawLine(122,85,Large_Font_Hight*2+Large_Font_Interval,Vertical);

	x = start_x_2;
	y = start_y+12;
	sprintf(buf,"ת");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x_2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"��");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);		/* �������ֱ�����ɫ */
	LCD_SetTextColor(Yellow);	/* ����������ɫ */

	x = start_x_2+Font_16_Width+8;
	y = start_y;

	/* ������岿������ */
	LCD_DrawRectFill(x,y,x+Large_Font_Width*5,y+Large_Font_Hight,Blue);

	sprintf(buf,"%d",0);
	LCD_DisplayStringLargeFont(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);	/* �������ֱ�����ɫ */
	LCD_SetTextColor(White); /* ����������ɫ */

	x = start_x_2+Large_Font_Width*4+Font_16_Width;
	y = start_y+12+Interval_Hight+Font_16_Hight;
	sprintf(buf,"R/S");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);
}

/****************************************************************************
* ������: NVIC_Configuration()
* �� ��:  ��ʼ���ж�������
* �� ��: ��
* �� ��: ��
****************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

   	/* ʹ��TIM2 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* ������: SPEED_TEST_INT()
* �� ��:  ��ʼ�����ٶȲ���
* �� ��: ��
* �� ��: ��
****************************************************************************/
void SPEED_TEST_INT()
{
 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	TIM_ICInitTypeDef  TIM_ICInitStructure;

	TIM_TimeBaseStructure.TIM_Period = 0xffff; //�趨��������ֵ��������ֵ��0xffff 
	TIM_TimeBaseStructure.TIM_Prescaler = 1000; //Ԥ��Ƶ1000
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;// ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����������ϼ���
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
  	  	
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	/* TIMʹ�� */
	TIM_Cmd(TIM2, ENABLE);
	
	/* ʹ��CC1�ж����� */
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
}

/****************************************************************************
* ������: main()
* �� ��: ����ģ���ϵĵ�һ��4��������Ч����Ӧ�Ĺ��ֱܷ�Ϊ������1����ͣ��������4
*		 �Թ̶�ת��30תÿ��˳ʱ��ת��������2�Ǽ��ټ�������3�Ǽ��ټ���ֱ�����
*        ��ת�ٷ�Χ��20r/s����50r/s������2�Ͱ���3ÿ��һ�����ӻ򽵵�10r/s�����ﵽ
*        20r/sʱ�ٰ�һ�¼��ٰ���3���������ת��50r/s�����ﵽ50r/sʱ�ٰ�һ�¼���
*		 ����2���������ת��20r/s�������ӻ򽵵�ת��ʱ�������ٶȵĸı䣬��ʹ���
*        �����������ת��ֵ��ͣ��˸���䣬��˳�����������2�����ʱ�ȴ�ת���ȶ���
*		 �����ӻ򽵵�ת��ʱ�����°�����ת�����ϸı䣬����Һ�����ϵ�ת�ٻ��д�
*   	 Լ2����ӳ١�
* �� ��: ��
* �� ��: ��
* ��ע��TIM3_Configuration(84); //20Hz��20r/s
*		TIM3_Configuration(75); //30Hz��30r/s
*		TIM3_Configuration(55); //40Hz��40r/s
*		TIM3_Configuration(0);  //50Hz��50r/s
****************************************************************************/
int main(void)
{
	uint8_t	Tim32[9],Speed32[3];
	unsigned char key=0;
	static int BaseNum;
	uint32_t mz=0;
	RCC_Configuration();
  	NVIC_Configuration();
	SysTick_Config(SYSCLK_FREQ_120MHz / 1000);
   
	STM3220F_LCD_Init();
	SPEED_TEST_INT();
	Motor_Init();
	LED_Init();

	InitKey();
	disp_lcd_msg();

	while(1)						
	{   
		key=GetKeyNum();
		delay_ms(1);
		
		switch(key)		
		{
			case 0x00:/*1*/
				i2cWrite_ch452(0x70,0x3F);//��ʾ0
				i2cWrite_ch452(0x72,0x3F);
				i2cWrite_ch452(0x74,0x3F);
				i2cWrite_ch452(0x76,0x3F);
				LED1_1;
				TIM3_Configuration(HZ_MIDDLE);
				Motor_Forward();
				Motor_Stop();
					Tim32[0]= '0';
					Tim32[1]= 'X';
					Tim32[2]= '0';
					Tim32[3]= '0';
					Tim32[4]= '0';
					Tim32[5]= '0';
					Tim32[6]= 'H';
					Tim32[7]= 'z';
				for(mz=0;mz<=1;mz++)
					{
						Speed32[mz]=Tim32[mz+4];
					}
				lcd_app(Tim32,Speed32);
				delay_ms(1);
				break;
			case 0x01+0x40:/*2*/ //����������0x01+0x40���ɿ���0x01
				i2cWrite_ch452(0x70,0x77);//��ʾA
				i2cWrite_ch452(0x72,0x77);
				i2cWrite_ch452(0x74,0x77);
				i2cWrite_ch452(0x76,0x77);
				LED2_1;
				Motor_Forward();
				BaseNum++;
				if(4==BaseNum)BaseNum=0;
				switch(BaseNum)
					{
						case 0:
					    	TIM3_Configuration(84); //20Hz
							break;
						case 1:
							TIM3_Configuration(75); //30Hz
							break;
						case 2:
							TIM3_Configuration(55); //40Hz
							break;
						case 3:
			   				TIM3_Configuration(0); //50Hz
							break;
					    default:
				     		break;
					}
				delay_ms(200000);
				if(TIM2Freq!=0)
					{
						Tim32[0]= '0';
						Tim32[1]= 'X';
						Tim32[2]=TIM2Freq/1000%10+0x30;
						Tim32[3]=TIM2Freq/100%10+0x30;
						Tim32[4]=TIM2Freq%100/10+0x30;
						Tim32[5]=TIM2Freq%10+0x30;
						Tim32[6]= 'H';
						Tim32[7]= 'z';
					}
				for(mz=0;mz<=1;mz++)
					{
						Speed32[mz]=Tim32[mz+4];
					}
				lcd_app(Tim32,Speed32);
				delay_ms(1);
				break;
			case 0x02+0x40: /*3*/	
				i2cWrite_ch452(0x70,0x7C); //��ʾB
				i2cWrite_ch452(0x72,0x7C);
				i2cWrite_ch452(0x74,0x7C);
				i2cWrite_ch452(0x76,0x7C);
				LED3_1;
				Motor_Forward();
				BaseNum--;
				if(0>BaseNum)BaseNum=3;
				switch(BaseNum)
					{
						case 0:
					    	TIM3_Configuration(84); //20Hz
							break;
						case 1:
							TIM3_Configuration(75); //30Hz
							break;
						case 2:
							TIM3_Configuration(55); //40Hz
							break;
						case 3:
			   				TIM3_Configuration(0); //50Hz
							break;
					    default:
				     		break;
					}
				delay_ms(200000);
				if(TIM2Freq!=0)
					{
					    Tim32[0]= '0';
						Tim32[1]= 'X';
						Tim32[2]=TIM2Freq/1000%10+0x30;
						Tim32[3]=TIM2Freq/100%10+0x30;
						Tim32[4]=TIM2Freq%100/10+0x30;
						Tim32[5]=TIM2Freq%10+0x30;
						Tim32[6]= 'H';
						Tim32[7]= 'z';
				     }
				for(mz=0;mz<=1;mz++)
					{
						Speed32[mz]=Tim32[mz+4];
					}
				lcd_app(Tim32,Speed32);
				delay_ms(1);
				break;
			case 0x03:/*4*/
				i2cWrite_ch452(0x70,0x06); //��ʾ1
				i2cWrite_ch452(0x72,0x06);
				i2cWrite_ch452(0x74,0x06);
				i2cWrite_ch452(0x76,0x06);
				LED4_1;
				BaseNum = 1;
				TIM3_Configuration(HZ_MIDDLE);
				Motor_Forward();
					Tim32[0]= '0';
					Tim32[1]= 'X';
					Tim32[2]=TIM2Freq/1000%10+0x30;
					Tim32[3]=TIM2Freq/100%10+0x30;
					Tim32[4]=TIM2Freq%100/10+0x30;
					Tim32[5]=TIM2Freq%10+0x30;
					Tim32[6]= 'H';
					Tim32[7]= 'z';
				for(mz=0;mz<=1;mz++)
					{
						Speed32[mz]=Tim32[mz+4];
					}
				lcd_app(Tim32,Speed32);
				delay_ms(1);
				break;
			default :
				     break;
		}
		if(key!=0x00)
			{
				if(TIM2Freq!=0)
					{
						Tim32[0]= '0';
						Tim32[1]= 'X';
						Tim32[2]=TIM2Freq/1000%10+0x30;
						Tim32[3]=TIM2Freq/100%10+0x30;
						Tim32[4]=TIM2Freq%100/10+0x30;
						Tim32[5]=TIM2Freq%10+0x30;
						Tim32[6]= 'H';
						Tim32[7]= 'z';
		 			}
				for(mz=0;mz<=1;mz++)
					{
						Speed32[mz]=Tim32[mz+4];
					}
				lcd_app(Tim32,Speed32);
			 }
		delay_ms(10000); //������Ӧʱ�䣬���µ�ʱ�䳤��
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
void AssertFailed(uint8_t* file, uint32_t line)
{ 
 /******�˴���������û����󱨸淽ʽ******/
  while (1)
  {}
}
#endif

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*****************/
