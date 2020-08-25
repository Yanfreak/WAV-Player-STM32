#ifndef __BMP_H__
#define __BMP_H__
#include "sys.h"
					    
//////////////////////////////////////////用户配置区//////////////////////////////////
#define BMP_USE_MALLOC		1 	//定义是否使用malloc,这里我们选择使用malloc	    
//////////////////////////////////////////////END/////////////////////////////////////

#define PIC_FORMAT_ERR		0x27	//格式错误
#define PIC_SIZE_ERR		0x28	//图片尺寸错误
#define PIC_WINDOW_ERR		0x29	//窗口设定错误
#define PIC_MEM_ERR			0x11	//内存错误

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//图片显示物理层接口  
//在移植的时候,必须由用户自己实现这几个函数
typedef struct 
{
	u16(*read_point)(u16,u16);			//u16 read_point(u16 x,u16 y)						读点函数
	void(*draw_point)(u16,u16,u16);		//void draw_point(u16 x,u16 y,u16 color)		    画点函数
 	void(*fill)(u16,u16,u16,u16,u16);	///void fill(u16 sx,u16 sy,u16 ex,u16 ey,u16 color) 单色填充函数 	 
 	void(*draw_hline)(u16,u16,u16,u16);	//void draw_hline(u16 x0,u16 y0,u16 len,u16 color)  画水平线函数	 
}_pic_phy; 		       
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//BMP编解码函数
u8 stdbmp_decode(const u8 *filename);
u8 minibmp_decode(u8 *filename,u16 x,u16 y,u16 width,u16 height,u16 acolor,u8 mode);//尺寸小于240*320的bmp图片解码
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif








