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
#include "stm322xg_eval_ov9655.h"
#include "stm322xg_eval_lcd.h"
#include "stm32f2xx_dcmi.h"
#include "stm32_eval_spi_sd.h"
#include "ff.h"
#include "stm322xg_eval_keypad.h"
#include "stm322xg_eval_sram.h"
#include "stm322xg_eval_systick.h"
#include "string.h"
/**********宏定义*********************************/
#define BUFF  0x68000010  /*图像数据缓存区*/
/***********BMP图像数据头********************************/
u8 tmp[66]={0x4d,0x42,0x42,0x58,0x02,0x00,  //"BM 文件大小"
		0x00,0x00,0x00,0x00,0x42,0x00,0x00,0x00,//"偏移量"
		0x28,0x00,0x00,0x00,0x40,0x01,0x00,0x00,//"结构体所需字节数，图像宽度"
		0xf0,0x00,0x00,0x00,0x01,0x00,0x10,0x00,//"图像高度，目标设备，比特数"
		0x03,0x00,0x00,0x00,0x00,0x58,0x02,0x00,//"压缩系数，图片大小"
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0xf8,0x00,0x00,0xE0,0x07,0x00,0x00,
        0x1f ,0x00,0x00,0x00};
/*********全局变量定义**********************************/
FATFS filesystem;		/* 文件系统卷标 */
FRESULT ret;			/* 文件操作返回值 */
FIL file;				/* 文件*/
UINT bw, br;
/***************************************************
*函数名:main
*输入参数:无
*返回值:无
*函数说明:主函数
*
****************************************************/
int main(void)
{ 
  u32 res;
  u8 key;
  u32 i;
  u8 *p=(u8 *)BUFF;
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDOff(LED1);
rty:
  STM3220F_LCD_Init();/*LCD初始化*/
  SRAM_Init();
  memset((u8 *)BUFF,0x00,320*240*2);
  LCD_Clear(White);
  res=SD_Init();
  if(res!=0)
  {
	  LCD_DisplayStringHZ(100,100,"Check sd card");
      return 1;
  }
  ret=f_mount(0,&filesystem);
  if(ret!=FR_OK)
  {
  	LCD_DisplayStringHZ(100,100,"Check filesystem");
    return 1;
  }
  Ov9655_I2C_Configuration();/*I2C初始化*/
  Ov9655_Dcmi_GPIO_Configuration();/*DCMI端口初始化*/
  Ov9655_DMA_Configuration(); /*DMA初始化*/
  Ov9655_Dcmi_Configuration(); /*DCMI配置*/
  DMA_Cmd(DMA2_Stream1, ENABLE); /*DMA使能*/	  
  DCMI_Cmd(ENABLE);				 /*DCMI使能*/
  DCMI->CR |=0x1; 				 /*使能图像扑捉*/
  memcpy(p,tmp,66);
  InitKey();
  while (1)
  {
		key=GetKeyNum();
		if(key==KEY01_DOWN)
		{
			DMA_Cmd(DMA2_Stream1, DISABLE);
			DCMI_Cmd(DISABLE);				 /*DCMI使能*/
            DMA_Cmd(DMA2_Stream1, DISABLE);
			DCMI->CR &=0xFFFFFFFE; 				 /*使能图像扑捉*/
			Ov9655_DMA1_Configuration();

			DMA_Cmd(DMA2_Stream1, ENABLE); /*DMA使能*/
			DCMI_Cmd(ENABLE);				 /*DCMI使能*/
			DCMI->CR |=0x1; 				 /*使能图像扑捉*/
			delay_ms(1000);
			DCMI->CR &=0xFFFFFFFE; 
			LCD_Clear(White);
			LCD_WriteBMP((uint16_t *)(p+66));
			ret = f_open(&file, "test.bmp", FA_WRITE | FA_CREATE_ALWAYS); /*打开文件，没有则创建*/
			if (ret)
			{
				LCD_DisplayStringHZ(100,100,"open file  error");
			}else
			{
				for(i=0;i<320*240*2+52;i=i+512)
				{
					ret=f_write(&file,(u8 *)(p+i),512,&bw);
				}
			}
			f_close(&file);
			goto rty;

     }
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
