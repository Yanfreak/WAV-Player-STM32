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

/*************头文件**********************************************************/
#include "main.h"
#include "stm322xg_eval_keypad.h"
/******************************************************************************
* 函数名：main
* 功  能：按下按键分别显示0、1、2、3、4、5、6、7、8、9、a、b、c、d、e、f
* 输  入：无
* 输  出：无  
* 备  注：无
********************************************************************************/
int main(void)
{ 
	uint8_t key=0;
    Keypad_Interrupt(0,0,0);
	InitKey();
	while(1)						
	{
        key=KeyCode;
		switch(key)
		{
			case KEY01_DOWN: /* 显示0 */
			  	i2cWrite_ch452(0x70,0x3F);
				i2cWrite_ch452(0x72,0x3F);
				i2cWrite_ch452(0x74,0x3F);
				i2cWrite_ch452(0x76,0x3F);
				break;
			case KEY02_DOWN: /* 显示1 */
				i2cWrite_ch452(0x70,0x06); 
				i2cWrite_ch452(0x72,0x06);
				i2cWrite_ch452(0x74,0x06);
				i2cWrite_ch452(0x76,0x06);
				break;
			case KEY03_DOWN: /* 显示2 */
				i2cWrite_ch452(0x70,0x5B); 
				i2cWrite_ch452(0x72,0x5B);
				i2cWrite_ch452(0x74,0x5B);
				i2cWrite_ch452(0x76,0x5B);
				break;
			case KEY04_DOWN: /* 显示3 */
				i2cWrite_ch452(0x70,0x4F); 
				i2cWrite_ch452(0x72,0x4F);
				i2cWrite_ch452(0x74,0x4F);
				i2cWrite_ch452(0x76,0x4F);
				break;
			case KEY05_DOWN: /* 显示4 */
				i2cWrite_ch452(0x70,0x66); 
				i2cWrite_ch452(0x72,0x66);
				i2cWrite_ch452(0x74,0x66);
				i2cWrite_ch452(0x76,0x66);
				break;
			case KEY06_DOWN: /* 显示5 */
				i2cWrite_ch452(0x70,0x6D); 
				i2cWrite_ch452(0x72,0x6D);
				i2cWrite_ch452(0x74,0x6D);
				i2cWrite_ch452(0x76,0x6D);
				break;
			case KEY07_DOWN: /* 显示6 */
				i2cWrite_ch452(0x70,0x7D); 
				i2cWrite_ch452(0x72,0x7D);
				i2cWrite_ch452(0x74,0x7D);
				i2cWrite_ch452(0x76,0x7D);
				break;
			case KEY08_DOWN: /* 显示7 */
				i2cWrite_ch452(0x70,0x07); 
				i2cWrite_ch452(0x72,0x07);
				i2cWrite_ch452(0x74,0x07);
				i2cWrite_ch452(0x76,0x07);
				break;
			case KEY09_DOWN: /* 显示8 */
				i2cWrite_ch452(0x70,0x7F); 
				i2cWrite_ch452(0x72,0x7F);
				i2cWrite_ch452(0x74,0x7F);
				i2cWrite_ch452(0x76,0x7F);
				break;
			case KEY10_DOWN: /* 显示9 */
				i2cWrite_ch452(0x70,0x6F); 
				i2cWrite_ch452(0x72,0x6F);
				i2cWrite_ch452(0x74,0x6F);
				i2cWrite_ch452(0x76,0x6F);
				break;
			case KEY11_DOWN: /* 显示a */
				i2cWrite_ch452(0x70,0x77); 
				i2cWrite_ch452(0x72,0x77);
				i2cWrite_ch452(0x74,0x77);
				i2cWrite_ch452(0x76,0x77);
				break;
			case KEY12_DOWN: /* 显示b */
				i2cWrite_ch452(0x70,0x7C); 
				i2cWrite_ch452(0x72,0x7C);
				i2cWrite_ch452(0x74,0x7C);
				i2cWrite_ch452(0x76,0x7C);
				break;
			case KEY13_DOWN: /* 显示c */
				i2cWrite_ch452(0x70,0x58); 
				i2cWrite_ch452(0x72,0x58);
				i2cWrite_ch452(0x74,0x58);
				i2cWrite_ch452(0x76,0x58);
				break;
			case KEY14_DOWN: /* 显示d */
				i2cWrite_ch452(0x70,0x5E); 
				i2cWrite_ch452(0x72,0x5E);
				i2cWrite_ch452(0x74,0x5E);
				i2cWrite_ch452(0x76,0x5E);
				break;
			case KEY15_DOWN: /* 显示e */
				i2cWrite_ch452(0x70,0x79); 
				i2cWrite_ch452(0x72,0x79);
				i2cWrite_ch452(0x74,0x79);
				i2cWrite_ch452(0x76,0x79);
				break;
			case KEY16_DOWN: /* 显示f */
				i2cWrite_ch452(0x70,0x71); 
				i2cWrite_ch452(0x72,0x71);
				i2cWrite_ch452(0x74,0x71);
				i2cWrite_ch452(0x76,0x71);
				break;
			default :
				break;
		}
		key=0;
	}  
}

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束*************/
