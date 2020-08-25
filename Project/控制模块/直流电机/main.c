/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： 直流电机
*   摘 要    ： 
*
*   当前版本 ： 
*   作 者    ： EFLAG
*   完成日期 ： 
*
*   取代版本 ：
*   原作者   ：郭伟 
*   修改内容 ：
*   完成日期 ：2012.08.01 
*
******************************************************************************/

/*************头文件**********************************************************/
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

/*************宏定义*********************************************************/
#define SYSCLK_FREQ_120MHz 120000000  //系统频率
#define HZ_MAX	0    //最大频率50Hz
#define HZ_MIN	99	 //理论上最小频率1HZ，但是电机不转，因为有摩擦阻力等
#define HZ_MIDDLE  71  //30Hz

/*************重定义*********************************************************/
extern void STM3220F_LCD_Init(void);
extern uint32_t TIM2Freq;
uint16_t Pulse=0;
uint32_t Speed_uint32;
static __IO uint32_t TimingDelay;
RCC_ClocksTypeDef RCC_Clocks;

/****************************************************************************
* 函数名: delay_ms()
* 功 能: 延时
* 输 入: 无
* 输 出: 无
****************************************************************************/
static void delay_ms(u32 nTime)			//毫秒
{
	u32 i;
	for(i=0;i<nTime*200;i++);
}

/****************************************************************************
* 函数名: RCC_Configuration()
* 功 能: 设置系统时钟
* 输 入: 无
* 输 出: 无
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
* 函数名: disp_lcd_msg()
* 功 能: 在lcd显示帮助消息
* 输 入: 无
* 输 出: 无
****************************************************************************/
void disp_lcd_msg(void)
{
	char buf[0x64];
	LCD_Clear(Blue); /* 清屏，背景蓝色 */
	LCD_SetBackColor(Blue); /* 设置文字背景颜色 */
	LCD_SetTextColor(White); /* 设置文字颜色 */

	sprintf(buf,"/************************************/");
	LCD_DisplayStringHZ(8, 8,(uint8_t *)buf);

	sprintf(buf,"*             直流电机转速实时监测   *");
	LCD_DisplayStringHZ(8, 37,(uint8_t *)buf);

	sprintf(buf,"/************************************/");
	LCD_DisplayStringHZ(8, 66,(uint8_t *)buf);
	LCD_Draw_Eflag_Logo(18,24);
}

/****************************************************************************
* 函数名: lcd_app()
* 功 能: lcd功能
* 输 入: 无
* 输 出: 无
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

	LCD_SetBackColor(Blue);	/* 设置文字背景颜色 */
	LCD_SetTextColor(White); /* 设置文字颜色 */

	start_x = 12;
	start_y = 133;

	x = start_x;
	y = start_y+12;
	sprintf(buf,"频率 采样值");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x+5*Font_16_Width/2;
	y += Interval_Hight+Font_16_Hight;
	LCD_DisplayStringHZ(x, y,adc_data1);

	start_x_2 = 144;
	LCD_DrawLine(122,85,Large_Font_Hight*2+Large_Font_Interval,Vertical);

	x = start_x_2;
	y = start_y+12;
	sprintf(buf,"转");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x_2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"速");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);		/* 设置文字背景颜色 */
	LCD_SetTextColor(Yellow);	/* 设置文字颜色 */

	x = start_x_2+Font_16_Width+8;
	y = start_y;

	/* 清大字体部分文字 */
	LCD_DrawRectFill(x,y,x+Large_Font_Width*5,y+Large_Font_Hight,Blue);

	LCD_DisplayStringLargeFont(x+20, y,temperature);

	LCD_SetBackColor(Blue);	/* 设置文字背景颜色 */
	LCD_SetTextColor(White); /* 设置文字颜色 */

	x = start_x_2+Large_Font_Width*4+Font_16_Width;
	y = start_y+12+Interval_Hight+Font_16_Hight;
	sprintf(buf,"R/S");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);
}


/****************************************************************************
* 函数名: lcd_app_void()
* 功 能: lcd功能
* 输 入: 无
* 输 出: 无
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

	LCD_SetBackColor(Blue);	/* 设置文字背景颜色 */
	LCD_SetTextColor(White); /* 设置文字颜色 */

	start_x = 12;
	start_y = 133;

	x = start_x;
	y = start_y+12;
	sprintf(buf,"频率 采样值");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x+5*Font_16_Width/2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"0x%04x",0);
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	start_x_2 = 144;
	LCD_DrawLine(122,85,Large_Font_Hight*2+Large_Font_Interval,Vertical);

	x = start_x_2;
	y = start_y+12;
	sprintf(buf,"转");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x_2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"速");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);		/* 设置文字背景颜色 */
	LCD_SetTextColor(Yellow);	/* 设置文字颜色 */

	x = start_x_2+Font_16_Width+8;
	y = start_y;

	/* 清大字体部分文字 */
	LCD_DrawRectFill(x,y,x+Large_Font_Width*5,y+Large_Font_Hight,Blue);

	sprintf(buf,"%d",0);
	LCD_DisplayStringLargeFont(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);	/* 设置文字背景颜色 */
	LCD_SetTextColor(White); /* 设置文字颜色 */

	x = start_x_2+Large_Font_Width*4+Font_16_Width;
	y = start_y+12+Interval_Hight+Font_16_Hight;
	sprintf(buf,"R/S");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);
}

/****************************************************************************
* 函数名: NVIC_Configuration()
* 功 能:  初始化中断向量表
* 输 入: 无
* 输 出: 无
****************************************************************************/
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);  
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

   	/* 使能TIM2 */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/****************************************************************************
* 函数名: SPEED_TEST_INT()
* 功 能:  初始化中速度测试
* 输 入: 无
* 输 出: 无
****************************************************************************/
void SPEED_TEST_INT()
{
 	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	TIM_ICInitTypeDef  TIM_ICInitStructure;

	TIM_TimeBaseStructure.TIM_Period = 0xffff; //设定的最大计数值，最大计数值是0xffff 
	TIM_TimeBaseStructure.TIM_Prescaler = 1000; //预分频1000
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;// 时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数方向向上计数
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
  	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
  	  	
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	/* TIM使能 */
	TIM_Cmd(TIM2, ENABLE);
	
	/* 使能CC1中断请求 */
	TIM_ITConfig(TIM2, TIM_IT_CC1, ENABLE);
}

/****************************************************************************
* 函数名: main()
* 功 能: 键盘模块上的第一排4个按键有效，对应的功能分别为：按键1是暂停键，按键4
*		 以固定转速30转每秒顺时针转动。按键2是加速键，按键3是减速减。直流电机
*        的转速范围是20r/s——50r/s，按键2和按键3每按一下增加或降低10r/s。当达到
*        20r/s时再按一下减速按键3会跳到最高转速50r/s。当达到50r/s时再按一下加速
*		 按键2会跳到最低转速20r/s。当增加或降低转速时，由于速度的改变，会使光电
*        传感器测出的转速值不停闪烁跳变，因此程序中增加了2秒的延时等待转速稳定。
*		 当增加或降低转速时，按下按键后转速马上改变，但是液晶屏上的转速会有大
*   	 约2秒的延迟。
* 输 入: 无
* 输 出: 无
* 备注：TIM3_Configuration(84); //20Hz，20r/s
*		TIM3_Configuration(75); //30Hz，30r/s
*		TIM3_Configuration(55); //40Hz，40r/s
*		TIM3_Configuration(0);  //50Hz，50r/s
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
				i2cWrite_ch452(0x70,0x3F);//显示0
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
			case 0x01+0x40:/*2*/ //按键按下是0x01+0x40，松开是0x01
				i2cWrite_ch452(0x70,0x77);//显示A
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
				i2cWrite_ch452(0x70,0x7C); //显示B
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
				i2cWrite_ch452(0x70,0x06); //显示1
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
		delay_ms(10000); //按键响应时间，按下的时间长短
	} 
}

#ifdef  USE_FULL_ASSERT
/******************************************************************************
* 函数名：AssertFailed
* 功  能：报告参数错误的位置
* 输  入：错误所在的文件名和错误所在文件中的行号
* 输  出：无  
* 备  注：无
********************************************************************************/
void AssertFailed(uint8_t* file, uint32_t line)
{ 
 /******此处可以添加用户错误报告方式******/
  while (1)
  {}
}
#endif

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*****************/
