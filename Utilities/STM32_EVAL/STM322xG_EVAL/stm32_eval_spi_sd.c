 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   ÎÄ¼şÃû³Æ £º main.c
*   Õª Òª    £º 
*
*   µ±Ç°°æ±¾ £º 
*   ×÷ Õß    £º EFLAG
*   Íê³ÉÈÕÆÚ £º 
*
*   È¡´ú°æ±¾ £º
*   Ô­×÷Õß   £º 
*   ĞŞ¸ÄÄÚÈİ £º
*   Íê³ÉÈÕÆÚ £º2012.08.01 
*
**********************************************************/
#include "stm32f2xx.h"
#include "stm32_eval_spi_sd.h"


						   
u8  SD_Type=0;//SD¿¨µÄÀàĞÍ

/************************************************************************************
*º¯ÊıÃû:SPIx_Init
*º¯ÊıËµÃ÷:SPIÄ£¿éµÄ³õÊ¼»¯´úÂë
*ÊäÈë²ÎÊı:ÎŞ
*
*·µ»ØÖµ:ÎŞ
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure I/O for Flash Chip select */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  //SPI CS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //¸´ÓÃÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* Deselect the FLASH: Chip Select high */
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
	 
	/* SPI1 configuration */
    //ÉèÖÃSPIµ¥Ïò»òÕßË«ÏòµÄÊı¾İÄ£Ê½:SPIÉèÖÃÎªË«ÏßË«ÏòÈ«Ë«¹¤
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
    //ÉèÖÃSPI¹¤×÷Ä£Ê½:ÉèÖÃÎªÖ÷SPI 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    //ÉèÖÃSPIµÄÊı¾İ´óĞ¡:SPI·¢ËÍ½ÓÊÕ8Î»Ö¡½á¹¹		
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    //Ñ¡ÔñÁË´®ĞĞÊ±ÖÓµÄÎÈÌ¬:Ê±ÖÓĞü¿Õ¸ß		
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    //Êı¾İ²¶»ñÓÚµÚ¶ş¸öÊ±ÖÓÑØ	
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    //NSSĞÅºÅÓÉÓ²¼ş£¨NSS¹Ü½Å£©»¹ÊÇÈí¼ş£¨Ê¹ÓÃSSIÎ»£©¹ÜÀí:ÄÚ²¿NSSĞÅºÅÓĞSSIÎ»¿ØÖÆ	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//SPI_NSS_Soft;
    //¶¨Òå²¨ÌØÂÊÔ¤·ÖÆµµÄÖµ:²¨ÌØÂÊÔ¤·ÖÆµÖµÎª256		
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    //Ö¸¶¨Êı¾İ´«Êä´ÓMSBÎ»»¹ÊÇLSBÎ»¿ªÊ¼:Êı¾İ´«Êä´ÓMSBÎ»¿ªÊ¼	
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    //CRCÖµ¼ÆËãµÄ¶àÏîÊ½	
	SPI_InitStructure.SPI_CRCPolynomial = 7;
    //¸ù¾İSPI_InitStructÖĞÖ¸¶¨µÄ²ÎÊı³õÊ¼»¯ÍâÉèSPIx¼Ä´æÆ÷	
	SPI_Init(SPI3, &SPI_InitStructure);  	
	/* Enable SPI1  */
	SPI_Cmd(SPI3, ENABLE); //Ê¹ÄÜSPIÍâÉè	
	SPIx_ReadWriteByte(0xff);//Æô¶¯´«Êä		 
}  
	 
//SPI ËÙ¶ÈÉèÖÃº¯Êı
//SpeedSet:
//SPI_SPEED_2   2·ÖÆµ   (SPI 36M@sys 72M)
//SPI_SPEED_8   8·ÖÆµ   (SPI 9M@sys 72M)
//SPI_SPEED_16  16·ÖÆµ  (SPI 4.5M@sys 72M)
//SPI_SPEED_256 256·ÖÆµ (SPI 281.25K@sys 72M)
/************************************************************************************
*º¯ÊıÃû: SPIx_SetSpeed
*º¯ÊıËµÃ÷: SPI ËÙ¶ÈÉèÖÃº¯Êı
*ÊäÈë²ÎÊı:SpeedSetËÙ¶ÈÑ¡Ôñ
*
*·µ»ØÖµ:ÎŞ
*
*
**************************************************************************************/

void SPIx_SetSpeed(u8 SpeedSet)
{
	SPI3->CR1&=0XFFC7;//Fsck=Fcpu/256
	if(SpeedSet==SPI_SPEED_2)//¶ş·ÖÆµ
	{
    	SPI3->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz	
	}else if(SpeedSet==SPI_SPEED_8)//°Ë·ÖÆµ 
	{
    	SPI3->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz	
	}else if(SpeedSet==SPI_SPEED_16)//Ê®Áù·ÖÆµ
	{
    	SPI3->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz
	}else			 	 //256·ÖÆµ
	{
    	SPI3->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz µÍËÙÄ£Ê½
	}	  
	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE); //Ê¹ÄÜSPIÍâÉè
	} 

/************************************************************************************
*º¯ÊıÃû:SPIx_ReadWriteByte
*º¯ÊıËµÃ÷:SPIx ¶ÁĞ´Ò»¸ö×Ö½Ú
*ÊäÈë²ÎÊı:TxData:ÒªĞ´ÈëµÄ×Ö½Ú
*
*·µ»ØÖµ:¶ÁÈ¡µ½µÄ×Ö½Ú
*
*
**************************************************************************************/

u8 SPIx_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:·¢ËÍ»º´æ¿Õ±êÖ¾Î»
	{
		retry++;
		if(retry>200)return 0;
	}			  
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI3, TxData); //Í¨¹ıÍâÉèSPIx·¢ËÍÒ»¸öÊı¾İ
	retry=0; 
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET); //¼ì²éÖ¸¶¨µÄSPI±êÖ¾Î»ÉèÖÃÓë·ñ:½ÓÊÜ»º´æ·Ç¿Õ±êÖ¾Î»
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI3); //·µ»ØÍ¨¹ıSPIx×î½ü½ÓÊÕµÄÊı¾İ				    
}
/************************************************************************************
*º¯ÊıÃû:SD_GetResponse
*º¯ÊıËµÃ÷: µÈ´ıSD¿¨»ØÓ¦
*ÊäÈë²ÎÊı:ÒªµÃµ½µÄ»ØÓ¦Öµ
*
*·µ»ØÖµ:0,³É¹¦µÃµ½ÁË¸Ã»ØÓ¦Öµ
*         ÆäËû,µÃµ½»ØÓ¦ÖµÊ§°Ü
*
**************************************************************************************/
u8 SD_GetResponse(u8 Response)
{
	u16 Count=0xFFF;//µÈ´ı´ÎÊı	   						  
	while ((SPIx_ReadWriteByte(0XFF)!=Response)&&Count)Count--;//µÈ´ıµÃµ½×¼È·µÄ»ØÓ¦  	  
	if (Count==0)return MSD_RESPONSE_FAILURE;//µÃµ½»ØÓ¦Ê§°Ü   
	else return MSD_RESPONSE_NO_ERROR;//ÕıÈ·»ØÓ¦
}
	
/************************************************************************************
*º¯ÊıÃû:SD_WaitDataReady
*º¯ÊıËµÃ÷: µÈ´ıSD¿¨Ğ´ÈëÍê³É
*ÊäÈë²ÎÊı:ÎŞ
*
*·µ»ØÖµ:0  ³É¹¦
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
		r1=SPIx_ReadWriteByte(0xFF)&0X1F;//¶Áµ½»ØÓ¦
		if(retry==0xfffe)return 1; 
		retry++;
		switch (r1)
		{					   
			case MSD_DATA_OK://Êı¾İ½ÓÊÕÕıÈ·ÁË	 
				r1=MSD_DATA_OK;
				break;  
			case MSD_DATA_CRC_ERROR:  //CRCĞ£Ñé´íÎó
				return MSD_DATA_CRC_ERROR;  
			case MSD_DATA_WRITE_ERROR://Êı¾İĞ´Èë´íÎó
				return MSD_DATA_WRITE_ERROR;  
			default://Î´Öª´íÎó    
				r1=MSD_DATA_OTHER_ERROR;
				break;	 
		}   
	}while(r1==MSD_DATA_OTHER_ERROR); //Êı¾İ´íÎóÊ±Ò»Ö±µÈ´ı
	retry=0;
	while(SPIx_ReadWriteByte(0XFF)==0)//¶Áµ½Êı¾İÎª0,ÔòÊı¾İ»¹Î´Ğ´Íê³É
	{
		retry++;
		if(retry>=0XFFFFFFFE)return 0XFF;//µÈ´ıÊ§°ÜÁË
	};	    
	return 0;//³É¹¦ÁË
}	 

/************************************************************************************
*º¯ÊıÃû:SD_SendCommand
*º¯ÊıËµÃ÷:ÏòSD¿¨·¢ËÍÒ»¸öÃüÁî
*ÊäÈë²ÎÊı:cmd   ÃüÁî 
*         arg  ÃüÁî²ÎÊı
*         crc   crcĞ£ÑéÖµ
*·µ»ØÖµ:SD¿¨·µ»ØµÄÏìÓ¦	
*
*
**************************************************************************************/														  
u8 SD_SendCommand(u8 cmd, u32 arg, u8 crc)
{
	u8 r1;	
	u8 Retry=0;	         
	Set_SD_CS;
	SPIx_ReadWriteByte(0xff);//¸ßËÙĞ´ÃüÁîÑÓÊ±
	SPIx_ReadWriteByte(0xff);     
	SPIx_ReadWriteByte(0xff);  	 
	//Æ¬Ñ¡¶ËÖÃµÍ£¬Ñ¡ÖĞSD¿¨
	Clr_SD_CS; 
	//·¢ËÍ
	SPIx_ReadWriteByte(cmd | 0x40);//·Ö±ğĞ´ÈëÃüÁî
	SPIx_ReadWriteByte(arg >> 24);
	SPIx_ReadWriteByte(arg >> 16);
	SPIx_ReadWriteByte(arg >> 8);
	SPIx_ReadWriteByte(arg);
	SPIx_ReadWriteByte(crc); 
	//µÈ´ıÏìÓ¦£¬»ò³¬Ê±ÍË³ö
	while((r1=SPIx_ReadWriteByte(0xFF))==0xFF)
	{
		Retry++;	    
		if(Retry>200)break; 
	}   
	//¹Ø±ÕÆ¬Ñ¡
	Set_SD_CS;
	//ÔÚ×ÜÏßÉÏ¶îÍâÔö¼Ó8¸öÊ±ÖÓ£¬ÈÃSD¿¨Íê³ÉÊ£ÏÂµÄ¹¤×÷
	SPIx_ReadWriteByte(0xFF);
	//·µ»Ø×´Ì¬Öµ
	return r1;
}	
	  																				 

/************************************************************************************
*º¯ÊıÃû: SD_SendCommand_NoDeassert
*º¯ÊıËµÃ÷: ÏòSD¿¨·¢ËÍÒ»¸öÃüÁî(½áÊøÊÇ²»Ê§ÄÜÆ¬Ñ¡£¬»¹ÓĞºóĞøÊı¾İ´«À´£©
*ÊäÈë²ÎÊı:cmd   ÃüÁî
*         arg  ÃüÁî²ÎÊı
*         crc   crcĞ£ÑéÖµ
*·µ»ØÖµ:SD¿¨·µ»ØµÄÏìÓ¦
*
*
**************************************************************************************/														  
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg, u8 crc)
{
	u8 Retry=0;	         
	u8 r1;			   
	SPIx_ReadWriteByte(0xff);//¸ßËÙĞ´ÃüÁîÑÓÊ±
	SPIx_ReadWriteByte(0xff);  	 	 
	Clr_SD_CS;//Æ¬Ñ¡¶ËÖÃµÍ£¬Ñ¡ÖĞSD¿¨	   
	//·¢ËÍ
	SPIx_ReadWriteByte(cmd | 0x40); //·Ö±ğĞ´ÈëÃüÁî
	SPIx_ReadWriteByte(arg >> 24);
	SPIx_ReadWriteByte(arg >> 16);
	SPIx_ReadWriteByte(arg >> 8);
	SPIx_ReadWriteByte(arg);
	SPIx_ReadWriteByte(crc);   
	//µÈ´ıÏìÓ¦£¬»ò³¬Ê±ÍË³ö
	while((r1=SPIx_ReadWriteByte(0xFF))==0xFF)
	{
		Retry++;	    
		if(Retry>200)break; 
	}  	  
	//·µ»ØÏìÓ¦Öµ
	return r1;
}
	
/************************************************************************************
*º¯ÊıÃû: SD_Idle_Sta
*º¯ÊıËµÃ÷:°ÑSD¿¨ÉèÖÃµ½¹ÒÆğÄ£Ê½
*ÊäÈë²ÎÊı:ÎŞ
*
*·µ»ØÖµ:0,³É¹¦ÉèÖÃ
*       1,ÉèÖÃÊ§°Ü
*
**************************************************************************************/
u8 SD_Idle_Sta(void)
{
	u16 i;
	u8 retry;	   	  
	for(i=0;i<0xf00;i++);//´¿ÑÓÊ±£¬µÈ´ıSD¿¨ÉÏµçÍê³É	 
	//ÏÈ²úÉú>74¸öÂö³å£¬ÈÃSD¿¨×Ô¼º³õÊ¼»¯Íê³É
	for(i=0;i<10;i++)SPIx_ReadWriteByte(0xFF); 
	//-----------------SD¿¨¸´Î»µ½idle¿ªÊ¼-----------------
	//Ñ­»·Á¬Ğø·¢ËÍCMD0£¬Ö±µ½SD¿¨·µ»Ø0x01,½øÈëIDLE×´Ì¬
	//³¬Ê±ÔòÖ±½ÓÍË³ö
	retry = 0;
	do
	{	   
		//·¢ËÍCMD0£¬ÈÃSD¿¨½øÈëIDLE×´Ì¬
		i = SD_SendCommand(CMD0, 0, 0x95);
		retry++;
	}while((i!=0x01)&&(retry<200));
	//Ìø³öÑ­»·ºó£¬¼ì²éÔ­Òò£º³õÊ¼»¯³É¹¦£¿or ÖØÊÔ³¬Ê±£¿
	if(retry==200)return 1; //Ê§°Ü
	return 0;//³É¹¦	 						  
}	
														    
/************************************************************************************
*º¯ÊıÃû:SD_Init
*º¯ÊıËµÃ÷:³õÊ¼»¯SD¿¨ Èç¹û³É¹¦·µ»Ø,Ôò»á×Ô¶¯ÉèÖÃSPIËÙ¶ÈÎª18Mhz
*ÊäÈë²ÎÊı:ÎŞ
*
*·µ»ØÖµ:0£ºNO_ERR
*       1£ºTIME_OUT
*       99£ºNO_CARD	
**************************************************************************************/															 
u8 SD_Init(void)
{		
						 
	u8 r1;      // ´æ·ÅSD¿¨µÄ·µ»ØÖµ
	u16 retry;  // ÓÃÀ´½øĞĞ³¬Ê±¼ÆÊı
	u8 buff[6];
	SPIx_Init();
	SPIx_SetSpeed(SPI_SPEED_256);//ÉèÖÃµ½µÍËÙÄ£Ê½		 
	Set_SD_CS;	
	if(SD_Idle_Sta()) return 1;//³¬Ê±·µ»Ø1 ÉèÖÃµ½idle Ä£Ê½Ê§°Ü	  
	//-----------------SD¿¨¸´Î»µ½idle½áÊø-----------------	 
	//»ñÈ¡¿¨Æ¬µÄSD°æ±¾ĞÅÏ¢
	Clr_SD_CS;	
	r1 = SD_SendCommand_NoDeassert(8, 0x1aa,0x87);	     
	//Èç¹û¿¨Æ¬°æ±¾ĞÅÏ¢ÊÇv1.0°æ±¾µÄ£¬¼´r1=0x05£¬Ôò½øĞĞÒÔÏÂ³õÊ¼»¯
	if(r1 == 0x05)
	{
		//ÉèÖÃ¿¨ÀàĞÍÎªSDV1.0£¬Èç¹ûºóÃæ¼ì²âµ½ÎªMMC¿¨£¬ÔÙĞŞ¸ÄÎªMMC
		SD_Type = SD_TYPE_V1;	   
		//Èç¹ûÊÇV1.0¿¨£¬CMD8Ö¸ÁîºóÃ»ÓĞºóĞøÊı¾İ
		//Æ¬Ñ¡ÖÃ¸ß£¬½áÊø±¾´ÎÃüÁî
		Set_SD_CS;
		//¶à·¢8¸öCLK£¬ÈÃSD½áÊøºóĞø²Ù×÷
		SPIx_ReadWriteByte(0xFF);	  
		//-----------------SD¿¨¡¢MMC¿¨³õÊ¼»¯¿ªÊ¼-----------------	 
		//·¢¿¨³õÊ¼»¯Ö¸ÁîCMD55+ACMD41
		// Èç¹ûÓĞÓ¦´ğ£¬ËµÃ÷ÊÇSD¿¨£¬ÇÒ³õÊ¼»¯Íê³É
		// Ã»ÓĞ»ØÓ¦£¬ËµÃ÷ÊÇMMC¿¨£¬¶îÍâ½øĞĞÏàÓ¦³õÊ¼»¯
		retry = 0;
		do
		{
			//ÏÈ·¢CMD55£¬Ó¦·µ»Ø0x01£»·ñÔò³ö´í
			r1 = SD_SendCommand(CMD55, 0, 0);
			if(r1 == 0XFF)return r1;//Ö»Òª²»ÊÇ0xff,¾Í½Ó×Å·¢ËÍ	  
			//µÃµ½ÕıÈ·ÏìÓ¦ºó£¬·¢ACMD41£¬Ó¦µÃµ½·µ»ØÖµ0x00£¬·ñÔòÖØÊÔ200´Î
			r1 = SD_SendCommand(ACMD41, 0, 0);
			retry++;
		}while((r1!=0x00) && (retry<400));
		// ÅĞ¶ÏÊÇ³¬Ê±»¹ÊÇµÃµ½ÕıÈ·»ØÓ¦
		// ÈôÓĞ»ØÓ¦£ºÊÇSD¿¨£»Ã»ÓĞ»ØÓ¦£ºÊÇMMC¿¨	  
		//----------MMC¿¨¶îÍâ³õÊ¼»¯²Ù×÷¿ªÊ¼------------
		if(retry==400)
		{
			retry = 0;
			//·¢ËÍMMC¿¨³õÊ¼»¯ÃüÁî£¨Ã»ÓĞ²âÊÔ£©
			do
			{
				r1 = SD_SendCommand(1,0,0);
				retry++;
			}while((r1!=0x00)&& (retry<400));
			if(retry==400)return 1;   //MMC¿¨³õÊ¼»¯³¬Ê±		    
			//Ğ´Èë¿¨ÀàĞÍ
			SD_Type = SD_TYPE_MMC;
		}
		//----------MMC¿¨¶îÍâ³õÊ¼»¯²Ù×÷½áÊø------------	    
		//ÉèÖÃSPIÎª¸ßËÙÄ£Ê½
		SPIx_SetSpeed(SPI_SPEED_4);   
		SPIx_ReadWriteByte(0xFF);	 
		//½ûÖ¹CRCĞ£Ñé	   
		r1 = SD_SendCommand(CMD59, 0, 0x95);
		if(r1 != 0x00)return r1;  //ÃüÁî´íÎó£¬·µ»Ør1   	   
		//ÉèÖÃSector Size
		r1 = SD_SendCommand(CMD16, 512, 0x95);
		if(r1 != 0x00)return r1;//ÃüÁî´íÎó£¬·µ»Ør1		 
		//-----------------SD¿¨¡¢MMC¿¨³õÊ¼»¯½áÊø-----------------
		
	}//SD¿¨ÎªV1.0°æ±¾µÄ³õÊ¼»¯½áÊø	 
	//ÏÂÃæÊÇV2.0¿¨µÄ³õÊ¼»¯
	//ÆäÖĞĞèÒª¶ÁÈ¡OCRÊı¾İ£¬ÅĞ¶ÏÊÇSD2.0»¹ÊÇSD2.0HC¿¨
	else if(r1 == 0x01)
	{
		//V2.0µÄ¿¨£¬CMD8ÃüÁîºó»á´«»Ø4×Ö½ÚµÄÊı¾İ£¬ÒªÌø¹ıÔÙ½áÊø±¾ÃüÁî
		buff[0] = SPIx_ReadWriteByte(0xFF);  //should be 0x00
		buff[1] = SPIx_ReadWriteByte(0xFF);  //should be 0x00
		buff[2] = SPIx_ReadWriteByte(0xFF);  //should be 0x01
		buff[3] = SPIx_ReadWriteByte(0xFF);  //should be 0xAA	    
		Set_SD_CS;	  
		SPIx_ReadWriteByte(0xFF);//the next 8 clocks			 
		//ÅĞ¶Ï¸Ã¿¨ÊÇ·ñÖ§³Ö2.7V-3.6VµÄµçÑ¹·¶Î§
		//if(buff[2]==0x01 && buff[3]==0xAA) //²»ÅĞ¶Ï£¬ÈÃÆäÖ§³ÖµÄ¿¨¸ü¶à
		{	  
			retry = 0;
			//·¢¿¨³õÊ¼»¯Ö¸ÁîCMD55+ACMD41
			do
			{
				r1 = SD_SendCommand(CMD55, 0, 0);
				if(r1!=0x01)return r1;	   
				r1 = SD_SendCommand(ACMD41, 0x40000000, 0);
				if(retry>200)return r1;  //³¬Ê±Ôò·µ»Ør1×´Ì¬  
			}while(r1!=0);		  
			//³õÊ¼»¯Ö¸Áî·¢ËÍÍê³É£¬½ÓÏÂÀ´»ñÈ¡OCRĞÅÏ¢		   
			//-----------¼ø±ğSD2.0¿¨°æ±¾¿ªÊ¼-----------
			r1 = SD_SendCommand_NoDeassert(CMD58, 0, 0);
			if(r1!=0x00)
			{
				Set_SD_CS;//ÊÍ·ÅSDÆ¬Ñ¡ĞÅºÅ
				return r1;  //Èç¹ûÃüÁîÃ»ÓĞ·µ»ØÕıÈ·Ó¦´ğ£¬Ö±½ÓÍË³ö£¬·µ»ØÓ¦´ğ	 
			}//¶ÁOCRÖ¸Áî·¢³öºó£¬½ô½Ó×ÅÊÇ4×Ö½ÚµÄOCRĞÅÏ¢
			buff[0] = SPIx_ReadWriteByte(0xFF);
			buff[1] = SPIx_ReadWriteByte(0xFF); 
			buff[2] = SPIx_ReadWriteByte(0xFF);
			buff[3] = SPIx_ReadWriteByte(0xFF);		 
			//OCR½ÓÊÕÍê³É£¬Æ¬Ñ¡ÖÃ¸ß
			Set_SD_CS;
			SPIx_ReadWriteByte(0xFF);	   
			//¼ì²é½ÓÊÕµ½µÄOCRÖĞµÄbit30Î»£¨CCS£©£¬È·¶¨ÆäÎªSD2.0»¹ÊÇSDHC
			//Èç¹ûCCS=1£ºSDHC   CCS=0£ºSD2.0
			if(buff[0]&0x40)SD_Type = SD_TYPE_V2HC;    //¼ì²éCCS	 
			else SD_Type = SD_TYPE_V2;	    
			//-----------¼ø±ğSD2.0¿¨°æ±¾½áÊø----------- 
			//ÉèÖÃSPIÎª¸ßËÙÄ£Ê½
			SPIx_SetSpeed(SPI_SPEED_4);  
		}	    
	}
	return r1;
}	 																			   

/************************************************************************************
*º¯ÊıÃû:SD_ReceiveData
*º¯ÊıËµÃ÷:´ÓSD¿¨ÖĞ¶Á»ØÖ¸¶¨³¤¶ÈµÄÊı¾İ£¬·ÅÖÃÔÚ¸ø¶¨Î»ÖÃ
*ÊäÈë²ÎÊı:*data(´æ·Å¶Á»ØÊı¾İµÄÄÚ´æ>len)
*         len(Êı¾İ³¤¶È£©
*         release(´«ÊäÍê³ÉºóÊÇ·ñÊÍ·Å×ÜÏßCSÖÃ¸ß 0£º²»ÊÍ·Å 1£ºÊÍ·Å£©
*·µ»ØÖµ:0£ºNO_ERR
*       other£º´íÎóĞÅÏ¢
*
**************************************************************************************/														  
u8 SD_ReceiveData(u8 *data, u16 len, u8 release)
{
	// Æô¶¯Ò»´Î´«Êä
	Clr_SD_CS;				  	  
	if(SD_GetResponse(0xFE))//µÈ´ıSD¿¨·¢»ØÊı¾İÆğÊ¼ÁîÅÆ0xFE
	{	  
		Set_SD_CS;
		return 1;
	}
	while(len--)//¿ªÊ¼½ÓÊÕÊı¾İ
	{
		*data=SPIx_ReadWriteByte(0xFF);
		data++;
	}
	//ÏÂÃæÊÇ2¸öÎ±CRC£¨dummy CRC£©
	SPIx_ReadWriteByte(0xFF);
	SPIx_ReadWriteByte(0xFF);
	if(release==RELEASE)//°´ĞèÊÍ·Å×ÜÏß£¬½«CSÖÃ¸ß
	{
		Set_SD_CS;//´«Êä½áÊø
		SPIx_ReadWriteByte(0xFF);
	}											  					    
	return 0;
}		
																			  
/************************************************************************************
*º¯ÊıÃû: SD_GetCID
*º¯ÊıËµÃ÷: »ñÈ¡SD¿¨µÄCIDĞÅÏ¢£¬°üÀ¨ÖÆÔìÉÌĞÅÏ¢
*ÊäÈë²ÎÊı:*cid_data(´æ·ÅCIDµÄÄÚ´æ£¬ÖÁÉÙ16Byte£©
*
*·µ»ØÖµ:0£ºNO_ERR
*       1£ºTIME_OUT
*       other£º´íÎóĞÅÏ¢
**************************************************************************************/														   
u8 SD_GetCID(u8 *cid_data)
{
	u8 r1;	   
	//·¢CMD10ÃüÁî£¬¶ÁCID
	r1 = SD_SendCommand(CMD10,0,0xFF);
	if(r1 != 0x00)return r1;  //Ã»·µ»ØÕıÈ·Ó¦´ğ£¬ÔòÍË³ö£¬±¨´í  
	SD_ReceiveData(cid_data,16,RELEASE);//½ÓÊÕ16¸ö×Ö½ÚµÄÊı¾İ	 
	return 0;
}		
																			  
/************************************************************************************
*º¯ÊıÃû:SD_GetCSD
*º¯ÊıËµÃ÷:»ñÈ¡SD¿¨µÄCSDĞÅÏ¢£¬°üÀ¨ÈİÁ¿ºÍËÙ¶ÈĞÅÏ¢
*ÊäÈë²ÎÊı:*cid_data(´æ·ÅCIDµÄÄÚ´æ£¬ÖÁÉÙ16Byte£©
*
*·µ»ØÖµ:0£ºNO_ERR
*       1£ºTIME_OUT
*       other£º´íÎóĞÅÏ¢
**************************************************************************************/														   
u8 SD_GetCSD(u8 *csd_data)
{
	u8 r1;	 
	r1=SD_SendCommand(CMD9,0,0xFF);//·¢CMD9ÃüÁî£¬¶ÁCSD
	if(r1)return r1;  //Ã»·µ»ØÕıÈ·Ó¦´ğ£¬ÔòÍË³ö£¬±¨´í  
	SD_ReceiveData(csd_data, 16, RELEASE);//½ÓÊÕ16¸ö×Ö½ÚµÄÊı¾İ 
	return 0;
}  

/************************************************************************************
*º¯ÊıÃû:SD_GetCapacity
*º¯ÊıËµÃ÷:»ñÈ¡SD¿¨µÄÈİÁ¿£¨×Ö½Ú£©
*ÊäÈë²ÎÊı:ÎŞ
*
*·µ»ØÖµ:0£º È¡ÈİÁ¿³ö´í
*       ÆäËû:SD¿¨µÄÈİÁ¿(×Ö½Ú)
*
**************************************************************************************/														  
u32 SD_GetCapacity(void)
{
	u8 csd[16];
	u32 Capacity;
	u8 r1;
	u16 i;
	u16 temp;  					    
	//È¡CSDĞÅÏ¢£¬Èç¹ûÆÚ¼ä³ö´í£¬·µ»Ø0
	if(SD_GetCSD(csd)!=0) return 0;	    
	//Èç¹ûÎªSDHC¿¨£¬°´ÕÕÏÂÃæ·½Ê½¼ÆËã
	if((csd[0]&0xC0)==0x40)
	{									  
    	Capacity=((u32)csd[8])<<8;
    	Capacity+=(u32)csd[9]+1;	 
    	Capacity = (Capacity)*1024;//µÃµ½ÉÈÇøÊı
    	Capacity*=512;//µÃµ½×ÖÚÊı			   
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
		Capacity *= (u32)temp;//×Ö½ÚÎªµ¥Î» 	  
    }
    return (u32)Capacity;
}	  
	  																			    
/************************************************************************************
*º¯ÊıÃû:SD_ReadSingleBlock
*º¯ÊıËµÃ÷:¶ÁSD¿¨µÄÒ»¸öblock
*ÊäÈë²ÎÊı:u32 sector È¡µØÖ·£¨sectorÖµ£¬·ÇÎïÀíµØÖ·£©
*         u8 *buffer Êı¾İ´æ´¢µØÖ·£¨´óĞ¡ÖÁÉÙ512byte£©
*·µ»ØÖµ:0£º ³É¹¦
*       other£ºÊ§°Ü
*
**************************************************************************************/															  
u8 SD_ReadSingleBlock(u32 sector, u8 *buffer)
{
	u8 r1;	    
	//ÉèÖÃÎª¸ßËÙÄ£Ê½
	SPIx_SetSpeed(SPI_SPEED_4);  		   
	//Èç¹û²»ÊÇSDHC£¬¸ø¶¨µÄÊÇsectorµØÖ·£¬½«Æä×ª»»³ÉbyteµØÖ·
	if(SD_Type!=SD_TYPE_V2HC)
	{
		sector = sector<<9;
	} 
	r1 = SD_SendCommand(CMD17, sector, 0);//¶ÁÃüÁî	 		    
	if(r1 != 0x00)return r1; 		   							  
	r1 = SD_ReceiveData(buffer, 512, RELEASE);		 
	if(r1 != 0)return r1;   //¶ÁÊı¾İ³ö´í£¡
	else return 0; 
}
	
////////////////////////////ÏÂÃæ2¸öº¯ÊıÎªUSB¶ÁĞ´ËùĞèÒªµÄ/////////////////////////
//¶¨ÒåSD¿¨µÄ¿é´óĞ¡	 				   
#define BLOCK_SIZE 512 

/************************************************************************************
*º¯ÊıÃû:MSD_WriteBuffer
*º¯ÊıËµÃ÷:Ğ´ÈëMSD/SDÊı¾İ
*ÊäÈë²ÎÊı:pBuffer:Êı¾İ´æ·ÅÇø
*         ReadAddr:Ğ´ÈëµÄÊ×µØÖ·
*         NumByteToRead:ÒªĞ´ÈëµÄ×Ö½ÚÊı
*·µ»ØÖµ:0,Ğ´ÈëÍê³É
*       ÆäËû,Ğ´ÈëÊ§°Ü
*
**************************************************************************************/

u8 MSD_WriteBuffer(u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite)
{
	u32 i,NbrOfBlock = 0, Offset = 0;
	u32 sector;
	u8 r1;
	NbrOfBlock = NumByteToWrite / BLOCK_SIZE;//µÃµ½ÒªĞ´ÈëµÄ¿éµÄÊıÄ¿	    
	Clr_SD_CS;	  		   
	while (NbrOfBlock--)//Ğ´ÈëÒ»¸öÉÈÇø
	{
		sector=WriteAddr+Offset;
		if(SD_Type==SD_TYPE_V2HC)sector>>=9;//Ö´ĞĞÓëÆÕÍ¨²Ù×÷Ïà·´µÄ²Ù×÷					  			 
		r1=SD_SendCommand_NoDeassert(CMD24,sector,0xff);//Ğ´ÃüÁî   
		if(r1)
		{
			Set_SD_CS;
			return 1;//Ó¦´ğ²»ÕıÈ·£¬Ö±½Ó·µ»Ø 	   
		}
		SPIx_ReadWriteByte(0xFE);//·ÅÆğÊ¼ÁîÅÆ0xFE   
		//·ÅÒ»¸ösectorµÄÊı¾İ
		for(i=0;i<512;i++)SPIx_ReadWriteByte(*pBuffer++);  
		//·¢2¸öByteµÄdummy CRC
		SPIx_ReadWriteByte(0xff);
		SPIx_ReadWriteByte(0xff); 
		if(SD_WaitDataReady())//µÈ´ıSD¿¨Êı¾İĞ´ÈëÍê³É
		{
			Set_SD_CS;
			return 2;    
		}
		Offset += 512;	   
	}	    
	//Ğ´ÈëÍê³É£¬Æ¬Ñ¡ÖÃ1
	Set_SD_CS;
	SPIx_ReadWriteByte(0xff);	 
	return 0;
}
	
/************************************************************************************
*º¯ÊıÃû:MSD_ReadBuffer
*º¯ÊıËµÃ÷:¶ÁÈ¡MSD/SDÊı¾İ
*ÊäÈë²ÎÊı:pBuffer:Êı¾İ´æ·ÅÇø
*         ReadAddr:¶ÁÈ¡µÄÊ×µØÖ·
*         NumByteToRead:Òª¶Á³öµÄ×Ö½ÚÊı
*·µ»ØÖµ:0,¶Á³öÍê³É
*       ÆäËû,¶Á³öÊ§°Ü
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
		if(SD_Type==SD_TYPE_V2HC)sector>>=9;//Ö´ĞĞÓëÆÕÍ¨²Ù×÷Ïà·´µÄ²Ù×÷					  			 
		r1=SD_SendCommand_NoDeassert(CMD17,sector,0xff);//¶ÁÃüÁî	 		    
		if(r1)//ÃüÁî·¢ËÍ´íÎó
		{
			Set_SD_CS;
			return r1;
		}	   							  
		r1=SD_ReceiveData(pBuffer,512,RELEASE);		 
		if(r1)//¶ÁÊı´íÎó
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
*º¯ÊıÃû: SD_WriteSingleBlock
*º¯ÊıËµÃ÷:Ğ´ÈëSD¿¨µÄÒ»¸öblock
*ÊäÈë²ÎÊı:sector ÉÈÇøµØÖ·£¨sectorÖµ£¬·ÇÎïÀíµØÖ·£©
*         *buffer Êı¾İ´æ´¢µØÖ·£¨´óĞ¡ÖÁÉÙ512byte£©
*·µ»ØÖµ:0£º ³É¹¦
*       other£ºÊ§°Ü
*
**************************************************************************************/															  
u8 SD_WriteSingleBlock(u32 sector, const u8 *data)
{
	u8 r1;
	u16 i;
	u16 retry;
	
	//ÉèÖÃÎª¸ßËÙÄ£Ê½
	//SPIx_SetSpeed(SPI_SPEED_HIGH);	   
	//Èç¹û²»ÊÇSDHC£¬¸ø¶¨µÄÊÇsectorµØÖ·£¬½«Æä×ª»»³ÉbyteµØÖ·
	if(SD_Type!=SD_TYPE_V2HC)
	{
		sector = sector<<9;
	}   
	r1 = SD_SendCommand(CMD24, sector, 0x00);
	if(r1 != 0x00)
	{
		return r1;  //Ó¦´ğ²»ÕıÈ·£¬Ö±½Ó·µ»Ø
	}
	
	//¿ªÊ¼×¼±¸Êı¾İ´«Êä
	Clr_SD_CS;
	//ÏÈ·Å3¸ö¿ÕÊı¾İ£¬µÈ´ıSD¿¨×¼±¸ºÃ
	SPIx_ReadWriteByte(0xff);
	SPIx_ReadWriteByte(0xff);
	SPIx_ReadWriteByte(0xff);
	//·ÅÆğÊ¼ÁîÅÆ0xFE
	SPIx_ReadWriteByte(0xFE);
	
	//·ÅÒ»¸ösectorµÄÊı¾İ
	for(i=0;i<512;i++)
		{
		SPIx_ReadWriteByte(*data++);
		}
	//·¢2¸öByteµÄdummy CRC
	SPIx_ReadWriteByte(0xff);
	SPIx_ReadWriteByte(0xff);
	
	//µÈ´ıSD¿¨Ó¦´ğ
	r1 = SPIx_ReadWriteByte(0xff);
	if((r1&0x1F)!=0x05)
	{
		Set_SD_CS;
		return r1;
	}
	
	//µÈ´ı²Ù×÷Íê³É
	retry = 0;
	while(!SPIx_ReadWriteByte(0xff))
	{
		retry++;
		if(retry>0xfffe)        //Èç¹û³¤Ê±¼äĞ´ÈëÃ»ÓĞÍê³É£¬±¨´íÍË³ö
		{
			Set_SD_CS;
			return 1;           //Ğ´Èë³¬Ê±·µ»Ø1
		}
	}	    
	//Ğ´ÈëÍê³É£¬Æ¬Ñ¡ÖÃ1
	Set_SD_CS;
	SPIx_ReadWriteByte(0xff);
	
	return 0;
}	
				           
/************************************************************************************
*º¯ÊıÃû:SD_ReadMultiBlock
*º¯ÊıËµÃ÷:¶ÁSD¿¨µÄ¶à¸öblock(Êµ¼Ê²âÊÔ¹ı)
*ÊäÈë²ÎÊı:u32 sector ÉÈÇøµØÖ·£¨sectorÖµ£¬·ÇÎïÀíµØÖ·£©
*         *buffer Êı¾İ´æ´¢µØÖ·£¨´óĞ¡ÖÁÉÙ512byte£©
*         count Á¬Ğø¶Ácount¸öblock 
*·µ»ØÖµ:0£º ³É¹¦
*      other£ºÊ§°Ü
*
**************************************************************************************/															  
u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count)
{
	u8 r1;	 			 
	//SPIx_SetSpeed(SPI_SPEED_HIGH);//ÉèÖÃÎª¸ßËÙÄ£Ê½  
	//Èç¹û²»ÊÇSDHC£¬½«sectorµØÖ·×ª³ÉbyteµØÖ·
	if(SD_Type!=SD_TYPE_V2HC)sector = sector<<9;  
	//SD_WaitDataReady();
	//·¢¶Á¶à¿éÃüÁî
	r1 = SD_SendCommand(CMD18, sector, 0);//¶ÁÃüÁî
	if(r1 != 0x00)return r1;	 
	do//¿ªÊ¼½ÓÊÕÊı¾İ
	{
		if(SD_ReceiveData(buffer, 512, NO_RELEASE) != 0x00)break; 
		buffer += 512;
	} while(--count);		 
	//È«²¿´«ÊäÍê±Ï£¬·¢ËÍÍ£Ö¹ÃüÁî
	SD_SendCommand(CMD12, 0, 0);
	//ÊÍ·Å×ÜÏß
	Set_SD_CS;
	SPIx_ReadWriteByte(0xFF);    
	if(count != 0)return count;   //Èç¹ûÃ»ÓĞ´«Íê£¬·µ»ØÊ£Óà¸öÊı	 
	else return 0;	 
}		
										  
/************************************************************************************
*º¯ÊıÃû:SD_WriteMultiBlock
*º¯ÊıËµÃ÷:Ğ´ÈëSD¿¨µÄN¸öblock
*ÊäÈë²ÎÊı:sector ÉÈÇøµØÖ·£¨sectorÖµ£¬·ÇÎïÀíµØÖ·£©
*         *buffer Êı¾İ´æ´¢µØÖ·£¨´óĞ¡ÖÁÉÙ512byte£©
*         count Ğ´ÈëµÄblockÊıÄ¿	
*·µ»ØÖµ:0£º ³É¹¦
*       other£ºÊ§°Ü
*
**************************************************************************************/															   
u8 SD_WriteMultiBlock(u32 sector, const u8 *data, u8 count)
{
	u8 r1;
	u16 i;	 		 	 
	if(SD_Type != SD_TYPE_V2HC)sector = sector<<9;//Èç¹û²»ÊÇSDHC£¬¸ø¶¨µÄÊÇsectorµØÖ·£¬½«Æä×ª»»³ÉbyteµØÖ·  
	if(SD_Type != SD_TYPE_MMC) r1 = SD_SendCommand(ACMD23, count, 0x00);//Èç¹ûÄ¿±ê¿¨²»ÊÇMMC¿¨£¬ÆôÓÃACMD23Ö¸ÁîÊ¹ÄÜÔ¤²Á³ı   
	r1 = SD_SendCommand(CMD25, sector, 0x00);//·¢¶à¿éĞ´ÈëÖ¸Áî
	if(r1 != 0x00)return r1;  //Ó¦´ğ²»ÕıÈ·£¬Ö±½Ó·µ»Ø	 
	Clr_SD_CS;//¿ªÊ¼×¼±¸Êı¾İ´«Êä   
	SPIx_ReadWriteByte(0xff);//ÏÈ·Å3¸ö¿ÕÊı¾İ£¬µÈ´ıSD¿¨×¼±¸ºÃ
	SPIx_ReadWriteByte(0xff);   
	//--------ÏÂÃæÊÇN¸ösectorĞ´ÈëµÄÑ­»·²¿·Ö
	do
	{
		//·ÅÆğÊ¼ÁîÅÆ0xFC ±íÃ÷ÊÇ¶à¿éĞ´Èë
		SPIx_ReadWriteByte(0xFC);	  
		//·ÅÒ»¸ösectorµÄÊı¾İ
		for(i=0;i<512;i++)
		{
			SPIx_ReadWriteByte(*data++);
		}
		//·¢2¸öByteµÄdummy CRC
		SPIx_ReadWriteByte(0xff);
		SPIx_ReadWriteByte(0xff);
		
		//µÈ´ıSD¿¨Ó¦´ğ
		r1 = SPIx_ReadWriteByte(0xff);
		if((r1&0x1F)!=0x05)
		{
			Set_SD_CS;    //Èç¹ûÓ¦´ğÎª±¨´í£¬Ôò´ø´íÎó´úÂëÖ±½ÓÍË³ö
			return r1;
		}		   
		//µÈ´ıSD¿¨Ğ´ÈëÍê³É
		if(SD_WaitDataReady()==1)
		{
			Set_SD_CS;    //µÈ´ıSD¿¨Ğ´ÈëÍê³É³¬Ê±£¬Ö±½ÓÍË³ö±¨´í
			return 1;
		}	   
	}while(--count);//±¾sectorÊı¾İ´«ÊäÍê³É  
	//·¢½áÊø´«ÊäÁîÅÆ0xFD
	r1 = SPIx_ReadWriteByte(0xFD);
	if(r1==0x00)
	{
		count =  0xfe;
	}		   
	if(SD_WaitDataReady()) //µÈ´ı×¼±¸ºÃ
	{
		Set_SD_CS;
		return 1;  
	}
	//Ğ´ÈëÍê³É£¬Æ¬Ñ¡ÖÃ1
	Set_SD_CS;
	SPIx_ReadWriteByte(0xff);  
	return count;   //·µ»ØcountÖµ£¬Èç¹ûĞ´ÍêÔòcount=0£¬·ñÔòcount=1
}
						  					  
/************************************************************************************
*º¯ÊıÃû:SD_Read_Bytes
*º¯ÊıËµÃ÷:ÔÚÖ¸¶¨ÉÈÇø,´Óoffset¿ªÊ¼¶Á³öbytes¸ö×Ö½Ú
*ÊäÈë²ÎÊı:sector ÉÈÇøµØÖ·£¨sectorÖµ£¬·ÇÎïÀíµØÖ·£©
*         *buf     Êı¾İ´æ´¢µØÖ·£¨´óĞ¡<=512byte£©
*         bytes   Òª¶Á³öµÄ×Ö½ÚÊı
*·µ»ØÖµ:0£º ³É¹¦
*       other£ºÊ§°Ü
*
**************************************************************************************/															   
u8 SD_Read_Bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes)
{
	u8 r1;u16 i=0;  
	r1=SD_SendCommand(CMD17,address<<9,0);//·¢ËÍ¶ÁÉÈÇøÃüÁî      
	if(r1)return r1;  //Ó¦´ğ²»ÕıÈ·£¬Ö±½Ó·µ»Ø
	Clr_SD_CS;//Ñ¡ÖĞSD¿¨
	if(SD_GetResponse(0xFE))//µÈ´ıSD¿¨·¢»ØÊı¾İÆğÊ¼ÁîÅÆ0xFE
	{
		Set_SD_CS; //¹Ø±ÕSD¿¨
		return 1;//¶ÁÈ¡Ê§°Ü
	}	 
	for(i=0;i<offset;i++)SPIx_ReadWriteByte(0xff);//Ìø¹ıoffsetÎ» 
	for(;i<offset+bytes;i++)*buf++=SPIx_ReadWriteByte(0xff);//¶ÁÈ¡ÓĞÓÃÊı¾İ	
	for(;i<512;i++) SPIx_ReadWriteByte(0xff); 	 //¶Á³öÊ£Óà×Ö½Ú
	SPIx_ReadWriteByte(0xff);//·¢ËÍÎ±CRCÂë
	SPIx_ReadWriteByte(0xff);  
	Set_SD_CS;//¹Ø±ÕSD¿¨
	return 0;
}
/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/













