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
#include "stm322xg_eval_lcd.h"
#include "stm322xg_eval_keypad.h"
#include "stm322xg_eval_nand.h"
#include "stm322xg_eval_sram.h"
/***********音频文件宏定义********************************/
#define AUDIO_FILE_SZE          990000/2    /*文件大小*/
#define AUIDO_START_ADDRESS     58 /*音频数据起始偏移量*/
#define  buffer   	((uint8_t*)0x68000100) //外部sram
#define  sizemax     3//最多存5首1m的音乐文件
/************全局变量定义*******************************/
__IO uint32_t cmd = AUDIO_PAUSE;/*播放状态*/
__IO uint32_t volume = 100;	 /*音量大小*/
USART_InitTypeDef USART_InitStructure;	   /* 串口初始化结构体*/
extern uint32_t NbrOfDataToRead;			   /* 记录允许接收数据的数量*/
extern __IO uint32_t RxCounter; 		   /* 记录已接收数量*/
NAND_IDTypeDef NAND_ID;
NAND_ADDRESS WriteReadAddr;
u16 Buffer1[0x1000];/*数据缓冲区*/
u16 BLOCKPOS[3];
u16 BLOCKsize[3];
void delay_Nms(unsigned short Nms); 
static __IO uint32_t TimingDelay; /*延时大小*/
void NVIC_Config(void);
RCC_ClocksTypeDef RCC_Clocks;  /*系统时钟频率*/
u8 BufFlag=0;
vu32 PageNumber = 64;
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
	uint16_t j;
	uint8_t i;
	uint8_t now=0;
	uint8_t  maxnum=1;
 NAND_IDTypeDef NAND_ID;
	unsigned char temp[30];
  STM3220F_LCD_Init();	   /*LCD初始化*/
  LCD_Clear(Blue);  	   /*清屏*/
  LCD_SetBackColor(Blue);  /*设置背景色*/
  LCD_SetTextColor(Yellow);/*设置前景色*/
	/*********systick初始化********************************/
 RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
  /* 初始化音频芯片和相关的外设 (I2S, I2C) */  
	NVIC_Config();	/* NVIC配置，打开USART1中断*/
	SRAM_Init();
	FSMC_NAND_Init();
  FSMC_NAND_ReadID(&NAND_ID);
  if(!((NAND_ID.Maker_ID == NAND_SAMSUNG_MakerID) && (NAND_ID.Device_ID == NAND_SAMSUNG_DeviceID)))
  {
      LCD_DisplayStringHZ(40,200,"NAND ERROR");
      while(1)
      {
        STM_EVAL_LEDToggle(LED4);
      }
		}
			for(i=0;i<128;i++)
    {
    	WriteReadAddr.Block=i;
    	WriteReadAddr.Zone=0;
    	WriteReadAddr.Page=0;
    	FSMC_NAND_EraseBlock(WriteReadAddr); 
    }
		sprintf(temp,"NAND FLASH OK");
    LCD_DisplayStringHZ(50,50,(uint8_t *)temp);
		delay_Nms(100);
		LCD_Clear(Blue);  	   /*清屏*/
		WriteReadAddr.Block=0;
	/* USART1 配置 ------------------------------------------------------*/
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
	 USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(COM1, &USART_InitStructure);	 /* 采用USART_InitStructure参数设置，配置串口USART1 */
	BLOCKPOS[0]=0;
	for(j=0;j<12;j++){
		/* 使能串口USART1的接收中断：当USART1的接收数据寄存器为非空时，将会产生发送中断*/
  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);
  
  /* 等待接收数据，并将其存储在RxBuffer中，直到接收数据量为NbrOfDataToRead时停止。*/
  while(RxCounter < NbrOfDataToRead)
  {}
 FSMC_NAND_WriteSmallPage(buffer, WriteReadAddr, PageNumber);	
			WriteReadAddr.Page=0x00;
			WriteReadAddr.Block+=1;
		RxCounter=0;
	}
	BLOCKsize[0]=	WriteReadAddr.Block;
	 Keypad_Interrupt(0,0,1);
   InitKey();			   /*按键初始化*/
 if (EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO, volume, I2S_AudioFreq_48k) == 0)
  {

    LCD_DisplayStringHZ(40,40, "按键1: 播    放");
		LCD_DisplayStringHZ(40,60, "按键2: 暂    停");
    LCD_DisplayStringHZ(40,80, "按键3: 上 一 首");
		LCD_DisplayStringHZ(40,100,"按键4: 下 一 首");
    LCD_DisplayStringHZ(40,120,"按键5: 音量减小");
		LCD_DisplayStringHZ(40,140,"按键6: 音量增大");
    LCD_DisplayStringHZ(40,160,"按键7: 停    止"); 
  }
  else
  {
    LCD_DisplayStringHZ(40,200, " 音频 ERROR ");
  }

  /**********开始传送音频数据，DMA方式*********************************/
//EVAL_AUDIO_Play((uint16_t*)(AUDIO_SAMPLE + AUIDO_START_ADDRESS), (AUDIO_FILE_SZE - AUIDO_START_ADDRESS));
	/*在LCD上显示状态信息 */
	EVAL_AUDIO_Play(Buffer1,4096);
	LCD_DisplayStringHZ(40,200, "播 放");
	 WriteReadAddr.Page=0;
  WriteReadAddr.Block=0;
  WriteReadAddr.Zone=0;
  cmd=AUDIO_RESUME;
		while(1)						
	{  
		delay_Nms(3);
		switch(KeyCode)
		{
			case KEY01_DOWN: 
       cmd=AUDIO_RESUME;
		 LCD_DisplayStringHZ(40,200, " 播 放 ");
break;			
			case KEY02_DOWN: 
				if(cmd==AUDIO_STOP);
		else{		 cmd=AUDIO_PAUSE;
       LCD_DisplayStringHZ(40,200, " 暂 停 ");}	
break;		
			 case KEY03_DOWN: 
					if(now>0)now=now-1;
			  WriteReadAddr.Page=0;
			  WriteReadAddr.Block=BLOCKPOS[now];
				sprintf(temp,"%d",(uint8_t *)now);
			LCD_DisplayStringHZ(140,200, temp);
			 KeyCode=0xff;
break;
				
			 case KEY04_DOWN: 
					if(now<(maxnum-1))now=now+1;
			  WriteReadAddr.Page=0;
			 WriteReadAddr.Block=BLOCKPOS[now];
				sprintf(temp,"%d",(uint8_t *)now);
			 LCD_DisplayStringHZ(140,200, temp);
			 KeyCode=0xff;
break;
			case KEY05_DOWN: 
					if (volume > 5)
          volume -= 5; 
        else
          volume = 0; 
		EVAL_AUDIO_VolumeCtl(volume);
				     sprintf(temp,"%d",(uint8_t *)volume);
					LCD_ClearLine(220);
				LCD_DisplayStringHZ(120,220, "-");
			LCD_DisplayStringHZ(140,220, temp);
				KeyCode=0xff;
break;
				
			case KEY06_DOWN:
					if (volume < 121)
          volume += 5; 
        else
          volume = 126; 
        EVAL_AUDIO_VolumeCtl(volume);
				sprintf(temp,"%d",(uint8_t*)volume);
				LCD_ClearLine(220);
			LCD_DisplayStringHZ(120,220, "+");
			LCD_DisplayStringHZ(140,220, temp);
				KeyCode=0xff;
break;
			case KEY07_DOWN:
				cmd=AUDIO_STOP;
			  EVAL_AUDIO_PauseResume(AUDIO_PAUSE);
			   WriteReadAddr.Page=0;
         WriteReadAddr.Block=BLOCKPOS[now];
         WriteReadAddr.Zone=0;
			 LCD_DisplayStringHZ(40,200,  " 停  止 ");
break;
			
			case KEY09_DOWN://确认键
break;
			case KEY10_DOWN:
				  EVAL_AUDIO_PauseResume(AUDIO_PAUSE);
			 LCD_DisplayStringHZ(40,200,  " 上  传   ");
				WriteReadAddr.Block=BLOCKPOS[maxnum-1]+BLOCKsize[maxnum-1];
			   	maxnum+=1;
				for(j=0;j<12;j++){
		/* 使能串口USART1的接收中断：当USART1的接收数据寄存器为非空时，将会产生发送中断*/
  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);
  
  /* 等待接收数据，并将其存储在RxBuffer中，直到接收数据量为NbrOfDataToRead时停止。*/
  while(RxCounter < NbrOfDataToRead)
  {}
 FSMC_NAND_WriteSmallPage(buffer, WriteReadAddr, PageNumber);	
			WriteReadAddr.Page=0x00;
			WriteReadAddr.Block+=1;
		RxCounter=0;
	}
		BLOCKPOS[maxnum-1]=BLOCKPOS[maxnum-2]+BLOCKsize[maxnum-2];
	  BLOCKsize[maxnum-1]=WriteReadAddr.Block-BLOCKPOS[maxnum-1];
	 WriteReadAddr.Block=BLOCKPOS[now];
	 LCD_DisplayStringHZ(40,200, " 播 放 ");
	KeyCode=0xff;
break;
			default:
			break;
			
}	
		if(cmd==AUDIO_STOP);
else{ 
	if(BufFlag==1)
     {
        BufFlag=0;
        FSMC_NAND_ReadSmallPage ((u8 *)Buffer1, WriteReadAddr,4);
        EVAL_AUDIO_Play(Buffer1,4096);
        WriteReadAddr.Page=WriteReadAddr.Page+4;
        if(WriteReadAddr.Page>63)
        {
            WriteReadAddr.Page=0;
            WriteReadAddr.Block+=1;
            if(WriteReadAddr.Block>(BLOCKPOS[now]+BLOCKsize[now]))
            {
              WriteReadAddr.Page=0;
              WriteReadAddr.Block=BLOCKPOS[now];
              WriteReadAddr.Zone=0;
            }
					}
				}
	EVAL_AUDIO_PauseResume(cmd);
}
	}  
}
 
void delay_Nms(unsigned short Nms) 
{ 
  SysTick->LOAD=(unsigned int)Nms*15000;  //毫秒级？ 
  SysTick->CTRL|=0x01;      //使能SysTick 
  while(!(SysTick->CTRL&(1<<16)));  //等待计数到0 
  SysTick->CTRL=0x00000004;    //关闭SysTick 
  SysTick->VAL=0x00000000;    //清空计数器 
} 
/******************************************************************************
* 函数名： NVIC_Config
* 功  能： 配置串口1的中断类型，并打开串口1的发送和接收中断功能
* 输  入：无 
* 输  出：无  
* 备  注：无
********************************************************************************/
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* 打开 USART1 中断配置 */
	NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM1_IRQn;  /* 设定为USART1中断 */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  /* 设定主优先级为0*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		   /* 设定次优先级为0*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   /* 中断打开*/
	NVIC_Init(&NVIC_InitStructure);
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
