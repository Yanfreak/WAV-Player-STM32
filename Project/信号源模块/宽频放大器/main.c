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
/****宏定义***************************************/
#define SQUARE   0x2028	  /*方波*/
#define SIN		 0x2000	  /*正弦波*/
#define TRIANGULAR   0x2002	/*三角波*/
/******全局变量*************************************/
int gFrequence;	   /*DDS输出频率*/
unsigned short CMD9833;	/*DDS输出波形*/
int gAmplitude ;	/*幅度调节*/
/*******函数声明************************************/
static void SPI_GPIO_Config(void);
void output(void);
void tlv5616(void);
void init_DA(void);
void Delay(__IO uint32_t nCount);
/***************************************************
*函数名:main
*输入参数:无
*返回值:无
*函数说明:主函数
*
****************************************************/
int main(void)
{ 
	gFrequence=3000;	/*设置频率频率不超过3M*/
	CMD9833=SIN; /*设置波形*/
	SPI_GPIO_Config();
	output();			/*输出*/
	init_DA();	 /*初始化DA*/
	tlv5616();
	while(1)
	{
	  for(gAmplitude=0x180;gAmplitude<=0x290;gAmplitude++)
	  {
	  	  Delay(1000000);
		  tlv5616();
	  }
	  for(gAmplitude=0x290;gAmplitude>0x180;gAmplitude--)
	  {
	  	Delay(1000000);
		tlv5616();
	  }
	}
}
/************************************************************************************
*函数名: SPI_GPIO_Config
*函数说明: 初始化SPI接口及SPI配置
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
static void SPI_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;	   /*定义SPI端口初始化结构体*/

  SPI_InitTypeDef  SPI_InitStructure;

  /* 使能SPI2时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);

  /* 使能相关GPIO管脚时钟*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);  /*使能SPI GPIO时钟*/	 

  /* SPI的复用功能*/  
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);     /*配置GPIO复用到SPI时钟引脚 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2); /*配置GPIO复用到SPI输入脚*/
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		/*定义SPI GPIO模式*/
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz; /*定义SPI GPIO速度*/
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;    /*定义SPI GPIO IO模式*/
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;   


  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* SPI 配置 -------------------------------------------------------*/
  SPI_I2S_DeInit(SPI2);
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   /*双线全双工*/
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b;  /*配置SPI数据长度*/
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;	
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;	 /*SPI 软件控制*/
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;  /*SPI时钟分频，最高频率为总线的1/4，最大分频为16分频。*/
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  /*配置SPI第一个发送位，是最高位还是最低位，MSB为最高位，LSB为最低位*/
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(SPI2,&SPI_InitStructure);
  SPI_Cmd(SPI2,ENABLE);
  

  /*配置DDS片选 PC2*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  

  /*配置DA片选 PG8*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  

  /*配置DA帧同步 PF10*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_25MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOF, &GPIO_InitStructure);

}
/************************************************************************************
*函数名:Delay
*函数说明:延时函数
*输入参数:
*		nCount：延时长度
*返回值:无
*
*
**************************************************************************************/
void Delay(__IO uint32_t nCount)
{
	  unsigned int count=2*nCount;
	  while(count--)
	  {
	  }
}
/************************************************************************************
*函数名:Write9833
*函数说明:向DDS发送命令函数
*输入参数:
*		dat：命令
*返回值:无
*
*
**************************************************************************************/
void Write9833(unsigned short dat)
{
	GPIO_ResetBits(GPIOC, GPIO_Pin_2); /*片选拉低*/
	Delay(1000);
	SPI_I2S_SendData(SPI2, dat);
	Delay(1000);
	GPIO_SetBits(GPIOC, GPIO_Pin_2);    /*片选拉高*/
}

/************************************************************************************
*函数名:output
*函数说明:配置DDS的频率和波形
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
void output(void)		  
{			
	int dds;
	short int dds1,dds2;

	if(gFrequence < 0)
	{
		gFrequence = 0;
	}

	if(gFrequence > 12000000)//12M
	{
		gFrequence = 12000000;
		gFrequence = 12000000;
	}

	dds=gFrequence*10.73741824;
	dds=dds<<2;
	dds1=dds;
	dds2=dds>>16;
	dds1=dds1>>2;
	dds2=dds2&0x7FFF;
	dds2=dds2|0x4000;
	dds1=dds1&0x7FFF;
	dds1=dds1|0x4000;
	Write9833(CMD9833|0x100);
	Write9833(dds1);
	Write9833(dds2);
	Write9833(0xc000);
	Write9833(CMD9833);
}

/************************************************************************************
*函数名: tlv5616
*函数说明:DA输出控制函数
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
 void tlv5616(void)
{	
	if(gAmplitude > 0x300)
		gAmplitude = 0x300;

	if(gAmplitude <= 0x00)
		gAmplitude = 0x0;

	GPIO_ResetBits(GPIOG, GPIO_Pin_8);  /*片选拉低*/
	Delay(1000);
	GPIO_ResetBits(GPIOF, GPIO_Pin_10); 
	Delay(1000);
	SPI_I2S_SendData(SPI2,(0x4000|gAmplitude));/*发送数据*/
	Delay(1000);
	GPIO_SetBits(GPIOF, GPIO_Pin_10);  
	Delay(1000);
	GPIO_SetBits(GPIOG, GPIO_Pin_8); /*片选拉高*/

}
/************************************************************************************
*函数名: init_DA
*函数说明: 初始化DA
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
void init_DA(void)
{
	GPIO_SetBits(GPIOG, GPIO_Pin_8); /*片选拉高*/
	GPIO_SetBits(GPIOF, GPIO_Pin_10);  /*帧同步信号拉高*/
	gAmplitude = 0x180;				  
	tlv5616();

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
