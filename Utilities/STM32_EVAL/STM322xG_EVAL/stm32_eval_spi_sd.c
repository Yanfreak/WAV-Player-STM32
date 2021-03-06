 /**********************************************************
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
**********************************************************/
#include "stm32f2xx.h"
#include "stm32_eval_spi_sd.h"


						   
u8  SD_Type=0;//SD卡的类型

/************************************************************************************
*函数名:SPIx_Init
*函数说明:SPI模块的初始化代码
*输入参数:无
*
*返回值:无
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
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  //复用推挽输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	/* Configure I/O for Flash Chip select */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;  //SPI CS
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;  //复用推挽输出
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	/* Deselect the FLASH: Chip Select high */
	GPIO_SetBits(GPIOD,GPIO_Pin_2);
	 
	/* SPI1 configuration */
    //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; 
    //设置SPI工作模式:设置为主SPI 
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    //设置SPI的数据大小:SPI发送接收8位帧结构		
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    //选择了串行时钟的稳态:时钟悬空高		
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    //数据捕获于第二个时钟沿	
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制	
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;//SPI_NSS_Soft;
    //定义波特率预分频的值:波特率预分频值为256		
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始	
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    //CRC值计算的多项式	
	SPI_InitStructure.SPI_CRCPolynomial = 7;
    //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器	
	SPI_Init(SPI3, &SPI_InitStructure);  	
	/* Enable SPI1  */
	SPI_Cmd(SPI3, ENABLE); //使能SPI外设	
	SPIx_ReadWriteByte(0xff);//启动传输		 
}  
	 
//SPI 速度设置函数
//SpeedSet:
//SPI_SPEED_2   2分频   (SPI 36M@sys 72M)
//SPI_SPEED_8   8分频   (SPI 9M@sys 72M)
//SPI_SPEED_16  16分频  (SPI 4.5M@sys 72M)
//SPI_SPEED_256 256分频 (SPI 281.25K@sys 72M)
/************************************************************************************
*函数名: SPIx_SetSpeed
*函数说明: SPI 速度设置函数
*输入参数:SpeedSet速度选择
*
*返回值:无
*
*
**************************************************************************************/

void SPIx_SetSpeed(u8 SpeedSet)
{
	SPI3->CR1&=0XFFC7;//Fsck=Fcpu/256
	if(SpeedSet==SPI_SPEED_2)//二分频
	{
    	SPI3->CR1|=0<<3;//Fsck=Fpclk/2=36Mhz	
	}else if(SpeedSet==SPI_SPEED_8)//八分频 
	{
    	SPI3->CR1|=2<<3;//Fsck=Fpclk/8=9Mhz	
	}else if(SpeedSet==SPI_SPEED_16)//十六分频
	{
    	SPI3->CR1|=3<<3;//Fsck=Fpclk/16=4.5Mhz
	}else			 	 //256分频
	{
    	SPI3->CR1|=7<<3; //Fsck=Fpclk/256=281.25Khz 低速模式
	}	  
	/* Enable SPI1  */
	SPI_Cmd(SPI1, ENABLE); //使能SPI外设
	} 

/************************************************************************************
*函数名:SPIx_ReadWriteByte
*函数说明:SPIx 读写一个字节
*输入参数:TxData:要写入的字节
*
*返回值:读取到的字节
*
*
**************************************************************************************/

u8 SPIx_ReadWriteByte(u8 TxData)
{		
	u8 retry=0;				 
	/* Loop while DR register in not emplty */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
	{
		retry++;
		if(retry>200)return 0;
	}			  
	/* Send byte through the SPI1 peripheral */
	SPI_I2S_SendData(SPI3, TxData); //通过外设SPIx发送一个数据
	retry=0; 
	/* Wait to receive a byte */
	while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET); //检查指定的SPI标志位设置与否:接受缓存非空标志位
	{
		retry++;
		if(retry>200)return 0;
	}	  						    
	/* Return the byte read from the SPI bus */
	return SPI_I2S_ReceiveData(SPI3); //返回通过SPIx最近接收的数据				    
}
/************************************************************************************
*函数名:SD_GetResponse
*函数说明: 等待SD卡回应
*输入参数:要得到的回应值
*
*返回值:0,成功得到了该回应值
*         其他,得到回应值失败
*
**************************************************************************************/
u8 SD_GetResponse(u8 Response)
{
	u16 Count=0xFFF;//等待次数	   						  
	while ((SPIx_ReadWriteByte(0XFF)!=Response)&&Count)Count--;//等待得到准确的回应  	  
	if (Count==0)return MSD_RESPONSE_FAILURE;//得到回应失败   
	else return MSD_RESPONSE_NO_ERROR;//正确回应
}
	
/************************************************************************************
*函数名:SD_WaitDataReady
*函数说明: 等待SD卡写入完成
*输入参数:无
*
*返回值:0  成功
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
		r1=SPIx_ReadWriteByte(0xFF)&0X1F;//读到回应
		if(retry==0xfffe)return 1; 
		retry++;
		switch (r1)
		{					   
			case MSD_DATA_OK://数据接收正确了	 
				r1=MSD_DATA_OK;
				break;  
			case MSD_DATA_CRC_ERROR:  //CRC校验错误
				return MSD_DATA_CRC_ERROR;  
			case MSD_DATA_WRITE_ERROR://数据写入错误
				return MSD_DATA_WRITE_ERROR;  
			default://未知错误    
				r1=MSD_DATA_OTHER_ERROR;
				break;	 
		}   
	}while(r1==MSD_DATA_OTHER_ERROR); //数据错误时一直等待
	retry=0;
	while(SPIx_ReadWriteByte(0XFF)==0)//读到数据为0,则数据还未写完成
	{
		retry++;
		if(retry>=0XFFFFFFFE)return 0XFF;//等待失败了
	};	    
	return 0;//成功了
}	 

/************************************************************************************
*函数名:SD_SendCommand
*函数说明:向SD卡发送一个命令
*输入参数:cmd   命令 
*         arg  命令参数
*         crc   crc校验值
*返回值:SD卡返回的响应	
*
*
**************************************************************************************/														  
u8 SD_SendCommand(u8 cmd, u32 arg, u8 crc)
{
	u8 r1;	
	u8 Retry=0;	         
	Set_SD_CS;
	SPIx_ReadWriteByte(0xff);//高速写命令延时
	SPIx_ReadWriteByte(0xff);     
	SPIx_ReadWriteByte(0xff);  	 
	//片选端置低，选中SD卡
	Clr_SD_CS; 
	//发送
	SPIx_ReadWriteByte(cmd | 0x40);//分别写入命令
	SPIx_ReadWriteByte(arg >> 24);
	SPIx_ReadWriteByte(arg >> 16);
	SPIx_ReadWriteByte(arg >> 8);
	SPIx_ReadWriteByte(arg);
	SPIx_ReadWriteByte(crc); 
	//等待响应，或超时退出
	while((r1=SPIx_ReadWriteByte(0xFF))==0xFF)
	{
		Retry++;	    
		if(Retry>200)break; 
	}   
	//关闭片选
	Set_SD_CS;
	//在总线上额外增加8个时钟，让SD卡完成剩下的工作
	SPIx_ReadWriteByte(0xFF);
	//返回状态值
	return r1;
}	
	  																				 

/************************************************************************************
*函数名: SD_SendCommand_NoDeassert
*函数说明: 向SD卡发送一个命令(结束是不失能片选，还有后续数据传来）
*输入参数:cmd   命令
*         arg  命令参数
*         crc   crc校验值
*返回值:SD卡返回的响应
*
*
**************************************************************************************/														  
u8 SD_SendCommand_NoDeassert(u8 cmd, u32 arg, u8 crc)
{
	u8 Retry=0;	         
	u8 r1;			   
	SPIx_ReadWriteByte(0xff);//高速写命令延时
	SPIx_ReadWriteByte(0xff);  	 	 
	Clr_SD_CS;//片选端置低，选中SD卡	   
	//发送
	SPIx_ReadWriteByte(cmd | 0x40); //分别写入命令
	SPIx_ReadWriteByte(arg >> 24);
	SPIx_ReadWriteByte(arg >> 16);
	SPIx_ReadWriteByte(arg >> 8);
	SPIx_ReadWriteByte(arg);
	SPIx_ReadWriteByte(crc);   
	//等待响应，或超时退出
	while((r1=SPIx_ReadWriteByte(0xFF))==0xFF)
	{
		Retry++;	    
		if(Retry>200)break; 
	}  	  
	//返回响应值
	return r1;
}
	
/************************************************************************************
*函数名: SD_Idle_Sta
*函数说明:把SD卡设置到挂起模式
*输入参数:无
*
*返回值:0,成功设置
*       1,设置失败
*
**************************************************************************************/
u8 SD_Idle_Sta(void)
{
	u16 i;
	u8 retry;	   	  
	for(i=0;i<0xf00;i++);//纯延时，等待SD卡上电完成	 
	//先产生>74个脉冲，让SD卡自己初始化完成
	for(i=0;i<10;i++)SPIx_ReadWriteByte(0xFF); 
	//-----------------SD卡复位到idle开始-----------------
	//循环连续发送CMD0，直到SD卡返回0x01,进入IDLE状态
	//超时则直接退出
	retry = 0;
	do
	{	   
		//发送CMD0，让SD卡进入IDLE状态
		i = SD_SendCommand(CMD0, 0, 0x95);
		retry++;
	}while((i!=0x01)&&(retry<200));
	//跳出循环后，检查原因：初始化成功？or 重试超时？
	if(retry==200)return 1; //失败
	return 0;//成功	 						  
}	
														    
/************************************************************************************
*函数名:SD_Init
*函数说明:初始化SD卡 如果成功返回,则会自动设置SPI速度为18Mhz
*输入参数:无
*
*返回值:0：NO_ERR
*       1：TIME_OUT
*       99：NO_CARD	
**************************************************************************************/															 
u8 SD_Init(void)
{		
						 
	u8 r1;      // 存放SD卡的返回值
	u16 retry;  // 用来进行超时计数
	u8 buff[6];
	SPIx_Init();
	SPIx_SetSpeed(SPI_SPEED_256);//设置到低速模式		 
	Set_SD_CS;	
	if(SD_Idle_Sta()) return 1;//超时返回1 设置到idle 模式失败	  
	//-----------------SD卡复位到idle结束-----------------	 
	//获取卡片的SD版本信息
	Clr_SD_CS;	
	r1 = SD_SendCommand_NoDeassert(8, 0x1aa,0x87);	     
	//如果卡片版本信息是v1.0版本的，即r1=0x05，则进行以下初始化
	if(r1 == 0x05)
	{
		//设置卡类型为SDV1.0，如果后面检测到为MMC卡，再修改为MMC
		SD_Type = SD_TYPE_V1;	   
		//如果是V1.0卡，CMD8指令后没有后续数据
		//片选置高，结束本次命令
		Set_SD_CS;
		//多发8个CLK，让SD结束后续操作
		SPIx_ReadWriteByte(0xFF);	  
		//-----------------SD卡、MMC卡初始化开始-----------------	 
		//发卡初始化指令CMD55+ACMD41
		// 如果有应答，说明是SD卡，且初始化完成
		// 没有回应，说明是MMC卡，额外进行相应初始化
		retry = 0;
		do
		{
			//先发CMD55，应返回0x01；否则出错
			r1 = SD_SendCommand(CMD55, 0, 0);
			if(r1 == 0XFF)return r1;//只要不是0xff,就接着发送	  
			//得到正确响应后，发ACMD41，应得到返回值0x00，否则重试200次
			r1 = SD_SendCommand(ACMD41, 0, 0);
			retry++;
		}while((r1!=0x00) && (retry<400));
		// 判断是超时还是得到正确回应
		// 若有回应：是SD卡；没有回应：是MMC卡	  
		//----------MMC卡额外初始化操作开始------------
		if(retry==400)
		{
			retry = 0;
			//发送MMC卡初始化命令（没有测试）
			do
			{
				r1 = SD_SendCommand(1,0,0);
				retry++;
			}while((r1!=0x00)&& (retry<400));
			if(retry==400)return 1;   //MMC卡初始化超时		    
			//写入卡类型
			SD_Type = SD_TYPE_MMC;
		}
		//----------MMC卡额外初始化操作结束------------	    
		//设置SPI为高速模式
		SPIx_SetSpeed(SPI_SPEED_4);   
		SPIx_ReadWriteByte(0xFF);	 
		//禁止CRC校验	   
		r1 = SD_SendCommand(CMD59, 0, 0x95);
		if(r1 != 0x00)return r1;  //命令错误，返回r1   	   
		//设置Sector Size
		r1 = SD_SendCommand(CMD16, 512, 0x95);
		if(r1 != 0x00)return r1;//命令错误，返回r1		 
		//-----------------SD卡、MMC卡初始化结束-----------------
		
	}//SD卡为V1.0版本的初始化结束	 
	//下面是V2.0卡的初始化
	//其中需要读取OCR数据，判断是SD2.0还是SD2.0HC卡
	else if(r1 == 0x01)
	{
		//V2.0的卡，CMD8命令后会传回4字节的数据，要跳过再结束本命令
		buff[0] = SPIx_ReadWriteByte(0xFF);  //should be 0x00
		buff[1] = SPIx_ReadWriteByte(0xFF);  //should be 0x00
		buff[2] = SPIx_ReadWriteByte(0xFF);  //should be 0x01
		buff[3] = SPIx_ReadWriteByte(0xFF);  //should be 0xAA	    
		Set_SD_CS;	  
		SPIx_ReadWriteByte(0xFF);//the next 8 clocks			 
		//判断该卡是否支持2.7V-3.6V的电压范围
		//if(buff[2]==0x01 && buff[3]==0xAA) //不判断，让其支持的卡更多
		{	  
			retry = 0;
			//发卡初始化指令CMD55+ACMD41
			do
			{
				r1 = SD_SendCommand(CMD55, 0, 0);
				if(r1!=0x01)return r1;	   
				r1 = SD_SendCommand(ACMD41, 0x40000000, 0);
				if(retry>200)return r1;  //超时则返回r1状态  
			}while(r1!=0);		  
			//初始化指令发送完成，接下来获取OCR信息		   
			//-----------鉴别SD2.0卡版本开始-----------
			r1 = SD_SendCommand_NoDeassert(CMD58, 0, 0);
			if(r1!=0x00)
			{
				Set_SD_CS;//释放SD片选信号
				return r1;  //如果命令没有返回正确应答，直接退出，返回应答	 
			}//读OCR指令发出后，紧接着是4字节的OCR信息
			buff[0] = SPIx_ReadWriteByte(0xFF);
			buff[1] = SPIx_ReadWriteByte(0xFF); 
			buff[2] = SPIx_ReadWriteByte(0xFF);
			buff[3] = SPIx_ReadWriteByte(0xFF);		 
			//OCR接收完成，片选置高
			Set_SD_CS;
			SPIx_ReadWriteByte(0xFF);	   
			//检查接收到的OCR中的bit30位（CCS），确定其为SD2.0还是SDHC
			//如果CCS=1：SDHC   CCS=0：SD2.0
			if(buff[0]&0x40)SD_Type = SD_TYPE_V2HC;    //检查CCS	 
			else SD_Type = SD_TYPE_V2;	    
			//-----------鉴别SD2.0卡版本结束----------- 
			//设置SPI为高速模式
			SPIx_SetSpeed(SPI_SPEED_4);  
		}	    
	}
	return r1;
}	 																			   

/************************************************************************************
*函数名:SD_ReceiveData
*函数说明:从SD卡中读回指定长度的数据，放置在给定位置
*输入参数:*data(存放读回数据的内存>len)
*         len(数据长度）
*         release(传输完成后是否释放总线CS置高 0：不释放 1：释放）
*返回值:0：NO_ERR
*       other：错误信息
*
**************************************************************************************/														  
u8 SD_ReceiveData(u8 *data, u16 len, u8 release)
{
	// 启动一次传输
	Clr_SD_CS;				  	  
	if(SD_GetResponse(0xFE))//等待SD卡发回数据起始令牌0xFE
	{	  
		Set_SD_CS;
		return 1;
	}
	while(len--)//开始接收数据
	{
		*data=SPIx_ReadWriteByte(0xFF);
		data++;
	}
	//下面是2个伪CRC（dummy CRC）
	SPIx_ReadWriteByte(0xFF);
	SPIx_ReadWriteByte(0xFF);
	if(release==RELEASE)//按需释放总线，将CS置高
	{
		Set_SD_CS;//传输结束
		SPIx_ReadWriteByte(0xFF);
	}											  					    
	return 0;
}		
																			  
/************************************************************************************
*函数名: SD_GetCID
*函数说明: 获取SD卡的CID信息，包括制造商信息
*输入参数:*cid_data(存放CID的内存，至少16Byte）
*
*返回值:0：NO_ERR
*       1：TIME_OUT
*       other：错误信息
**************************************************************************************/														   
u8 SD_GetCID(u8 *cid_data)
{
	u8 r1;	   
	//发CMD10命令，读CID
	r1 = SD_SendCommand(CMD10,0,0xFF);
	if(r1 != 0x00)return r1;  //没返回正确应答，则退出，报错  
	SD_ReceiveData(cid_data,16,RELEASE);//接收16个字节的数据	 
	return 0;
}		
																			  
/************************************************************************************
*函数名:SD_GetCSD
*函数说明:获取SD卡的CSD信息，包括容量和速度信息
*输入参数:*cid_data(存放CID的内存，至少16Byte）
*
*返回值:0：NO_ERR
*       1：TIME_OUT
*       other：错误信息
**************************************************************************************/														   
u8 SD_GetCSD(u8 *csd_data)
{
	u8 r1;	 
	r1=SD_SendCommand(CMD9,0,0xFF);//发CMD9命令，读CSD
	if(r1)return r1;  //没返回正确应答，则退出，报错  
	SD_ReceiveData(csd_data, 16, RELEASE);//接收16个字节的数据 
	return 0;
}  

/************************************************************************************
*函数名:SD_GetCapacity
*函数说明:获取SD卡的容量（字节）
*输入参数:无
*
*返回值:0： 取容量出错
*       其他:SD卡的容量(字节)
*
**************************************************************************************/														  
u32 SD_GetCapacity(void)
{
	u8 csd[16];
	u32 Capacity;
	u8 r1;
	u16 i;
	u16 temp;  					    
	//取CSD信息，如果期间出错，返回0
	if(SD_GetCSD(csd)!=0) return 0;	    
	//如果为SDHC卡，按照下面方式计算
	if((csd[0]&0xC0)==0x40)
	{									  
    	Capacity=((u32)csd[8])<<8;
    	Capacity+=(u32)csd[9]+1;	 
    	Capacity = (Capacity)*1024;//得到扇区数
    	Capacity*=512;//得到字谑�			   
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
		Capacity *= (u32)temp;//字节为单位 	  
    }
    return (u32)Capacity;
}	  
	  																			    
/************************************************************************************
*函数名:SD_ReadSingleBlock
*函数说明:读SD卡的一个block
*输入参数:u32 sector 取地址（sector值，非物理地址）
*         u8 *buffer 数据存储地址（大小至少512byte）
*返回值:0： 成功
*       other：失败
*
**************************************************************************************/															  
u8 SD_ReadSingleBlock(u32 sector, u8 *buffer)
{
	u8 r1;	    
	//设置为高速模式
	SPIx_SetSpeed(SPI_SPEED_4);  		   
	//如果不是SDHC，给定的是sector地址，将其转换成byte地址
	if(SD_Type!=SD_TYPE_V2HC)
	{
		sector = sector<<9;
	} 
	r1 = SD_SendCommand(CMD17, sector, 0);//读命令	 		    
	if(r1 != 0x00)return r1; 		   							  
	r1 = SD_ReceiveData(buffer, 512, RELEASE);		 
	if(r1 != 0)return r1;   //读数据出错！
	else return 0; 
}
	
////////////////////////////下面2个函数为USB读写所需要的/////////////////////////
//定义SD卡的块大小	 				   
#define BLOCK_SIZE 512 

/************************************************************************************
*函数名:MSD_WriteBuffer
*函数说明:写入MSD/SD数据
*输入参数:pBuffer:数据存放区
*         ReadAddr:写入的首地址
*         NumByteToRead:要写入的字节数
*返回值:0,写入完成
*       其他,写入失败
*
**************************************************************************************/

u8 MSD_WriteBuffer(u8* pBuffer, u32 WriteAddr, u32 NumByteToWrite)
{
	u32 i,NbrOfBlock = 0, Offset = 0;
	u32 sector;
	u8 r1;
	NbrOfBlock = NumByteToWrite / BLOCK_SIZE;//得到要写入的块的数目	    
	Clr_SD_CS;	  		   
	while (NbrOfBlock--)//写入一个扇区
	{
		sector=WriteAddr+Offset;
		if(SD_Type==SD_TYPE_V2HC)sector>>=9;//执行与普通操作相反的操作					  			 
		r1=SD_SendCommand_NoDeassert(CMD24,sector,0xff);//写命令   
		if(r1)
		{
			Set_SD_CS;
			return 1;//应答不正确，直接返回 	   
		}
		SPIx_ReadWriteByte(0xFE);//放起始令牌0xFE   
		//放一个sector的数据
		for(i=0;i<512;i++)SPIx_ReadWriteByte(*pBuffer++);  
		//发2个Byte的dummy CRC
		SPIx_ReadWriteByte(0xff);
		SPIx_ReadWriteByte(0xff); 
		if(SD_WaitDataReady())//等待SD卡数据写入完成
		{
			Set_SD_CS;
			return 2;    
		}
		Offset += 512;	   
	}	    
	//写入完成，片选置1
	Set_SD_CS;
	SPIx_ReadWriteByte(0xff);	 
	return 0;
}
	
/************************************************************************************
*函数名:MSD_ReadBuffer
*函数说明:读取MSD/SD数据
*输入参数:pBuffer:数据存放区
*         ReadAddr:读取的首地址
*         NumByteToRead:要读出的字节数
*返回值:0,读出完成
*       其他,读出失败
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
		if(SD_Type==SD_TYPE_V2HC)sector>>=9;//执行与普通操作相反的操作					  			 
		r1=SD_SendCommand_NoDeassert(CMD17,sector,0xff);//读命令	 		    
		if(r1)//命令发送错误
		{
			Set_SD_CS;
			return r1;
		}	   							  
		r1=SD_ReceiveData(pBuffer,512,RELEASE);		 
		if(r1)//读数错误
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
*函数名: SD_WriteSingleBlock
*函数说明:写入SD卡的一个block
*输入参数:sector 扇区地址（sector值，非物理地址）
*         *buffer 数据存储地址（大小至少512byte）
*返回值:0： 成功
*       other：失败
*
**************************************************************************************/															  
u8 SD_WriteSingleBlock(u32 sector, const u8 *data)
{
	u8 r1;
	u16 i;
	u16 retry;
	
	//设置为高速模式
	//SPIx_SetSpeed(SPI_SPEED_HIGH);	   
	//如果不是SDHC，给定的是sector地址，将其转换成byte地址
	if(SD_Type!=SD_TYPE_V2HC)
	{
		sector = sector<<9;
	}   
	r1 = SD_SendCommand(CMD24, sector, 0x00);
	if(r1 != 0x00)
	{
		return r1;  //应答不正确，直接返回
	}
	
	//开始准备数据传输
	Clr_SD_CS;
	//先放3个空数据，等待SD卡准备好
	SPIx_ReadWriteByte(0xff);
	SPIx_ReadWriteByte(0xff);
	SPIx_ReadWriteByte(0xff);
	//放起始令牌0xFE
	SPIx_ReadWriteByte(0xFE);
	
	//放一个sector的数据
	for(i=0;i<512;i++)
		{
		SPIx_ReadWriteByte(*data++);
		}
	//发2个Byte的dummy CRC
	SPIx_ReadWriteByte(0xff);
	SPIx_ReadWriteByte(0xff);
	
	//等待SD卡应答
	r1 = SPIx_ReadWriteByte(0xff);
	if((r1&0x1F)!=0x05)
	{
		Set_SD_CS;
		return r1;
	}
	
	//等待操作完成
	retry = 0;
	while(!SPIx_ReadWriteByte(0xff))
	{
		retry++;
		if(retry>0xfffe)        //如果长时间写入没有完成，报错退出
		{
			Set_SD_CS;
			return 1;           //写入超时返回1
		}
	}	    
	//写入完成，片选置1
	Set_SD_CS;
	SPIx_ReadWriteByte(0xff);
	
	return 0;
}	
				           
/************************************************************************************
*函数名:SD_ReadMultiBlock
*函数说明:读SD卡的多个block(实际测试过)
*输入参数:u32 sector 扇区地址（sector值，非物理地址）
*         *buffer 数据存储地址（大小至少512byte）
*         count 连续读count个block 
*返回值:0： 成功
*      other：失败
*
**************************************************************************************/															  
u8 SD_ReadMultiBlock(u32 sector, u8 *buffer, u8 count)
{
	u8 r1;	 			 
	//SPIx_SetSpeed(SPI_SPEED_HIGH);//设置为高速模式  
	//如果不是SDHC，将sector地址转成byte地址
	if(SD_Type!=SD_TYPE_V2HC)sector = sector<<9;  
	//SD_WaitDataReady();
	//发读多块命令
	r1 = SD_SendCommand(CMD18, sector, 0);//读命令
	if(r1 != 0x00)return r1;	 
	do//开始接收数据
	{
		if(SD_ReceiveData(buffer, 512, NO_RELEASE) != 0x00)break; 
		buffer += 512;
	} while(--count);		 
	//全部传输完毕，发送停止命令
	SD_SendCommand(CMD12, 0, 0);
	//释放总线
	Set_SD_CS;
	SPIx_ReadWriteByte(0xFF);    
	if(count != 0)return count;   //如果没有传完，返回剩余个数	 
	else return 0;	 
}		
										  
/************************************************************************************
*函数名:SD_WriteMultiBlock
*函数说明:写入SD卡的N个block
*输入参数:sector 扇区地址（sector值，非物理地址）
*         *buffer 数据存储地址（大小至少512byte）
*         count 写入的block数目	
*返回值:0： 成功
*       other：失败
*
**************************************************************************************/															   
u8 SD_WriteMultiBlock(u32 sector, const u8 *data, u8 count)
{
	u8 r1;
	u16 i;	 		 	 
	if(SD_Type != SD_TYPE_V2HC)sector = sector<<9;//如果不是SDHC，给定的是sector地址，将其转换成byte地址  
	if(SD_Type != SD_TYPE_MMC) r1 = SD_SendCommand(ACMD23, count, 0x00);//如果目标卡不是MMC卡，启用ACMD23指令使能预擦除   
	r1 = SD_SendCommand(CMD25, sector, 0x00);//发多块写入指令
	if(r1 != 0x00)return r1;  //应答不正确，直接返回	 
	Clr_SD_CS;//开始准备数据传输   
	SPIx_ReadWriteByte(0xff);//先放3个空数据，等待SD卡准备好
	SPIx_ReadWriteByte(0xff);   
	//--------下面是N个sector写入的循环部分
	do
	{
		//放起始令牌0xFC 表明是多块写入
		SPIx_ReadWriteByte(0xFC);	  
		//放一个sector的数据
		for(i=0;i<512;i++)
		{
			SPIx_ReadWriteByte(*data++);
		}
		//发2个Byte的dummy CRC
		SPIx_ReadWriteByte(0xff);
		SPIx_ReadWriteByte(0xff);
		
		//等待SD卡应答
		r1 = SPIx_ReadWriteByte(0xff);
		if((r1&0x1F)!=0x05)
		{
			Set_SD_CS;    //如果应答为报错，则带错误代码直接退出
			return r1;
		}		   
		//等待SD卡写入完成
		if(SD_WaitDataReady()==1)
		{
			Set_SD_CS;    //等待SD卡写入完成超时，直接退出报错
			return 1;
		}	   
	}while(--count);//本sector数据传输完成  
	//发结束传输令牌0xFD
	r1 = SPIx_ReadWriteByte(0xFD);
	if(r1==0x00)
	{
		count =  0xfe;
	}		   
	if(SD_WaitDataReady()) //等待准备好
	{
		Set_SD_CS;
		return 1;  
	}
	//写入完成，片选置1
	Set_SD_CS;
	SPIx_ReadWriteByte(0xff);  
	return count;   //返回count值，如果写完则count=0，否则count=1
}
						  					  
/************************************************************************************
*函数名:SD_Read_Bytes
*函数说明:在指定扇区,从offset开始读出bytes个字节
*输入参数:sector 扇区地址（sector值，非物理地址）
*         *buf     数据存储地址（大小<=512byte）
*         bytes   要读出的字节数
*返回值:0： 成功
*       other：失败
*
**************************************************************************************/															   
u8 SD_Read_Bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes)
{
	u8 r1;u16 i=0;  
	r1=SD_SendCommand(CMD17,address<<9,0);//发送读扇区命令      
	if(r1)return r1;  //应答不正确，直接返回
	Clr_SD_CS;//选中SD卡
	if(SD_GetResponse(0xFE))//等待SD卡发回数据起始令牌0xFE
	{
		Set_SD_CS; //关闭SD卡
		return 1;//读取失败
	}	 
	for(i=0;i<offset;i++)SPIx_ReadWriteByte(0xff);//跳过offset位 
	for(;i<offset+bytes;i++)*buf++=SPIx_ReadWriteByte(0xff);//读取有用数据	
	for(;i<512;i++) SPIx_ReadWriteByte(0xff); 	 //读出剩余字节
	SPIx_ReadWriteByte(0xff);//发送伪CRC码
	SPIx_ReadWriteByte(0xff);  
	Set_SD_CS;//关闭SD卡
	return 0;
}
/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/













