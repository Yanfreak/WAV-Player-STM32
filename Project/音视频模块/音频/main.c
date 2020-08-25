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
#include "stm322xg_eval_sram.h"
/***********��Ƶ�ļ��궨��********************************/
#define AUDIO_FILE_SZE          990000/2    /*�ļ���С*/
#define AUIDO_START_ADDRESS     58 /*��Ƶ������ʼƫ����*/
#define  buffer   	((uint8_t*)0x68000100) //�ⲿsram
#define  sizemax     3//����5��1m�������ļ�
/************ȫ�ֱ�������*******************************/
__IO uint32_t cmd = AUDIO_PAUSE;/*����״̬*/
__IO uint32_t volume = 100;	 /*������С*/
USART_InitTypeDef USART_InitStructure;	   /* ���ڳ�ʼ���ṹ��*/
extern uint32_t NbrOfDataToRead;			   /* ��¼����������ݵ�����*/
extern __IO uint32_t RxCounter; 		   /* ��¼�ѽ�������*/
NAND_IDTypeDef NAND_ID;
NAND_ADDRESS WriteReadAddr;
u16 Buffer1[0x1000];/*���ݻ�����*/
u16 BLOCKPOS[3];
u16 BLOCKsize[3];
void delay_Nms(unsigned short Nms); 
static __IO uint32_t TimingDelay; /*��ʱ��С*/
void NVIC_Config(void);
RCC_ClocksTypeDef RCC_Clocks;  /*ϵͳʱ��Ƶ��*/
u8 BufFlag=0;
vu32 PageNumber = 64;
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
	uint16_t j;
	uint8_t i;
	uint8_t now=0;
	uint8_t  maxnum=1;
 NAND_IDTypeDef NAND_ID;
	unsigned char temp[30];
  STM3220F_LCD_Init();	   /*LCD��ʼ��*/
  LCD_Clear(Blue);  	   /*����*/
  LCD_SetBackColor(Blue);  /*���ñ���ɫ*/
  LCD_SetTextColor(Yellow);/*����ǰ��ɫ*/
	/*********systick��ʼ��********************************/
 RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
		SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
  /* ��ʼ����ƵоƬ����ص����� (I2S, I2C) */  
	NVIC_Config();	/* NVIC���ã���USART1�ж�*/
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
		LCD_Clear(Blue);  	   /*����*/
		WriteReadAddr.Block=0;
	/* USART1 ���� ------------------------------------------------------*/
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

  STM_EVAL_COMInit(COM1, &USART_InitStructure);	 /* ����USART_InitStructure�������ã����ô���USART1 */
	BLOCKPOS[0]=0;
	for(j=0;j<12;j++){
		/* ʹ�ܴ���USART1�Ľ����жϣ���USART1�Ľ������ݼĴ���Ϊ�ǿ�ʱ��������������ж�*/
  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);
  
  /* �ȴ��������ݣ�������洢��RxBuffer�У�ֱ������������ΪNbrOfDataToReadʱֹͣ��*/
  while(RxCounter < NbrOfDataToRead)
  {}
 FSMC_NAND_WriteSmallPage(buffer, WriteReadAddr, PageNumber);	
			WriteReadAddr.Page=0x00;
			WriteReadAddr.Block+=1;
		RxCounter=0;
	}
	BLOCKsize[0]=	WriteReadAddr.Block;
	 Keypad_Interrupt(0,0,1);
   InitKey();			   /*������ʼ��*/
 if (EVAL_AUDIO_Init(OUTPUT_DEVICE_AUTO, volume, I2S_AudioFreq_48k) == 0)
  {

    LCD_DisplayStringHZ(40,40, "����1: ��    ��");
		LCD_DisplayStringHZ(40,60, "����2: ��    ͣ");
    LCD_DisplayStringHZ(40,80, "����3: �� һ ��");
		LCD_DisplayStringHZ(40,100,"����4: �� һ ��");
    LCD_DisplayStringHZ(40,120,"����5: ������С");
		LCD_DisplayStringHZ(40,140,"����6: ��������");
    LCD_DisplayStringHZ(40,160,"����7: ͣ    ֹ"); 
  }
  else
  {
    LCD_DisplayStringHZ(40,200, " ��Ƶ ERROR ");
  }

  /**********��ʼ������Ƶ���ݣ�DMA��ʽ*********************************/
//EVAL_AUDIO_Play((uint16_t*)(AUDIO_SAMPLE + AUIDO_START_ADDRESS), (AUDIO_FILE_SZE - AUIDO_START_ADDRESS));
	/*��LCD����ʾ״̬��Ϣ */
	EVAL_AUDIO_Play(Buffer1,4096);
	LCD_DisplayStringHZ(40,200, "�� ��");
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
		 LCD_DisplayStringHZ(40,200, " �� �� ");
break;			
			case KEY02_DOWN: 
				if(cmd==AUDIO_STOP);
		else{		 cmd=AUDIO_PAUSE;
       LCD_DisplayStringHZ(40,200, " �� ͣ ");}	
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
			 LCD_DisplayStringHZ(40,200,  " ͣ  ֹ ");
break;
			
			case KEY09_DOWN://ȷ�ϼ�
break;
			case KEY10_DOWN:
				  EVAL_AUDIO_PauseResume(AUDIO_PAUSE);
			 LCD_DisplayStringHZ(40,200,  " ��  ��   ");
				WriteReadAddr.Block=BLOCKPOS[maxnum-1]+BLOCKsize[maxnum-1];
			   	maxnum+=1;
				for(j=0;j<12;j++){
		/* ʹ�ܴ���USART1�Ľ����жϣ���USART1�Ľ������ݼĴ���Ϊ�ǿ�ʱ��������������ж�*/
  USART_ITConfig(EVAL_COM1, USART_IT_RXNE, ENABLE);
  
  /* �ȴ��������ݣ�������洢��RxBuffer�У�ֱ������������ΪNbrOfDataToReadʱֹͣ��*/
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
	 LCD_DisplayStringHZ(40,200, " �� �� ");
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
  SysTick->LOAD=(unsigned int)Nms*15000;  //���뼶�� 
  SysTick->CTRL|=0x01;      //ʹ��SysTick 
  while(!(SysTick->CTRL&(1<<16)));  //�ȴ�������0 
  SysTick->CTRL=0x00000004;    //�ر�SysTick 
  SysTick->VAL=0x00000000;    //��ռ����� 
} 
/******************************************************************************
* �������� NVIC_Config
* ��  �ܣ� ���ô���1���ж����ͣ����򿪴���1�ķ��ͺͽ����жϹ���
* ��  �룺�� 
* ��  ������  
* ��  ע����
********************************************************************************/
void NVIC_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	/* �� USART1 �ж����� */
	NVIC_InitStructure.NVIC_IRQChannel = EVAL_COM1_IRQn;  /* �趨ΪUSART1�ж� */
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  /* �趨�����ȼ�Ϊ0*/
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		   /* �趨�����ȼ�Ϊ0*/
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			   /* �жϴ�*/
	NVIC_Init(&NVIC_InitStructure);
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
