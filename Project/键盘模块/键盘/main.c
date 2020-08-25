/****************************************************************************
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
******************************************************************************/

/*************ͷ�ļ�**********************************************************/
#include "main.h"
#include "stm322xg_eval_keypad.h"
/******************************************************************************
* ��������main
* ��  �ܣ����°����ֱ���ʾ0��1��2��3��4��5��6��7��8��9��a��b��c��d��e��f
* ��  �룺��
* ��  ������  
* ��  ע����
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
			case KEY01_DOWN: /* ��ʾ0 */
			  	i2cWrite_ch452(0x70,0x3F);
				i2cWrite_ch452(0x72,0x3F);
				i2cWrite_ch452(0x74,0x3F);
				i2cWrite_ch452(0x76,0x3F);
				break;
			case KEY02_DOWN: /* ��ʾ1 */
				i2cWrite_ch452(0x70,0x06); 
				i2cWrite_ch452(0x72,0x06);
				i2cWrite_ch452(0x74,0x06);
				i2cWrite_ch452(0x76,0x06);
				break;
			case KEY03_DOWN: /* ��ʾ2 */
				i2cWrite_ch452(0x70,0x5B); 
				i2cWrite_ch452(0x72,0x5B);
				i2cWrite_ch452(0x74,0x5B);
				i2cWrite_ch452(0x76,0x5B);
				break;
			case KEY04_DOWN: /* ��ʾ3 */
				i2cWrite_ch452(0x70,0x4F); 
				i2cWrite_ch452(0x72,0x4F);
				i2cWrite_ch452(0x74,0x4F);
				i2cWrite_ch452(0x76,0x4F);
				break;
			case KEY05_DOWN: /* ��ʾ4 */
				i2cWrite_ch452(0x70,0x66); 
				i2cWrite_ch452(0x72,0x66);
				i2cWrite_ch452(0x74,0x66);
				i2cWrite_ch452(0x76,0x66);
				break;
			case KEY06_DOWN: /* ��ʾ5 */
				i2cWrite_ch452(0x70,0x6D); 
				i2cWrite_ch452(0x72,0x6D);
				i2cWrite_ch452(0x74,0x6D);
				i2cWrite_ch452(0x76,0x6D);
				break;
			case KEY07_DOWN: /* ��ʾ6 */
				i2cWrite_ch452(0x70,0x7D); 
				i2cWrite_ch452(0x72,0x7D);
				i2cWrite_ch452(0x74,0x7D);
				i2cWrite_ch452(0x76,0x7D);
				break;
			case KEY08_DOWN: /* ��ʾ7 */
				i2cWrite_ch452(0x70,0x07); 
				i2cWrite_ch452(0x72,0x07);
				i2cWrite_ch452(0x74,0x07);
				i2cWrite_ch452(0x76,0x07);
				break;
			case KEY09_DOWN: /* ��ʾ8 */
				i2cWrite_ch452(0x70,0x7F); 
				i2cWrite_ch452(0x72,0x7F);
				i2cWrite_ch452(0x74,0x7F);
				i2cWrite_ch452(0x76,0x7F);
				break;
			case KEY10_DOWN: /* ��ʾ9 */
				i2cWrite_ch452(0x70,0x6F); 
				i2cWrite_ch452(0x72,0x6F);
				i2cWrite_ch452(0x74,0x6F);
				i2cWrite_ch452(0x76,0x6F);
				break;
			case KEY11_DOWN: /* ��ʾa */
				i2cWrite_ch452(0x70,0x77); 
				i2cWrite_ch452(0x72,0x77);
				i2cWrite_ch452(0x74,0x77);
				i2cWrite_ch452(0x76,0x77);
				break;
			case KEY12_DOWN: /* ��ʾb */
				i2cWrite_ch452(0x70,0x7C); 
				i2cWrite_ch452(0x72,0x7C);
				i2cWrite_ch452(0x74,0x7C);
				i2cWrite_ch452(0x76,0x7C);
				break;
			case KEY13_DOWN: /* ��ʾc */
				i2cWrite_ch452(0x70,0x58); 
				i2cWrite_ch452(0x72,0x58);
				i2cWrite_ch452(0x74,0x58);
				i2cWrite_ch452(0x76,0x58);
				break;
			case KEY14_DOWN: /* ��ʾd */
				i2cWrite_ch452(0x70,0x5E); 
				i2cWrite_ch452(0x72,0x5E);
				i2cWrite_ch452(0x74,0x5E);
				i2cWrite_ch452(0x76,0x5E);
				break;
			case KEY15_DOWN: /* ��ʾe */
				i2cWrite_ch452(0x70,0x79); 
				i2cWrite_ch452(0x72,0x79);
				i2cWrite_ch452(0x74,0x79);
				i2cWrite_ch452(0x76,0x79);
				break;
			case KEY16_DOWN: /* ��ʾf */
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

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*************/
