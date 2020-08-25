/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： sessor.c
*   摘 要    ： 温度、湿度传感器实验主程序
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
#include "stm322xg_eval_lcd.h"
#include "sensor.h"

/*************宏定义**********************************************************/

/*************各通道16位采样命令**********************************************/
#define AIN0                    0X0C00
#define AIN1                    0X1C00
#define AIN2                    0X2C00
#define AIN3                    0X3C00
#define AIN4                    0X4C00
#define AIN5                    0X5C00
#define AIN6                    0X6C00
#define AIN7                    0X7C00
#define AIN8                    0X8C00
#define AIN9                    0X9C00
#define AIN10                   0XAC00

/*************自检模式采样命令**********************************************/
#define VrefH_L_DIV2            0XBC00		/* 内部 (VrefH-VrefL)/2 */
#define VrefL	                0XCC00		/* 内部 内部 VrefL */
#define VrefH    	            0XDC00		/* 内部 VrefH */

/*************PowerDown命令*************************************************/
#define AD_POWERDOWN            0XEC00

/*************通道数义******************************************************/
#define ADC_CHANNEL_CNT			15

/*************ADC Vref+*****************************************************/
#define ADC_Vref_POS			2.5f

/*************ADC Vref-*****************************************************/
#define ADC_Vref_NEG			0

/*************ADC 变比******************************************************/
#define ADC_SCALE_FACT			(ADC_Vref_POS-ADC_Vref_NEG)/(1<<12)

/*************湿度传感器输出信号放大倍数************************************/
#define ADC_HUMIDITY_AMP_FACT	1.0f

/*************传感器电源电压************************************************/
#define ADC_SENSOR_VCC			5.0f

/*************函数声明********************************************************/
u16 tlv2543_read_data(u16 cmd);
void tlv2543_init(void);
void disp_lcd_msg(void);
void lcd_app(u16 adc_data2,float humidity);
void Senssor_RCC_Configuration(void);
void Senssor_GPIO_Configuration(void);
void Senssor_SPI_Configuration(void);
void Senssor_delay(__IO uint32_t nCount);
void Senssor_delay_ms(int z);
int Sessor(void);
extern void STM3220F_LCD_Init(void);

/****************************************************************************
* 函数名: Sessor()
* 功 能: ADC采样主函数
* 输 入: 无
* 输 出: 无
****************************************************************************/
int Sessor(void)
{
	float humidity = 0;
	float volt;
	uint16_t adc_data2 = 0;

	Senssor_RCC_Configuration();
	Senssor_GPIO_Configuration();
	Senssor_SPI_Configuration();

	tlv2543_init();	/* 初始化ADC */
	SensorDataInit(); /* 初始化Sensor Data */
	STM3220F_LCD_Init(); /* LCD初始化 */
	disp_lcd_msg();/* 显示LCD信息 */

	while(1)
	{
		/* 获得测量值，读取adc 通道1, 16bit */
		adc_data2 = tlv2543_read_data(AIN1);

		/* 获得湿度 */
		volt = adc_data2*ADC_SCALE_FACT/ADC_HUMIDITY_AMP_FACT;
		humidity = HumiditySensor(volt); /* 获得湿度 */

		/* 显示 */
		lcd_app(adc_data2,humidity);
		Senssor_delay_ms(2000);
	}
}

/****************************************************************************
* 函数名: tlv2543_read_data()
* 功 能: 获得ADC数据
* 输 入: ADC命令
* 输 出: 采样值
****************************************************************************/
u16 tlv2543_read_data(u16 cmd)
{
	u16 data;

	/* 等待spi tx为空 */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	GPIO_ResetBits(GPIOC, GPIO_Pin_3); /*  ADC CS = 0 */
	Senssor_delay(50);
	SPI_I2S_SendData(SPI2,cmd);	/* 发送ADC命令 */

	/* 等待spi rx不为空 */
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);
	SPI_I2S_ReceiveData(SPI2); /* 读取adc数据 */
	
	Senssor_delay(50); /* 等待adc变低 */
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7)==RESET); /* 等待adc转换完成 */

	/* 开始发送和读取adc数据 */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI2,cmd);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);
	data = SPI_I2S_ReceiveData(SPI2);
	GPIO_SetBits(GPIOC, GPIO_Pin_3); /* ADC CS = 1 */

	Senssor_delay(50); /* 等待adc变低 */
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7)==RESET); /* 等待adc转换完成 */
	data=data>>4;
	return data;
}

/****************************************************************************
* 函数名: tlv2543_init()
* 功 能: 初始化ADC
* 输 入: 无
* 输 出: 无
****************************************************************************/
void tlv2543_init(void)
{
	/* 写0x0000,复位ADC */
	tlv2543_read_data(0x0);
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

	sprintf(buf,"*                   湿度实时监测     *");
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
void lcd_app(u16 adc_data2,float humidity)
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
	sprintf(buf,"ADC0 采样值");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x+5*Font_16_Width/2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"0x%04x",adc_data2);
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	start_x_2 = 144;
	LCD_DrawLine(122,85,Large_Font_Hight*2+Large_Font_Interval,Vertical);

	x = start_x_2;
	y = start_y+12;
	sprintf(buf,"湿");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x_2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"度");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);		/* 设置文字背景颜色 */
	LCD_SetTextColor(Yellow);	/* 设置文字颜色 */

	x = start_x_2+Font_16_Width+8;
	y = start_y;

	/* 清大字体部分文字 */
	LCD_DrawRectFill(x,y,x+Large_Font_Width*5,y+Large_Font_Hight,Blue);

	sprintf(buf,"%4.1f",humidity);
	LCD_DisplayStringLargeFont(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);	/* 设置文字背景颜色 */
	LCD_SetTextColor(White); /* 设置文字颜色 */

	if(strlen(buf)<=4)	/* 温度数字小于等于4字节长才显示单位 */
	{
		x = start_x_2+Large_Font_Width*4+Font_16_Width;
		y = start_y+12+Interval_Hight+Font_16_Hight;
		sprintf(buf," %%");
		LCD_DisplayStringHZ(x, y,(uint8_t *)buf);
	}
}

/****************************************************************************
* 函数名: Senssor_RCC_Configuration()
* 功 能: 配置不同的系统时钟
* 输 入: 无 
* 输 出: 无
****************************************************************************/
void Senssor_RCC_Configuration(void)
{
 	/*在此阶段处理器的时钟已经被初始化完成，时钟初始化工作由SystemInit函数实现，SystemInit函数存放在startup_stm32f2xx.s的汇编启动文件中。
    *在系统上电初始化到跳转到main函数之前，启动代码会先调用SystemInit函数。	
    *如果用户希望重新配置时钟可以在此处再次调用 SystemInit()，它的函数实现存放在system_stm32f2xx.c文件中。*/
    SystemInit();

    RCC_PCLK2Config(RCC_HCLK_Div2); /* PCLK2 = HCLK/2 */

	/* 使能时钟 */
    RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB
                         | RCC_APB1Periph_SPI2, ENABLE);
}

/****************************************************************************
* 函数名: Senssor_GPIO_Configuration()
* 功 能: 配置不同的GPIO端口
* 输 入: 无 
* 输 出: 无
****************************************************************************/
void Senssor_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);

	/* DA_CS 推免输出 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	/* AD_EOC开漏输出 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	/* DDS_CS、AD_CS推免输出 */ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	/* DA_FS推免输出 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	GPIO_SetBits(GPIOF, GPIO_Pin_10);
}

/****************************************************************************
* 函数名: Senssor_SPI_Configuration()
* 功 能: 配置SPI2
* 输 入: 无 
* 输 出: 无
****************************************************************************/
void Senssor_SPI_Configuration(void)
{
	SPI_InitTypeDef   SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* 使能SPI时钟 */
	SPI_DeInit(SPI2);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* 使能GPIO时钟 */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/* 使用SPI的复用功能*/  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	
	/* 配置SPI SCK管脚 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* 配置SPI MOSI管脚 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* 配置SPI MISO管脚 */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOG,GPIO_Pin_8); /* DA  CS PG8=1, 拉高电压 */
	GPIO_SetBits(GPIOC,GPIO_Pin_2); /* DDS CS PC2=1，拉高电压 */
	GPIO_SetBits(GPIOC,GPIO_Pin_3); /* AD  CS PC4=1，拉高电压 */

	/* 配置SPI */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; /* 全双工模式 */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; /* 主模式 */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; /* 16位数据线 */
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; /* SCK常低 */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; /* SCK第一个沿采样 */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; /* 软件控制NSS */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	/* 64倍预分频 */
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; /* 字节顺序为MSB在前	*/
	SPI_Init(SPI2,&SPI_InitStructure); /* 初始化SPI2 */

	SPI_Cmd(SPI2,ENABLE);
}

/****************************************************************************
* 函数名: Senssor_delay()
* 功 能: 短暂延时
* 输 入: 延时值 
* 输 出: 无
****************************************************************************/
void Senssor_delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}

/****************************************************************************
* 函数名: Senssor_delay_ms()
* 功 能: 长时延迟(大约0.72ms)
* 输 入: 延时值 
* 输 出: 无
****************************************************************************/
void Senssor_delay_ms(int z)
{
	int j,k;
	for(j=0;j<z;j++)
	{
		for(k=0;k<10000;k++);
	}
}

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束**************/
