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
	for(i=0;i<nCount*1000;i++);
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

	/**************GPIO 时钟使能*****************************/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);

	/**************AD转换完成信号管脚*****************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 	//浮空输入
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;//无推挽
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	/*****************AD片选管脚**************************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
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

	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_DeInit(SPI2);
	/*************SPI2时钟使能******************************/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    /************SPI对应管脚配置*******************************/
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	
	/*SPI SCK 管脚配置 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* SPI MOSI 管脚配置*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* SPI MISO 管脚配置 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOC,GPIO_Pin_3);  //AD片选拉高

	/***********SPI配置********************************/
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;		//全双工模式
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;							//主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;						//16位数据线
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;								//SCK常低
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;							//SCK第一个沿采样
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;								//软件控制NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;		//64倍预分频
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	//字节顺序为MSB在前
	SPI_InitStructure.SPI_CRCPolynomial=1;
	SPI_Init(SPI2,&SPI_InitStructure);										//初始化SPI2

	/*********SPI2使能**********************************/
	SPI_Cmd(SPI2,ENABLE);
}
/************************************************************************************
*函数名:AD_read_data
*函数说明:读取AD转换器的转换值
*输入参数:
*		 cmd:读取转换通道号
*返回值:无
*
*
**************************************************************************************/
 u16 AD_read_data(u16 cmd)
{
	u16 data;

	/* 等待SPI发送寄存器为空-------------------------------------------------*/
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

	/* 拉低片选--------------------------------------------------------*/
	GPIO_ResetBits(GPIOC, GPIO_Pin_3);

	/* 等待建立时间 -----------------------------------------------*/
	DelayMs(5);

	/*发送读取通道号------------------------------------------------------*/
	SPI_I2S_SendData(SPI2,cmd);

	/* 等待接收寄存器为非空 ---------------------------------------------*/
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);

	/* 读取数据 -----------------------------------------------------*/
	SPI_I2S_ReceiveData(SPI2);

	/* 等待建立时间 -----------------------------------------------*/
	DelayMs(5);

	/* 等待ADC转换完成信号------------------------------------------*/
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7)==RESET);

	/* 再次发送读取通道号读取数据 -----------------------------------------*/
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

	SPI_I2S_SendData(SPI2,cmd);

	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);

	data = SPI_I2S_ReceiveData(SPI2);

	/* 片选拉高 --------------------------------------------------------*/
	GPIO_SetBits(GPIOC, GPIO_Pin_3);

	DelayMs(5);
	/*等待下次转换完成------------------------------------------*/
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7)==RESET);

	data=data>>4;

	return data;
}
/************************************************************************************
*函数名:AD_interface
*函数说明:界面显示程序
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
static void AD_interface(void)
{
	char temp[32];
	LCD_Clear(Blue);
	LCD_DrawRect(0,0,239,319);
	LCD_DrawLine(0,20,320,Horizontal);
	sprintf(temp,"电压采集测试");
	LCD_DisplayStringHZ(108,4,(uint8_t *)temp);	
	sprintf(temp,"电压采集值:");
	LCD_DisplayStringHZ(50,50,(uint8_t *)temp);
	LCD_DisplayStringHZ(240,150,"V");
	LCD_DrawLine(0,220,320,Horizontal);
	sprintf(temp,"北京亿旗创新科技发展有限公司");
	LCD_DisplayStringHZ(60,222,(uint8_t *)temp);	
}

/************************************************************************************
*函数名:main
*函数说明:主程序 外部AD采样函数
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
int main(void)
{ 
	u16 adc_data;
	float volat; 
	char temp[32];
	GPIO_Configuration();
	SPI_Configuration();
	STM3220F_LCD_Init();
	LCD_SetBackColor(Blue);
	LCD_SetTextColor(Yellow);
	AD_interface();
	while(1)
	{
		adc_data = AD_read_data(0x8c00);
		volat=(float)adc_data/4096*2.5;
		sprintf(temp,"%4.1f",volat);
		LCD_DisplayStringLargeFont(100,100,(uint8_t *)temp);
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
