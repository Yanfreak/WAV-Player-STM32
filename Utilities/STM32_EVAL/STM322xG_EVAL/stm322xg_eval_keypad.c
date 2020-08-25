 /****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： stm322xg_eval_keypad.c
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
#include "stm32f2xx.h"
#include "stm322xg_eval_keypad.h"
#include <stdio.h>
 u8 KeyCode;

/******************************************************************************
* 函数名：GPIO_Configuration
* 功  能：GPIO配置，开漏输出
* 输  入：延迟时间
* 输  出：无  
* 备  注：无
********************************************************************************/
static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 	
}

/******************************************************************************
* 函数名：GPIO_SDA_Out_MODULE
* 功  能：设置DQ引脚为开漏输出模式
* 输  入：无
* 输  出：无  
* 备  注：无
********************************************************************************/
static void GPIO_SDA_Out_MODULE(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/******************************************************************************
* 函数名：GPIO_SDA_Iput_MODULE
* 功  能：设置DQ引脚为浮空输入模式
* 输  入：无
* 输  出：无  
* 备  注：无
********************************************************************************/
static void GPIO_SDA_Iput_MODULE(void)
{
   	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
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

/******************************************************************************
* 函数名：I2C_Start
* 功  能：IO模拟IIC起始信号
* 输  入：无
* 输  出：无  
* 备  注：无
********************************************************************************/
static void I2C_Start(void)
{
	SDA_SET;
	SCL_SET;
	Delay(1);
	SDA_CLR; /* 时钟为高，拉低数据线电平 */
	Delay(1);
	SCL_CLR; /* 钳住I2C总线，准备接收或发送数据 */
	Delay(1);
}

/******************************************************************************
* 函数名：I2C_Stop
* 功  能：IO模拟IIC终止信号
* 输  入：无
* 输  出：无  
* 备  注：无
********************************************************************************/
static void I2C_Stop(void)
{
	SDA_CLR; /* 时钟为高，拉高数据线电平 */
	SCL_CLR;										  
	Delay(1);
	SCL_SET;
	Delay(1);
	SDA_SET; /* 发送I2C总线结束信号 */
	Delay(1);
}

/******************************************************************************
* 函数名：WriteI2CByte
* 功  能：IO模拟IIC写8位数据
* 输  入：8位数据
* 输  出：无  
* 备  注：无
********************************************************************************/
static void WriteI2CByte(unsigned char b)
{
    /*向I2C总线写一个字节*/
	char i;
	for(i=0;i<8;i++)
	{
		if((b<<i)&0x80)
		{	
		   	SDA_SET;
			Delay(1);
			SCL_CLR;
			Delay(1);
			SCL_SET;
			Delay(1);
			SCL_CLR;
			Delay(1);
		}
		else
		{
			/*发送0，在SCL为高电平时使SDA信号为低*/
			SDA_CLR;
			Delay(1);
			SCL_CLR;
			Delay(1);      
			SCL_SET;
			Delay(1);
			SCL_CLR;
			Delay(1);
		}
	}	
}

/******************************************************************************
* 函数名：i2cRead_ch452
* 功  能：IO模拟IIC读操作
* 输  入：读取数据的目的地址，数据数组首地址
* 输  出：1  
* 备  注：无
********************************************************************************/
unsigned char i2cRead_ch452(unsigned char slaveAddr,unsigned char *data)
{
    /*从I2C总线读一个字节*/
	int key_num=0,i;
	I2C_Start();
	WriteI2CByte(slaveAddr);
			Delay(1);
           	SDA_SET;
			Delay(1); 
			SCL_CLR;
			Delay(1);
			SCL_SET;
			Delay(1);
			SCL_CLR;
			Delay(1);     /*应答*/
	GPIO_SDA_Iput_MODULE();
	for(i=0;i<8;i++)
	{
	  Delay(1);
	  SCL_SET;
	  key_num=((key_num)<<1)|GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11);
	  Delay(2);
	  SCL_CLR;
	  Delay(1);
	}
	I2C_Stop();
	GPIO_SDA_Out_MODULE();
	*data = (char)(key_num&0xff);
	return 1;	
}

/******************************************************************************
* 函数名：i2cWrite_ch452
* 功  能：IO模拟IIC写操作
* 输  入：写入数据的目的地址，要写入的数据
* 输  出：1  
* 备  注：无
********************************************************************************/
unsigned char i2cWrite_ch452(unsigned char slaveAddr,unsigned char data)
{
	I2C_Start();
	WriteI2CByte(slaveAddr);
	SDA_SET;
	Delay(1);
	SCL_CLR;
	Delay(1);
	SCL_SET;
	Delay(1);
	SCL_CLR;
	Delay(1);
	WriteI2CByte(data);
	Delay(1);
	SDA_SET;
	Delay(1);
	SCL_CLR;
	Delay(1);
	SCL_SET;
	Delay(1);
	SCL_CLR;
	Delay(1); 
	I2C_Stop();
	return 1;
}

/******************************************************************************
* 函数名：unsigned char GetKeyNum(void)
* 功  能：获取键盘键值
* 输  入：无
* 输  出：键值  
* 备  注：无
********************************************************************************/
unsigned char GetKeyNum(void)
{
	unsigned char tmp;
	i2cRead_ch452(0x6F,&tmp);
	return(tmp);
}

/******************************************************************************
* 函数名：int I2C_ByteWrite(unsigned short data,unsigned char slaveAddr)
* 功  能：写AIC23控制寄存器
* 输  入：AIC23控制寄存器地址和控制字
* 输  出：1  
* 备  注：无
********************************************************************************/
int I2C_ByteWrite(unsigned short data,unsigned char slaveAddr)
{
	unsigned short tmp;
	unsigned char dataH,dataL;
	tmp=(slaveAddr<<9)| data;
	dataH=(tmp>>8)&0x00ff;
	dataL=tmp&0x00ff;
	I2C_Start();
	WriteI2CByte((0x1A<<1)& 0xfe );
	        SDA_CLR;
			Delay(1);
			SCL_CLR;
			Delay(1);
			SCL_SET;
			Delay(1);
			SCL_CLR;
			Delay(1);
	WriteI2CByte(dataH);
			Delay(1);
         	SDA_CLR;
			Delay(1);
			SCL_CLR;
			Delay(1);
			SCL_SET;
			Delay(1);
			SCL_CLR;
			Delay(1);     /*应答*/
	WriteI2CByte(dataL);
			Delay(1);
         	SDA_CLR;
			Delay(1);
			SCL_CLR;
			Delay(1);
			SCL_SET;
			Delay(1);
			SCL_CLR;
			Delay(1);
	I2C_Stop();
	return 1;
}

/******************************************************************************
* 函数名：WriteI2CByte
* 功  能：键盘初始化函数
* 输  入：无
* 输  出：无  
* 备  注：无
********************************************************************************/
void InitKey(void)
{
	GPIO_Configuration();
	Delay(5);
	i2cWrite_ch452(0x64,0x01);  /*芯片复位*/
	Delay(100);
	i2cWrite_ch452(0x68,CH452SYS_SSPD|CH452SYS_DISP);	   
	i2cWrite_ch452(0x62,0x40);  /*全亮*/
	Delay(100000);
	i2cWrite_ch452(0x64,0x01);  /*全灭*/
	Delay(5000);
	i2cWrite_ch452(0x68,CH452SYS_SSPD|CH452SYS_DISP|CH452SYS_KEYB);/*启动键盘*/
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
 void Keypad_Interrupt(u32 group,u32 PreemptionPriority,u32 SubPriority)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;
    NVIC_InitTypeDef   NVIC_InitStructure;
	//设置向量表的位置和偏移
	#ifdef  VECT_TAB_RAM  
		/* Set the Vector Table base location at 0x20000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 		//向量表位于RAM
	#else  /* VECT_TAB_FLASH  */
		/* Set the Vector Table base location at 0x08000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   //向量表位于FLASH
	#endif
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF, &GPIO_InitStructure); 
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOF, EXTI_PinSource11);
	EXTI_InitStructure.EXTI_Line = EXTI_Line11;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;	   //下降沿触发EXTI_Trigger_Rising;  
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	NVIC_PriorityGroupConfig(group);
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = PreemptionPriority;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = SubPriority;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
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

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*************/
