/****************************************************************************
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
******************************************************************************/

/*************头文件**********************************************************/
#include "main.h"
#include "stm322xg_eval_lcd.h"
#include "stm32f2xx.h"
#include "stm32f2xx_exti.h"
#include "mouse.h"
#include "ps2.h" 
#include "stm32_eval.h"
#include "stm322xg_eval.h"
#include "stm322xg_eval_systick.h"

/*************宏定义**********************************************************/
#define Large_Font_Hight	64
#define Font_16_Hight		16
#define Large_Font_Width	32
#define Font_16_Width		16
#define Large_Font_Interval	16
#define	Interval_Hight		8
#define SYSCLK_FREQ_120MHz 120000000

/*************外部函数声明****************************************************/
extern void STM3220F_LCD_Init(void);

/*************函数声明**********************************************************/
static void Delay(u32 nTime);
static void RCC_Configuration(void);
static void NVIC_Configuration(void);
static void disp_lcd_msg(void);
static void disp_lcd_welcome(void);
static void Delay(u32 nTime);
static void lcd_app(int8_t mouse_x,int8_t mouse_y);

/******************************************************************************
* 函数名：main		 
* 功  能：鼠标测试,	x、y值显示的是动作幅度变化的快慢,即鼠标移动速率。X代表横坐标,
*         Y代表纵坐标，当鼠标向左移动时，X值根据鼠标移动速率从0到127（2的7方）变
*         化，当鼠标向右移动时，X值根据鼠标移动速率从0到-127变化。当鼠标向下移动
*         时，Y值根据鼠标移动速率从0到127变化,当鼠标向上移动时,Y值根据鼠标移动速
*         率从0到-127变化。 
* 输  入：无
* 输  出：无  
* 备  注：鼠标移动的越快，数字越大。
********************************************************************************/
int main(void)
{
	char buf1[0x64];					 
	uint8_t  t=0;
	int8_t mouse_x[2],mouse_y[2];
	uint32_t errcnt=0;
	SysTick_Config(SYSCLK_FREQ_120MHz / 1000);
	RCC_Configuration();
//	NVIC_Configuration();
	STM3220F_LCD_Init(); //初始化液晶屏

	disp_lcd_welcome();	 //欢迎界面显示
	Delay(200000);
  	disp_lcd_msg();	  //测试界面显示

RST:t=Init_Mouse();
	Delay(1);
	if(t==0)
	{
		sprintf(buf1,"鼠标ok !");
		LCD_DisplayStringHZ(125, 150,(uint8_t *)buf1);
		Delay(100000);
	}
	else
	{
		sprintf(buf1,"鼠标错误!");
		LCD_DisplayStringHZ(125, 150,(uint8_t *)buf1);
		Delay(100000);
		goto RST;
	}

	sprintf(buf1,"鼠标ID=0");
	LCD_DisplayStringHZ(125, 150,(uint8_t *)buf1);
	Delay(100000);

	if(MOUSE_ID==3)
	{
		sprintf(buf1,"鼠标按键=3");
		LCD_DisplayStringHZ(125, 150,(uint8_t *)buf1);
		Delay(100000);
	}

	LCD_Clear(Blue); //清屏，背景蓝色
	disp_lcd_msg();  //测试界面显示
	lcd_app(0,0);

	while(1)
	{		
		if(PS2_Status&0x80)//得到了一次数据
			{	
				Mouse_Data_Pro();//处理数据
			
				mouse_x[0]=MouseX.x_pos;//鼠标水平移动速率
				mouse_y[0]=MouseX.y_pos;//鼠标竖直移动速率

				lcd_app(mouse_x[0],mouse_y[0]);	 //显示

				if(MouseX.bt_mask&0x01)
					{
						sprintf(buf1,"您点击的鼠标左键");
						LCD_DisplayStringHZ(145,157,(uint8_t *)buf1);
					}
				if(MouseX.bt_mask&0x02)
					{
						sprintf(buf1,"您点击的鼠标右键");
						LCD_DisplayStringHZ(145, 157,(uint8_t *)buf1);
					}	    
				if(MouseX.bt_mask&0x04)
					{
						sprintf(buf1,"您点击的鼠标滑轮");
						LCD_DisplayStringHZ(145, 157,(uint8_t *)buf1);
					} 		 
				PS2_Status=MOUSE;
				PS2_En_Data_Report();//使能数据报告
			}
		else if(PS2_Status&0x40)
			{
				errcnt++;
				PS2_Status=MOUSE;
			}
		t++;
		Delay(100);
		if(t==200)t=0;
	} 
}

/******************************************************************************
* 函数名：Delay
* 功  能：延迟函数
* 输  入：延迟时间
* 输  出：无  
* 备  注：无
********************************************************************************/
static void Delay(u32 nTime)			
{
	int i;
	for(i=0;i<nTime*200;i++);
}

/****************************************************************************
* 函数名: RCC_Configuration()
* 功 能: 设置系统时钟
* 输 入: 无
* 输 出: 无
****************************************************************************/
static void RCC_Configuration(void)
{
	/*在此阶段处理器的时钟已经被初始化完成，时钟初始化工作由SystemInit函数实现，SystemInit函数存放在startup_stm32f2xx.s的汇编启动文件中。
    *在系统上电初始化到跳转到main函数之前，启动代码会先调用SystemInit函数。	
    *如果用户希望重新配置时钟可以在此处再次调用 SystemInit()，它的函数实现存放在system_stm32f2xx.c文件中。*/
	SystemInit();

	/* 时钟使能 */
   	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA |RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC
		|RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE );
}

/****************************************************************************
* 函数名: NVIC_Configuration()
* 功 能:  初始化中断向量表
* 输 入: 无
* 输 出: 无
****************************************************************************/
static void NVIC_Configuration(void)
{
	/* 配置NVIC抢占优先级比特 */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置优先级分组：先占优先级2级,从优先级2级
	
	//设置向量表的位置和偏移
	#ifdef  VECT_TAB_RAM  
		
		/* 设置向量表起始 0x20000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);//向量表位于RAM

	#else 					   
		/* 设置向量表起始 0x08000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//向量表位于FLASH
	#endif	
}

/****************************************************************************
* 函数名: disp_lcd_welcome()
* 功 能: 在lcd显示帮助消息
* 输 入: 无
* 输 出: 无
****************************************************************************/
void disp_lcd_welcome(void)
{
	char buf[0x64];
	LCD_Clear(Blue); /* 清屏，背景蓝色 */
	LCD_SetBackColor(Blue); /* 设置文字背景颜色 */
	LCD_SetTextColor(White); /* 设置文字颜色 */

	sprintf(buf,"/************************************/");
	LCD_DisplayStringHZ(8, 16,(uint8_t *)buf);

	sprintf(buf,"欢迎进行鼠标测试");
	LCD_DisplayStringHZ(100, 150,(uint8_t *)buf);

	sprintf(buf,"/************************************/");
	LCD_DisplayStringHZ(8, 210,(uint8_t *)buf);

	LCD_Draw_Eflag_Logo(120,60);
}

/****************************************************************************
* 函数名: disp_lcd_msg()
* 功 能: 在lcd显示帮助消息
* 输 入: 无
* 输 出: 无
****************************************************************************/
static void disp_lcd_msg(void)
{
	char buf[0x64];
	LCD_Clear(Blue); /* 清屏，背景蓝色 */
	LCD_SetBackColor(Blue); /* 设置文字背景颜色 */
	LCD_SetTextColor(White); /* 设置文字颜色 */

	sprintf(buf,"/************************************/");
	LCD_DisplayStringHZ(8, 8,(uint8_t *)buf);

	sprintf(buf,"*                  鼠标测试          *");
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
static void lcd_app(int8_t mouse_x,int8_t mouse_y)
{
	char buf[0x20];
	int start_x,start_y;
	int start_x_2;
	int x,y;

	LCD_SetBackColor(Blue);	/* 设置文字背景颜色 */
	LCD_SetTextColor(White); /* 设置文字颜色 */

	start_x = 12;
	start_y = 93;

	x = start_x;
	y = start_y+12;
	sprintf(buf,"32位采样值");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x+2*Font_16_Width/2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"0x%08x",mouse_x); //以8位16进制打印，不足位补0，大于或等于32位按照本身的长度显示
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	start_x_2 = 144;
	LCD_DrawLine(122,start_y,Large_Font_Hight*2+Large_Font_Interval,Vertical);

	x = start_x_2;
	y = start_y+12;
	sprintf(buf,"X");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x_2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"值");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);		/* 设置文字背景颜色 */
	LCD_SetTextColor(Yellow);	/* 设置文字颜色 */

	x = start_x_2+Font_16_Width+8;
	y = start_y;

	/* 清大字体部分文字 */
	LCD_DrawRectFill(x,y,x+Large_Font_Width*5,y+Large_Font_Hight,Blue);

	sprintf(buf,"%d",mouse_x);
	LCD_DisplayStringLargeFont(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);	/* 设置文字背景颜色 */
	LCD_SetTextColor(White); /* 设置文字颜色 */

	start_y += Large_Font_Hight+Large_Font_Interval;

	x = start_x;
	y = start_y+12;
	sprintf(buf,"32位采样值");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x+2*Font_16_Width/2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"0x%08x",mouse_y);//以8位16进制打印，不足位补0，大于或等于32位按照本身的长度显示
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x_2;
	y = start_y+12;
	sprintf(buf,"Y");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x_2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"值");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);	/* 设置文字背景颜色 */
	LCD_SetTextColor(Yellow); /* 设置文字颜色 */

	x = start_x_2+Font_16_Width+8;
	y = start_y;

	/* 清大字体部分文字 */
	LCD_DrawRectFill(x,y,x+Large_Font_Width*5,y+Large_Font_Hight,Blue);

	sprintf(buf,"%d",mouse_y);
	LCD_DisplayStringLargeFont(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);	/* 设置文字背景颜色 */
	LCD_SetTextColor(White); /* 设置文字颜色 */
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
