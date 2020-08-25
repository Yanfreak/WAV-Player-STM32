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
#include "stm32_eval.h"
#include "stm322xg_eval_lcd.h"
#include "stdio.h"
/************************************************************************************
*函数名: DelayMs
*函数说明:延时函数
*输入参数:
*		 nCount：延时长度
*返回值:无
*
*
**************************************************************************************/
void DelayMs(uint32_t nCount)
{
	int i=0;
	for(i=0;i<nCount*10000;i++);
}
/************************************************************************************
*函数名:GPIO_Configuration
*函数说明:SPI对应管脚以及片选管脚初始化
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	/*************使能管脚时钟******************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOG,ENABLE);
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_UP;	 
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	/**************将管脚复用到SPI2*****************************/
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource14,GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2);
	/**************配置片选管脚*****************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_UP; 
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	/************配置帧同步管脚*******************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType =GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_DOWN; 	
	GPIO_Init(GPIOF,&GPIO_InitStructure);	
}
/************************************************************************************
*函数名:SPI_Configuration
*函数说明:SPI参数配置
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
static void SPI_Configuration(void)
{
	SPI_InitTypeDef   SPI_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);/*使能SPI2时钟*/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; /*SPI主模式*/
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;/*数据长度16位*/
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;	/*空闲时极性*/
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;/*数据采样沿选择*/
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;	/*时钟分频*/
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; /*数据高位在前*/
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2,&SPI_InitStructure);

	SPI_Cmd(SPI2,ENABLE);/*使能SPI2*/
}
/************************************************************************************
*函数名: tlv5616
*函数说明:TLV5616发送命令函数
*输入参数:
*		 DAT：命令码
*返回值:无
*
*
**************************************************************************************/
static void tlv5616(u16 DAT)
{	

	GPIO_ResetBits(GPIOG, GPIO_Pin_8); /*片选拉低*/
	DelayMs(5);
	GPIO_ResetBits(GPIOF, GPIO_Pin_10); /*帧同步拉低*/
	DelayMs(3);
	SPI_I2S_SendData(SPI2,DAT);	/*SPI发送数据*/
	DelayMs(3);
	GPIO_SetBits(GPIOF, GPIO_Pin_10);  /*片选拉高*/
	DelayMs(5);
	GPIO_SetBits(GPIOG, GPIO_Pin_8);  /*帧同步拉高*/

}
/************************************************************************************
*函数名:Breath_interface
*函数说明:界面显示程序
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
static void Breath_interface(void)
{
	char temp[32];
	LCD_Clear(Blue);
	LCD_DrawRect(0,0,239,319);
	LCD_DrawLine(0,20,320,Horizontal);
	sprintf(temp,"呼吸灯测试");
	LCD_DisplayStringHZ(108,4,(uint8_t *)temp);	
	sprintf(temp,"DA输入值:");
	LCD_DisplayStringHZ(50,50,(uint8_t *)temp);
	LCD_DrawLine(0,220,320,Horizontal);
	sprintf(temp,"北京亿旗创新科技发展有限公司");
	LCD_DisplayStringHZ(60,222,(uint8_t *)temp);
}

/************************************************************************************
*函数名:main
*函数说明:主程序
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
int main(void)
{ 
	int i;
	char temp[32]; /*字符缓冲区*/
	GPIO_Configuration();/*GPIO管脚配置*/
	SPI_Configuration();/*SPI配置*/
	STM3220F_LCD_Init();/*LCD初始化*/
	LCD_SetBackColor(Blue);/*设置背景为蓝色*/
	LCD_SetTextColor(Yellow);/*设置前景色为黄色*/
	GPIO_SetBits(GPIOG, GPIO_Pin_8);/*片选拉高*/  
	GPIO_SetBits(GPIOF, GPIO_Pin_10); /*帧同步拉高*/
	Breath_interface();
	while(1)
	{
		for(i = 0x600 ; i < 0xc00 ; i++)
		{
			tlv5616(0x4000|i); 
			DelayMs(3);				  /*DA写入值逐渐增大*/
			sprintf(temp,"%d",i);
			LCD_DisplayStringLargeFont(100,100,(uint8_t *)temp);
		}
		for(i = 0xc00 ; i >= 0x700 ; i--)
		{
			tlv5616(0x4000|i); 
			DelayMs(3);			  /*写入值逐渐减小*/
			sprintf(temp,"%d",i);
			LCD_DisplayStringLargeFont(100,100,(uint8_t *)temp);
		}
		DelayMs(1000);
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


/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
