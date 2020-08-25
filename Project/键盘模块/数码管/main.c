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
/************************************************************************************
*������:
*����˵��:
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
void Delay(int uCount)
{
	int i;
	i=50*uCount;
	while(i--);
}
/*************ͷ�ļ�**********************************************************/
#include "main.h"
#include "stm322xg_eval_keypad.h"

/******************************************************************************
* ��������main
* ��  �ܣ������������ʾ0��1��2��3��4��5��6��7��8��9��a��1234��5678��ʱ����2��
* ��  �룺��
* ��  ������  
* ��  ע����
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
				case 0:/*��ʾ4��0*/
				  	i2cWrite_ch452(0x70,0x3F);
					i2cWrite_ch452(0x72,0x3F);
					i2cWrite_ch452(0x74,0x3F);
					i2cWrite_ch452(0x76,0x3F);
					Delay(200000);
					break;
				case 1:/*��ʾ4��1*/
					i2cWrite_ch452(0x70,0x06); 
					i2cWrite_ch452(0x72,0x06);
					i2cWrite_ch452(0x74,0x06);
					i2cWrite_ch452(0x76,0x06);
					Delay(200000);
					break;
				case 2: /*��ʾ4��2*/
					i2cWrite_ch452(0x70,0x5B); 
					i2cWrite_ch452(0x72,0x5B);
					i2cWrite_ch452(0x74,0x5B);
					i2cWrite_ch452(0x76,0x5B);
					Delay(200000);
					break;
				case 3:/*��ʾ4��3*/
					i2cWrite_ch452(0x70,0x4F); 
					i2cWrite_ch452(0x72,0x4F);
					i2cWrite_ch452(0x74,0x4F);
					i2cWrite_ch452(0x76,0x4F);
					Delay(200000);
					break;
				case 4:/*��ʾ4��4*/
					i2cWrite_ch452(0x70,0x66); 
					i2cWrite_ch452(0x72,0x66);
					i2cWrite_ch452(0x74,0x66);
					i2cWrite_ch452(0x76,0x66);
					Delay(200000);
					break;
				case 5: /*��ʾ4��5*/
					i2cWrite_ch452(0x70,0x6D); 
					i2cWrite_ch452(0x72,0x6D);
					i2cWrite_ch452(0x74,0x6D);
					i2cWrite_ch452(0x76,0x6D);
					Delay(200000);
					break;
				case 6: /*��ʾ4��6*/
					i2cWrite_ch452(0x70,0x7D); 
					i2cWrite_ch452(0x72,0x7D);
					i2cWrite_ch452(0x74,0x7D);
					i2cWrite_ch452(0x76,0x7D);
					Delay(200000);
					break;
				case 7: /*��ʾ4��7*/
					i2cWrite_ch452(0x70,0x07); 
					i2cWrite_ch452(0x72,0x07);
					i2cWrite_ch452(0x74,0x07);
					i2cWrite_ch452(0x76,0x07);
					Delay(200000);
					break;
				case 8: /*��ʾ4��8*/
					i2cWrite_ch452(0x70,0x7F); 
					i2cWrite_ch452(0x72,0x7F);
					i2cWrite_ch452(0x74,0x7F);
					i2cWrite_ch452(0x76,0x7F);
					Delay(200000);
					break;
				case 9: /*��ʾ4��9*/
					i2cWrite_ch452(0x70,0x6F); 
					i2cWrite_ch452(0x72,0x6F);
					i2cWrite_ch452(0x74,0x6F);
					i2cWrite_ch452(0x76,0x6F);
					Delay(200000);
					break;
				case 10: /*��ʾ1��Сд��ĸa��������4������ܽ���仯*/
					i2cWrite_ch452(0x70,0x77); 
					Delay(200000);
					i2cWrite_ch452(0x72,0x77);
					Delay(200000);			
					i2cWrite_ch452(0x74,0x77);
					Delay(200000);
					i2cWrite_ch452(0x76,0x77);
					Delay(200000);
					break;
				case 11:/*��ʾ1234*/			 
					i2cWrite_ch452(0x76,0x06);
				  	Delay(200000);
					i2cWrite_ch452(0x74,0x5B);
					Delay(200000);
					i2cWrite_ch452(0x72,0x4F);
					Delay(200000);
					i2cWrite_ch452(0x70,0x66);
					Delay(200000);
					break;
				case 12: /*��ʾ5678*/	
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

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*************/
