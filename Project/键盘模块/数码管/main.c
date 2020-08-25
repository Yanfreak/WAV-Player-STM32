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
******************************************************************************/
/************************************************************************************
*函数名:
*函数说明:
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
void Delay(int uCount)
{
	int i;
	i=50*uCount;
	while(i--);
}
/*************头文件**********************************************************/
#include "main.h"
#include "stm322xg_eval_keypad.h"

/******************************************************************************
* 函数名：main
* 功  能：数码管依次显示0、1、2、3、4、5、6、7、8、9、a、1234、5678，时间间隔2秒
* 输  入：无
* 输  出：无  
* 备  注：无
********************************************************************************/
int main(void)
{ 
	uint32_t i;
	InitKey();

	while(1)
	{
		for(i=0;i<=12;i++)
		{
			switch(i)
			{
				case 0:/*显示4个0*/
				  	i2cWrite_ch452(0x70,0x3F);
					i2cWrite_ch452(0x72,0x3F);
					i2cWrite_ch452(0x74,0x3F);
					i2cWrite_ch452(0x76,0x3F);
					Delay(200000);
					break;
				case 1:/*显示4个1*/
					i2cWrite_ch452(0x70,0x06); 
					i2cWrite_ch452(0x72,0x06);
					i2cWrite_ch452(0x74,0x06);
					i2cWrite_ch452(0x76,0x06);
					Delay(200000);
					break;
				case 2: /*显示4个2*/
					i2cWrite_ch452(0x70,0x5B); 
					i2cWrite_ch452(0x72,0x5B);
					i2cWrite_ch452(0x74,0x5B);
					i2cWrite_ch452(0x76,0x5B);
					Delay(200000);
					break;
				case 3:/*显示4个3*/
					i2cWrite_ch452(0x70,0x4F); 
					i2cWrite_ch452(0x72,0x4F);
					i2cWrite_ch452(0x74,0x4F);
					i2cWrite_ch452(0x76,0x4F);
					Delay(200000);
					break;
				case 4:/*显示4个4*/
					i2cWrite_ch452(0x70,0x66); 
					i2cWrite_ch452(0x72,0x66);
					i2cWrite_ch452(0x74,0x66);
					i2cWrite_ch452(0x76,0x66);
					Delay(200000);
					break;
				case 5: /*显示4个5*/
					i2cWrite_ch452(0x70,0x6D); 
					i2cWrite_ch452(0x72,0x6D);
					i2cWrite_ch452(0x74,0x6D);
					i2cWrite_ch452(0x76,0x6D);
					Delay(200000);
					break;
				case 6: /*显示4个6*/
					i2cWrite_ch452(0x70,0x7D); 
					i2cWrite_ch452(0x72,0x7D);
					i2cWrite_ch452(0x74,0x7D);
					i2cWrite_ch452(0x76,0x7D);
					Delay(200000);
					break;
				case 7: /*显示4个7*/
					i2cWrite_ch452(0x70,0x07); 
					i2cWrite_ch452(0x72,0x07);
					i2cWrite_ch452(0x74,0x07);
					i2cWrite_ch452(0x76,0x07);
					Delay(200000);
					break;
				case 8: /*显示4个8*/
					i2cWrite_ch452(0x70,0x7F); 
					i2cWrite_ch452(0x72,0x7F);
					i2cWrite_ch452(0x74,0x7F);
					i2cWrite_ch452(0x76,0x7F);
					Delay(200000);
					break;
				case 9: /*显示4个9*/
					i2cWrite_ch452(0x70,0x6F); 
					i2cWrite_ch452(0x72,0x6F);
					i2cWrite_ch452(0x74,0x6F);
					i2cWrite_ch452(0x76,0x6F);
					Delay(200000);
					break;
				case 10: /*显示1个小写字母a，依次在4个数码管交替变化*/
					i2cWrite_ch452(0x70,0x77); 
					Delay(200000);
					i2cWrite_ch452(0x72,0x77);
					Delay(200000);			
					i2cWrite_ch452(0x74,0x77);
					Delay(200000);
					i2cWrite_ch452(0x76,0x77);
					Delay(200000);
					break;
				case 11:/*显示1234*/			 
					i2cWrite_ch452(0x76,0x06);
				  	Delay(200000);
					i2cWrite_ch452(0x74,0x5B);
					Delay(200000);
					i2cWrite_ch452(0x72,0x4F);
					Delay(200000);
					i2cWrite_ch452(0x70,0x66);
					Delay(200000);
					break;
				case 12: /*显示5678*/	
					i2cWrite_ch452(0x76,0x6D);
					Delay(200000);
					i2cWrite_ch452(0x74,0x7D);
					Delay(200000);
					i2cWrite_ch452(0x72,0x07);
					Delay(200000);
					i2cWrite_ch452(0x70,0x7F);
					Delay(200000);
					break;
				default :
					break;
			}
		} 
		i = 0;
	} 
}

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*************/
