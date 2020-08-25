 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� stm322xg_eval_lcd.c
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
/*ͷ�ļ�----------------------------------------------------*/
#include <stdio.h>
#include "stm32f2xx.h"
#include "stm322xg_eval_lcd.h"
#include "stm32_eval.h"
#include "fonts.h"
#include "logo.h"
#include "fonts1.h"


/*����LCD�������� -----------------------------------------------------------*/
typedef struct
{
	__IO uint16_t LCD_REG;
	__IO uint16_t LCD_RAM;
}
LCD_TypeDef;

/* ����LCD�������ķ��ʵ�ַ */
#define LCD_BASE        ((uint32_t)(0x60000002 | 0x0C000000))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

static sFONT *LCD_Currentfonts;	/*��������*/
 __IO uint16_t s_TextColor = 0x0000;	/*ǰ��ɫ*/
 __IO uint16_t s_BackColor = 0xFFFF;    /*����ɫ*/

/* ����͸����ʶλ */
static __IO uint8_t s_Transparent = 0;
#define LCD_PIXEL_WIDTH          0x0140	 /*Һ�����*/
#define LCD_PIXEL_HEIGHT         0x00F0	 /*Һ���߶�*/
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;
/***************************************************
*������:LCD_Delay
*�������:
*	  	nCount����ʱ����
*����ֵ: ��
*����˵��: LCD��ʱ����
*
****************************************************/
void LCD_Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}


/***************************************************
*������:Lcd_Write_Com
*�������:
*     LCD_Reg��LCd����Ĵ���
*����ֵ: ��
*����˵��: LCDд����Ĵ���
*
****************************************************/
void Lcd_Write_Com(u16 LCD_Reg)
{
	 LCD->LCD_REG = LCD_Reg;
}

/***************************************************
*������:STM3220F_LCD_Init
*�������:��
*����ֵ: ��
*����˵��: LCD��ʼ������
*
****************************************************/
void STM3220F_LCD_Init(void)
{
	/* ����LCD���ƿ���GPIO */
	LCD_CtrlLinesConfig();

	/* ����FSMC�ӿڣ��������� */
	LCD_FSMCConfig();

	/* FSMC���ú������ӳٲ��ܷ��������豸  */
	LCD_Delay(200000);

    LCD_WriteReg(0x0000,0x0001);	//�򿪾���
	LCD_Delay(10);
    LCD_WriteReg(0x0003,0xA8A4);	//0xA8A4
	LCD_Delay(10);
    LCD_WriteReg(0x000C,0x0000);
	LCD_Delay(10);   
    LCD_WriteReg(0x000D,0x080C);
	LCD_Delay(10);   
    LCD_WriteReg(0x000E,0x2B00);
	LCD_Delay(10);   
    LCD_WriteReg(0x001E,0x00B0);
	LCD_Delay(10);   
    LCD_WriteReg(0x0001,0x6B3F);	//�����������320*240  0x6B3F 0x2B3F
	LCD_Delay(10);
    LCD_WriteReg(0x0002,0x0600);
	LCD_Delay(10);
    LCD_WriteReg(0x0010,0x0000);
	LCD_Delay(10);
    LCD_WriteReg(0x0011,0x6068);	//�������ݸ�ʽ  16λɫ 
	LCD_Delay(10);
    LCD_WriteReg(0x0005,0x0000);
	LCD_Delay(10);
    LCD_WriteReg(0x0006,0x0000);
	LCD_Delay(10);
    LCD_WriteReg(0x0016,0xEF1C);
	LCD_Delay(10);
    LCD_WriteReg(0x0017,0x0003);
	LCD_Delay(10);
    LCD_WriteReg(0x0007,0x0233);
	LCD_Delay(10);
    LCD_WriteReg(0x000B,0x0000);
	LCD_Delay(10);
    LCD_WriteReg(0x000F,0x0000);	//ɨ�迪ʼ��ַ
	LCD_Delay(10);
    LCD_WriteReg(0x0041,0x0000);
	LCD_Delay(10);
    LCD_WriteReg(0x0042,0x0000);
	LCD_Delay(10);
    LCD_WriteReg(0x0048,0x0000);
	LCD_Delay(10);
    LCD_WriteReg(0x0049,0x013F);
	LCD_Delay(10);
    LCD_WriteReg(0x004A,0x0000);
	LCD_Delay(10);
    LCD_WriteReg(0x004B,0x0000);
	LCD_Delay(10);
    LCD_WriteReg(0x0044,0xEF00);
	LCD_Delay(10);
    LCD_WriteReg(0x0045,0x0000);
	LCD_Delay(10);
    LCD_WriteReg(0x0046,0x013F);
	LCD_Delay(10);
    LCD_WriteReg(0x0030,0x0707);
	LCD_Delay(10);
    LCD_WriteReg(0x0031,0x0204);
	LCD_Delay(10);
    LCD_WriteReg(0x0032,0x0204);
	LCD_Delay(10);
    LCD_WriteReg(0x0033,0x0502);
	LCD_Delay(10);
    LCD_WriteReg(0x0034,0x0507);
	LCD_Delay(10);
    LCD_WriteReg(0x0035,0x0204);
	LCD_Delay(10);
    LCD_WriteReg(0x0036,0x0204);
	LCD_Delay(10);
    LCD_WriteReg(0x0037,0x0502);
	LCD_Delay(10);
    LCD_WriteReg(0x003A,0x0302);
	LCD_Delay(10);
    LCD_WriteReg(0x003B,0x0302);
	LCD_Delay(10);
    LCD_WriteReg(0x0023,0x0000);
	LCD_Delay(10);
    LCD_WriteReg(0x0024,0x0000);
	LCD_Delay(10);
    LCD_WriteReg(0x0025,0x8000);
	LCD_Delay(10);
    LCD_WriteReg(0x004f,0x0000);	//����ַ0
	LCD_Delay(10);
    LCD_WriteReg(0x004e,0x0000);	//����ַ0
	LCD_Delay(10);

    Lcd_Write_Com(0x0022);

	LCD_WriteReg(0x00, 0x0001);
	LCD_WriteReg(0x02, 0x0600); 
	LCD_WriteReg(0x0C, 0x0007);
	LCD_WriteReg(0x0D, 0x0006); 
	LCD_WriteReg(0x0E, 0x3200);
	LCD_WriteReg(0x1E, 0x00BB);
	LCD_WriteReg(0x03, 0x6A64); 

	LCD_WriteReg(0x0F, 0x0000);
	LCD_WriteReg(0x44, 0xEF00);
	LCD_WriteReg(0x45, 0x0000);
	LCD_WriteReg(0x46, 0x013F);
	LCD_WriteReg(0x30, 0x0000);
	LCD_WriteReg(0x31, 0x0706);
	LCD_WriteReg(0x32, 0x0206);
	LCD_WriteReg(0x33, 0x0300);
	LCD_WriteReg(0x34, 0x0002);
	LCD_WriteReg(0x35, 0x0000);
	LCD_WriteReg(0x36, 0x0707);
	LCD_WriteReg(0x37, 0x0200);
	LCD_WriteReg(0x3A, 0x0908);
	LCD_WriteReg(0x3B, 0x0F0D);


	LCD_WriteReg(0x28, 0x0006);
	LCD_WriteReg(0x2F, 0x12EB);
	LCD_WriteReg(0x26, 0x7000);
	LCD_WriteReg(0x20, 0xB0E3); 
	LCD_WriteReg(0x27, 0x0044); 
	LCD_WriteReg(0x2E, 0x7E45);

	LCD_WriteReg(0x10, 0x0000);
	LCD_Delay(10);
  
}

/***************************************************
*������:LCD_SetTextColor
*�������:
*     Color:��ɫ����
*����ֵ: ��
*����˵��: ����ǰ����ɫ
*
****************************************************/
void LCD_SetTextColor(__IO uint16_t Color)
{
	s_TextColor = Color;
}

/***************************************************
*������:LCD_SetBackColor
*�������:
*     Color:��ɫ����
*����ֵ: ��
*����˵��: ���ñ���ɫ
*
****************************************************/
void LCD_SetBackColor(__IO uint16_t Color)
{
	s_BackColor = Color;
}

/***************************************************
*������:LCD_ClearLine
*�������:
*      Line��LCD������
*����ֵ:��
*����˵��:���ָ������Line 0---240
*
****************************************************/
void LCD_ClearLine(uint8_t Line)
{
	LCD_DisplayStringHZ(0, Line, "                                         ");
}


/***************************************************
*������:LCD_Clear
*�������:
     Color: ��ɫ����
*����ֵ: ��
*����˵��: ��ָ����ɫ����
*
****************************************************/
void LCD_Clear(uint16_t Color)
{
	uint32_t index = 0;

	LCD_SetCursor(0, 0);	/* ���ù��λ�� */

	LCD_WriteRAM_Prepare(); 	/* ׼��д�Դ� */

	for (index = 0; index < 320 * 240; index++)
	{
		LCD->LCD_RAM = Color;
	}
}


/***************************************************
*������:LCD_SetCursor
*�������:				   -----------------
*	   Xpos:LCD ������	   |	|		   |
*	   Ypos:LCD ������	   |	|		   |
*����ֵ:��				   |----|(x,y)     |
*����˵��:���ù��λ��	   |			   |
*						   |---------------|
****************************************************/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	LCD_WriteReg(0x4e, Ypos);
	LCD_WriteReg(0x4f, Xpos);
}

/***************************************************
*������:LCD_SetTransparent
*�������:_mode
*����ֵ: ��
*����˵��: ���ñ����Ƿ�͸��
*
****************************************************/
void LCD_SetTransparent(uint8_t _mode)
{
	if (_mode == 0)
	{
		s_Transparent = 0;	/* ����Ϊ������͸�� */
	}
	else
	{
		s_Transparent = 1;	/* ����Ϊ����͸�� */
	}
}
/***************************************************
*������:LCD_SetFont
*�������:
*		fonts����������
*����ֵ:��
*����˵��:����Ҫ��ʾ���ı�����
*
****************************************************/
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}
/***************************************************
*������:LCD_GetFont
*�������:��
*����ֵ:
*      sFONT����������
*����˵��:���ص�ǰ��ʾ�ı�����������
*
****************************************************/
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}

/*******************************************************
* ������:LCD_DrawChar  
* ���������
*		Xpos	: X����
*		Ypos	: Y����
*		ch		: �ַ�
*		width	: �ַ����,Ӣ���ַ���8,������16
* ����ֵ����
* ����˵��:��һ���ַ�
**********************************************************/
void LCD_DrawChar(uint16_t Xpos, uint16_t Ypos, uint8_t ch , uint8_t width)
{
	uint32_t index = 0, i = 0;
	uint16_t *c;
	uint8_t code1;
	uint8_t Yaddress;
	Yaddress = Ypos;

	
	if(ch<' ')
	{
		return;
	}

	code1 = ch-' ';	/* ascii���� */
	
	if(ch >=0x80)  
	{
		return;  /*�����Ǻ���,����*/
	}
	c = (uint16_t *)&LCD_Currentfonts->table[code1 * LCD_Currentfonts->Height];

	LCD_SetCursor(Xpos, Ypos);

	for(index = 0; index < LCD_Currentfonts->Height; index++)
	{
		LCD_WriteRAM_Prepare(); /* ׼��д������*/
		for(i = 0; i < LCD_Currentfonts->Width; i++)
		{

			if((((c[index] & ((0x80 << ((LCD_Currentfonts->Width / 12 ) * 8 ) ) >> i)) == 0x00) &&(LCD_Currentfonts->Width <= 12))||
			(((c[index] & (0x1 << i)) == 0x00)&&(LCD_Currentfonts->Width > 12 )))

			{
				LCD_WriteRAM(s_BackColor);
			}
			else
			{
				LCD_WriteRAM(s_TextColor);
			} 
	}
	Yaddress++;
	LCD_SetCursor(Xpos, Yaddress);
 }
}
	
/*****************************************************
* ������:LCD_DrawCharHZ  
* �������:
*		Xpos	: X����
*		Ypos	: Y����
*		c		: ָ���ַ������ָ��
*		width	: �ַ����,Ӣ���ַ���8,������16
* ����ֵ: None
* ����˵��:��һ���ַ�,֧�ֺ���
******************************************************/
void LCD_DrawCharHZ(uint16_t Xpos, uint16_t Ypos, const uint8_t *c, uint8_t width)
{
	uint32_t index = 0, i = 0;
	uint8_t Yaddress;

	Yaddress = Ypos;

	LCD_SetCursor(Xpos, Ypos);

	if (s_Transparent == 0)	//������͸��
	{
		for (index = 0; index < 16; index++)	/* �ַ��߶� */
		{
			LCD_WriteRAM_Prepare();
			for (i = 0; i < width; i++)	/* �ַ���� */
			{
				if ((c[index * (width / 8) + i / 8] & (0x80 >> (i % 8))) == 0x00)
				{
					LCD_WriteRAM(s_BackColor);
				}
				else
				{
					LCD_WriteRAM(s_TextColor);
				}
			}
			Yaddress++;
			LCD_SetCursor(Xpos, Yaddress);
		}
	}
	else	//����͸��
	{
		for (index = 0; index < 16; index++)	/* �ַ��߶� */
		{
			for (i = 0; i < width; i++)	/* �ַ���� */
			{
				if ((c[index * (width / 8) + i / 8] & (0x80 >> (i % 8))) != 0x00)
				{

					LCD_WriteRAM_Prepare();
					LCD_WriteRAM(s_TextColor);
				}
			}
			Yaddress++;
			LCD_SetCursor(Xpos, Yaddress);
		}
	}
}
/*******************************************
* ������: LCD_DisplayStringHZ 
* ���������
*		Xpos	: X����
*		Ypos	: Y����
*		c		: �ַ���ָ��
* ����ֵ�� ��
*����˵������һ���ַ���	֧�ֺ����ִ�
*********************************************/
void LCD_DisplayStringHZ(uint16_t Xpos, uint16_t Ypos, uint8_t *ptr)
{
	uint32_t i = 0;
	uint8_t code1;
	uint8_t code2;
	uint32_t address;
	uint32_t k;

	while ((*ptr != 0) & (i < 40))
	{
		code1 = *ptr;	/* ascii���� */
		if (code1 < 0x80)
		{
			LCD_DrawCharHZ(Xpos, Ypos, &Ascii16[code1 * 16], 8);	/* 16 ��ʾ1���ַ���ģ���ֽ��� */
			Xpos += 8;		/* �е�ַ+8 */
		}
		else	/* �������� */
		{
			code2 = *++ptr;
			if (code2 == 0)
			{
				break;
			}

				for (k = 0; k < HZ_COUNT; k++)
				{
					address = k * 34;

					if ((code1 == g_Hz16[address + 0]) && (code2 == g_Hz16[address + 1]))
					{
						address += 2;
						break;
					}				
				}
			    address = (uint32_t)&g_Hz16[address];

			LCD_DrawCharHZ(Xpos, Ypos, (const uint8_t *)address, 16);
			Xpos += 16;		/* �е�ַ+16 */
		}
		ptr++;			/* ָ����һ���ַ� */
		i++;
	}
}
/*******************************************
* ��������LCD_DisplayString  
* ���������
*		Xpos	: X����
*		Ypos	: Y����
*		c		: �ַ���ָ��
* ����ֵ�� None
*����˵������һ���ַ���
**************************************************/
void LCD_DisplayString(uint16_t Xpos, uint16_t Ypos, uint8_t *ptr)
{
	uint32_t i = 0;

	while ((*ptr != 0) & (i < 40))
	{
		LCD_DrawChar(Xpos, Ypos, *ptr, LCD_Currentfonts->Width);	/* 16 ��ʾ1���ַ���ģ���ֽ��� */
		Xpos += LCD_Currentfonts->Width;		/* �е�ַ+8 */
		ptr++;			/* ָ����һ���ַ� */
		i++;
	}
}

/*******************************************
* ��������LCD_DisplayChar  
* ���������
*		Xpos	: X����
*		Ypos	: Y����
*		c		: �ַ�
* ����ֵ�� None
*����˵������һ���ַ�
*��ע:���ݾ�ST����Ӧ��
**************************************************/
void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t c)
{
	LCD_DrawChar(Ypos, Xpos, c, LCD_Currentfonts->Width);	/*  */
}

/*********************************************************
* ������: LCD_DrawCharLargeFont 
* �������:
*		Xpos	: X����
*		Ypos	: Y����
*		c : ָ���ַ������ָ��
*		width : �ַ����
* ����ֵ: None
* ����˵��:��һ�����ַ�
**********************************************************/
void LCD_DrawCharLargeFont(uint16_t Xpos, uint16_t Ypos, const uint8_t *c, uint8_t width)
{
	uint32_t index = 0, i = 0;
	uint8_t Yaddress;

	Yaddress = Ypos;

	LCD_SetCursor(Xpos, Ypos);

	if (s_Transparent == 0)	//������͸��
	{
		for (index = 0; index < 64; index++)	/* �ַ��߶� */
		{
			LCD_WriteRAM_Prepare();
			for (i = 0; i < width; i++)	/* �ַ���� */
			{
				if ((c[index * (width / 8) + i / 8] & (0x80 >> (i % 8))) == 0x00)
				{
					LCD_WriteRAM(s_BackColor);
				}
				else
				{
					LCD_WriteRAM(s_TextColor);
				}
			}
			Yaddress++;
			LCD_SetCursor(Xpos, Yaddress);
		}
	}
	else	//����͸��
	{
		for (index = 0; index < 64; index++)	/* �ַ��߶� */
		{
			for (i = 0; i < width; i++)	/* �ַ���� */
			{
				if ((c[index * (width / 8) + i / 8] & (0x80 >> (i % 8))) != 0x00)
				{
					LCD_WriteRAM_Prepare();
					LCD_WriteRAM(s_TextColor);
				}
			}
			Yaddress++;
			LCD_SetCursor(Xpos, Yaddress);
		}
	}
}
/**************************************************
* ��������LCD_DisplayStringLargeFont
* ���������
*		Xpos	: X����
*		Ypos	: Y����
*		c : ָ���ַ������ָ��
*		width : �ַ����
* ����ֵ����
*  ����˵������һ�����ַ����ַ���
*/
void LCD_DisplayStringLargeFont(uint16_t Xpos, uint16_t Ypos, uint8_t *ptr)
{
	uint32_t i = 0;
	uint8_t code1;

	if(ptr==NULL)
		return;

	while ((*ptr != 0))
	{
		code1 = *ptr;	// ascii����
		if ((code1 > '9')||(code1 < '-'))
		{
			Xpos += 32;
		}
		else
		{
			code1 = code1-'-';

			// 256 ��ʾ1���ַ���ģ���ֽ���
			LCD_DrawCharLargeFont(Xpos, Ypos, &large_font_64[code1 * 256], 32);

			// �����'.'����'/'�ַ����Ϊ24�������ַ����Ϊ32
			if((code1<3)&&(code1!=0))
				Xpos += 24;
			else
				Xpos += 32;
		}

		ptr++;			// ָ����һ���ַ�
		i++;
	}
}

/***************************************************
*������:LCD_DisplayStringLine
*�������:
*		Line��LCD������
*		ptr������ʾ���ַ�
*����ֵ:��
*����˵��:��ָ����������ʾ�ַ�
*
****************************************************/
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr)
{
  uint16_t refcolumn = 0;
  LCD_DisplayString(refcolumn,Line,ptr);
 }

/**
  * @brief  ��Eflag LOGO
  * @param
  *		Xpos	: X����
  *		Ypos	: Y����
  * @retval None
  */
void LCD_Draw_Eflag_Logo(uint16_t x,uint16_t y)
{
	int i,j,k;
	uint16_t color;

	LCD_SetCursor(x,y);

	for (i = 0; i < 42; i++)
	{
		for(j=0;j<72;j++)
		{
			k = i*72+j;
			color = gImage_logo[k*2] | gImage_logo[k*2+1]<<8;
			LCD_WriteRAM_Prepare();
			LCD_WriteRAM(color);
		}
		y++;
		LCD_SetCursor(x, y);
	}

}
/******************************************************
* ��������LCD_DrawPoint  
* ���������
*		Xpos	: X����
*		Ypos	: Y����
*		Color	: ������ɫ
* ����ֵ����
*����˵������1������
*******************************************************/
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t Color)
{
	
	LCD_SetCursor(x, y);	// ���ù��λ��

	LCD->LCD_REG = 0x22;
	LCD->LCD_RAM = Color;
}
/**********************************************************
* ������:LCD_DrawPoints  
* ���������
*		_points	: ����������
*		_offset	: ƫ������
*		_Size	: �����
*		_Color	: ��ɫ
* ����ֵ����
*����˵������һ���
*************************************************************/
void LCD_DrawPoints(POINT_T _offset, POINT_T *_points, uint16_t _Size, uint16_t _Color)
{
#if 0
	uint16_t i;

	for (i = 0 ; i < _Size; i++)
	{
		LCD_DrawPoint(_points[i].x + _offset.x, _points[i].y + _offset.y,_Color);
	}
#else
	uint16_t i;

	for (i = 0 ; i < _Size-1; i++)
	{
		LCD_BresenhamLine(_points[i].x + _offset.x, _points[i].y + _offset.y
					,_points[i+1].x + _offset.x, _points[i+1].y + _offset.y,_Color);
	}
#endif

}

/**
  * ��������LCD_BresenhamLine 
  * ���������
  *		x1, y1	: ��ʼ������
  *		x2, y2	: ��ֹ������
  *		c		: ֱ����ɫ
  * ����ֵ����
  *	����˵���� Bresenham�㷨������仭һ��ֱ��	
  */
void LCD_BresenhamLine (uint16_t x1 , uint16_t y1 , uint16_t x2 , uint16_t y2 , int c)
{
	int dx , dy ;
	int tx , ty ;
	int inc1 , inc2 ;
	int d , iTag ;
	int x , y ;

	LCD_DrawPoint(x1 , y1 , c);

	/* ��������غϣ���������Ķ�����*/
	if ( x1 == x2 && y1 == y2 )
	{
		return;
	}

	iTag = 0 ;
	/* dx = abs ( x2 - x1 ); */
	if (x2 >= x1)
	{
		dx = x2 - x1;
	}
	else
	{
		dx = x1 - x2;
	}

	/* dy = abs ( y2 - y1 ); */
	if (y2 >= y1)
	{
		dy = y2 - y1;
	}
	else
	{
		dy = y1 - y2;
	}

	if ( dx < dy )   /*���dyΪ�Ƴ������򽻻��ݺ����ꡣ*/
	{
		uint16_t temp;

		iTag = 1 ;
		temp = x1; x1 = y1; y1 = temp;
		temp = x2; x2 = y2; y2 = temp;
		temp = dx; dx = dy; dy = temp;
	}
	tx = x2 > x1 ? 1 : -1 ;    /*ȷ������1���Ǽ�1*/
	ty = y2 > y1 ? 1 : -1 ;
	x = x1 ;
	y = y1 ;
	inc1 = 2 * dy ;
	inc2 = 2 * ( dy - dx );
	d = inc1 - dx ;
	while ( x != x2 )     /*ѭ������*/
	{
		if ( d < 0 )
		{
			d += inc1 ;
		}
		else
		{
			y += ty ;
			d += inc2 ;
		}
		if ( iTag )
		{
			LCD_DrawPoint ( y , x , c ) ;
		}
		else
		{
			LCD_DrawPoint ( x , y , c ) ;
		}
		x += tx ;
	}
	return;
}

/**************************************************
* ��������LCD_DrawLine  
* ���������
*		Xpos	: ��ʼ��X����
*		YPos	: ��ʼ��Y����
*		Length	: ����
*		Direction:����(Horizontal��Vertical)
* ����ֵ�� None
* ����˵��������ֱ�߻�ˮƽ��
**************************************************/
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
	uint32_t i = 0;

	/* �����ߵ���ʼ���� */
	LCD_SetCursor(Xpos, Ypos);

	if (Direction == Horizontal)	/* ˮƽ���� */
	{
		LCD_WriteRAM_Prepare();
		for (i = 0; i < Length; i++)
		{
			LCD_WriteRAM(s_TextColor);
		}
	}
	else	/* ��ֱ���� */
	{
		for (i = 0; i < Length; i++)
		{
			LCD_WriteRAM_Prepare();
			LCD_WriteRAM(s_TextColor);
			Ypos++;
			LCD_SetCursor(Xpos, Ypos);
		}
	}
}

/*********************************************************
* ������:  LCD_DrawRect
* �������:
*		Xpos	: ��ʼ��X����
*		YPos	: ��ʼ��Y����
*		Height	: �߶�
*		Width	: ���
* ����ֵ: None
* ����˵��:�����κ���
**********************************************************/
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
{
	LCD_DrawLine(Xpos, Ypos, Width, Horizontal);  			/* �� */
	LCD_DrawLine(Xpos, Ypos + Height, Width, Horizontal);	/* �� */

	LCD_DrawLine(Xpos, Ypos, Height, Vertical);				/* �� */
	LCD_DrawLine(Xpos + Width, Ypos, Height + 1, Vertical);	/* �� */
}
  /**************************************************
* ��������LCD_DrawLine  
* ���������
*		Xpos	: ��ʼ��X����
*		YPos	: ��ʼ��Y����
*		Length	: ����
*		Direction:����(Horizontal��Vertical)
* ����ֵ�� None
* ����˵��������ֱ�߻�ˮƽ��
**************************************************/
void LCD_DrawLine_Color(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction,uint16_t Color)
{
	uint32_t i = 0;

	/* �����ߵ���ʼ���� */
	LCD_SetCursor(Xpos, Ypos);

	if (Direction == Horizontal)	/* ˮƽ���� */
	{
		LCD_WriteRAM_Prepare();
		for (i = 0; i < Length; i++)
		{
			LCD_WriteRAM(Color);
		}
	}
	else	/* ��ֱ���� */
	{
		for (i = 0; i < Length; i++)
		{
			LCD_WriteRAM_Prepare();
			LCD_WriteRAM(Color);
			Ypos++;
			LCD_SetCursor(Xpos, Ypos);
		}
	}
}
/**********************************************
*������:LCD_DrawRectFill  
*�������:
*		x1,y1	: �����������
*		x2,y2	: �����յ�����
*		color	: �����ɫ
*����ֵ: ��
*����˵��:��һ��ʵ�ľ���
***********************************************/
void LCD_DrawRectFill(u16 x1,u16 y1,u16 x2,u16 y2,u16 color)
{
	u16 i,j;
	u16 width,height;
	u16 start_x,start_y,end_x,end_y;

	if(x2>x1)
	{
		start_x = x1;
		end_x = x2;
	}
	else
	{
		start_x = x2;
		end_x = x1;
	}

	if(y2>y1)
	{
		start_y = y1;
		end_y = y2;
	}
	else
	{
		start_y = y2;
		end_y = y1;
	}
	width = end_x-start_x;
	height = end_y-start_y;

	for(i=0;i<height;i++)
	{
		LCD_SetCursor(start_x, start_y);
		LCD_WriteRAM_Prepare();
		for(j=0;j<width;j++)
		{
			LCD_WriteRAM(color);
		}
		start_y++;
	}
}

/***********************************************
* ������: LCD_DrawCircle
* ���������
*		Xpos		: X����
*		YPos		: Y����
*		Radius		: �뾶
* ����ֵ����
*����˵��:��Բ����
**********************************************/
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
	int32_t  D;			/* Decision Variable */
	uint32_t  CurX;		/* ��ǰ X ֵ */
	uint32_t  CurY;		/* ��ǰ Y ֵ */

	D = 3 - (Radius << 1);
	CurX = 0;
	CurY = Radius;

	while (CurX <= CurY)
	{
		LCD_SetCursor(Xpos + CurX, Ypos + CurY);
		LCD_WriteRAM_Prepare(); /* ׼��д GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos + CurX, Ypos - CurY);
		LCD_WriteRAM_Prepare(); /* ׼��д  GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos - CurX, Ypos + CurY);
		LCD_WriteRAM_Prepare(); /* ׼��д  GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos - CurX, Ypos - CurY);
		LCD_WriteRAM_Prepare(); /* ׼��д  GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos + CurY, Ypos + CurX);
		LCD_WriteRAM_Prepare(); /* ׼��д  GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos + CurY, Ypos - CurX);
		LCD_WriteRAM_Prepare(); /* ׼��д  GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos - CurY, Ypos + CurX);
		LCD_WriteRAM_Prepare(); /* ׼��д  GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos - CurY, Ypos - CurX);
		LCD_WriteRAM_Prepare(); /* ׼��д  GRAM */
		LCD_WriteRAM(s_TextColor);

		if (D < 0)
		{
			D += (CurX << 2) + 6;
		}
		else
		{
			D += ((CurX - CurY) << 2) + 10;
			CurY--;
		}
		CurX++;
	}
}

/*********************************************
* ������: LCD_DrawMonoPict
* �������:
*		Pict		: ͼƬ����ָ��
* ����ֵ����
*����˵��: ����ɫͼƬ
***********************************************/
void LCD_DrawMonoPict(const uint32_t *Pict)
{
	uint32_t index = 0, i = 0;

	LCD_SetCursor(0, 320);

	LCD_WriteRAM_Prepare(); /* ׼��д GRAM */
	for (index = 0; index < 2400; index++)
	{
		for (i = 0; i < 32; i++)
		{
			if ((Pict[index] & (1 << i)) == 0x00)
			{
				LCD_WriteRAM(s_BackColor);
			}
			else
			{
				LCD_WriteRAM(s_TextColor);
			}
		}
	}
}

/*******************************************
* ������: LCD_WriteBMP
* ���������
*		ptr			: ͼƬ����ָ��
* ����ֵ����
* ����˵������λͼ
************************************************/
void LCD_WriteBMP(const uint16_t *ptr)
{
	uint32_t index = 0;
	const uint16_t *p;
	LCD_SetCursor(0, 0);
	LCD_WriteRAM_Prepare();

	p = ptr;
	for (index = 0; index < 320 * 240; index++)
	{
		LCD->LCD_RAM = *p++;
	}

}
/************************************************************************************
*������:LCD_SetDisplayWindow
*����˵��:����LCD��ʾ����
*�������:
*		 Xpos��X����
*		 Ypos  Y����
*		 Height�����ڸ߶�
*		 Width�����ڿ��
*����ֵ:��
*
*
**************************************************************************************/
void LCD_SetDisplayWindow(uint16_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
{
  if(Xpos >= Height)
  {
    LCD_WriteReg(0x50, (Xpos - Height + 1));
  }
  else
  {
    LCD_WriteReg(0x50, 0);
  }
  LCD_WriteReg(0x51, Xpos);
  if(Ypos >= Width)
  {
    LCD_WriteReg(0x52, (Ypos - Width + 1));
  }  
  else
  {
    LCD_WriteReg(0x52, 0);
  }
  LCD_WriteReg(0x53, Ypos);
  LCD_SetCursor(Xpos, Ypos);
}
/**********************************************
* ��������  LCD_DrawFullRect
* ���������
*		Xpos	: x����
*		Ypos	: y����
*		Height	�����ο�߶�
*		Width	�����ο���
* ����ֵ����
*����˵���� ��һ�����������ο�
*************************************************/
void LCD_DrawFullRect(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height)
{
  LCD_SetTextColor(s_TextColor);

  LCD_DrawLine(Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);
  LCD_DrawLine((Xpos + Height), Ypos, Width, LCD_DIR_HORIZONTAL);
  
  LCD_DrawLine(Xpos, Ypos, Height, LCD_DIR_VERTICAL);
  LCD_DrawLine(Xpos, (Ypos - Width + 1), Height, LCD_DIR_VERTICAL);

  Width -= 2;
  Height--;
  Ypos--;

  LCD_SetTextColor(s_BackColor);

  while(Height--)
  {
    LCD_DrawLine(++Xpos, Ypos, Width, LCD_DIR_HORIZONTAL);    
  }

  LCD_SetTextColor(s_TextColor);
}


/**********************************************
* ��������  LCD_WriteReg
* ���������
*		LCD_Reg			: �Ĵ�����ַ
*		LCD_RegValue	: �Ĵ���ֵ
* ����ֵ����
*����˵���� �޸�LCD�������ļĴ�����ֵ
*************************************************/
void LCD_WriteReg(__IO uint16_t LCD_Reg, uint16_t LCD_RegValue)
{
	LCD->LCD_REG = LCD_Reg;
	LCD->LCD_RAM = LCD_RegValue;
}

/******************************************
* ������: LCD_ReadReg
* ���������
*		LCD_Reg		: �Ĵ�����ַ
* ����ֵ�� �Ĵ�����ֵ
*����˵������LCD�������ļĴ�����ֵ
************************************************/
uint16_t LCD_ReadReg(__IO uint16_t LCD_Reg)
{

	LCD->LCD_REG = LCD_Reg;

	return (LCD->LCD_RAM);
}

/************************************************
* ������: LCD_WriteRAM_Prepare
* �����������
* ����ֵ����
* ����˵���� �����Դ�Ĵ�����ַ
************************************************/
void LCD_WriteRAM_Prepare(void)
{
	LCD->LCD_REG = 0x22;
}

/*********************************************
* ��������LCD_WriteRAM
* ���������
*		RGB_Code	: ��ɫ����
*����ֵ����
*����˵����д�Դ棬�Դ��ַ�Զ�����
**********************************************/
void LCD_WriteRAM(uint16_t RGB_Code)
{

	LCD->LCD_RAM = RGB_Code;
}

/**********************************************
* ��������LCD_ReadRAM 
* �����������
* ����ֵ�� �Դ�����
*����˵���� ���Դ棬��ַ�Զ�����
*/
uint16_t LCD_ReadRAM(void)
{

	return LCD->LCD_RAM;
}

/*************************************************
* ��������LCD_CtrlLinesConfig  
* �����������
* ����ֵ����
*����˵��������LCD���ƿ��ߣ�FSMC�ܽ�����Ϊ���ù���
*/
void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* ����GPIOʱ�Ӻ͸���ʱ�� */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD |RCC_AHB1Periph_GPIOC |RCC_AHB1Periph_GPIOF |  RCC_AHB1Periph_GPIOE, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |
                                GPIO_Pin_10 | GPIO_Pin_14 | GPIO_Pin_15 | 
                                GPIO_Pin_4 |GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;

  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource4, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 |
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
                                GPIO_Pin_15;
  GPIO_Init(GPIOE, &GPIO_InitStructure);

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource7 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource8 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource9 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource10 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource11 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource12 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource13 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource14 , GPIO_AF_FSMC);
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource15 , GPIO_AF_FSMC);
 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;    
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource12, GPIO_AF_FSMC);

  
  /* ���� PF.00(A0 (RS))  Ϊ����������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_FSMC);

}

/***************************************************
*������:LCD_FSMCConfig
*�������:��
*����ֵ: ��
*����˵��: FSMC��������
*
****************************************************/
void LCD_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;
   
  /* ʹ��FSMC����ʱ��*/
  RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG , ENABLE);

  p.FSMC_AddressSetupTime = 1;
  p.FSMC_AddressHoldTime = 0;
  p.FSMC_DataSetupTime = 9;
  p.FSMC_BusTurnAroundDuration = 0;
  p.FSMC_CLKDivision = 0;
  p.FSMC_DataLatency = 0;
  p.FSMC_AccessMode = FSMC_AccessMode_A;

  FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM4;
  FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
  FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
  FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
  FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_AsynchronousWait = FSMC_AsynchronousWait_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
  FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
  FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
  FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
  FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
  FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
  FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

  FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);   

  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM4, ENABLE);
}


