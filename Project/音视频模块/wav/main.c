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
#include "main.h"
#include "ff.h"
#include "stm32_eval_spi_sd.h"
#include "stm322xg_eval_lcd.h"
#include "stm322xg_eval_keypad.h"
#include "stm32_eval.h"
#include "stm322xg_eval_sram.h"

/* 定义 PUTCHAR_PROTOTYPE 函数*/
#ifdef __GNUC__	  
  /* 将 PUTCHAR_PROTOTYPE 定义为 __io_putchar函数 ，在 GCC/RAISONANCE模式下，采用small printf实现方法
  *  可以通过 LD Linker->Libraries->Small printf 选项 设置 'Yes'，来实现printf调用 __io_putchar() 
  */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  /* 默认情况下，printf函数会调用fputc（）函数*/
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**************界面信息宏定义*****************************/
#define MESSAGE1   "  EITP音频测试  " 
#define MESSAGE2   " Device running on  " 
#define MESSAGE3   "北京亿旗创新科技发展有限公司" 
/***********音频文件宏定义********************************/
#define AUDIO_FILE_SZE          990000/2    /*文件大小*/
#define AUIDO_START_ADDRESS     58 /*音频数据起始偏移量*/
/************全局变量定义*******************************/
__IO uint32_t cmd = AUDIO_PAUSE;/*播放状态*/
extern unsigned char KeyCode;	 /*按键值*/
__IO uint32_t volume = 115;	 /*音量大小*/

static __IO uint32_t TimingDelay; /*延时大小*/
RCC_ClocksTypeDef RCC_Clocks;  /*系统时钟频率*/
u8 BufFlag=0;

/*********缓冲区大小*****************************************/
#define BUFFER_SIZE        0x100
#define WRITE_READ_ADDR    0x800
/*************全局变量定义***********************************/
FATFS filesystem;		/* 文件系统卷标 */

FRESULT ret;			/* 文件操作返回值 */

FIL file;				/* 文件*/

DIR dir;				/* 路径 */

FILINFO fno;			/*文件信息 */

UINT bw, br;

uint16_t buff[512];

//#define BankADDR  ((uint32_t)0x68000010);
//uint16_t *bank=(uint16_t *)BankADDR;
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

	int i;
    STM3220F_LCD_Init();	   /*LCD初始化*/
    InitKey();
	SRAM_Init();
    LCD_Clear(Blue);  	   /*清屏*/
    LCD_SetBackColor(Blue);  /*设置背景色*/
    LCD_SetTextColor(Yellow);/*设置前景色*/
    LCD_DrawRect(0,0,239,319);
    LCD_DrawLine(0,20,320,Horizontal);
    LCD_DrawLine(0,220,320,Horizontal);
    LCD_DisplayStringHZ(108,4, MESSAGE1);
    LCD_DisplayStringHZ(60,222, MESSAGE3);
/***********systick初始化********************************/
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

/* 初始化音频芯片和相关的外设 (I2S, I2C) */  
    if (EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO, volume, I2S_AudioFreq_48k) == 0)
    {
		LCD_DisplayStringHZ(10,200, " WAV音频解码  ");    
    }
    else
    {
    	LCD_DisplayStringHZ(10,200, " 音频 ERROR ");
    }

  	if(SD_Init()==0)
    {
        LCD_DisplayStringHZ(80,140,"SD init no err"); 
    }/*SD卡初始化*/

	if(f_mount(0, &filesystem) != FR_OK) /*文件系统挂载*/
	{
		LCD_DisplayStringHZ(80,170,"could not open filesystem\n");
	}
	Delay(200);	
	ret = f_open(&file, "5.wav", FA_READ);  	
	if (ret)
	{
		LCD_DisplayStringHZ(130,160,"not exist the test file\n");
	}
		
	i=0;
	while(1)
	{
		f_read(&file,(uint16_t*)buff,sizeof(buff),&br);	//读完以后自动跳到下一帧读下一个buff大小的字节	
		SRAM_WriteBuffer(buff,(uint32_t)(WRITE_READ_ADDR+i*512*1),512);
		i++;
		if(i>=1024)
		break;
	 }
	 EVAL_AUDIO_Play((uint16_t*)(0x68000000+WRITE_READ_ADDR+AUIDO_START_ADDRESS),400000*32);
	 Delay(100000);	
}
/************************************************************************************
*函数名: EVAL_AUDIO_TransferComplete_CallBack
*函数说明:DMA传输完成时执行的回调函数
*输入参数:
*		 pBuffer：数据存放地址
*		 Size：数据大小
*返回值:无
*
*
**************************************************************************************/
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
#ifdef AUDIO_MAL_MODE_NORMAL  

  /*如果是NORMAL模式则从头继续开始传输数据*/
  //EVAL_AUDIO_Play((uint16_t*)(AUDIO_SAMPLE + AUIDO_START_ADDRESS), (AUDIO_FILE_SZE - AUIDO_START_ADDRESS));
#else 

  /* 显示数据传输完成 */
  LCD_DisplayStringLine(Line8, " All Buffer Reached ");   

#endif
}

/************************************************************************************
*函数名:EVAL_AUDIO_HalfTransfer_CallBack
*函数说明:
*输入参数:DMA传输一半时执行的回调函数
*		 pBuffer：数据存放地址
*		 Size：数据大小
*返回值:无
*
*
**************************************************************************************/
void EVAL_AUDIO_HalfTransfer_CallBack(uint32_t pBuffer, uint32_t Size)
{  
#ifdef AUDIO_MAL_MODE_CIRCULAR
  
  /* 在LCD上显示信息 */
  LCD_DisplayStringLine(Line8, " 1/2 Buffer Reached "); 
  
#endif 

}

/************************************************************************************
*函数名:EVAL_AUDIO_Error_CallBack
*函数说明:DMA FIFO错误时执行的回调函数
*输入参数:
*		  无
*返回值:无
*
*
**************************************************************************************/
void EVAL_AUDIO_Error_CallBack(void* pData)
{
  /* 在LCD上显示信息 */
  LCD_SetBackColor(Red);
  LCD_DisplayString(10,140, "     DMA  ERROR     ");
  while (1)
  {}
}
#ifndef USE_DEFAULT_TIMEOUT_CALLBACK
/************************************************************************************
*函数名: Codec_TIMEOUT_UserCallback
*函数说明:超时执行的回调函数
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
uint32_t Codec_TIMEOUT_UserCallback(void)
{
  /* 在LCD上显示信息*/
  LCD_DisplayString(10,140, "  CODEC I2C  ERROR  ");  
  while (1)
  {   
  }
}
#endif 
/************************************************************************************
*函数名:Delay
*函数说明:延时函数
*输入参数:
*		 nTime：延时长度
*返回值:无
*
*
**************************************************************************************/
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;
  
  while(TimingDelay != 0);
}

/************************************************************************************
*函数名:TimingDelay_Decrement
*函数说明:自减函数
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
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
