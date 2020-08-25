 /****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� stm322xg_eval_keypad.h
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
#ifndef __KEYPAD_H
#define __KEYPAD_H

/*************�궨��**********************************************************/
/******************************************************************************
*          [GPOE][INTM][SSPD][DPLR]0[KEYB][DISP]
*            0     1     0      0  0   1    0
*	GPOE(ͨ���������ʹ��--��DIG6��DIG7����):0:������ʾ�����������ֹͨ�����
*	INTM(�����ж������ʽ):0:�͵�ƽ��Ч(��ƽ������ж�) 1:�͵�ƽ����(�����ж�)
*	SSPD(��˸�ٶ�/Ƶ��):0:����(Լ1.5Hz) 1:���٣�Լ3Hz��
*	DPLR(������DIG �������):0:�͵�ƽ��Ч    1:�ߵ�ƽ��Ч
*	KEYB(����ɨ�蹦�ܵ�ʹ��):0:�رռ���ɨ��  1:���ü���ɨ��
*   DISP(��ʾ�������ܵ�ʹ��):0:�ر���ʾ����  1:������ʾ����
*******************************************************************************/  
#define CH452SYS_INTM	(1<<5) /* 100000 */
#define CH452SYS_SSPD	(1<<4)	
#define CH452SYS_DPLR	(1<<3)	
#define CH452SYS_KEYB	(1<<1)		
#define CH452SYS_DISP	(1<<0)
/*********������ֵ����**********************************/
#define KEY01_DOWN	0x40
#define KEY01_UP	0x00
#define KEY02_DOWN  0x41
#define KEY02_UP	0x01
#define KEY03_DOWN	0x42
#define KEY03_UP	0x02
#define KEY04_DOWN	0x43
#define KEY04_UP	0x03
#define KEY05_DOWN	0x48
#define KEY05_UP	0x08
#define KEY06_DOWN	0x49
#define KEY06_UP	0x09
#define KEY07_DOWN	0x4a
#define KEY07_UP	0x0a
#define KEY08_DOWN	0x4b
#define KEY08_UP	0x0b
#define KEY09_DOWN	0x50
#define KEY09_UP	0x10
#define KEY10_DOWN	0x51
#define KEY10_UP	0x11
#define KEY11_DOWN	0x52
#define KEY11_UP	0x12
#define KEY12_DOWN	0x53
#define KEY12_UP	0x13
#define KEY13_DOWN	0x58
#define KEY13_UP	0x18
#define KEY14_DOWN	0x59
#define KEY14_UP	0x19
#define KEY15_DOWN	0x5a
#define KEY15_UP	0x1a
#define KEY16_DOWN	0x5b
#define KEY16_UP	0x1b

#define SDA_SET		GPIO_SetBits(GPIOB,GPIO_Pin_11) /* �������ݵ�ƽ */
#define SDA_CLR		GPIO_ResetBits(GPIOB,GPIO_Pin_11) /* �������ݵ�ƽ */
#define SCL_SET		GPIO_SetBits(GPIOB,GPIO_Pin_10)	/* ����ʱ�ӵ�ƽ */
#define SCL_CLR		GPIO_ResetBits(GPIOB,GPIO_Pin_10) /* ����ʱ�ӵ�ƽ */


#define KEYPAD_INT_GROUP       NVIC_PriorityGroup_0
#define KEYPAD_PRI      1
#define KEYPAD_SUBPRI   0
/*************��������********************************************************/

unsigned char i2cRead_ch452(unsigned char slaveAddr,unsigned char *data);
unsigned char i2cWrite_ch452(unsigned char slaveAddr,unsigned char data);
void InitKey(void);
int I2C_ByteWrite(unsigned short data,unsigned char slaveAddr);
extern u8 KeyCode;
void Keypad_Interrupt(u32 group,u32 PreemptionPriority,u32 SubPriority);
unsigned char GetKeyNum(void);
#endif

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*************/
