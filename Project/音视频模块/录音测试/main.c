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
/* 头文件 ------------------------------------------------------------------*/
#include "stm322xg_eval_nand.h"
#include "stm32_eval.h"
#include "stm322xg_eval_systick.h" 
#include "stm322xg_eval_lcd.h"
#include "main.h"
/* 宏定义-----------------------------------------------------------*/
/***********缓冲区大小***************************************/


/*************NAND FLASH ID*************************************/
#define NAND_SAMSUNG_MakerID     0xEC
#define NAND_SAMSUNG_DeviceID    0xF1   /* K9F1G08U0A,  128MB */
#define SalveAddress  0x1A<<1
#define ClockSpeed              100000
u32 ecc;
/* 全局变量定义 ---------------------------------------------------------*/
/**********NAND FLASH ID变量****************************************/
NAND_IDTypeDef NAND_ID;

/************NAND FLASH读写地址结构体变量**************************************/
NAND_ADDRESS WriteReadAddr;
volatile u8 WRflag=0;
/*************数据缓冲区*************************************/
u16 RxBuffer[BUFFER_SIZE];
/*************函数声明******************************/
void AIC_I2C_Init(void);
void I2S_GPIO_Config(void);
void NVIC_Configuration(void);
void AIC23B_Init(void);
void I2S_ConfigRx(void);
/***************************************************
*函数名:main
*输入参数:无
*返回值:无
*函数说明:主函数
*
****************************************************/
int main(void)
{ 
    int i;
    char temp[32];
    STM_EVAL_LEDInit(LED2);
    STM_EVAL_LEDInit(LED3);
    STM_EVAL_LEDInit(LED4);
    STM_EVAL_LEDOff(LED2);
    STM_EVAL_LEDOff(LED3);
    STM_EVAL_LEDOff(LED4);
    NVIC_Configuration();
    STM3220F_LCD_Init();	   /*LCD初始化*/
    LCD_Clear(Blue);  	   /*清屏*/
    LCD_SetBackColor(Blue);  /*设置背景色*/
    LCD_SetTextColor(Yellow);/*设置前景色*/
    LCD_DrawRect(0,0,239,319);
    LCD_DrawLine(0,20,320,Horizontal);
    sprintf(temp,"录音测试");
    LCD_DisplayStringHZ(130,4,(uint8_t *)temp);
    LCD_DrawLine(0,220,320,Horizontal);
    sprintf(temp,"北京亿旗创新科技发展有限公司");
    LCD_DisplayStringHZ(60,222,(uint8_t *)temp);
    /* NAND FLASH初始化*/
    FSMC_NAND_Init();
    
    /***NAND FLASH复位***/
    FSMC_NAND_Reset();
    /* 读取NAND FLASH的ID */
    FSMC_NAND_ReadID(&NAND_ID);

    /* 验证 NAND FLASH ID是否正确 */  
    if(!((NAND_ID.Maker_ID == NAND_SAMSUNG_MakerID) && (NAND_ID.Device_ID == NAND_SAMSUNG_DeviceID)))
    {
        sprintf(temp,"NAND FLASH ERROR");
        LCD_DisplayStringHZ(60,80,(uint8_t *)temp);
        while(1)
        {
            delay_ms(1000);
            STM_EVAL_LEDToggle(LED4);    /*NAND FLASH异常*/
        }
    }
    I2S_GPIO_Config();
    AIC_I2C_Init();
    AIC23B_Init();
    for(i=0;i<1024;i++)
    {
    	WriteReadAddr.Block=i;
    	WriteReadAddr.Zone=0;
    	WriteReadAddr.Page=0;
    	FSMC_NAND_EraseBlock(WriteReadAddr); 
    	delay_ms(1);
    }
    sprintf(temp,"NAND FLASH OK");
    LCD_DisplayStringHZ(50,50,(uint8_t *)temp);
    STM_EVAL_LEDOn(LED2);
    sprintf(temp,"录音进行中...");
    LCD_DisplayStringHZ(50,80,(uint8_t *)temp);
    WriteReadAddr.Block=0;
    I2S_ConfigRx();
    while(1)
    {
    	if(WRflag)        /*将数据写到NAND FLASH*/
    	{
    		WRflag=0;
    		ecc=FSMC_NAND_WriteSmallPage((u8 *)RxBuffer,WriteReadAddr,1);
    	}
    }
}
/************************************************************************************
*函数名:I2S_GPIO_Config
*函数说明: I2S的GPIO配置
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
void I2S_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOG, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource12,GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource13,GPIO_AF_SPI2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource15,GPIO_AF_SPI2); 

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
    GPIO_Init(GPIOG, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC,GPIO_Pin_2);
    GPIO_SetBits(GPIOC,GPIO_Pin_3);
    GPIO_SetBits(GPIOG,GPIO_Pin_8);
}
/************************************************************************************
*函数名: AIC_I2C_Init
*函数说明:初始化I2C的GPIO管脚以及配置I2C
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
void AIC_I2C_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    I2C_InitTypeDef   I2C_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType=GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_I2C2);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_I2C2);
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = SalveAddress;
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = ClockSpeed;
    I2C_Init(I2C2, &I2C_InitStructure);
    I2C_Cmd(I2C2, ENABLE);
}
/************************************************************************************
*函数名:NVIC_Configuration
*函数说明: 中断初始化
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM  
  /* 如果中断向量表在SRAM中 */ 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
#else  /* VECT_TAB_FLASH  */
  /* 如果中断向量表在Flash当中 */ 
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
#endif


  /* SPI2中断设置*/
  NVIC_InitStructure.NVIC_IRQChannel =SPI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
/************************************************************************************
*函数名:I2S_ConfigRx
*函数说明:配置I2S为接收功能
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
void I2S_ConfigRx(void)
{
    I2S_InitTypeDef I2S_InitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    SPI_I2S_DeInit(SPI2);
    I2S_InitStructure.I2S_Standard = I2S_Standard_Phillips;
    I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
    I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
    I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
    I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_48k;
    I2S_InitStructure.I2S_Mode =I2S_Mode_MasterRx;
    I2S_Init(SPI2, &I2S_InitStructure);
    SPI_I2S_ITConfig(SPI2, SPI_I2S_IT_RXNE, ENABLE); 
    I2S_Cmd(SPI2,ENABLE);     
}

/************************************************************************************
*函数名:AIC_ByteWrite
*函数说明: 向AIC23B发送命令
*输入参数:
*        data：写寄存器数据
*        WriteAddr:寄存器地址
*返回值:无
*
*
**************************************************************************************/
void AIC_ByteWrite(u16 data,u8 WriteAddr)
{
	u8 dataH,dataL;
	u16 datatmp=0;
	datatmp=WriteAddr;
	data|=datatmp<<9;
	dataH=(data&0xFF00)>>8;
	dataL=data&0x00FF;
	while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));
	/***发送起始信号****/
	I2C_GenerateSTART(I2C2,ENABLE);
	/***检测EV5位并清除 */
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_MODE_SELECT));
	/***发送AIC23B基地址***/
	I2C_Send7bitAddress(I2C2,SalveAddress,I2C_Direction_Transmitter);
	/****检测EV6位并清除 ***/
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
	/*发送高位数据*/
	I2C_SendData(I2C2,dataH);
	/**检测EV8位并清除*/
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
    /*发送数据地位*/
	I2C_SendData(I2C2,dataL);
	/*检测EV8位并清除 */
	while(!I2C_CheckEvent(I2C2,I2C_EVENT_MASTER_BYTE_TRANSMITTED));
	/***发送终止信号*/
	I2C_GenerateSTOP(I2C2,ENABLE);
}
/************************************************************************************
*函数名:AIC23B_Init
*函数说明: AIC23B初始化
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
void AIC23B_Init(void)
{
  AIC_ByteWrite(0x00FF,0x06);  /*关闭电源*/
  AIC_ByteWrite(0x009f,0x00); /*左声道输入线音量控制1f*/
  AIC_ByteWrite(0x009f,0x01); /*右声道输入线音量控制1f*/
  AIC_ByteWrite(0x0070,0x02); /*左通道耳机音量控制70*/
  AIC_ByteWrite(0x0070,0x03); /*右通道耳机音量控制70*/
  AIC_ByteWrite(0x0014,0x04);  /*模拟通道控制14*/
  AIC_ByteWrite(0x0004,0x05);  /*数字通道控制4*/
  AIC_ByteWrite(0x0012,0x07);	/*数字接口控制52*/
  AIC_ByteWrite(0x0000,0x08);  /*采样率控制20*/
  AIC_ByteWrite(0x00c1,0x09);  /*数字接口激活c1*/
  AIC_ByteWrite(0x0019,0x06);  /*上电*/
}

#ifdef  DEBUG
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
void assert_failed(u8* file, u32 line)
{ 
 /****此处可以添加用户错误报告方式**********************/
  while (1)
  {
  }
}
#endif
/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
