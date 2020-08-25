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
#include "stm322xg_eval_lcd.h"
#include "stm322xg_eval_keypad.h"
#include "stm322xg_eval_nand.h"
/**************������Ϣ�궨��*****************************/
#define MESSAGE1   "  EITP¼������  " 
#define MESSAGE2   " Device running on  " 
#define MESSAGE3   "�������촴�¿Ƽ���չ���޹�˾" 
/***********NAND FLASH ID********************************/
#define NAND_SAMSUNG_MakerID     0xEC
#define NAND_SAMSUNG_DeviceID    0xF1   /* K9F1G08U0A,  128MB */
/************ȫ�ֱ�������*******************************/
__IO uint32_t cmd = AUDIO_PAUSE;/*����״̬*/
 extern unsigned char KeyCode;	 /*����ֵ*/
__IO uint32_t volume = 126;	 /*������С*/

static __IO uint32_t TimingDelay; /*��ʱ��С*/
RCC_ClocksTypeDef RCC_Clocks;  /*ϵͳʱ��Ƶ��*/

u16 Buffer1[0x1000];/*���ݻ�����*/
u8 BufFlag=0;     /*��ȡ��־*/
NAND_ADDRESS WriteReadAddr;
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
  NAND_IDTypeDef NAND_ID;
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDOff(LED2);
  STM3220F_LCD_Init();	   /*LCD��ʼ��*/
  InitKey();			   /*������ʼ��*/
  LCD_Clear(Blue);  	   /*����*/
  LCD_SetBackColor(Blue);  /*���ñ���ɫ*/
  LCD_SetTextColor(Yellow);/*����ǰ��ɫ*/
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
  /* ��ʼ����ƵоƬ����ص����� (I2S, I2C) */  
  if (EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO, volume, I2S_AudioFreq_48k) == 0)
  {
    LCD_DisplayStringHZ(40,200, " ��Ƶ OK  "); 
    LCD_DisplayStringHZ(40,80," ¼��������... ");   
  }
  else
  {
    LCD_DisplayStringHZ(40,200, " ��Ƶ ERROR ");
  }
  /**********��ʼ������Ƶ���ݣ�DMA��ʽ*********************************/
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
 // EVAL_AUDIO_Play((uint16_t*)(AUDIO_SAMPLE + AUIDO_START_ADDRESS), (AUDIO_FILE_SZE - AUIDO_START_ADDRESS));
  
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
