 /**********************************************************
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
**********************************************************/
#include "stm32f2xx.h"
#include "stm32_eval_spi_sd.h"


						   
u8  SD_Type=0;//SD��������

/************************************************************************************
*������:SPIx_Init
*����˵��:SPIģ��ĳ�ʼ������
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/

void SPIx_Init(void)
{	 
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE );	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOD,ENABLE);

	/* Configure SPI1 pins: SCK, MISO and MOSI */
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI3);  
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI3);	
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI3);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //�����������
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure I/O for Flash Chip select */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  //SPI CS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //�����������
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* Deselect the FLASH: Chip Select high */
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
	 
	/* SPI1 configuration */
    //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
    //����SPI����ģʽ:����Ϊ��SPI 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ		
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    //ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�		
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    //���ݲ����ڵڶ���ʱ����	
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    //NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//SPI_NSS_Soft;
    //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256		
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ	
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    //CRCֵ����Ķ���ʽ	
	SPI_InitStructure.SPI_CRCPolynomial = 7;
    //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���	
	SPI_Init(SPI3, &SPI_InitStructure);  	
	/* Enable SPI1  */
	SPI_Cmd(SPI3, ENABLE); //ʹ��SPI����	
	SPIx_ReadWriteByte(0xff);//��������		 
}  
	 
//SPI �ٶ����ú���
//SpeedSet:
//SPI_SPEED_2   2��Ƶ   (SPI 36M@sys 72M)
//SPI_SPEED_8   8��Ƶ   (SPI 9M@sys 72M)
//SPI_SPEED_16  16��Ƶ  (SPI 4.5M@sys 72M)
//SPI_SPEED_256 256��Ƶ (SPI 281.25K@sys 72M)
/************************************************************************************
*������: SPIx_SetSpeed
*����˵��: SPI �ٶ����ú���
*�������:SpeedSet�ٶ�ѡ��
*
*����ֵ:��
*
*
**************************************************************************************/

void SPIx_SetSpeed(u8 SpeedSet)
{
	SPI3->CR1&=0XFFC7;//Fsck=Fcpu/256
	if(SpeedSet==SPI_SPEED_2)//����Ƶ
	{
    	SPI3->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz	
	}else if(SpeedSet==SPI_SPEED_8)//�˷�Ƶ 
	{
    	SPI3->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz	
	}else if(SpeedSet==SPI_SPEED_16)//ʮ����Ƶ
	{
    	SPI3->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz
	}else			 	 //256��Ƶ
	{
    	SPI3->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz ����ģʽ
	}	  
	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE); //ʹ��SPI����
	} 

/************************************************************************************
*������:SPIx_ReadWriteByte
*����˵��:SPIx ��дһ���ֽ�
*�������:TxData:Ҫд����ֽ�
*
*����ֵ:��ȡ�����ֽ�
*
*
**************************************************************************************/

u8 SPIx_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) //���ָ����SPI��־λ�������:���ͻ���ձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}			  
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI3, TxData); //ͨ������SPIx����һ������
	retry=0; 
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET); //���ָ����SPI��־λ�������:���ܻ���ǿձ�־λ
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI3); //����ͨ��SPIx������յ�����				    
}
/************************************************************************************
*������:SD_GetResponse
*����˵��: �ȴ�SD����Ӧ
*�������:Ҫ�õ��Ļ�Ӧֵ
*
*����ֵ:0,�ɹ��õ��˸û�Ӧֵ
*         ����,�õ���Ӧֵʧ��
*
**************************************************************************************/
u8 SD_GetResponse(u8 Response)
{
	u16 Count=0xFFF;//�ȴ�����	   						  
	while ((SPIx_ReadWriteByte(0XFF)!=Response)&&Count)Count--;//�ȴ��õ�׼ȷ�Ļ�Ӧ  	  
	if (Count==0)return MSD_RESPONSE_FAILURE;//�õ���Ӧʧ��   
	else return MSD_RESPONSE_NO_ERROR;//��ȷ��Ӧ
}
	
/************************************************************************************
*������:SD_WaitDataReady
*����˵��: �ȴ�SD��д�����
*�������:��
*
*����ֵ:0  �ɹ�
*
*
**************************************************************************************/

u8 SD_WaitDataReady(void)
{
	u8 r1=MSD_DATA_OTHER_ERROR;
	u32 retry;
	retry=0;
	do
	{
		r1=SPIx_ReadWriteByte(0xFF)&0X1F;//������Ӧ
		if(retry==0xfffe)return 1; 
		retry++;
		switch (r1)
		{					   
			case MSD_DATA_OK://���ݽ�����ȷ��	 
				r1=MSD_DATA_OK;
				break;  
			case MSD_DATA_CRC_ERROR:  //CRCУ�����
				return MSD_DATA_CRC_ERROR;  
			case MSD_DATA_WRITE_ERROR://����д�����
				return MSD_DATA_WRITE_ERROR;  
			default://δ֪����    
				r1=MSD_DATA_OTHER_ERROR;
				break;	 
		}   
	}while(r1==MSD_DATA_OTHER_ERROR); //���ݴ���ʱһֱ�ȴ�
	retry=0;
	while(SPIx_ReadWriteByte(0XFF)==0)//��������Ϊ0,�����ݻ�δд���
	{
		retry++;
		if(retry>=0XFFFFFFFE)return 0XFF;//�ȴ�ʧ����
	};	    
	return 0;//�ɹ���
}	 

/************************************************************************************
*������:SD_SendCommand
*����˵��:��SD������һ������
*�������:cmd   ���� 
*         arg  �������
*         crc   crcУ��ֵ
*����ֵ:SD�����ص���Ӧ	
*
*
**************************************************************************************/														  
u8 SD_SendCommand(u8 cmd, u32 arg, u8 crc)
{
	u8 r1;	
	u8 Retry=0;	         
	Set_SD_CS;
	SPIx_ReadWriteByte(0xff);//����д������ʱ
	SPIx_ReadWriteByte(0xff);     
	SPIx_ReadWriteByte(0xff);  	 
	//Ƭѡ���õͣ�ѡ��SD��
	Clr_SD_CS; 
	//����
	SPIx_ReadWriteByte(cmd | 0x40);//�ֱ�д������
	SPIx_ReadWriteByte(arg >> 24);
	SPIx_ReadWriteByte(arg >> 16);
	SPIx_ReadWriteByte(arg >> 8);
	SPIx_ReadWriteByte(arg);
	SPIx_ReadWriteByte(crc); 
	//�ȴ���Ӧ����ʱ�˳�
	while((r1=SPIx_ReadWriteByte(0xFF))==0xFF)
	{
		Retry++;	    
		if(Retry>200)break; 
	}   
	//�ر�Ƭѡ
	Set_SD_CS;
	//�������϶�������8��ʱ�ӣ���SD�����ʣ�µĹ���
	SPIx_ReadWriteByte(0xFF);
	//����״ֵ̬
	return r1;
}	
	  																				 

/************************************************************************************
*������: SD_SendCommand_NoDeassert
*����˵��: ��SD������һ������(�����ǲ�ʧ��Ƭѡ�����к������ݴ�����
*�������:cmd   ����
*         arg  �������
*         crc   crcУ��ֵ
*����ֵ:SD�����ص���Ӧ
*
*
**************************************************************************************/														  
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg, u8 crc)
{
	u8 Retry=0;	         
	u8 r1;			   
	SPIx_ReadWriteByte(0xff);//����д������ʱ
	SPIx_ReadWriteByte(0xff);  	 	 
	Clr_SD_CS;//Ƭѡ���õͣ�ѡ��SD��	   
	//����
	SPIx_ReadWriteByte(cmd | 0x40); //�ֱ�д������
	SPIx_ReadWriteByte(arg >> 24);
	SPIx_ReadWriteByte(arg >> 16);
	SPIx_ReadWriteByte(arg >> 8);
	SPIx_ReadWriteByte(arg);
	SPIx_ReadWriteByte(crc);   
	//�ȴ���Ӧ����ʱ�˳�
	while((r1=SPIx_ReadWriteByte(0xFF))==0xFF)
	{
		Retry++;	    
		if(Retry>200)break; 
	}  	  
	//������Ӧֵ
	return r1;
}
	
/************************************************************************************
*������: SD_Idle_Sta
*����˵��:��SD�����õ�����ģʽ
*�������:��
*
*����ֵ:0,�ɹ�����
*       1,����ʧ��
*
**************************************************************************************/
u8 SD_Idle_Sta(void)
{
	u16 i;
	u8 retry;	   	  
	for(i=0;i<0xf00;i++);//����ʱ���ȴ�SD���ϵ����	 
	//�Ȳ���>74�����壬��SD���Լ���ʼ�����
	for(i=0;i<10;i++)SPIx_ReadWriteByte(0xFF); 
	//-----------------SD����λ��idle��ʼ-----------------
	//ѭ����������CMD0��ֱ��SD������0x01,����IDLE״̬
	//��ʱ��ֱ���˳�
	retry = 0;
	do
	{	   
		//����CMD0����SD������IDLE״̬
		i = SD_SendCommand(CMD0, 0, 0x95);
		retry++;
	}while((i!=0x01)&&(retry<200));
	//����ѭ���󣬼��ԭ�򣺳�ʼ���ɹ���or ���Գ�ʱ��
	if(retry==200)return 1; //ʧ��
	return 0;//�ɹ�	 						  
}	
														    
/************************************************************************************
*������:SD_Init
*����˵��:��ʼ��SD�� ����ɹ�����,����Զ�����SPI�ٶ�Ϊ18Mhz
*�������:��
*
*����ֵ:0��NO_ERR
*       1��TIME_OUT
*       99��NO_CARD	
**************************************************************************************/															 
u8 SD_Init(void)
{		
						 
	u8 r1;      // ���SD���ķ���ֵ
	u16 retry;  // �������г�ʱ����
	u8 buff[6];
	SPIx_Init();
	SPIx_SetSpeed(SPI_SPEED_256);//���õ�����ģʽ		 
	Set_SD_CS;	
	if(SD_Idle_Sta()) return 1;//��ʱ����1 ���õ�idle ģʽʧ��	  
	//-----------------SD����λ��idle����-----------------	 
	//��ȡ��Ƭ��SD�汾��Ϣ
	Clr_SD_CS;	
	r1 = SD_SendCommand_NoDeassert(8, 0x1aa,0x87);	     
	//�����Ƭ�汾��Ϣ��v1.0�汾�ģ���r1=0x05����������³�ʼ��
	if(r1 == 0x05)
	{
		//���ÿ�����ΪSDV1.0����������⵽ΪMMC�������޸�ΪMMC
		SD_Type = SD_TYPE_V1;	   
		//�����V1.0����CMD8ָ���û�к�������
		//Ƭѡ�øߣ�������������
		Set_SD_CS;
		//�෢8��CLK����SD������������
		SPIx_ReadWriteByte(0xFF);	  
		//-----------------SD����MMC����ʼ����ʼ-----------------	 
		//������ʼ��ָ��CMD55+ACMD41
		// �����Ӧ��˵����SD�����ҳ�ʼ�����
		// û�л�Ӧ��˵����MMC�������������Ӧ��ʼ��
		retry = 0;
		do
		{
			//�ȷ�CMD55��Ӧ����0x01���������
			r1 = SD_SendCommand(CMD55, 0, 0);
			if(r1 == 0XFF)return r1;//ֻҪ����0xff,�ͽ��ŷ���	  
			//�õ���ȷ��Ӧ�󣬷�ACMD41��Ӧ�õ�����ֵ0x00����������200��
			r1 = SD_SendCommand(ACMD41, 0, 0);
			retry++;
		}while((r1!=0x00) && (retry<400));
		// �ж��ǳ�ʱ���ǵõ���ȷ��Ӧ
		// ���л�Ӧ����SD����û�л�Ӧ����MMC��	  
		//----------MMC�������ʼ��������ʼ------------
		if(retry==400)
		{
			retry = 0;
			//����MMC����ʼ�����û�в��ԣ�
			do
			{
				r1 = SD_SendCommand(1,0,0);
				retry++;
			}while((r1!=0x00)&& (retry<400));
			if(retry==400)return 1;   //MMC����ʼ����ʱ		    
			//д�뿨����
			SD_Type = SD_TYPE_MMC;
		}
		//----------MMC�������ʼ����������------------	    
		//����SPIΪ����ģʽ
		SPIx_SetSpeed(SPI_SPEED_4);   
		SPIx_ReadWriteByte(0xFF);	 
		//��ֹCRCУ��	   
		r1 = SD_SendCommand(CMD59, 0, 0x95);
		if(r1 != 0x00)return r1;  //������󣬷���r1   	   
		//����Sector Size
		r1 = SD_SendCommand(CMD16, 512, 0x95);
		if(r1 != 0x00)return r1;//������󣬷���r1		 
		//-----------------SD����MMC����ʼ������-----------------
		
	}//SD��ΪV1.0�汾�ĳ�ʼ������	 
	//������V2.0���ĳ�ʼ��
	//������Ҫ��ȡOCR���ݣ��ж���SD2.0����SD2.0HC��
	else if(r1 == 0x01)
	{
		//V2.0�Ŀ���CMD8�����ᴫ��4�ֽڵ����ݣ�Ҫ�����ٽ���������
		buff[0] = SPIx_ReadWriteByte(0xFF);  //should be 0x00
		buff[1] = SPIx_ReadWriteByte(0xFF);  //should be 0x00
		buff[2] = SPIx_ReadWriteByte(0xFF);  //should be 0x01
		buff[3] = SPIx_ReadWriteByte(0xFF);  //should be 0xAA	    
		Set_SD_CS;	  
		SPIx_ReadWriteByte(0xFF);//the next 8 clocks			 
		//�жϸÿ��Ƿ�֧��2.7V-3.6V�ĵ�ѹ��Χ
		//if(buff[2]==0x01 && buff[3]==0xAA) //���жϣ�����֧�ֵĿ�����
		{	  
			retry = 0;
			//������ʼ��ָ��CMD55+ACMD41
			do
			{
				r1 = SD_SendCommand(CMD55, 0, 0);
				if(r1!=0x01)return r1;	   
				r1 = SD_SendCommand(ACMD41, 0x40000000, 0);
				if(retry>200)return r1;  //��ʱ�򷵻�r1״̬  
			}while(r1!=0);		  
			//��ʼ��ָ�����ɣ���������ȡOCR��Ϣ		   
			//-----------����SD2.0���汾��ʼ-----------
			r1 = SD_SendCommand_NoDeassert(CMD58, 0, 0);
			if(r1!=0x00)
			{
				Set_SD_CS;//�ͷ�SDƬѡ�ź�
				return r1;  //�������û�з�����ȷӦ��ֱ���˳�������Ӧ��	 
			}//��OCRָ����󣬽�������4�ֽڵ�OCR��Ϣ
			buff[0] = SPIx_ReadWriteByte(0xFF);
			buff[1] = SPIx_ReadWriteByte(0xFF); 
			buff[2] = SPIx_ReadWriteByte(0xFF);
			buff[3] = SPIx_ReadWriteByte(0xFF);		 
			//OCR������ɣ�Ƭѡ�ø�
			Set_SD_CS;
			SPIx_ReadWriteByte(0xFF);	   
			//�����յ���OCR�е�bit30λ��CCS����ȷ����ΪSD2.0����SDHC
			//���CCS=1��SDHC   CCS=0��SD2.0
			if(buff[0]&0x40)SD_Type = SD_TYPE_V2HC;    //���CCS	 
			else SD_Type = SD_TYPE_V2;	    
			//-----------����SD2.0���汾����----------- 
			//����SPIΪ����ģʽ
			SPIx_SetSpeed(SPI_SPEED_4);  
		}	    
	}
	return r1;
}	 																			   

/************************************************************************************
*������:SD_ReceiveData
*����˵��:��SD���ж���ָ�����ȵ����ݣ������ڸ���λ��
*�������:*data(��Ŷ������ݵ��ڴ�>len)
*         len(���ݳ��ȣ�
*         release(������ɺ��Ƿ��ͷ�����CS�ø� 0�����ͷ� 1���ͷţ�
*����ֵ:0��NO_ERR
*       other��������Ϣ
*
**************************************************************************************/														  
u8 SD_ReceiveData(u8 *data, u16 len, u8 release)
{
	// ����һ�δ���
	Clr_SD_CS;				  	  
	if(SD_GetResponse(0xFE))//�ȴ�SD������������ʼ����0xFE
	{	  
		Set_SD_CS;
		return 1;
	}
	while(len--)//��ʼ��������
	{
		*data=SPIx_ReadWriteByte(0xFF);
		data++;
	}
	//������2��αCRC��dummy CRC��
	SPIx_ReadWriteByte(0xFF);
	SPIx_ReadWriteByte(0xFF);
	if(release==RELEASE)//�����ͷ����ߣ���CS�ø�
	{
		Set_SD_CS;//�������
		SPIx_ReadWriteByte(0xFF);
	}											  					    
	return 0;
}		
																			  
/************************************************************************************
*������: SD_GetCID
*����˵��: ��ȡSD����CID��Ϣ��������������Ϣ
*�������:*cid_data(���CID���ڴ棬����16Byte��
*
*����ֵ:0��NO_ERR
*       1��TIME_OUT
*       other��������Ϣ
**************************************************************************************/														   
u8 SD_GetCID(u8 *cid_data)
{
	u8 r1;	   
	//��CMD10�����CID
	r1 = SD_SendCommand(CMD10,0,0xFF);
	if(r1 != 0x00)return r1;  //û������ȷӦ�����˳�������  
	SD_ReceiveData(cid_data,16,RELEASE);//����16���ֽڵ�����	 
	return 0;
}		
																			  
/************************************************************************************
*������:SD_GetCSD
*����˵��:��ȡSD����CSD��Ϣ�������������ٶ���Ϣ
*�������:*cid_data(���CID���ڴ棬����16Byte��
*
*����ֵ:0��NO_ERR
*       1��TIME_OUT
*       other��������Ϣ
**************************************************************************************/														   
u8 SD_GetCSD(u8 *csd_data)
{
	u8 r1;	 
	r1=SD_SendCommand(CMD9,0,0xFF);//��CMD9�����CSD
	if(r1)return r1;  //û������ȷӦ�����˳�������  
	SD_ReceiveData(csd_data, 16, RELEASE);//����16���ֽڵ����� 
	return 0;
}  

/************************************************************************************
*������:SD_GetCapacity
*����˵��:��ȡSD�����������ֽڣ�
*�������:��
*
*����ֵ:0�� ȡ��������
*       ����:SD��������(�ֽ�)
*
**************************************************************************************/														  
u32 SD_GetCapacity(void)
{
	u8 csd[16];
	u32 Capacity;
	u8 r1;
	u16 i;
	u16 temp;  					    
	//ȡCSD��Ϣ������ڼ��������0
	if(SD_GetCSD(csd)!=0) return 0;	    
	//���ΪSDHC�����������淽ʽ����
	if((csd[0]&0xC0)==0x40)
	{									  
    	Capacity=((u32)csd[8])<<8;
    	Capacity+=(u32)csd[9]+1;	 
    	Capacity = (Capacity)*1024;//�õ�������
    	Capacity*=512;//�õ������			   
	}else
	{		    
		i = csd[6]&0x03;
		i<<=8;
		i += csd[7];
		i<<=2;
		i += ((csd[8]&0xc0)>>6);
		//C_SIZE_MULT
		r1 = csd[9]&0x03;
		r1<<=1;
		r1 += ((csd[10]&0x80)>>7);	 
		r1+=2;//BLOCKNR
		temp = 1;
		while(r1)
			{
    			temp*=2;
    			r1--;
			}
		Capacity = ((u32)(i+1))*((u32)temp);	 
		// READ_BL_LEN
		i = csd[5]&0x0f;
		//BLOCK_LEN
		temp = 1;
		while(i)
			{
    			temp*=2;
    			i--;
			}
		//The final result
		Capacity *= (u32)temp;//�ֽ�Ϊ��λ 	  
    }
    return (u32)Capacity;
}	  
	  																			    
/************************************************************************************
*������:SD_ReadSingleBlock
*����˵��:��SD����һ��block
*�������:u32 sector ȡ��ַ��sectorֵ���������ַ��
*         u8 *buffer ���ݴ洢��ַ����С����512byte��
*����ֵ:0�� �ɹ�
*       other��ʧ��
*
**************************************************************************************/															  
u8 SD_ReadSingleBlock(u32 sector, u8 *buffer)
{
	u8 r1;	    
	//����Ϊ����ģʽ
	SPIx_SetSpeed(SPI_SPEED_4);  		   
	//�������SDHC����������sector��ַ������ת����byte��ַ
	if(SD_Type!=SD_TYPE_V2HC)
	{
		sector = sector<<9;
	} 
	r1 = SD_SendCommand(CMD17, sector, 0);//������	 		    
	if(r1 != 0x00)return r1; 		   							  
	r1 = SD_ReceiveData(buffer, 512, RELEASE);		 
	if(r1 != 0)return r1;   //�����ݳ���
	else return 0; 
}
	
////////////////////////////����2������ΪUSB��д����Ҫ��/////////////////////////
//����SD���Ŀ��С	 				   
#define BLOCK_SIZE 512 

/************************************************************************************
*������:MSD_WriteBuffer
*����˵��:д��MSD/SD����
*�������:pBuffer:���ݴ����
*         ReadAddr:д����׵�ַ
*         NumByteToRead:Ҫд����ֽ���
*����ֵ:0,д�����
*       ����,д��ʧ��
*
**************************************************************************************/

u8 MSD_WriteBuffer(u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite)
{
	u32 i,NbrOfBlock = 0, Offset = 0;
	u32 sector;
	u8 r1;
	NbrOfBlock = NumByteToWrite / BLOCK_SIZE;//�õ�Ҫд��Ŀ����Ŀ	    
	Clr_SD_CS;	  		   
	while (NbrOfBlock--)//д��һ������
	{
		sector=WriteAddr+Offset;
		if(SD_Type==SD_TYPE_V2HC)sector>>=9;//ִ������ͨ�����෴�Ĳ���					  			 
		r1=SD_SendCommand_NoDeassert(CMD24,sector,0xff);//д����   
		if(r1)
		{
			Set_SD_CS;
			return 1;//Ӧ����ȷ��ֱ�ӷ��� 	   
		}
		SPIx_ReadWriteByte(0xFE);//����ʼ����0xFE   
		//��һ��sector������
		for(i=0;i<512;i++)SPIx_ReadWriteByte(*pBuffer++);  
		//��2��Byte��dummy CRC
		SPIx_ReadWriteByte(0xff);
		SPIx_ReadWriteByte(0xff); 
		if(SD_WaitDataReady())//�ȴ�SD������д�����
		{
			Set_SD_CS;
			return 2;    
		}
		Offset += 512;	   
	}	    
	//д����ɣ�Ƭѡ��1
	Set_SD_CS;
	SPIx_ReadWriteByte(0xff);	 
	return 0;
}
	
/************************************************************************************
*������:MSD_ReadBuffer
*����˵��:��ȡMSD/SD����
*�������:pBuffer:���ݴ����
*         ReadAddr:��ȡ���׵�ַ
*         NumByteToRead:Ҫ�������ֽ���
*����ֵ:0,�������
*       ����,����ʧ��
*
**************************************************************************************/
u8 MSD_ReadBuffer(u8* pBuffer, u32 ReadAddr, u32 NumByteToRead)
{
	u32 NbrOfBlock=0,Offset=0;
	u32 sector=0;
	u8 r1=0;   	 
	NbrOfBlock=NumByteToRead/BLOCK_SIZE;	  
	Clr_SD_CS;
	while (NbrOfBlock --)
	{	
		sector=ReadAddr+Offset;
		if(SD_Type==SD_TYPE_V2HC)sector>>=9;//ִ������ͨ�����෴�Ĳ���					  			 
		r1=SD_SendCommand_NoDeassert(CMD17,sector,0xff);//������	 		    
		if(r1)//����ʹ���
		{
			Set_SD_CS;
			return r1;
		}	   							  
		r1=SD_ReceiveData(pBuffer,512,RELEASE);		 
		if(r1)//��������
		{
			Set_SD_CS;
			return r1;
		}
		pBuffer+=512;	 					    
		Offset+=512;				 	 
	}	 	 
	Set_SD_CS;
	SPIx_ReadWriteByte(0xff);	 
	return 0;
}
	
/************************************************************************************
*������: SD_WriteSingleBlock
*����˵��:д��SD����һ��block
*�������:sector ������ַ��sectorֵ���������ַ��
*         *buffer ���ݴ洢��ַ����С����512byte��
*����ֵ:0�� �ɹ�
*       other��ʧ��
*
**************************************************************************************/															  
u8 SD_WriteSingleBlock(u32 sector, const u8 *data)
{
	u8 r1;
	u16 i;
	u16 retry;
	
	//����Ϊ����ģʽ
	//SPIx_SetSpeed(SPI_SPEED_HIGH);	   
	//�������SDHC����������sector��ַ������ת����byte��ַ
	if(SD_Type!=SD_TYPE_V2HC)
	{
		sector = sector<<9;
	}   
	r1 = SD_SendCommand(CMD24, sector, 0x00);
	if(r1 != 0x00)
	{
		return r1;  //Ӧ����ȷ��ֱ�ӷ���
	}
	
	//��ʼ׼�����ݴ���
	Clr_SD_CS;
	//�ȷ�3�������ݣ��ȴ�SD��׼����
	SPIx_ReadWriteByte(0xff);
	SPIx_ReadWriteByte(0xff);
	SPIx_ReadWriteByte(0xff);
	//����ʼ����0xFE
	SPIx_ReadWriteByte(0xFE);
	
	//��һ��sector������
	for(i=0;i<512;i++)
		{
		SPIx_ReadWriteByte(*data++);
		}
	//��2��Byte��dummy CRC
	SPIx_ReadWriteByte(0xff);
	SPIx_ReadWriteByte(0xff);
	
	//�ȴ�SD��Ӧ��
	r1 = SPIx_ReadWriteByte(0xff);
	if((r1&0x1F)!=0x05)
	{
		Set_SD_CS;
		return r1;
	}
	
	//�ȴ��������
	retry = 0;
	while(!SPIx_ReadWriteByte(0xff))
	{
		retry++;
		if(retry>0xfffe)        //�����ʱ��д��û����ɣ������˳�
		{
			Set_SD_CS;
			return 1;           //д�볬ʱ����1
		}
	}	    
	//д����ɣ�Ƭѡ��1
	Set_SD_CS;
	SPIx_ReadWriteByte(0xff);
	
	return 0;
}	
				           
/************************************************************************************
*������:SD_ReadMultiBlock
*����˵��:��SD���Ķ��block(ʵ�ʲ��Թ�)
*�������:u32 sector ������ַ��sectorֵ���������ַ��
*         *buffer ���ݴ洢��ַ����С����512byte��
*         count ������count��block 
*����ֵ:0�� �ɹ�
*      other��ʧ��
*
**************************************************************************************/															  
u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count)
{
	u8 r1;	 			 
	//SPIx_SetSpeed(SPI_SPEED_HIGH);//����Ϊ����ģʽ  
	//�������SDHC����sector��ַת��byte��ַ
	if(SD_Type!=SD_TYPE_V2HC)sector = sector<<9;  
	//SD_WaitDataReady();
	//�����������
	r1 = SD_SendCommand(CMD18, sector, 0);//������
	if(r1 != 0x00)return r1;	 
	do//��ʼ��������
	{
		if(SD_ReceiveData(buffer, 512, NO_RELEASE) != 0x00)break; 
		buffer += 512;
	} while(--count);		 
	//ȫ��������ϣ�����ֹͣ����
	SD_SendCommand(CMD12, 0, 0);
	//�ͷ�����
	Set_SD_CS;
	SPIx_ReadWriteByte(0xFF);    
	if(count != 0)return count;   //���û�д��꣬����ʣ�����	 
	else return 0;	 
}		
										  
/************************************************************************************
*������:SD_WriteMultiBlock
*����˵��:д��SD����N��block
*�������:sector ������ַ��sectorֵ���������ַ��
*         *buffer ���ݴ洢��ַ����С����512byte��
*         count д���block��Ŀ	
*����ֵ:0�� �ɹ�
*       other��ʧ��
*
**************************************************************************************/															   
u8 SD_WriteMultiBlock(u32 sector, const u8 *data, u8 count)
{
	u8 r1;
	u16 i;	 		 	 
	if(SD_Type != SD_TYPE_V2HC)sector = sector<<9;//�������SDHC����������sector��ַ������ת����byte��ַ  
	if(SD_Type != SD_TYPE_MMC) r1 = SD_SendCommand(ACMD23, count, 0x00);//���Ŀ�꿨����MMC��������ACMD23ָ��ʹ��Ԥ����   
	r1 = SD_SendCommand(CMD25, sector, 0x00);//�����д��ָ��
	if(r1 != 0x00)return r1;  //Ӧ����ȷ��ֱ�ӷ���	 
	Clr_SD_CS;//��ʼ׼�����ݴ���   
	SPIx_ReadWriteByte(0xff);//�ȷ�3�������ݣ��ȴ�SD��׼����
	SPIx_ReadWriteByte(0xff);   
	//--------������N��sectorд���ѭ������
	do
	{
		//����ʼ����0xFC �����Ƕ��д��
		SPIx_ReadWriteByte(0xFC);	  
		//��һ��sector������
		for(i=0;i<512;i++)
		{
			SPIx_ReadWriteByte(*data++);
		}
		//��2��Byte��dummy CRC
		SPIx_ReadWriteByte(0xff);
		SPIx_ReadWriteByte(0xff);
		
		//�ȴ�SD��Ӧ��
		r1 = SPIx_ReadWriteByte(0xff);
		if((r1&0x1F)!=0x05)
		{
			Set_SD_CS;    //���Ӧ��Ϊ��������������ֱ���˳�
			return r1;
		}		   
		//�ȴ�SD��д�����
		if(SD_WaitDataReady()==1)
		{
			Set_SD_CS;    //�ȴ�SD��д����ɳ�ʱ��ֱ���˳�����
			return 1;
		}	   
	}while(--count);//��sector���ݴ������  
	//��������������0xFD
	r1 = SPIx_ReadWriteByte(0xFD);
	if(r1==0x00)
	{
		count =  0xfe;
	}		   
	if(SD_WaitDataReady()) //�ȴ�׼����
	{
		Set_SD_CS;
		return 1;  
	}
	//д����ɣ�Ƭѡ��1
	Set_SD_CS;
	SPIx_ReadWriteByte(0xff);  
	return count;   //����countֵ�����д����count=0������count=1
}
						  					  
/************************************************************************************
*������:SD_Read_Bytes
*����˵��:��ָ������,��offset��ʼ����bytes���ֽ�
*�������:sector ������ַ��sectorֵ���������ַ��
*         *buf     ���ݴ洢��ַ����С<=512byte��
*         bytes   Ҫ�������ֽ���
*����ֵ:0�� �ɹ�
*       other��ʧ��
*
**************************************************************************************/															   
u8 SD_Read_Bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes)
{
	u8 r1;u16 i=0;  
	r1=SD_SendCommand(CMD17,address<<9,0);//���Ͷ���������      
	if(r1)return r1;  //Ӧ����ȷ��ֱ�ӷ���
	Clr_SD_CS;//ѡ��SD��
	if(SD_GetResponse(0xFE))//�ȴ�SD������������ʼ����0xFE
	{
		Set_SD_CS; //�ر�SD��
		return 1;//��ȡʧ��
	}	 
	for(i=0;i<offset;i++)SPIx_ReadWriteByte(0xff);//����offsetλ 
	for(;i<offset+bytes;i++)*buf++=SPIx_ReadWriteByte(0xff);//��ȡ��������	
	for(;i<512;i++) SPIx_ReadWriteByte(0xff); 	 //����ʣ���ֽ�
	SPIx_ReadWriteByte(0xff);//����αCRC��
	SPIx_ReadWriteByte(0xff);  
	Set_SD_CS;//�ر�SD��
	return 0;
}
/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/













