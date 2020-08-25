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
/**************界面信息宏定义*****************************/
#define MESSAGE1   "  EITP录音测试  " 
#define MESSAGE2   " Device running on  " 
#define MESSAGE3   "北京亿旗创新科技发展有限公司" 
/***********NAND FLASH ID********************************/
#define NAND_SAMSUNG_MakerID     0xEC
#define NAND_SAMSUNG_DeviceID    0xF1   /* K9F1G08U0A,  128MB */
/************全局变量定义*******************************/
__IO uint32_t cmd = AUDIO_PAUSE;/*播放状态*/
 extern unsigned char KeyCode;	 /*按键值*/
__IO uint32_t volume = 126;	 /*音量大小*/

static __IO uint32_t TimingDelay; /*延时大小*/
RCC_ClocksTypeDef RCC_Clocks;  /*系统时钟频率*/

u16 Buffer1[0x1000];/*数据缓冲区*/
u8 BufFlag=0;     /*读取标志*/
NAND_ADDRESS WriteReadAddr;
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
  NAND_IDTypeDef NAND_ID;
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDOff(LED2);
  STM3220F_LCD_Init();	   /*LCD初始化*/
  InitKey();			   /*按键初始化*/
  LCD_Clear(Blue);  	   /*清屏*/
  LCD_SetBackColor(Blue);  /*设置背景色*/
  LCD_SetTextColor(Yellow);/*设置前景色*/
  LCD_DrawRect(0,0,239,319);
  LCD_DrawLine(0,20,320,Horizontal);
  LCD_DrawLine(0,220,320,Horizontal);
  LCD_DisplayStringHZ(108,4, MESSAGE1);
  LCD_DisplayStringHZ(60,222, MESSAGE3);
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
  FSMC_NAND_Init();
  FSMC_NAND_ReadID(&NAND_ID);
  if(!((NAND_ID.Maker_ID == NAND_SAMSUNG_MakerID) && (NAND_ID.Device_ID == NAND_SAMSUNG_DeviceID)))
  {
      LCD_DisplayStringHZ(40,200,"NAND ERROR");
      while(1)
      {
        STM_EVAL_LEDToggle(LED4);
        Delay(1000);
      }
  }
  /* 初始化音频芯片和相关的外设 (I2S, I2C) */  
  if (EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO, volume, I2S_AudioFreq_48k) == 0)
  {
    LCD_DisplayStringHZ(40,200, " 音频 OK  "); 
    LCD_DisplayStringHZ(40,80," 录音播放中... ");   
  }
  else
  {
    LCD_DisplayStringHZ(40,200, " 音频 ERROR ");
  }
  /**********开始传送音频数据，DMA方式*********************************/
  EVAL_AUDIO_Play(Buffer1,4096);
  WriteReadAddr.Page=0;
  WriteReadAddr.Block=0;
  WriteReadAddr.Zone=0;
  cmd=AUDIO_RESUME;
  STM_EVAL_LEDOn(LED2);
  while (1)
  {  
     if(BufFlag==1)
     {
        BufFlag=0;
        FSMC_NAND_ReadSmallPage ((u8 *)Buffer1, WriteReadAddr,4);
        EVAL_AUDIO_Play(Buffer1,4096);
        WriteReadAddr.Page=WriteReadAddr.Page+4;
        if(WriteReadAddr.Page>63)
        {
            WriteReadAddr.Page=0;
            WriteReadAddr.Block++;
            if(WriteReadAddr.Block>1023)
            {
                DMA_Cmd(DMA1_Stream4,DISABLE);
                I2S_Cmd(SPI2,DISABLE);
                while(1)
                {
                    Delay(1000);
                    STM_EVAL_LEDToggle(LED3);
                }
            }
        }
     }
  }
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
 // EVAL_AUDIO_Play((uint16_t*)(AUDIO_SAMPLE + AUIDO_START_ADDRESS), (AUDIO_FILE_SZE - AUIDO_START_ADDRESS));
  
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
