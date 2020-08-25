 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： stm322xg_eval_ov9655.c
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
#include "stm322xg_eval_ov9655.h"
#include "main.h"
#include "stm322xg_eval.h"
#include "stm32f2xx.h"
#include "stm32f2xx_dcmi.h"
/*****宏定义*************************************************/
#define  TIMEOUT  20	  /*超时时间*/
#define DCMI_DR_ADDRESS     0x50050028	 /*Ov9655地址*/
#define FSMC_LCD_ADDRESS    0x6C000004	 /*LCD地址*/
#define QVGA_SIZE  /* 320x240 */
/***************************************************
*函数名:Cam_Delay
*输入参数:
*	   nTime：延时长度
*返回值: 无
*函数说明:延时函数
*
****************************************************/
void Cam_Delay(uint32_t nTime)
{
  unsigned int i;
  for(i=0;i<nTime*100;i++);

} 
/***************************************************
*函数名:DCMI_OV9655_Reset
*输入参数: 无
*返回值:  无
*函数说明: Ov9655初始化
*
****************************************************/
void DCMI_OV9655_Reset(void)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, SCCB_REG_RESET);    
}

/***************************************************
*函数名:DCMI_OV9655_QVGASizeSetup
*输入参数:无
*返回值:  无
*函数说明:QVGA图像大小设置
*
****************************************************/
void DCMI_OV9655_QVGASizeSetup(void)
{  
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x00, 0x00); //gain
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x01, 0x80); //blue gain
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x02, 0x80); //red gain
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x03, 0x02); // vertical frame control 02 
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x04, 0x03);  //common control1
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x09, 0x01);  //common control2
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0b, 0x57);  //productor id
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0e, 0x61);  //common control5--0x61
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0f, 0x40);  //common control6---0x40
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x11, 0x01);  //clock
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x12, 0x63); //data fomat
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x13, 0xc7); //common control8
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x14, 0x3a); //common control9 0x3a
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x16, 0x24);  //register 16
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x17, 0x18);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x18, 0x04);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x19, 0x01);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1a, 0x81);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1e, 0x00); /*0x20 1e*/
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x24, 0x3c);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x25, 0x36);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x26, 0x72);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x27, 0x08);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x28, 0x08);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x29, 0x15);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2a, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2b, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2c, 0x08);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x32, 0x12);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x33, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x34, 0x3f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x35, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x36, 0x3a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x38, 0x72);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x39, 0x57);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3a, 0x8c);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3b, 0x04);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3d, 0x99);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3e, 0x02); 
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3f, 0xc1);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x40, 0xD0); //c0
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x41, 0x41);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x42, 0xc0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x43, 0x0a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x44, 0xf0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x45, 0x46);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x46, 0x62);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x47, 0x2a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x48, 0x3c);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4a, 0xfc);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4b, 0xfc);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4c, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4d, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4e, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4f, 0x98);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x50, 0x98);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x51, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x52, 0x28);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x53, 0x70);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x54, 0x98);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x58, 0x1a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x59, 0x85);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5a, 0xa9);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5b, 0x64);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5c, 0x84);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5d, 0x53);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5e, 0x0e);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5f, 0xf0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x60, 0xf0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x61, 0xf0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x62, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x63, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x64, 0x02);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x65, 0x20);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x66, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x69, 0x0a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6b, 0x5a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6c, 0x04);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6d, 0x55);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6e, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6f, 0x9d);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x70, 0x21);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x71, 0x78);
  Cam_Delay(TIMEOUT);
  Cam_Delay(TIMEOUT);  
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x72, 0x11); 
  Cam_Delay(TIMEOUT);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x73, 0x01);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x74, 0x10); 
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x75, 0x10); 
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x76, 0x01);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x77, 0x02);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7A, 0x12);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7B, 0x08);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7C, 0x16);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7D, 0x30);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7E, 0x5e);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7F, 0x72);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x80, 0x82);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x81, 0x8e);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x82, 0x9a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x83, 0xa4);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x84, 0xac);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x85, 0xb8);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x86, 0xc3);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x87, 0xd6);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x88, 0xe6);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x89, 0xf2);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8a, 0x24);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8c, 0x80);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x90, 0x7d);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x91, 0x7b);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9d, 0x02);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9e, 0x02);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9f, 0x7a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa0, 0x79);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa1, 0x40);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa4, 0x50);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa5, 0x68);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa6, 0x4a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa8, 0xc1);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa9, 0xef);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xaa, 0x92);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xab, 0x04);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xac, 0x80);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xad, 0x80);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xae, 0x80);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xaf, 0x80);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb2, 0xf2);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb3, 0x20);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb4, 0x20);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb5, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb6, 0xaf);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb6, 0xaf);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbb, 0xae);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbc, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbd, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbe, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbf, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbf, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc0, 0xaa);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc1, 0xc0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc2, 0x01);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc3, 0x4e);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc6, 0x05);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc7, 0x81);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc9, 0xe0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xca, 0xe8);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcb, 0xf0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcc, 0xd8);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcd, 0x93);
  Cam_Delay(TIMEOUT);
}

/***************************************************
*函数名:DCMI_OV9655_QQVGASizeSetup
*输入参数:无
*返回值: 无
*函数说明:QQVGA大小设置
*
****************************************************/
void DCMI_OV9655_QQVGASizeSetup(void)
{
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x00, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x01, 0x80);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x02, 0x80);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x03, 0x02);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x04, 0x03);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x09, 0x01);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0b, 0x57);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0e, 0x61);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x0f, 0x40);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x11, 0x01);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x12, 0x62);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x13, 0xc7);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x14, 0x3a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x16, 0x24);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x17, 0x18);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x18, 0x04);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x19, 0x01);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1a, 0x81);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x1e, 0x20);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x24, 0x3c);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x25, 0x36);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x26, 0x72);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x27, 0x08);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x28, 0x08);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x29, 0x15);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2a, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2b, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x2c, 0x08);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x32, 0xa4);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x33, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x34, 0x3f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x35, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x36, 0x3a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x38, 0x72);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x39, 0x57);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3a, 0x8c);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3b, 0x04);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3d, 0x99);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3e, 0x0e); 
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x3f, 0xc1);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x40, 0xc0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x41, 0x41);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x42, 0xc0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x43, 0x0a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x44, 0xf0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x45, 0x46);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x46, 0x62);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x47, 0x2a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x48, 0x3c);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4a, 0xfc);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4b, 0xfc);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4c, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4d, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4e, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x4f, 0x98);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x50, 0x98);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x51, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x52, 0x28);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x53, 0x70);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x54, 0x98);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x58, 0x1a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x59, 0x85);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5a, 0xa9);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5b, 0x64);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5c, 0x84);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5d, 0x53);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5e, 0x0e);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x5f, 0xf0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x60, 0xf0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x61, 0xf0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x62, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x63, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x64, 0x02);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x65, 0x20);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x66, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x69, 0x0a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6b, 0x5a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6c, 0x04);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6d, 0x55);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6e, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x6f, 0x9d);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x70, 0x21);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x71, 0x78);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x72, 0x22); 
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x73, 0x02);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x74, 0x10);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x75, 0x10); 
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x76, 0x01);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x77, 0x02);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7A, 0x12);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7B, 0x08);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7C, 0x16);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7D, 0x30);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7E, 0x5e);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x7F, 0x72);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x80, 0x82);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x81, 0x8e);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x82, 0x9a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x83, 0xa4);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x84, 0xac);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x85, 0xb8);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x86, 0xc3);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x87, 0xd6);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x88, 0xe6);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x89, 0xf2);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8a, 0x24);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x8c, 0x80);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x90, 0x7d);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x91, 0x7b);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9d, 0x02);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9e, 0x02);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x9f, 0x7a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa0, 0x79);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa1, 0x40);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa4, 0x50);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa5, 0x68);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa6, 0x4a);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa8, 0xc1);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xa9, 0xef);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xaa, 0x92);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xab, 0x04);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xac, 0x80);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xad, 0x80);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xae, 0x80);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xaf, 0x80);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb2, 0xf2);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb3, 0x20);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb4, 0x20);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb5, 0x00);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb6, 0xaf);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xb6, 0xaf);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbb, 0xae);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbc, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbd, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbe, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbf, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xbf, 0x7f);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc0, 0xaa);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc1, 0xc0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc2, 0x01);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc3, 0x4e);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc6, 0x05);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc7, 0x82);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xc9, 0xe0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xca, 0xe8);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcb, 0xf0);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcc, 0xd8);
  Cam_Delay(TIMEOUT);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0xcd, 0x93);
  Cam_Delay(TIMEOUT);
}

/***************************************************
*函数名:main
*输入参数:
*		OV9655ID：Ov9655的ID
*返回值:无
*函数说明:读取Ov9655的ID
*
****************************************************/
void DCMI_OV9655_ReadID(OV9655_IDTypeDef* OV9655ID)
{
  OV9655ID->Manufacturer_ID1 = DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_MIDH);
  OV9655ID->Manufacturer_ID2 = DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_MIDL);
  OV9655ID->Version = DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_VER);
  OV9655ID->PID = DCMI_SingleRandomRead(OV9655_DEVICE_WRITE_ADDRESS, OV9655_PID);
}

/***************************************************
*函数名:DCMI_OV9655_SetPrescaler
*输入参数:
*	  OV9655_Prescaler：分频值
*返回值:无
*函数说明:DCMI预分频
*
****************************************************/
void DCMI_OV9655_SetPrescaler(uint8_t OV9655_Prescaler)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_CLKRC, OV9655_Prescaler);
}

/***************************************************
*函数名:DCMI_OV9655_SelectOutputFormat
*输入参数:
*	   OV9655_OuputFormat：Ov9655输出格式。OUTPUT_FORMAT_RAWRGB_DATA，OUTPUT_FORMAT_RAWRGB_INTERP
*						OUTPUT_FORMAT_YUV，OUTPUT_FORMAT_RGB
*返回值: 无
*函数说明:Ov9655输出格式
*
****************************************************/
void DCMI_OV9655_SelectOutputFormat(uint8_t OV9655_OuputFormat)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, OV9655_OuputFormat);
}

/***************************************************
*函数名:DCMI_OV9655_SelectFormatResolution
*输入参数:
*		OV9655_FormatResolution：格式选择。FORMAT_CTRL_15fpsVGA，FORMAT_CTRL_30fpsVGA_NoVArioPixel
*								FORMAT_CTRL_30fpsVGA_VArioPixel
*返回值:
*函数说明:
*
****************************************************/
void DCMI_OV9655_SelectFormatResolution(uint8_t OV9655_FormatResolution)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, OV9655_FormatResolution);
}

/***************************************************
*函数名:DCMI_OV9655_SetRegister
*输入参数:
*		OV9655_Register：寄存器
*		Register_Val：值
*返回值:
*函数说明:
*
****************************************************/
void DCMI_OV9655_SetRegister(uint8_t OV9655_Register, uint8_t Register_Val)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_Register, Register_Val);
}

/***************************************************
*函数名:DCMI_OV9655_HREFControl
*输入参数:
*		OV9665_HREFControl：控制信号
*返回值: 无
*函数说明: 选择
*
****************************************************/
void DCMI_OV9655_HREFControl(uint8_t OV9665_HREFControl)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_HREF, OV9665_HREFControl);
}

/***************************************************
*函数名:DCMI_OV9655_SelectRGBOption
*输入参数:
*	  OV9665_RGBOption：RGB选择
*返回值:无
*函数说明:选择RGB格式
*
****************************************************/
void DCMI_OV9655_SelectRGBOption(uint8_t OV9665_RGBOption)
{
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM15, OV9665_RGBOption);
}

/***************************************************
*函数名:DCMI_SingleRandomWrite
*输入参数:
*		Device：Ov9655基地址
*		Addr：寄存器地址
*		Data：写入数据
*返回值:
*函数说明:写一个字节到指定的Ov9655寄存器
*
****************************************************/
void DCMI_SingleRandomWrite(uint8_t Device, uint16_t Addr, uint8_t Data)
{
  /* 发送起始信号 */
  I2C_GenerateSTART(I2C2, ENABLE);

  /*检测EV5位并清除*/
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
   
  /*发送DCMI设备基地址*/
  I2C_Send7bitAddress(I2C2, Device, I2C_Direction_Transmitter);
 
  /*检测EV6位并清除*/
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); 
 
  /* 发送地址*/
  I2C_SendData(I2C2, (uint8_t)(Addr));

  /* 检测EV8位并清除 */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  /* 发送数据 */
  I2C_SendData(I2C2, Data);    

  /* 检测EV8位并清除 */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));  
 
  /* 发送终止信号*/
  I2C_GenerateSTOP(I2C2, ENABLE);
}

/***************************************************
*函数名:DCMI_SingleRandomRead
*输入参数:
*		Device：Ov9655基地址
*		Addr ：寄存器地址
*返回值: 读取到的寄存器值
*函数说明:
*
****************************************************/
uint8_t DCMI_SingleRandomRead(uint8_t Device, uint16_t Addr)
{
  uint8_t Data = 0;

  /* 发送起始信号 */
  I2C_GenerateSTART(I2C2, ENABLE);

  /* 检测EV5位并清除*/
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
  
  /* 发送DCMI基地址*/
  I2C_Send7bitAddress(I2C2, Device, I2C_Direction_Transmitter);
 
  /*检测EV6位并清除*/
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)); 

  /* 发送地址 */
  I2C_SendData(I2C2, (uint8_t)(Addr));

  /* 检测EV6位并清除 */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));
  
  I2C2->SR1 |= (uint16_t)0x0400; 

  /* 发送起始信号 */
  I2C_GenerateSTART(I2C2, ENABLE);
  
  /* 检测EV6并清除 */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));
  
  /* 发送寄存器地址 */
  I2C_Send7bitAddress(I2C2, Device, I2C_Direction_Receiver);
   
  /* 检测EV6并清除*/
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)); 
 
  /*失能应答信号 */
  I2C_AcknowledgeConfig(I2C2, DISABLE);

  /* 检测EV7位并清除 */
  while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED));  
    
  /* 发送终止信号 */
  I2C_GenerateSTOP(I2C2, ENABLE);

  /* 接收数据 */
  Data = I2C_ReceiveData(I2C2);

  /* 返回数据*/
  return Data;
}
/***************************************************
*函数名:Ov9655_I2C_Configuration
*输入参数:无
*返回值: 无
*函数说明:I2C初始化
*
****************************************************/
void Ov9655_I2C_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef I2C_InitStruct;

 /* I2C使能时钟*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2, ENABLE);
  /* GPIO时钟使能 */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 
  GPIO_DeInit(GPIOB); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; 
  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
    
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_I2C2);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_I2C2);  
   
  I2C_DeInit(I2C2);
    
  /* I2C配置*/
  I2C_InitStruct.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStruct.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStruct.I2C_OwnAddress1 = 0xFE;
  I2C_InitStruct.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStruct.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStruct.I2C_ClockSpeed = 10000;
  I2C_Init(I2C2, &I2C_InitStruct);
  I2C_Cmd(I2C2, ENABLE);
}
/***************************************************
*函数名:Ov9655_Dcmi_GPIO_Configuration
*输入参数:无
*返回值: 无
*函数说明: DCMI接口初始化
*
****************************************************/
void Ov9655_Dcmi_GPIO_Configuration(void)
{
  DCMI_InitTypeDef  DCMI_InitStructure;
  GPIO_InitTypeDef  GPIO_InitStructure;
  u16 j;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB | 
                         RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOE, ENABLE); 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  RCC_AHB2PeriphClockCmd(RCC_AHB2Periph_DCMI, ENABLE);
  DCMI_DeInit();
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource4, GPIO_AF_DCMI);  //HS
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource7, GPIO_AF_DCMI);  //VS
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_DCMI);  //CLK

  GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_DCMI); //D7
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_DCMI); //D6
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource6, GPIO_AF_DCMI); //D5
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource4, GPIO_AF_DCMI); //D4

  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_DCMI); //D3
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_DCMI); //D2
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_DCMI); //D1
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_DCMI); //D0

  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 |GPIO_Pin_1 |GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
                          
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;
  
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_SetBits(GPIOE, GPIO_Pin_0);
  GPIO_SetBits(GPIOE, GPIO_Pin_1);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9;

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;

  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 ;
	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ; 	
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  /* PCLK(PA6) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* HSYNC(PA4) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  /* VSYNC(PB7) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_2;   //RESET
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP ;	 
  GPIO_Init(GPIOE, &GPIO_InitStructure);
  GPIO_SetBits(GPIOE, GPIO_Pin_2);

  GPIO_ResetBits(GPIOE, GPIO_Pin_2);	 //RESET
  for(j=0;j<6000;j++);
  GPIO_SetBits(GPIOE, GPIO_Pin_2);	   
  
  /* DCMI配置 */ 
  DCMI_InitStructure.DCMI_CaptureMode = DCMI_CaptureMode_Continuous;
  DCMI_InitStructure.DCMI_SynchroMode = DCMI_SynchroMode_Hardware;
  DCMI_InitStructure.DCMI_PCKPolarity = DCMI_PCKPolarity_Falling;
  DCMI_InitStructure.DCMI_VSPolarity = DCMI_VSPolarity_High;
  DCMI_InitStructure.DCMI_HSPolarity = DCMI_HSPolarity_High;
  DCMI_InitStructure.DCMI_CaptureRate = DCMI_CaptureRate_All_Frame;
  DCMI_InitStructure.DCMI_ExtendedDataMode = DCMI_ExtendedDataMode_8b;
  
  DCMI_Init(&DCMI_InitStructure);
}
/***************************************************
*函数名:Ov9655_DMA_Configuration
*输入参数:无
*返回值: 无
*函数说明: DMA配置
*
****************************************************/
void Ov9655_DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  /* 使能DMA2时钟 */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  
  
  /* DMA2 Stream1 配置 */ 
  DMA_DeInit(DMA2_Stream1);

  DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;	
  DMA_InitStructure.DMA_Memory0BaseAddr =FSMC_LCD_ADDRESS;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize =1;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
}
/***************************************************
*函数名:Ov9655_DMA_Configuration
*输入参数:无
*返回值: 无
*函数说明: DMA配置
*
****************************************************/

void Ov9655_DMA1_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  /* 使能DMA2时钟 */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  
  
  /* DMA2 Stream1 配置 */ 
  DMA_DeInit(DMA2_Stream1);

  DMA_InitStructure.DMA_Channel = DMA_Channel_1;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = DCMI_DR_ADDRESS;	
  DMA_InitStructure.DMA_Memory0BaseAddr =0x68000010+68;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
  DMA_InitStructure.DMA_BufferSize =320*240/2;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
  DMA_Init(DMA2_Stream1, &DMA_InitStructure);
}

/***************************************************
*函数名:LCD_Window_Configuration
*输入参数:无
*返回值: 无
*函数说明:设置窗口
*
****************************************************/
void LCD_Window_Configuration(void)
{
#ifdef QVGA_SIZE
  LCD_SetDisplayWindow(0, 0, 240, 320);
#endif
  LCD_WriteRAM_Prepare(); 
}
/***************************************************
*函数名:Ov9655_Dcmi_Configuration
*输入参数:无
*返回值:无
*函数说明:DCMI配置
*
****************************************************/
void Ov9655_Dcmi_Configuration(void)
{
  u16 j;
  for(j=0;j<60000;j++);
#ifdef QQVGA_SIZE  
 /*复位 */
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x12, 0x80);
  /* 设置OV9655大小 */
  DCMI_OV9655_QQVGASizeSetup();
 /* 设置 RGB565 模式 */
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, 0x63);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM15, 0x10);
  /*  HRef 信号*/
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM10, 0x08);
#endif

#ifdef QVGA_SIZE 
  /* 复位 */
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS,0x12, 0x80);	
  /* 设置OV9655大小 */
  DCMI_OV9655_QVGASizeSetup();
  /* 设置 RGB565 模式 */
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM7, 0x63);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM15, 0x10);

  /* 设置HRef 信号*/
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_COM10, 0x08);
  DCMI_SingleRandomWrite(OV9655_DEVICE_WRITE_ADDRESS, OV9655_MVFP, 0x10);
#endif  
}
/**
  * @}
  */ 

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
