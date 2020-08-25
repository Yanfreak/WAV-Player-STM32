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
#include "ps2.h" 
#include "stm322xg_eval_systick.h"

/*PS2������ʱ��Ƶ����10~20Khz(���33K),��/�͵�ƽ�ĳ���ʱ��Ϊ25~50us֮��*/

/*************�궨��**********************************************************/
u8 PS2_Status=CMDMODE; //PS2_Status��ǰ״̬��־,Ĭ��Ϊ����ģʽ 
u8 PS2_DATA_BUF[16];   //ps2���ݻ�����
u8 BIT_Count=0;	//λ������

/****************************************************************************
* ������: 
* �� ��: ����PS2��IO�ܽ�
* �� ��: ��
* �� ��: ��
****************************************************************************/	   
void PS2_SET_SCL_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 	
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_UP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);	
}

void PS2_SET_SCL_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void PS2_SET_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

void PS2_SET_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

/****************************************************************************
* ������: EXTI9_5_IRQHandler()
* �� ��: �ж�9~5������
* �� ��: ��
* �� ��: ��
* ��ע��ÿ11��bit,Ϊ����1���ֽڣ�ÿ������һ����(11λ)��,�豸���ٻ�ȴ�50ms�ٷ�����һ����
****************************************************************************/	   
void EXTI15_10_IRQHandler(void)
{	  	 
	static u8 tempdata=0;
	static u8 parity=0;  	   
	if(EXTI_GetITStatus(EXTI_Line12) != RESET) //���ָ����EXTI7��·�������������
		{
			EXTI_ClearITPendingBit(EXTI_Line12);  //���EXTI7��·����λ  
			if(BIT_Count==0)
			{
				parity=0;
				tempdata=0;
			}
			BIT_Count++;    
			if(BIT_Count>1&&BIT_Count<10)//����������
			{	  
				tempdata>>=1;
				if(PS2_SDA)
				{
					tempdata|=0x80;
					parity++;//��¼1�ĸ���
				}   
			}
			else if(BIT_Count==10)//�õ�У��λ
				{
					if(PS2_SDA)parity|=0x80;//У��λΪ1
				}	   
			if(BIT_Count==11)//���յ�1���ֽڵ�������
			{
				BIT_Count=parity&0x7f;//ȡ��1�ĸ���	  
				if(((BIT_Count%2==0)&&(parity&0x80))||((BIT_Count%2==1)&&(parity&0x80)==0))//��żУ��OK
				{					       
					BIT_Count=PS2_Status&0x0f;		
					PS2_DATA_BUF[BIT_Count]=tempdata;//��������
					if(BIT_Count<15)PS2_Status++;    //���ݳ��ȼ�1
					BIT_Count=PS2_Status&0x30;	     //�õ�ģʽ	  
					switch(BIT_Count)
					{
						case CMDMODE://����ģʽ��,ÿ�յ�һ���ֽڶ�������������
							PS2_Dis_Data_Report();//��ֹ���ݴ���
							PS2_Status|=1<<7; //��ǵõ�����
							break;
						case KEYBOARD:
							break;
						case MOUSE:
							if(MOUSE_ID==0)//��׼���,3���ֽ�
								{
									if((PS2_Status&0x0f)==3)
										{
											PS2_Status|=1<<7;//��ǵõ�����
											PS2_Dis_Data_Report();//��ֹ���ݴ���
										}
								}
							else if(MOUSE_ID==3)//��չ���,4���ֽ�
								{
									if((PS2_Status&0x0f)==4)
										{
											PS2_Status|=1<<7;//��ǵõ�����
											PS2_Dis_Data_Report();//��ֹ���ݴ���
										}
								}	 
							break;
						}		   		 
					}
				else
					{
						PS2_Status|=1<<6;//���У�����
						PS2_Status&=0xf0;//����������ݼ�����
					}
				BIT_Count=0;
				} 	 	  
		}			  
}

/****************************************************************************
* ������: PS2_Dis_Data_Report()
* �� ��: ��ʱ��������,��ֹ���ݴ���
* �� ��: ��
* �� ��: ��
****************************************************************************/
void PS2_Dis_Data_Report(void)
{
	PS2_Set_Int(0);   //�ر��ж�
	PS2_SET_SCL_OUT();//����SCLΪ���
	Clr_PS2_SCL_OUT;    //���ƴ���
}

/****************************************************************************
* ������: PS2_En_Data_Report()
* �� ��: ʹ�����ݴ��䣬�ͷ�ʱ����
* �� ��: ��
* �� ��: ��
****************************************************************************/
void PS2_En_Data_Report(void)
{
	PS2_SET_SCL_IN(); //����SCLΪ����
	PS2_SET_SDA_IN(); //SDA IN
	Set_PS2_SCL_OUT;  //����   
	Set_PS2_SDA_OUT; 
	PS2_Set_Int(1);   //�����ж�
}

/****************************************************************************
* ������: PS2_Set_Int()
* �� ��: PS2�ж��������ã�en:1������;0���ر�;
* �� ��: 0��1
* �� ��: ��
****************************************************************************/
void PS2_Set_Int(u8 en)
{
	if(en)EXTI->IMR|=1<<12;//������line7�ϵ��ж�
	else EXTI->IMR&=~(1<<12);//����line7�ϵ��ж�   
}

/****************************************************************************
* ������: Wait_PS2_Scl()
* �� ��: �ȴ�PS2ʱ����sta״̬�ı䣬sta:1���ȴ���Ϊ1;0���ȴ���Ϊ0��
* �� ��: 0��1
* �� ��: ����ֵ:0��ʱ���߱����sta;1����ʱ���;
****************************************************************************/	
u8 Wait_PS2_Scl(u8 sta)
{
	u16 t=0;
	sta=!sta;
	while(PS2_SCL==sta)
		{
			delay_us(1);
			t++;
			if(t>16000)return 1;//ʱ��������豸����10ms�ڼ�����״̬)
		}
	return 0;//��������
}

/****************************************************************************
* ������: PS2_Get_Byte()
* �� ��: �ڷ�������/����֮��,�ȴ��豸Ӧ��,�ú���������ȡӦ��
* �� ��: ��
* �� ��: ����0����PS2_Status.6=1��������˴���
****************************************************************************/
u8 PS2_Get_Byte(void)
{
	u16 t=0;
	u8 temp=0;
	while(1)//���ȴ�55ms
		{
			t++;
			delay_us(10);
			if(PS2_Status&0x80)//�õ���һ������
				{
					temp=PS2_DATA_BUF[PS2_Status&0x0f-1];
					PS2_Status&=0x70;//��������������յ����ݱ�
					break;	
				}
			else if(t>5500||PS2_Status&0x40)break;//��ʱ���/���մ�
		}
	PS2_En_Data_Report();//ʹ�����ݴ���
	return temp;    
}	 

/****************************************************************************
* ������: PS2_Send_Cmd()
* �� ��: ����һ�����PS2
* �� ��: 
* �� ��: ����ֵ:0���޴���,����,�������
****************************************************************************/  
u8 PS2_Send_Cmd(u8 cmd)
{
	u8 i;
	u8 high=0;//��¼1�ĸ���		 
	PS2_Set_Int(0);   //�����ж�
	PS2_SET_SCL_OUT();//����SCLΪ���
	PS2_SET_SDA_OUT();//SDA OUT
	Clr_PS2_SCL_OUT;//����ʱ����
	delay_us(120);//��������100us
	Clr_PS2_SDA_OUT;//����������
	delay_us(10);
	PS2_SET_SCL_IN();//�ͷ�ʱ����,����PS2�豸�õ���һ��λ,��ʼλ
	Set_PS2_SCL_OUT;
	if(Wait_PS2_Scl(0)==0)//�ȴ�ʱ������
	{									  
		for(i=0;i<8;i++)
			{
				if(cmd&0x01)
				{
					Set_PS2_SDA_OUT;
					high++;
				}
			else Clr_PS2_SDA_OUT;   
			cmd>>=1;
			//��Щ�ط�û�м�����,��Ϊ��Щ�ط����������ѭ��
			Wait_PS2_Scl(1);//�ȴ�ʱ������ ����8��λ
			Wait_PS2_Scl(0);//�ȴ�ʱ������
			}
		if((high%2)==0)Set_PS2_SDA_OUT//����У��λ 10
		else Clr_PS2_SDA_OUT;
		Wait_PS2_Scl(1); //�ȴ�ʱ������ 10λ
		Wait_PS2_Scl(0); //�ȴ�ʱ������
		Set_PS2_SDA_OUT;   //����ֹͣλ  11	  
		Wait_PS2_Scl(1);//�ȴ�ʱ������ 11λ
		PS2_SET_SDA_IN();//SDA in
		Wait_PS2_Scl(0);//�ȴ�ʱ������

		if(PS2_SDA==0)Wait_PS2_Scl(1);//�ȴ�ʱ������ 12λ 
		else 
			{
				PS2_En_Data_Report();
				return 1;//����ʧ��
			}		
	}
	else 
		{
			PS2_En_Data_Report();
			return 2;//����ʧ��
		}
	
	PS2_En_Data_Report();
	return 0;    //���ͳɹ�
}

/****************************************************************************
* ������: PS2_Init()
* �� ��: PS2��ʼ��
* �� ��: 
* �� ��: ����ֵ:0���޴���,����,�������
****************************************************************************/  	
void PS2_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO
	NVIC_InitTypeDef NVIC_InitStructure;	//�ж�
	EXTI_InitTypeDef EXTI_InitStructure;	//�ⲿ�ж���

	RCC_AHB1PeriphClockCmd(	RCC_AHB1Periph_GPIOC|RCC_AHB1Periph_GPIOF, ENABLE );
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
							  			
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_UP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_UP;	
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);

	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource12); //ѡ��PF7���ڵ�GPIO�ܽ������ⲿ�ж���·EXIT7   		   
	 			
	EXTI_InitStructure.EXTI_Line = EXTI_Line12;	//�ⲿ��·EXIT7
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//�����ⲿ�ж�ģʽ:���� EXTI��·Ϊ�¼����� 
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�ⲿ�жϴ�����ѡ��:����������·�½���Ϊ�ж�����
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //ʹ���ⲿ�ж���״̬
	EXTI_Init(&EXTI_InitStructure); //����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn; //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ⲿ�ж�ͨ��
	NVIC_Init(&NVIC_InitStructure); //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���	 
} 

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*****************/
