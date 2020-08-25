 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� main.c
*   ժ Ҫ    �� 
*
*   ��ǰ�汾 �� 
*   �� ��    �� EFLAG
*   ������� �� 
*
*   ȡ���汾 ��
*   ԭ����   �� 
*   �޸����� ��
*   ������� ��2012.08.01 
*
**********************************************************/

/* ͷ�ļ� ------------------------------------------------------------------*/
#include "main.h"
#include "ff.h"
#include "stm32_eval_spi_sd.h"
#include "stm322xg_eval_lcd.h"
#include "stm322xg_eval_keypad.h"
#include "stm32_eval.h"
#include "stm322xg_eval_sram.h"

/* ���� PUTCHAR_PROTOTYPE ����*/
#ifdef __GNUC__	  
  /* �� PUTCHAR_PROTOTYPE ����Ϊ __io_putchar���� ���� GCC/RAISONANCEģʽ�£�����small printfʵ�ַ���
  *  ����ͨ�� LD Linker->Libraries->Small printf ѡ�� ���� 'Yes'����ʵ��printf���� __io_putchar() 
  */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  /* Ĭ������£�printf���������fputc��������*/
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/**************������Ϣ�궨��*****************************/
#define MESSAGE1   "  EITP��Ƶ����  " 
#define MESSAGE2   " Device running on  " 
#define MESSAGE3   "�������촴�¿Ƽ���չ���޹�˾" 
/***********��Ƶ�ļ��궨��********************************/
#define AUDIO_FILE_SZE          990000/2    /*�ļ���С*/
#define AUIDO_START_ADDRESS     58 /*��Ƶ������ʼƫ����*/
/************ȫ�ֱ�������*******************************/
__IO uint32_t cmd = AUDIO_PAUSE;/*����״̬*/
extern unsigned char KeyCode;	 /*����ֵ*/
__IO uint32_t volume = 115;	 /*������С*/

static __IO uint32_t TimingDelay; /*��ʱ��С*/
RCC_ClocksTypeDef RCC_Clocks;  /*ϵͳʱ��Ƶ��*/
u8 BufFlag=0;

/*********��������С*****************************************/
#define BUFFER_SIZE        0x100
#define WRITE_READ_ADDR    0x800
/*************ȫ�ֱ�������***********************************/
FATFS filesystem;		/* �ļ�ϵͳ��� */

FRESULT ret;			/* �ļ���������ֵ */

FIL file;				/* �ļ�*/

DIR dir;				/* ·�� */

FILINFO fno;			/*�ļ���Ϣ */

UINT bw, br;

uint16_t buff[512];

//#define BankADDR  ((uint32_t)0x68000010);
//uint16_t *bank=(uint16_t *)BankADDR;
/************************************************************************************
*������:main
*����˵��:������
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
int main(void)
{ 

	int i;
    STM3220F_LCD_Init();	   /*LCD��ʼ��*/
    InitKey();
	SRAM_Init();
    LCD_Clear(Blue);  	   /*����*/
    LCD_SetBackColor(Blue);  /*���ñ���ɫ*/
    LCD_SetTextColor(Yellow);/*����ǰ��ɫ*/
    LCD_DrawRect(0,0,239,319);
    LCD_DrawLine(0,20,320,Horizontal);
    LCD_DrawLine(0,220,320,Horizontal);
    LCD_DisplayStringHZ(108,4, MESSAGE1);
    LCD_DisplayStringHZ(60,222, MESSAGE3);
/***********systick��ʼ��********************************/
    RCC_GetClocksFreq(&RCC_Clocks);
    SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);

/* ��ʼ����ƵоƬ����ص����� (I2S, I2C) */  
    if (EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO, volume, I2S_AudioFreq_48k) == 0)
    {
		LCD_DisplayStringHZ(10,200, " WAV��Ƶ����  ");    
    }
    else
    {
    	LCD_DisplayStringHZ(10,200, " ��Ƶ ERROR ");
    }

  	if(SD_Init()==0)
    {
        LCD_DisplayStringHZ(80,140,"SD init no err"); 
    }/*SD����ʼ��*/

	if(f_mount(0, &filesystem) != FR_OK) /*�ļ�ϵͳ����*/
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
		f_read(&file,(uint16_t*)buff,sizeof(buff),&br);	//�����Ժ��Զ�������һ֡����һ��buff��С���ֽ�	
		SRAM_WriteBuffer(buff,(uint32_t)(WRITE_READ_ADDR+i*512*1),512);
		i++;
		if(i>=1024)
		break;
	 }
	 EVAL_AUDIO_Play((uint16_t*)(0x68000000+WRITE_READ_ADDR+AUIDO_START_ADDRESS),400000*32);
	 Delay(100000);	
}
/************************************************************************************
*������: EVAL_AUDIO_TransferComplete_CallBack
*����˵��:DMA�������ʱִ�еĻص�����
*�������:
*		 pBuffer�����ݴ�ŵ�ַ
*		 Size�����ݴ�С
*����ֵ:��
*
*
**************************************************************************************/
void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size)
{
#ifdef AUDIO_MAL_MODE_NORMAL  

  /*�����NORMALģʽ���ͷ������ʼ��������*/
  //EVAL_AUDIO_Play((uint16_t*)(AUDIO_SAMPLE + AUIDO_START_ADDRESS), (AUDIO_FILE_SZE - AUIDO_START_ADDRESS));
#else 

  /* ��ʾ���ݴ������ */
  LCD_DisplayStringLine(Line8, " All Buffer Reached ");   

#endif
}

/************************************************************************************
*������:EVAL_AUDIO_HalfTransfer_CallBack
*����˵��:
*�������:DMA����һ��ʱִ�еĻص�����
*		 pBuffer�����ݴ�ŵ�ַ
*		 Size�����ݴ�С
*����ֵ:��
*
*
**************************************************************************************/
void EVAL_AUDIO_HalfTransfer_CallBack(uint32_t pBuffer, uint32_t Size)
{  
#ifdef AUDIO_MAL_MODE_CIRCULAR
  
  /* ��LCD����ʾ��Ϣ */
  LCD_DisplayStringLine(Line8, " 1/2 Buffer Reached "); 
  
#endif 

}

/************************************************************************************
*������:EVAL_AUDIO_Error_CallBack
*����˵��:DMA FIFO����ʱִ�еĻص�����
*�������:
*		  ��
*����ֵ:��
*
*
**************************************************************************************/
void EVAL_AUDIO_Error_CallBack(void* pData)
{
  /* ��LCD����ʾ��Ϣ */
  LCD_SetBackColor(Red);
  LCD_DisplayString(10,140, "     DMA  ERROR     ");
  while (1)
  {}
}
#ifndef USE_DEFAULT_TIMEOUT_CALLBACK
/************************************************************************************
*������: Codec_TIMEOUT_UserCallback
*����˵��:��ʱִ�еĻص�����
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
uint32_t Codec_TIMEOUT_UserCallback(void)
{
  /* ��LCD����ʾ��Ϣ*/
  LCD_DisplayString(10,140, "  CODEC I2C  ERROR  ");  
  while (1)
  {   
  }
}
#endif 
/************************************************************************************
*������:Delay
*����˵��:��ʱ����
*�������:
*		 nTime����ʱ����
*����ֵ:��
*
*
**************************************************************************************/
void Delay(__IO uint32_t nTime)
{
  TimingDelay = nTime;
  
  while(TimingDelay != 0);
}

/************************************************************************************
*������:TimingDelay_Decrement
*����˵��:�Լ�����
*�������:��
*
*����ֵ:��
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
*������:assert_failed
*����˵��:������������λ��
*	
*�������:
*		file:�������ڵ��ļ���
*		line:���������ļ��е��к�
*	
*����ֵ:��	
*****************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
 /****�˴���������û����󱨸淽ʽ**********************/

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
