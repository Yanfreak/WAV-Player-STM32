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
#include "stm322xg_eval_lcd.h"
#include "stm32f2xx.h"
#include "stm32f2xx_exti.h"
#include "mouse.h"
#include "ps2.h" 
#include "stm32_eval.h"
#include "stm322xg_eval.h"
#include "stm322xg_eval_systick.h"

/*************�궨��**********************************************************/
#define Large_Font_Hight	64
#define Font_16_Hight		16
#define Large_Font_Width	32
#define Font_16_Width		16
#define Large_Font_Interval	16
#define	Interval_Hight		8
#define SYSCLK_FREQ_120MHz 120000000

/*************�ⲿ��������****************************************************/
extern void STM3220F_LCD_Init(void);

/*************��������**********************************************************/
static void Delay(u32 nTime);
static void RCC_Configuration(void);
static void NVIC_Configuration(void);
static void disp_lcd_msg(void);
static void disp_lcd_welcome(void);
static void Delay(u32 nTime);
static void lcd_app(int8_t mouse_x,int8_t mouse_y);

/******************************************************************************
* ��������main		 
* ��  �ܣ�������,	x��yֵ��ʾ���Ƕ������ȱ仯�Ŀ���,������ƶ����ʡ�X���������,
*         Y���������꣬����������ƶ�ʱ��Xֵ��������ƶ����ʴ�0��127��2��7������
*         ��������������ƶ�ʱ��Xֵ��������ƶ����ʴ�0��-127�仯������������ƶ�
*         ʱ��Yֵ��������ƶ����ʴ�0��127�仯,����������ƶ�ʱ,Yֵ��������ƶ���
*         �ʴ�0��-127�仯�� 
* ��  �룺��
* ��  ������  
* ��  ע������ƶ���Խ�죬����Խ��
********************************************************************************/
int main(void)
{
	char buf1[0x64];					 
	uint8_t  t=0;
	int8_t mouse_x[2],mouse_y[2];
	uint32_t errcnt=0;
	SysTick_Config(SYSCLK_FREQ_120MHz / 1000);
	RCC_Configuration();
//	NVIC_Configuration();
	STM3220F_LCD_Init(); //��ʼ��Һ����

	disp_lcd_welcome();	 //��ӭ������ʾ
	Delay(200000);
  	disp_lcd_msg();	  //���Խ�����ʾ

RST:t=Init_Mouse();
	Delay(1);
	if(t==0)
	{
		sprintf(buf1,"���ok !");
		LCD_DisplayStringHZ(125, 150,(uint8_t *)buf1);
		Delay(100000);
	}
	else
	{
		sprintf(buf1,"������!");
		LCD_DisplayStringHZ(125, 150,(uint8_t *)buf1);
		Delay(100000);
		goto RST;
	}

	sprintf(buf1,"���ID=0");
	LCD_DisplayStringHZ(125, 150,(uint8_t *)buf1);
	Delay(100000);

	if(MOUSE_ID==3)
	{
		sprintf(buf1,"��갴��=3");
		LCD_DisplayStringHZ(125, 150,(uint8_t *)buf1);
		Delay(100000);
	}

	LCD_Clear(Blue); //������������ɫ
	disp_lcd_msg();  //���Խ�����ʾ
	lcd_app(0,0);

	while(1)
	{		
		if(PS2_Status&0x80)//�õ���һ������
			{	
				Mouse_Data_Pro();//��������
			
				mouse_x[0]=MouseX.x_pos;//���ˮƽ�ƶ�����
				mouse_y[0]=MouseX.y_pos;//�����ֱ�ƶ�����

				lcd_app(mouse_x[0],mouse_y[0]);	 //��ʾ

				if(MouseX.bt_mask&0x01)
					{
						sprintf(buf1,"�������������");
						LCD_DisplayStringHZ(145,157,(uint8_t *)buf1);
					}
				if(MouseX.bt_mask&0x02)
					{
						sprintf(buf1,"�����������Ҽ�");
						LCD_DisplayStringHZ(145, 157,(uint8_t *)buf1);
					}	    
				if(MouseX.bt_mask&0x04)
					{
						sprintf(buf1,"���������껬��");
						LCD_DisplayStringHZ(145, 157,(uint8_t *)buf1);
					} 		 
				PS2_Status=MOUSE;
				PS2_En_Data_Report();//ʹ�����ݱ���
			}
		else if(PS2_Status&0x40)
			{
				errcnt++;
				PS2_Status=MOUSE;
			}
		t++;
		Delay(100);
		if(t==200)t=0;
	} 
}

/******************************************************************************
* ��������Delay
* ��  �ܣ��ӳٺ���
* ��  �룺�ӳ�ʱ��
* ��  ������  
* ��  ע����
********************************************************************************/
static void Delay(u32 nTime)			
{
	int i;
	for(i=0;i<nTime*200;i++);
}

/****************************************************************************
* ������: RCC_Configuration()
* �� ��: ����ϵͳʱ��
* �� ��: ��
* �� ��: ��
****************************************************************************/
static void RCC_Configuration(void)
{
	/*�ڴ˽׶δ�������ʱ���Ѿ�����ʼ����ɣ�ʱ�ӳ�ʼ��������SystemInit����ʵ�֣�SystemInit���������startup_stm32f2xx.s�Ļ�������ļ��С�
    *��ϵͳ�ϵ��ʼ������ת��main����֮ǰ������������ȵ���SystemInit������	
    *����û�ϣ����������ʱ�ӿ����ڴ˴��ٴε��� SystemInit()�����ĺ���ʵ�ִ����system_stm32f2xx.c�ļ��С�*/
	SystemInit();

	/* ʱ��ʹ�� */
   	RCC_AHB1PeriphClockCmd( RCC_AHB1Periph_GPIOA |RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_GPIOC
		|RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOE | RCC_AHB1Periph_GPIOF, ENABLE );
}

/****************************************************************************
* ������: NVIC_Configuration()
* �� ��:  ��ʼ���ж�������
* �� ��: ��
* �� ��: ��
****************************************************************************/
static void NVIC_Configuration(void)
{
	/* ����NVIC��ռ���ȼ����� */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�������ȼ����飺��ռ���ȼ�2��,�����ȼ�2��
	
	//�����������λ�ú�ƫ��
	#ifdef  VECT_TAB_RAM  
		
		/* ������������ʼ 0x20000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);//������λ��RAM

	#else 					   
		/* ������������ʼ 0x08000000 */ 
		NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);//������λ��FLASH
	#endif	
}

/****************************************************************************
* ������: disp_lcd_welcome()
* �� ��: ��lcd��ʾ������Ϣ
* �� ��: ��
* �� ��: ��
****************************************************************************/
void disp_lcd_welcome(void)
{
	char buf[0x64];
	LCD_Clear(Blue); /* ������������ɫ */
	LCD_SetBackColor(Blue); /* �������ֱ�����ɫ */
	LCD_SetTextColor(White); /* ����������ɫ */

	sprintf(buf,"/************************************/");
	LCD_DisplayStringHZ(8, 16,(uint8_t *)buf);

	sprintf(buf,"��ӭ����������");
	LCD_DisplayStringHZ(100, 150,(uint8_t *)buf);

	sprintf(buf,"/************************************/");
	LCD_DisplayStringHZ(8, 210,(uint8_t *)buf);

	LCD_Draw_Eflag_Logo(120,60);
}

/****************************************************************************
* ������: disp_lcd_msg()
* �� ��: ��lcd��ʾ������Ϣ
* �� ��: ��
* �� ��: ��
****************************************************************************/
static void disp_lcd_msg(void)
{
	char buf[0x64];
	LCD_Clear(Blue); /* ������������ɫ */
	LCD_SetBackColor(Blue); /* �������ֱ�����ɫ */
	LCD_SetTextColor(White); /* ����������ɫ */

	sprintf(buf,"/************************************/");
	LCD_DisplayStringHZ(8, 8,(uint8_t *)buf);

	sprintf(buf,"*                  ������          *");
	LCD_DisplayStringHZ(8, 37,(uint8_t *)buf);

	sprintf(buf,"/************************************/");
	LCD_DisplayStringHZ(8, 66,(uint8_t *)buf);

	LCD_Draw_Eflag_Logo(18,24);
}

/****************************************************************************
* ������: lcd_app()
* �� ��: lcd����
* �� ��: ��
* �� ��: ��
****************************************************************************/
static void lcd_app(int8_t mouse_x,int8_t mouse_y)
{
	char buf[0x20];
	int start_x,start_y;
	int start_x_2;
	int x,y;

	LCD_SetBackColor(Blue);	/* �������ֱ�����ɫ */
	LCD_SetTextColor(White); /* ����������ɫ */

	start_x = 12;
	start_y = 93;

	x = start_x;
	y = start_y+12;
	sprintf(buf,"32λ����ֵ");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x+2*Font_16_Width/2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"0x%08x",mouse_x); //��8λ16���ƴ�ӡ������λ��0�����ڻ����32λ���ձ���ĳ�����ʾ
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	start_x_2 = 144;
	LCD_DrawLine(122,start_y,Large_Font_Hight*2+Large_Font_Interval,Vertical);

	x = start_x_2;
	y = start_y+12;
	sprintf(buf,"X");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x_2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"ֵ");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);		/* �������ֱ�����ɫ */
	LCD_SetTextColor(Yellow);	/* ����������ɫ */

	x = start_x_2+Font_16_Width+8;
	y = start_y;

	/* ������岿������ */
	LCD_DrawRectFill(x,y,x+Large_Font_Width*5,y+Large_Font_Hight,Blue);

	sprintf(buf,"%d",mouse_x);
	LCD_DisplayStringLargeFont(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);	/* �������ֱ�����ɫ */
	LCD_SetTextColor(White); /* ����������ɫ */

	start_y += Large_Font_Hight+Large_Font_Interval;

	x = start_x;
	y = start_y+12;
	sprintf(buf,"32λ����ֵ");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x+2*Font_16_Width/2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"0x%08x",mouse_y);//��8λ16���ƴ�ӡ������λ��0�����ڻ����32λ���ձ���ĳ�����ʾ
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x_2;
	y = start_y+12;
	sprintf(buf,"Y");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x_2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"ֵ");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);	/* �������ֱ�����ɫ */
	LCD_SetTextColor(Yellow); /* ����������ɫ */

	x = start_x_2+Font_16_Width+8;
	y = start_y;

	/* ������岿������ */
	LCD_DrawRectFill(x,y,x+Large_Font_Width*5,y+Large_Font_Hight,Blue);

	sprintf(buf,"%d",mouse_y);
	LCD_DisplayStringLargeFont(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);	/* �������ֱ�����ɫ */
	LCD_SetTextColor(White); /* ����������ɫ */
}

#ifdef  USE_FULL_ASSERT
/******************************************************************************
* ��������AssertFailed
* ��  �ܣ�������������λ��
* ��  �룺�������ڵ��ļ����ʹ��������ļ��е��к�
* ��  ������  
* ��  ע����
********************************************************************************/
void AssertFailed(uint8_t* file, uint32_t line)
{ 
 /******�˴���������û����󱨸淽ʽ******/
  while (1)
  {}
}
#endif

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����*****************/
