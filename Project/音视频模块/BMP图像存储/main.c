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

/* ͷ�ļ�------------------------------------------------------------------*/
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
/**********�궨��*********************************/
#define BUFF  0x68000010  /*ͼ�����ݻ�����*/
/***********BMPͼ������ͷ********************************/
u8 tmp[66]={0x4d,0x42,0x42,0x58,0x02,0x00,  //"BM �ļ���С"
		0x00,0x00,0x00,0x00,0x42,0x00,0x00,0x00,//"ƫ����"
		0x28,0x00,0x00,0x00,0x40,0x01,0x00,0x00,//"�ṹ�������ֽ�����ͼ����"
		0xf0,0x00,0x00,0x00,0x01,0x00,0x10,0x00,//"ͼ��߶ȣ�Ŀ���豸��������"
		0x03,0x00,0x00,0x00,0x00,0x58,0x02,0x00,//"ѹ��ϵ����ͼƬ��С"
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0xf8,0x00,0x00,0xE0,0x07,0x00,0x00,
        0x1f ,0x00,0x00,0x00};
/*********ȫ�ֱ�������**********************************/
FATFS filesystem;		/* �ļ�ϵͳ��� */
FRESULT ret;			/* �ļ���������ֵ */
FIL file;				/* �ļ�*/
UINT bw, br;
/***************************************************
*������:main
*�������:��
*����ֵ:��
*����˵��:������
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
  STM3220F_LCD_Init();/*LCD��ʼ��*/
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
  Ov9655_I2C_Configuration();/*I2C��ʼ��*/
  Ov9655_Dcmi_GPIO_Configuration();/*DCMI�˿ڳ�ʼ��*/
  Ov9655_DMA_Configuration(); /*DMA��ʼ��*/
  Ov9655_Dcmi_Configuration(); /*DCMI����*/
  DMA_Cmd(DMA2_Stream1, ENABLE); /*DMAʹ��*/	  
  DCMI_Cmd(ENABLE);				 /*DCMIʹ��*/
  DCMI->CR |=0x1; 				 /*ʹ��ͼ����׽*/
  memcpy(p,tmp,66);
  InitKey();
  while (1)
  {
		key=GetKeyNum();
		if(key==KEY01_DOWN)
		{
			DMA_Cmd(DMA2_Stream1, DISABLE);
			DCMI_Cmd(DISABLE);				 /*DCMIʹ��*/
            DMA_Cmd(DMA2_Stream1, DISABLE);
			DCMI->CR &=0xFFFFFFFE; 				 /*ʹ��ͼ����׽*/
			Ov9655_DMA1_Configuration();

			DMA_Cmd(DMA2_Stream1, ENABLE); /*DMAʹ��*/
			DCMI_Cmd(ENABLE);				 /*DCMIʹ��*/
			DCMI->CR |=0x1; 				 /*ʹ��ͼ����׽*/
			delay_ms(1000);
			DCMI->CR &=0xFFFFFFFE; 
			LCD_Clear(White);
			LCD_WriteBMP((uint16_t *)(p+66));
			ret = f_open(&file, "test.bmp", FA_WRITE | FA_CREATE_ALWAYS); /*���ļ���û���򴴽�*/
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


/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
