/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� sessor.c
*   ժ Ҫ    �� �¶ȡ�ʪ�ȴ�����ʵ��������
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
#include "stm322xg_eval_lcd.h"
#include "sensor.h"

/*************�궨��**********************************************************/

/*************��ͨ��16λ��������**********************************************/
#define AIN0                    0X0C00
#define AIN1                    0X1C00
#define AIN2                    0X2C00
#define AIN3                    0X3C00
#define AIN4                    0X4C00
#define AIN5                    0X5C00
#define AIN6                    0X6C00
#define AIN7                    0X7C00
#define AIN8                    0X8C00
#define AIN9                    0X9C00
#define AIN10                   0XAC00

/*************�Լ�ģʽ��������**********************************************/
#define VrefH_L_DIV2            0XBC00		/* �ڲ� (VrefH-VrefL)/2 */
#define VrefL	                0XCC00		/* �ڲ� �ڲ� VrefL */
#define VrefH    	            0XDC00		/* �ڲ� VrefH */

/*************PowerDown����*************************************************/
#define AD_POWERDOWN            0XEC00

/*************ͨ������******************************************************/
#define ADC_CHANNEL_CNT			15

/*************ADC Vref+*****************************************************/
#define ADC_Vref_POS			2.5f

/*************ADC Vref-*****************************************************/
#define ADC_Vref_NEG			0

/*************ADC ���******************************************************/
#define ADC_SCALE_FACT			(ADC_Vref_POS-ADC_Vref_NEG)/(1<<12)

/*************ʪ�ȴ���������źŷŴ���************************************/
#define ADC_HUMIDITY_AMP_FACT	1.0f

/*************��������Դ��ѹ************************************************/
#define ADC_SENSOR_VCC			5.0f

/*************��������********************************************************/
u16 tlv2543_read_data(u16 cmd);
void tlv2543_init(void);
void disp_lcd_msg(void);
void lcd_app(u16 adc_data2,float humidity);
void Senssor_RCC_Configuration(void);
void Senssor_GPIO_Configuration(void);
void Senssor_SPI_Configuration(void);
void Senssor_delay(__IO uint32_t nCount);
void Senssor_delay_ms(int z);
int Sessor(void);
extern void STM3220F_LCD_Init(void);

/****************************************************************************
* ������: Sessor()
* �� ��: ADC����������
* �� ��: ��
* �� ��: ��
****************************************************************************/
int Sessor(void)
{
	float humidity = 0;
	float volt;
	uint16_t adc_data2 = 0;

	Senssor_RCC_Configuration();
	Senssor_GPIO_Configuration();
	Senssor_SPI_Configuration();

	tlv2543_init();	/* ��ʼ��ADC */
	SensorDataInit(); /* ��ʼ��Sensor Data */
	STM3220F_LCD_Init(); /* LCD��ʼ�� */
	disp_lcd_msg();/* ��ʾLCD��Ϣ */

	while(1)
	{
		/* ��ò���ֵ����ȡadc ͨ��1, 16bit */
		adc_data2 = tlv2543_read_data(AIN1);

		/* ���ʪ�� */
		volt = adc_data2*ADC_SCALE_FACT/ADC_HUMIDITY_AMP_FACT;
		humidity = HumiditySensor(volt); /* ���ʪ�� */

		/* ��ʾ */
		lcd_app(adc_data2,humidity);
		Senssor_delay_ms(2000);
	}
}

/****************************************************************************
* ������: tlv2543_read_data()
* �� ��: ���ADC����
* �� ��: ADC����
* �� ��: ����ֵ
****************************************************************************/
u16 tlv2543_read_data(u16 cmd)
{
	u16 data;

	/* �ȴ�spi txΪ�� */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	GPIO_ResetBits(GPIOC, GPIO_Pin_3); /*  ADC CS = 0 */
	Senssor_delay(50);
	SPI_I2S_SendData(SPI2,cmd);	/* ����ADC���� */

	/* �ȴ�spi rx��Ϊ�� */
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);
	SPI_I2S_ReceiveData(SPI2); /* ��ȡadc���� */
	
	Senssor_delay(50); /* �ȴ�adc��� */
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7)==RESET); /* �ȴ�adcת����� */

	/* ��ʼ���ͺͶ�ȡadc���� */
	while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(SPI2,cmd);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);
	data = SPI_I2S_ReceiveData(SPI2);
	GPIO_SetBits(GPIOC, GPIO_Pin_3); /* ADC CS = 1 */

	Senssor_delay(50); /* �ȴ�adc��� */
	while(GPIO_ReadInputDataBit(GPIOG,GPIO_Pin_7)==RESET); /* �ȴ�adcת����� */
	data=data>>4;
	return data;
}

/****************************************************************************
* ������: tlv2543_init()
* �� ��: ��ʼ��ADC
* �� ��: ��
* �� ��: ��
****************************************************************************/
void tlv2543_init(void)
{
	/* д0x0000,��λADC */
	tlv2543_read_data(0x0);
}

/****************************************************************************
* ������: disp_lcd_msg()
* �� ��: ��lcd��ʾ������Ϣ
* �� ��: ��
* �� ��: ��
****************************************************************************/
void disp_lcd_msg(void)
{
	char buf[0x64];
	LCD_Clear(Blue); /* ������������ɫ */
	LCD_SetBackColor(Blue); /* �������ֱ�����ɫ */
	LCD_SetTextColor(White); /* ����������ɫ */

	sprintf(buf,"/************************************/");
	LCD_DisplayStringHZ(8, 8,(uint8_t *)buf);

	sprintf(buf,"*                   ʪ��ʵʱ���     *");
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
void lcd_app(u16 adc_data2,float humidity)
{
	#define Large_Font_Hight	64
	#define Font_16_Hight		16
	#define Large_Font_Width	32
	#define Font_16_Width		16

	#define Large_Font_Interval	16
	#define	Interval_Hight		8

	char buf[0x20];
	int start_x,start_y;
	int start_x_2;
	int x,y;

	LCD_SetBackColor(Blue);	/* �������ֱ�����ɫ */
	LCD_SetTextColor(White); /* ����������ɫ */

	start_x = 12;
	start_y = 133;

	x = start_x;
	y = start_y+12;
	sprintf(buf,"ADC0 ����ֵ");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x+5*Font_16_Width/2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"0x%04x",adc_data2);
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	start_x_2 = 144;
	LCD_DrawLine(122,85,Large_Font_Hight*2+Large_Font_Interval,Vertical);

	x = start_x_2;
	y = start_y+12;
	sprintf(buf,"ʪ");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	x = start_x_2;
	y += Interval_Hight+Font_16_Hight;
	sprintf(buf,"��");
	LCD_DisplayStringHZ(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);		/* �������ֱ�����ɫ */
	LCD_SetTextColor(Yellow);	/* ����������ɫ */

	x = start_x_2+Font_16_Width+8;
	y = start_y;

	/* ������岿������ */
	LCD_DrawRectFill(x,y,x+Large_Font_Width*5,y+Large_Font_Hight,Blue);

	sprintf(buf,"%4.1f",humidity);
	LCD_DisplayStringLargeFont(x, y,(uint8_t *)buf);

	LCD_SetBackColor(Blue);	/* �������ֱ�����ɫ */
	LCD_SetTextColor(White); /* ����������ɫ */

	if(strlen(buf)<=4)	/* �¶�����С�ڵ���4�ֽڳ�����ʾ��λ */
	{
		x = start_x_2+Large_Font_Width*4+Font_16_Width;
		y = start_y+12+Interval_Hight+Font_16_Hight;
		sprintf(buf," %%");
		LCD_DisplayStringHZ(x, y,(uint8_t *)buf);
	}
}

/****************************************************************************
* ������: Senssor_RCC_Configuration()
* �� ��: ���ò�ͬ��ϵͳʱ��
* �� ��: �� 
* �� ��: ��
****************************************************************************/
void Senssor_RCC_Configuration(void)
{
 	/*�ڴ˽׶δ�������ʱ���Ѿ�����ʼ����ɣ�ʱ�ӳ�ʼ��������SystemInit����ʵ�֣�SystemInit���������startup_stm32f2xx.s�Ļ�������ļ��С�
    *��ϵͳ�ϵ��ʼ������ת��main����֮ǰ������������ȵ���SystemInit������	
    *����û�ϣ����������ʱ�ӿ����ڴ˴��ٴε��� SystemInit()�����ĺ���ʵ�ִ����system_stm32f2xx.c�ļ��С�*/
    SystemInit();

    RCC_PCLK2Config(RCC_HCLK_Div2); /* PCLK2 = HCLK/2 */

	/* ʹ��ʱ�� */
    RCC_APB2PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB
                         | RCC_APB1Periph_SPI2, ENABLE);
}

/****************************************************************************
* ������: Senssor_GPIO_Configuration()
* �� ��: ���ò�ͬ��GPIO�˿�
* �� ��: �� 
* �� ��: ��
****************************************************************************/
void Senssor_GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ��ʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);

	/* DA_CS ������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	/* AD_EOC��©��� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN; 
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	/* DDS_CS��AD_CS������� */ 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	/* DA_FS������� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOF,&GPIO_InitStructure);
	GPIO_SetBits(GPIOF, GPIO_Pin_10);
}

/****************************************************************************
* ������: Senssor_SPI_Configuration()
* �� ��: ����SPI2
* �� ��: �� 
* �� ��: ��
****************************************************************************/
void Senssor_SPI_Configuration(void)
{
	SPI_InitTypeDef   SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	/* ʹ��SPIʱ�� */
	SPI_DeInit(SPI2);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	/* ʹ��GPIOʱ�� */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	/* ʹ��SPI�ĸ��ù���*/  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2);	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
	
	/* ����SPI SCK�ܽ� */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* ����SPI MOSI�ܽ� */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* ����SPI MISO�ܽ� */
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_15;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_SetBits(GPIOG,GPIO_Pin_8); /* DA  CS PG8=1, ���ߵ�ѹ */
	GPIO_SetBits(GPIOC,GPIO_Pin_2); /* DDS CS PC2=1�����ߵ�ѹ */
	GPIO_SetBits(GPIOC,GPIO_Pin_3); /* AD  CS PC4=1�����ߵ�ѹ */

	/* ����SPI */
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; /* ȫ˫��ģʽ */
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master; /* ��ģʽ */
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_16b; /* 16λ������ */
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low; /* SCK���� */
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge; /* SCK��һ���ز��� */
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft; /* �������NSS */
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;	/* 64��Ԥ��Ƶ */
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB; /* �ֽ�˳��ΪMSB��ǰ	*/
	SPI_Init(SPI2,&SPI_InitStructure); /* ��ʼ��SPI2 */

	SPI_Cmd(SPI2,ENABLE);
}

/****************************************************************************
* ������: Senssor_delay()
* �� ��: ������ʱ
* �� ��: ��ʱֵ 
* �� ��: ��
****************************************************************************/
void Senssor_delay(__IO uint32_t nCount)
{
  while(nCount--)
  {
  }
}

/****************************************************************************
* ������: Senssor_delay_ms()
* �� ��: ��ʱ�ӳ�(��Լ0.72ms)
* �� ��: ��ʱֵ 
* �� ��: ��
****************************************************************************/
void Senssor_delay_ms(int z)
{
	int j,k;
	for(j=0;j<z;j++)
	{
		for(k=0;k<10000;k++);
	}
}

/****** (C) ��Ȩ2012�������촴�¿Ƽ���չ���޹�˾ ******�ĵ�����**************/
