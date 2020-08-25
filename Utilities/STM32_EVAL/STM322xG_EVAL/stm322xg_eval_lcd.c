 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： stm322xg_eval_lcd.c
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
/*头文件----------------------------------------------------*/
#include <stdio.h>
#include "stm32f2xx.h"
#include "stm322xg_eval_lcd.h"
#include "stm32_eval.h"
#include "fonts.h"
#include "logo.h"
#include "fonts1.h"


/*定义LCD数据类型 -----------------------------------------------------------*/
typedef struct
{
	__IO uint16_t LCD_REG;
	__IO uint16_t LCD_RAM;
}
LCD_TypeDef;

/* 定义LCD驱动器的访问地址 */
#define LCD_BASE        ((uint32_t)(0x60000002 | 0x0C000000))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

static sFONT *LCD_Currentfonts;	/*字体类型*/
 __IO uint16_t s_TextColor = 0x0000;	/*前景色*/
 __IO uint16_t s_BackColor = 0xFFFF;    /*背景色*/

/* 背景透明标识位 */
static __IO uint8_t s_Transparent = 0;
#define LCD_PIXEL_WIDTH          0x0140	 /*液晶宽度*/
#define LCD_PIXEL_HEIGHT         0x00F0	 /*液晶高度*/
u16 POINT_COLOR = 0x0000,BACK_COLOR = 0xFFFF;
/***************************************************
*函数名:LCD_Delay
*输入参数:
*	  	nCount：延时长度
*返回值: 无
*函数说明: LCD延时函数
*
****************************************************/
void LCD_Delay(vu32 nCount)
{
  for(; nCount != 0; nCount--);
}


/***************************************************
*函数名:Lcd_Write_Com
*输入参数:
*     LCD_Reg：LCd命令寄存器
*返回值: 无
*函数说明: LCD写命令寄存器
*
****************************************************/
void Lcd_Write_Com(u16 LCD_Reg)
{
	 LCD->LCD_REG = LCD_Reg;
}

/***************************************************
*函数名:STM3220F_LCD_Init
*输入参数:无
*返回值: 无
*函数说明: LCD初始化函数
*
****************************************************/
void STM3220F_LCD_Init(void)
{
	/* 配置LCD控制口线GPIO */
	LCD_CtrlLinesConfig();

	/* 配置FSMC接口，数据总线 */
	LCD_FSMCConfig();

	/* FSMC重置后必须加延迟才能访问总线设备  */
	LCD_Delay(200000);

    LCD_WriteReg(0x0000,0x0001);	//打开晶振
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
    LCD_WriteReg(0x0001,0x6B3F);	//驱动输出控制320*240  0x6B3F 0x2B3F
	LCD_Delay(10);
    LCD_WriteReg(0x0002,0x0600);
	LCD_Delay(10);
    LCD_WriteReg(0x0010,0x0000);
	LCD_Delay(10);
    LCD_WriteReg(0x0011,0x6068);	//定义数据格式  16位色 
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
    LCD_WriteReg(0x000F,0x0000);	//扫描开始地址
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
    LCD_WriteReg(0x004f,0x0000);	//行首址0
	LCD_Delay(10);
    LCD_WriteReg(0x004e,0x0000);	//列首址0
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
*函数名:LCD_SetTextColor
*输入参数:
*     Color:颜色类型
*返回值: 无
*函数说明: 设置前景颜色
*
****************************************************/
void LCD_SetTextColor(__IO uint16_t Color)
{
	s_TextColor = Color;
}

/***************************************************
*函数名:LCD_SetBackColor
*输入参数:
*     Color:颜色类型
*返回值: 无
*函数说明: 设置背景色
*
****************************************************/
void LCD_SetBackColor(__IO uint16_t Color)
{
	s_BackColor = Color;
}

/***************************************************
*函数名:LCD_ClearLine
*输入参数:
*      Line：LCD纵坐标
*返回值:无
*函数说明:清除指定的行Line 0---240
*
****************************************************/
void LCD_ClearLine(uint8_t Line)
{
	LCD_DisplayStringHZ(0, Line, "                                         ");
}


/***************************************************
*函数名:LCD_Clear
*输入参数:
     Color: 颜色种类
*返回值: 无
*函数说明: 以指定颜色清屏
*
****************************************************/
void LCD_Clear(uint16_t Color)
{
	uint32_t index = 0;

	LCD_SetCursor(0, 0);	/* 设置光标位置 */

	LCD_WriteRAM_Prepare(); 	/* 准备写显存 */

	for (index = 0; index < 320 * 240; index++)
	{
		LCD->LCD_RAM = Color;
	}
}


/***************************************************
*函数名:LCD_SetCursor
*输入参数:				   -----------------
*	   Xpos:LCD 横坐标	   |	|		   |
*	   Ypos:LCD 纵坐标	   |	|		   |
*返回值:无				   |----|(x,y)     |
*函数说明:设置光标位置	   |			   |
*						   |---------------|
****************************************************/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
	LCD_WriteReg(0x4e, Ypos);
	LCD_WriteReg(0x4f, Xpos);
}

/***************************************************
*函数名:LCD_SetTransparent
*输入参数:_mode
*返回值: 无
*函数说明: 设置背景是否透明
*
****************************************************/
void LCD_SetTransparent(uint8_t _mode)
{
	if (_mode == 0)
	{
		s_Transparent = 0;	/* 设置为背景不透明 */
	}
	else
	{
		s_Transparent = 1;	/* 设置为背景透明 */
	}
}
/***************************************************
*函数名:LCD_SetFont
*输入参数:
*		fonts：字体类型
*返回值:无
*函数说明:设置要显示的文本字体
*
****************************************************/
void LCD_SetFont(sFONT *fonts)
{
  LCD_Currentfonts = fonts;
}
/***************************************************
*函数名:LCD_GetFont
*输入参数:无
*返回值:
*      sFONT：字体类型
*函数说明:返回当前显示文本的字体类型
*
****************************************************/
sFONT *LCD_GetFont(void)
{
  return LCD_Currentfonts;
}

/*******************************************************
* 函数名:LCD_DrawChar  
* 输入参数：
*		Xpos	: X坐标
*		Ypos	: Y坐标
*		ch		: 字符
*		width	: 字符宽度,英文字符是8,汉字是16
* 返回值：无
* 函数说明:画一个字符
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

	code1 = ch-' ';	/* ascii代码 */
	
	if(ch >=0x80)  
	{
		return;  /*可能是汉字,返回*/
	}
	c = (uint16_t *)&LCD_Currentfonts->table[code1 * LCD_Currentfonts->Height];

	LCD_SetCursor(Xpos, Ypos);

	for(index = 0; index < LCD_Currentfonts->Height; index++)
	{
		LCD_WriteRAM_Prepare(); /* 准备写缓冲区*/
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
* 函数名:LCD_DrawCharHZ  
* 输入参数:
*		Xpos	: X坐标
*		Ypos	: Y坐标
*		c		: 指向字符点阵的指针
*		width	: 字符宽度,英文字符是8,汉字是16
* 返回值: None
* 函数说明:画一个字符,支持汉字
******************************************************/
void LCD_DrawCharHZ(uint16_t Xpos, uint16_t Ypos, const uint8_t *c, uint8_t width)
{
	uint32_t index = 0, i = 0;
	uint8_t Yaddress;

	Yaddress = Ypos;

	LCD_SetCursor(Xpos, Ypos);

	if (s_Transparent == 0)	//背景不透明
	{
		for (index = 0; index < 16; index++)	/* 字符高度 */
		{
			LCD_WriteRAM_Prepare();
			for (i = 0; i < width; i++)	/* 字符宽度 */
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
	else	//背景透明
	{
		for (index = 0; index < 16; index++)	/* 字符高度 */
		{
			for (i = 0; i < width; i++)	/* 字符宽度 */
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
* 函数名: LCD_DisplayStringHZ 
* 输入参数：
*		Xpos	: X坐标
*		Ypos	: Y坐标
*		c		: 字符串指针
* 返回值： 无
*函数说明：画一个字符串	支持汉子字串
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
		code1 = *ptr;	/* ascii代码 */
		if (code1 < 0x80)
		{
			LCD_DrawCharHZ(Xpos, Ypos, &Ascii16[code1 * 16], 8);	/* 16 表示1个字符字模的字节数 */
			Xpos += 8;		/* 列地址+8 */
		}
		else	/* 汉字内码 */
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
			Xpos += 16;		/* 列地址+16 */
		}
		ptr++;			/* 指向下一个字符 */
		i++;
	}
}
/*******************************************
* 函数名：LCD_DisplayString  
* 输入参数：
*		Xpos	: X坐标
*		Ypos	: Y坐标
*		c		: 字符串指针
* 返回值： None
*函数说明：画一个字符串
**************************************************/
void LCD_DisplayString(uint16_t Xpos, uint16_t Ypos, uint8_t *ptr)
{
	uint32_t i = 0;

	while ((*ptr != 0) & (i < 40))
	{
		LCD_DrawChar(Xpos, Ypos, *ptr, LCD_Currentfonts->Width);	/* 16 表示1个字符字模的字节数 */
		Xpos += LCD_Currentfonts->Width;		/* 列地址+8 */
		ptr++;			/* 指向下一个字符 */
		i++;
	}
}

/*******************************************
* 函数名：LCD_DisplayChar  
* 输入参数：
*		Xpos	: X坐标
*		Ypos	: Y坐标
*		c		: 字符
* 返回值： None
*函数说明：画一个字符
*备注:兼容旧ST发布应用
**************************************************/
void LCD_DisplayChar(uint16_t Xpos, uint16_t Ypos, uint8_t c)
{
	LCD_DrawChar(Ypos, Xpos, c, LCD_Currentfonts->Width);	/*  */
}

/*********************************************************
* 函数名: LCD_DrawCharLargeFont 
* 输入参数:
*		Xpos	: X坐标
*		Ypos	: Y坐标
*		c : 指向字符点阵的指针
*		width : 字符宽度
* 返回值: None
* 函数说明:画一个大字符
**********************************************************/
void LCD_DrawCharLargeFont(uint16_t Xpos, uint16_t Ypos, const uint8_t *c, uint8_t width)
{
	uint32_t index = 0, i = 0;
	uint8_t Yaddress;

	Yaddress = Ypos;

	LCD_SetCursor(Xpos, Ypos);

	if (s_Transparent == 0)	//背景不透明
	{
		for (index = 0; index < 64; index++)	/* 字符高度 */
		{
			LCD_WriteRAM_Prepare();
			for (i = 0; i < width; i++)	/* 字符宽度 */
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
	else	//背景透明
	{
		for (index = 0; index < 64; index++)	/* 字符高度 */
		{
			for (i = 0; i < width; i++)	/* 字符宽度 */
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
* 函数名：LCD_DisplayStringLargeFont
* 输入参数：
*		Xpos	: X坐标
*		Ypos	: Y坐标
*		c : 指向字符点阵的指针
*		width : 字符宽度
* 返回值：无
*  函数说明：画一个大字符的字符串
*/
void LCD_DisplayStringLargeFont(uint16_t Xpos, uint16_t Ypos, uint8_t *ptr)
{
	uint32_t i = 0;
	uint8_t code1;

	if(ptr==NULL)
		return;

	while ((*ptr != 0))
	{
		code1 = *ptr;	// ascii代码
		if ((code1 > '9')||(code1 < '-'))
		{
			Xpos += 32;
		}
		else
		{
			code1 = code1-'-';

			// 256 表示1个字符字模的字节数
			LCD_DrawCharLargeFont(Xpos, Ypos, &large_font_64[code1 * 256], 32);

			// 如果是'.'或者'/'字符宽度为24，数字字符宽度为32
			if((code1<3)&&(code1!=0))
				Xpos += 24;
			else
				Xpos += 32;
		}

		ptr++;			// 指向下一个字符
		i++;
	}
}

/***************************************************
*函数名:LCD_DisplayStringLine
*输入参数:
*		Line：LCD纵坐标
*		ptr：待显示的字符
*返回值:无
*函数说明:在指定行坐标显示字符
*
****************************************************/
void LCD_DisplayStringLine(uint16_t Line, uint8_t *ptr)
{
  uint16_t refcolumn = 0;
  LCD_DisplayString(refcolumn,Line,ptr);
 }

/**
  * @brief  画Eflag LOGO
  * @param
  *		Xpos	: X坐标
  *		Ypos	: Y坐标
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
* 函数名：LCD_DrawPoint  
* 输入参数：
*		Xpos	: X坐标
*		Ypos	: Y坐标
*		Color	: 像素颜色
* 返回值：无
*函数说明：画1个像素
*******************************************************/
void LCD_DrawPoint(uint16_t x, uint16_t y, uint16_t Color)
{
	
	LCD_SetCursor(x, y);	// 设置光标位置

	LCD->LCD_REG = 0x22;
	LCD->LCD_RAM = Color;
}
/**********************************************************
* 函数名:LCD_DrawPoints  
* 输入参数：
*		_points	: 点坐标数组
*		_offset	: 偏移坐标
*		_Size	: 点个数
*		_Color	: 颜色
* 返回值：无
*函数说明：画一组点
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
  * 函数名：LCD_BresenhamLine 
  * 输入参数：
  *		x1, y1	: 起始点坐标
  *		x2, y2	: 终止点坐标
  *		c		: 直线颜色
  * 返回值：无
  *	函数说明： Bresenham算法在两点间画一条直线	
  */
void LCD_BresenhamLine (uint16_t x1 , uint16_t y1 , uint16_t x2 , uint16_t y2 , int c)
{
	int dx , dy ;
	int tx , ty ;
	int inc1 , inc2 ;
	int d , iTag ;
	int x , y ;

	LCD_DrawPoint(x1 , y1 , c);

	/* 如果两点重合，结束后面的动作。*/
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

	if ( dx < dy )   /*如果dy为计长方向，则交换纵横坐标。*/
	{
		uint16_t temp;

		iTag = 1 ;
		temp = x1; x1 = y1; y1 = temp;
		temp = x2; x2 = y2; y2 = temp;
		temp = dx; dx = dy; dy = temp;
	}
	tx = x2 > x1 ? 1 : -1 ;    /*确定是增1还是减1*/
	ty = y2 > y1 ? 1 : -1 ;
	x = x1 ;
	y = y1 ;
	inc1 = 2 * dy ;
	inc2 = 2 * ( dy - dx );
	d = inc1 - dx ;
	while ( x != x2 )     /*循环画点*/
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
* 函数名：LCD_DrawLine  
* 输入参数：
*		Xpos	: 起始点X坐标
*		YPos	: 起始点Y坐标
*		Length	: 长度
*		Direction:方向(Horizontal，Vertical)
* 返回值： None
* 函数说明：画垂直线或水平线
**************************************************/
void LCD_DrawLine(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction)
{
	uint32_t i = 0;

	/* 设置线的起始坐标 */
	LCD_SetCursor(Xpos, Ypos);

	if (Direction == Horizontal)	/* 水平绘制 */
	{
		LCD_WriteRAM_Prepare();
		for (i = 0; i < Length; i++)
		{
			LCD_WriteRAM(s_TextColor);
		}
	}
	else	/* 垂直绘制 */
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
* 函数名:  LCD_DrawRect
* 输入参数:
*		Xpos	: 起始点X坐标
*		YPos	: 起始点Y坐标
*		Height	: 高度
*		Width	: 宽度
* 返回值: None
* 函数说明:画矩形函数
**********************************************************/
void LCD_DrawRect(uint16_t Xpos, uint16_t Ypos, uint8_t Height, uint16_t Width)
{
	LCD_DrawLine(Xpos, Ypos, Width, Horizontal);  			/* 顶 */
	LCD_DrawLine(Xpos, Ypos + Height, Width, Horizontal);	/* 底 */

	LCD_DrawLine(Xpos, Ypos, Height, Vertical);				/* 左 */
	LCD_DrawLine(Xpos + Width, Ypos, Height + 1, Vertical);	/* 右 */
}
  /**************************************************
* 函数名：LCD_DrawLine  
* 输入参数：
*		Xpos	: 起始点X坐标
*		YPos	: 起始点Y坐标
*		Length	: 长度
*		Direction:方向(Horizontal，Vertical)
* 返回值： None
* 函数说明：画垂直线或水平线
**************************************************/
void LCD_DrawLine_Color(uint16_t Xpos, uint16_t Ypos, uint16_t Length, uint8_t Direction,uint16_t Color)
{
	uint32_t i = 0;

	/* 设置线的起始坐标 */
	LCD_SetCursor(Xpos, Ypos);

	if (Direction == Horizontal)	/* 水平绘制 */
	{
		LCD_WriteRAM_Prepare();
		for (i = 0; i < Length; i++)
		{
			LCD_WriteRAM(Color);
		}
	}
	else	/* 垂直绘制 */
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
*函数名:LCD_DrawRectFill  
*输入参数:
*		x1,y1	: 矩形起点坐标
*		x2,y2	: 矩形终点坐标
*		color	: 填充颜色
*返回值: 无
*函数说明:画一个实心矩形
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
* 函数名: LCD_DrawCircle
* 输入参数：
*		Xpos		: X坐标
*		YPos		: Y坐标
*		Radius		: 半径
* 返回值：无
*函数说明:画圆函数
**********************************************/
void LCD_DrawCircle(uint16_t Xpos, uint16_t Ypos, uint16_t Radius)
{
	int32_t  D;			/* Decision Variable */
	uint32_t  CurX;		/* 当前 X 值 */
	uint32_t  CurY;		/* 当前 Y 值 */

	D = 3 - (Radius << 1);
	CurX = 0;
	CurY = Radius;

	while (CurX <= CurY)
	{
		LCD_SetCursor(Xpos + CurX, Ypos + CurY);
		LCD_WriteRAM_Prepare(); /* 准备写 GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos + CurX, Ypos - CurY);
		LCD_WriteRAM_Prepare(); /* 准备写  GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos - CurX, Ypos + CurY);
		LCD_WriteRAM_Prepare(); /* 准备写  GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos - CurX, Ypos - CurY);
		LCD_WriteRAM_Prepare(); /* 准备写  GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos + CurY, Ypos + CurX);
		LCD_WriteRAM_Prepare(); /* 准备写  GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos + CurY, Ypos - CurX);
		LCD_WriteRAM_Prepare(); /* 准备写  GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos - CurY, Ypos + CurX);
		LCD_WriteRAM_Prepare(); /* 准备写  GRAM */
		LCD_WriteRAM(s_TextColor);

		LCD_SetCursor(Xpos - CurY, Ypos - CurX);
		LCD_WriteRAM_Prepare(); /* 准备写  GRAM */
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
* 函数名: LCD_DrawMonoPict
* 输入参数:
*		Pict		: 图片点阵指针
* 返回值：无
*函数说明: 画单色图片
***********************************************/
void LCD_DrawMonoPict(const uint32_t *Pict)
{
	uint32_t index = 0, i = 0;

	LCD_SetCursor(0, 320);

	LCD_WriteRAM_Prepare(); /* 准备写 GRAM */
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
* 函数名: LCD_WriteBMP
* 输入参数：
*		ptr			: 图片点阵指针
* 返回值：无
* 函数说明：画位图
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
*函数名:LCD_SetDisplayWindow
*函数说明:设置LCD显示窗口
*输入参数:
*		 Xpos：X坐标
*		 Ypos  Y坐标
*		 Height：窗口高度
*		 Width：窗口宽度
*返回值:无
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
* 函数名：  LCD_DrawFullRect
* 输入参数：
*		Xpos	: x坐标
*		Ypos	: y坐标
*		Height	：矩形框高度
*		Width	：矩形框宽度
* 返回值：无
*函数说明： 画一个完整填充矩形框
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
* 函数名：  LCD_WriteReg
* 输入参数：
*		LCD_Reg			: 寄存器地址
*		LCD_RegValue	: 寄存器值
* 返回值：无
*函数说明： 修改LCD控制器的寄存器的值
*************************************************/
void LCD_WriteReg(__IO uint16_t LCD_Reg, uint16_t LCD_RegValue)
{
	LCD->LCD_REG = LCD_Reg;
	LCD->LCD_RAM = LCD_RegValue;
}

/******************************************
* 函数名: LCD_ReadReg
* 输入参数：
*		LCD_Reg		: 寄存器地址
* 返回值： 寄存器的值
*函数说明：读LCD控制器的寄存器的值
************************************************/
uint16_t LCD_ReadReg(__IO uint16_t LCD_Reg)
{

	LCD->LCD_REG = LCD_Reg;

	return (LCD->LCD_RAM);
}

/************************************************
* 函数名: LCD_WriteRAM_Prepare
* 输入参数：无
* 返回值：无
* 函数说明： 设置显存寄存器地址
************************************************/
void LCD_WriteRAM_Prepare(void)
{
	LCD->LCD_REG = 0x22;
}

/*********************************************
* 函数名：LCD_WriteRAM
* 输入参数：
*		RGB_Code	: 颜色代码
*返回值：无
*函数说明：写显存，显存地址自动增加
**********************************************/
void LCD_WriteRAM(uint16_t RGB_Code)
{

	LCD->LCD_RAM = RGB_Code;
}

/**********************************************
* 函数名：LCD_ReadRAM 
* 输入参数：无
* 返回值： 显存数据
*函数说明： 读显存，地址自动增加
*/
uint16_t LCD_ReadRAM(void)
{

	return LCD->LCD_RAM;
}

/*************************************************
* 函数名：LCD_CtrlLinesConfig  
* 输入参数：无
* 返回值：无
*函数说明：配置LCD控制口线，FSMC管脚设置为复用功能
*/
void LCD_CtrlLinesConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  /* 设置GPIO时钟和复用时钟 */
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

  
  /* 设置 PF.00(A0 (RS))  为复用推挽输出 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource1, GPIO_AF_FSMC);

}

/***************************************************
*函数名:LCD_FSMCConfig
*输入参数:无
*返回值: 无
*函数说明: FSMC总线配置
*
****************************************************/
void LCD_FSMCConfig(void)
{
  FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
  FSMC_NORSRAMTimingInitTypeDef  p;
   
  /* 使能FSMC总线时钟*/
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


