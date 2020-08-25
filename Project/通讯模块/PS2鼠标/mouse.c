/****************************************************************************
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
*****************************************************************************/

/*************头文件**********************************************************/	 
#include "mouse.h"

/*************外部函数********************************************************/	 
extern void delay_ms(u16 nms);
extern void delay_us(u32 nus);

/*************定义变量和结构体************************************************/	 
u8 MOUSE_ID;//用来标记鼠标ID 
PS2_Mouse MouseX;

/****************************************************************************
* 函数名: Mouse_Data_Pro()
* 功 能: 处理MOUSE的数据
* 输 入: 无
* 输 出: 无
****************************************************************************/ 
void Mouse_Data_Pro(void)
{			    			    
	MouseX.x_pos=(signed char)PS2_DATA_BUF[1];
	MouseX.y_pos=(signed char)PS2_DATA_BUF[2];
	MouseX.y_pos=-MouseX.y_pos;//Y方向反了,纠正过来
	MouseX.z_pos=(signed char)PS2_DATA_BUF[3];		  
	MouseX.bt_mask=PS2_DATA_BUF[0]&0X07;//取出掩码
}	

/****************************************************************************
* 函数名: Init_Mouse()
* 功 能: 初始化鼠标
* 输 入: 无
* 输 出: 0或1
****************************************************************************/  
u8 Init_Mouse(void)
{
	u8 t;		 
	PS2_Init();							     
	delay_ms(800); //等待上电复位完成		 	 
	PS2_Status=CMDMODE; //进入命令模式
	t=PS2_Send_Cmd(PS_RESET);  //复位鼠标
			 
	if(t!=0)return 1; //复位失败
	t=PS2_Get_Byte();	  
	if(t!=0XFA)return 2; //没有应答
	t=0;
	while((PS2_Status&0x80)==0)//等待复位完毕 
		{
			t++;
			delay_ms(50);      
			if(t>50)return 3;
		}
	PS2_Get_Byte();//得到0XAA
	PS2_Get_Byte();//得到ID 

	//进入滚轮模式的特殊初始化序列
	PS2_Send_Cmd(SET_SAMPLE_RATE);//进入设置采样率
	if(PS2_Get_Byte()!=0XFA)return 4;//传输失败
	PS2_Send_Cmd(0XC8);//采样率200
	if(PS2_Get_Byte()!=0XFA)return 5;//传输失败
	PS2_Send_Cmd(SET_SAMPLE_RATE);//进入设置采样率
	if(PS2_Get_Byte()!=0XFA)return 6;//传输失败
	PS2_Send_Cmd(0X64);//采样率100
	if(PS2_Get_Byte()!=0XFA)return 7;//传输失败
	PS2_Send_Cmd(SET_SAMPLE_RATE);//进入设置采样率
	if(PS2_Get_Byte()!=0XFA)return 8;//传输失败
	PS2_Send_Cmd(0X50);//采样率80
	if(PS2_Get_Byte()!=0XFA)return 9;//传输失败

	//序列完成		   
	PS2_Send_Cmd(GET_DEVICE_ID); //读取ID
	if(PS2_Get_Byte()!=0XFA)return 10;//传输失败
	MOUSE_ID=PS2_Get_Byte();//得到MOUSE ID	 
	
	PS2_Send_Cmd(SET_SAMPLE_RATE);//再次进入设置采样率
	if(PS2_Get_Byte()!=0XFA)return 11;//传输失败
	PS2_Send_Cmd(0X0A);//采样率10
	if(PS2_Get_Byte()!=0XFA)return 12;//传输失败		 
	PS2_Send_Cmd(GET_DEVICE_ID); //读取ID
	if(PS2_Get_Byte()!=0XFA)return 13;//传输失败
	MOUSE_ID=PS2_Get_Byte();//得到MOUSE ID		 
	
	PS2_Send_Cmd(SET_RESOLUTION);  //设置分辨率
	if(PS2_Get_Byte()!=0XFA)return 14;//传输失败   
	PS2_Send_Cmd(0X03);//8点/mm
	if(PS2_Get_Byte()!=0XFA)return 15;//传输失败 
	PS2_Send_Cmd(SET_SCALING11);   //设置缩放比率为1:1
	if(PS2_Get_Byte()!=0XFA)return 16;//传输失败 
	
	PS2_Send_Cmd(SET_SAMPLE_RATE); //设置采样率
	if(PS2_Get_Byte()!=0XFA)return 17;//传输失败   
	PS2_Send_Cmd(0X28);//40
	if(PS2_Get_Byte()!=0XFA)return 18;//传输失败 
	
	PS2_Send_Cmd(EN_DATA_REPORT);   //使能数据报告
	if(PS2_Get_Byte()!=0XFA)return 19;//传输失败
	
	PS2_Status=MOUSE;//进入鼠标模式
	return 0;//无错误,初始化成功   
}

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*****************/
