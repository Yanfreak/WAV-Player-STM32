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
*****************************************************************************/

/*************ͷ�ļ�**********************************************************/	 
#include "mouse.h"

/*************�ⲿ����********************************************************/	 
extern void delay_ms(u16 nms);
extern void delay_us(u32 nus);

/*************��������ͽṹ��************************************************/	 
u8 MOUSE_ID;//����������ID 
PS2_Mouse MouseX;

/****************************************************************************
* ������: Mouse_Data_Pro()
* �� ��: ����MOUSE������
* �� ��: ��
* �� ��: ��
****************************************************************************/ 
void Mouse_Data_Pro(void)
{			    			    
	MouseX.x_pos=(signed char)PS2_DATA_BUF[1];
	MouseX.y_pos=(signed char)PS2_DATA_BUF[2];
	MouseX.y_pos=-MouseX.y_pos;//Y������,��������
	MouseX.z_pos=(signed char)PS2_DATA_BUF[3];		  
	MouseX.bt_mask=PS2_DATA_BUF[0]&0X07;//ȡ������
}	

/****************************************************************************
* ������: Init_Mouse()
* �� ��: ��ʼ�����
* �� ��: ��
* �� ��: 0��1
****************************************************************************/  
u8 Init_Mouse(void)
{
	u8 t;		 
	PS2_Init();							     
	delay_ms(800); //�ȴ��ϵ縴λ���		 	 
	PS2_Status=CMDMODE; //��������ģʽ
	t=PS2_Send_Cmd(PS_RESET);  //��λ���
			 
	if(t!=0)return 1; //��λʧ��
	t=PS2_Get_Byte();	  
	if(t!=0XFA)return 2; //û��Ӧ��
	t=0;
	while((PS2_Status&0x80)==0)//�ȴ���λ��� 
		{
			t++;
			delay_ms(50);      
			if(t>50)return 3;
		}
	PS2_Get_Byte();//�õ�0XAA
	PS2_Get_Byte();//�õ�ID 

	//�������ģʽ�������ʼ������
	PS2_Send_Cmd(SET_SAMPLE_RATE);//�������ò�����
	if(PS2_Get_Byte()!=0XFA)return 4;//����ʧ��
	PS2_Send_Cmd(0XC8);//������200
	if(PS2_Get_Byte()!=0XFA)return 5;//����ʧ��
	PS2_Send_Cmd(SET_SAMPLE_RATE);//�������ò�����
	if(PS2_Get_Byte()!=0XFA)return 6;//����ʧ��
	PS2_Send_Cmd(0X64);//������100
	if(PS2_Get_Byte()!=0XFA)return 7;//����ʧ��
	PS2_Send_Cmd(SET_SAMPLE_RATE);//�������ò�����
	if(PS2_Get_Byte()!=0XFA)return 8;//����ʧ��
	PS2_Send_Cmd(0X50);//������80
	if(PS2_Get_Byte()!=0XFA)return 9;//����ʧ��

	//�������		   
	PS2_Send_Cmd(GET_DEVICE_ID); //��ȡID
	if(PS2_Get_Byte()!=0XFA)return 10;//����ʧ��
	MOUSE_ID=PS2_Get_Byte();//�õ�MOUSE ID	 
	
	PS2_Send_Cmd(SET_SAMPLE_RATE);//�ٴν������ò�����
	if(PS2_Get_Byte()!=0XFA)return 11;//����ʧ��
	PS2_Send_Cmd(0X0A);//������10
	if(PS2_Get_Byte()!=0XFA)return 12;//����ʧ��		 
	PS2_Send_Cmd(GET_DEVICE_ID); //��ȡID
	if(PS2_Get_Byte()!=0XFA)return 13;//����ʧ��
	MOUSE_ID=PS2_Get_Byte();//�õ�MOUSE ID		 
	
	PS2_Send_Cmd(SET_RESOLUTION);  //���÷ֱ���
	if(PS2_Get_Byte()!=0XFA)return 14;//����ʧ��   
	PS2_Send_Cmd(0X03);//8��/mm
	if(PS2_Get_Byte()!=0XFA)return 15;//����ʧ�� 
	PS2_Send_Cmd(SET_SCALING11);   //�������ű���Ϊ1:1
	if(PS2_Get_Byte()!=0XFA)return 16;//����ʧ�� 
	
	PS2_Send_Cmd(SET_SAMPLE_RATE); //���ò�����
	if(PS2_Get_Byte()!=0XFA)return 17;//����ʧ��   
	PS2_Send_Cmd(0X28);//40
	if(PS2_Get_Byte()!=0XFA)return 18;//����ʧ�� 
	
	PS2_Send_Cmd(EN_DATA_REPORT);   //ʹ�����ݱ���
	if(PS2_Get_Byte()!=0XFA)return 19;//����ʧ��
	
	PS2_Status=MOUSE;//�������ģʽ
	return 0;//�޴���,��ʼ���ɹ�   
}

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*****************/
