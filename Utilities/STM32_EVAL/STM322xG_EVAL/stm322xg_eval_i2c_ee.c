 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： stm322xg_eval.i2c_ee.c
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
/* 头文件 ------------------------------------------------------------------*/
#include "stm322xg_eval_i2c_ee.h"

/******全局变量定义****************************************************/
__IO uint16_t  sEEAddress = 0;   
__IO uint32_t  sEETimeout = sEE_LONG_TIMEOUT;   
__IO uint16_t* sEEDataReadPointer;   
__IO uint8_t*  sEEDataWritePointer;  
__IO uint8_t   sEEDataNum;

/***************************************************
*函数名:sEE_DeInit
*输入参数:无
*返回值:无
*函数说明:无
*
****************************************************/
void sEE_DeInit(void)
{
  sEE_LowLevel_DeInit(); 
}

/***************************************************
*函数名:sEE_Init
*输入参数:无
*返回值:无
*函数说明:eeprom初始化
*
****************************************************/
void sEE_Init(void)
{ 
  I2C_InitTypeDef  I2C_InitStructure;
  
  sEE_LowLevel_Init();
  
  /* I2C初始化配置 */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;  
  /* 使能I2C */
  I2C_Cmd(sEE_I2C, ENABLE);
  /* 将配置写入I2C寄存器 */
  I2C_Init(sEE_I2C, &I2C_InitStructure);
  /* 使能I2C的DMA */
  I2C_DMACmd(sEE_I2C, ENABLE);
  sEEAddress = 0xA0;
}

/***************************************************
*函数名:sEE_ReadBuffer
*输入参数:
*		  pBuffer:数据缓存区
*		  ReadAddr:eeprom地址
*		  NumByteToRead:读取字节数
*返回值:操作成功返回sEE_OK
*函数说明:从eeprom的ReadAddr开始读取NumByteToRead个字节
*		  到缓冲区pBuffer中，可以支持多种型号eeprom 
****************************************************/
uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead)
{  
  /*设置指针，该指针用于DMA*/
  sEEDataReadPointer = NumByteToRead;
  
  /*总线忙 */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /*发送起始信号*/
  I2C_GenerateSTART(sEE_I2C, ENABLE);
  
  /*检测 EV5 位并清除 */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /*发送eeprom的基地址 */
  I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);

  /*检测 EV6 位并清除 */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  } 

#if  defined sEE_M24C08 || defined sEE_AT24C02 
  
  /*发送eeprom的内部空间地址进行读写 */
  I2C_SendData(sEE_I2C, ReadAddr);  

#elif defined (sEE_M24C64_32)

  /*发送eeprom的内部空间地址进行读写 */
  I2C_SendData(sEE_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));    

  /*检测 EV8 位 并清除 */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }

  /*发送eeprom的内部空间地址进行读写 */
  I2C_SendData(sEE_I2C, (uint8_t)(ReadAddr & 0x00FF));    
  
#endif /*!< sEE_M24C08 */

  /*!< 检测 EV8 位 并清除 */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF) == RESET)
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /*第二次发送起始信号 */  
  I2C_GenerateSTART(sEE_I2C, ENABLE);
  
  /*检测 EV5 位并清除 */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  } 
  
  /*发送eeprom的基地址，读模式 */
  I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Receiver);  
  
  /* 如果读取数据字节数为1，不启用DMA */
  /* 轮询接收一个字节数据-*/
  if ((uint16_t)(*NumByteToRead) < 2)
  {
    /*等待ADDR置位 */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_ADDR) == RESET)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }     
    
    /*取消应答功能 */
    I2C_AcknowledgeConfig(sEE_I2C, DISABLE);   
    
    /* 通过读SR1和SR2来清除ADDR寄存器 */
    (void)sEE_I2C->SR2;
    
    /*发送结束信号*/
    I2C_GenerateSTOP(sEE_I2C, ENABLE);
    
    /* 等待接收数据*/
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_RXNE) == RESET)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }
    
    /*从eeprom接收一个字节数据到缓冲区中 */
    *pBuffer = I2C_ReceiveData(sEE_I2C);
    
    /*要读取的字节数减一 */
    (uint16_t)(*NumByteToRead)--;        
    
    /*等待确定停止位被清除 */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(sEE_I2C->CR1 & I2C_CR1_STOP)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }  
    
    /*使能应答机制，开启新的接收 */
    I2C_AcknowledgeConfig(sEE_I2C, ENABLE);    
  }
  else/* 接收数据大于一个自己使用DMA方式-----------------*/
  {
    /*检测EV6 位并清除 */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }  
    
    /* 配置DMA接收通道的缓冲区地址，大小 */
    sEE_LowLevel_DMAConfig((uint32_t)pBuffer, (uint16_t)(*NumByteToRead), sEE_DIRECTION_RX);
    
    /* 通知DMA下一个结束信号是数据传输的完成信号 */
    I2C_DMALastTransferCmd(sEE_I2C, ENABLE); 
    
    /* 使能DMA的接收数据流 */
    DMA_Cmd(sEE_I2C_DMA_STREAM_RX, ENABLE);    
  }
  
  /* 如果操作成功返回sEE_OK */
  return sEE_OK;
}

/***************************************************
*函数名:sEE_WritePage
*输入参数:
*		  pBuffer:数据缓存区
*		  ReadAddr:eeprom地址
*		  NumByteToRead:写入字节数
*返回值:
*		 uint32_t：操作成功返回sEE_OK
*函数说明:将pBuffer中的NumByteToWrite个字节写入
*		  到eeprom的WriteAddr地址下
****************************************************/
uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite)
{ 
   /*设置待写入数据缓冲区的地址，DMA中用到*/
  sEEDataWritePointer = NumByteToWrite;  
  
  /*如果总线忙 */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /*发送起始信号 */
  I2C_GenerateSTART(sEE_I2C, ENABLE);
  
  /*检测 EV5位并清除 */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /*发送eeprom的基地址 */
  sEETimeout = sEE_FLAG_TIMEOUT;
  I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);

  /*检测 EV6 位并清除 */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }

#if defined sEE_M24C08 || defined sEE_AT24C02
  
  /*发送eeprom的内部地址，只有一个字节的地址 */
  I2C_SendData(sEE_I2C, WriteAddr);
  
#elif defined(sEE_M24C64_32)
  
  /*发送eeprom的内部地址 高位字节地址 */
  I2C_SendData(sEE_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /*检测 EV8 位并清除 */
  sEETimeout = sEE_FLAG_TIMEOUT;  
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }  
  
  /*发送eeprom的内部地址 低位字节地址 */
  I2C_SendData(sEE_I2C, (uint8_t)(WriteAddr & 0x00FF));
  
#endif /*!< sEE_M24C08 */  
  
  /*!< 检测EV8 位并清除 */
  sEETimeout = sEE_FLAG_TIMEOUT; 
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }  
  
  /*配置发送DMA的数据地址指针，数据大小等 */
  sEE_LowLevel_DMAConfig((uint32_t)pBuffer, (uint8_t)(*NumByteToWrite), sEE_DIRECTION_TX);
  
  /* 使能发送DMA数据流 */
  DMA_Cmd(sEE_I2C_DMA_STREAM_TX, ENABLE);
  
  /* 如果操作成功，返回sEE_OK */
  return sEE_OK;
}

/***************************************************
*函数名:sEE_WriteBuffer
*输入参数:
*		  pBuffer:数据缓存区
*		  ReadAddr:eeprom写入内部地址
*		  NumByteToWrite:写入数据字节数
*返回值:无
*函数说明:将pBuffer中的NumByteToWrite个字节写入
*		  到eeprom的WriteAddr地址下
****************************************************/
void sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, count = 0;
  uint16_t Addr = 0;

  Addr = WriteAddr % sEE_PAGESIZE;
  count = sEE_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
  NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
 
  /*如果地址是页对齐  */
  if(Addr == 0) 
  {
    /*如果写入的数据小于eeprom一页的大小 */
    if(NumOfPage == 0) 
    {
      /* 将数据大小保存 */
      sEEDataNum = NumOfSingle;

      /* 开始写数据 */
      sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));

      /* 等待DMA传输完成 */
      sEETimeout = sEE_LONG_TIMEOUT;
      while (sEEDataNum > 0)
      {
        if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
      }
      sEE_WaitEepromStandbyState();
    }
    /*如果写入的数据大于eeprom一页的大小 */
    else  
    {
      while(NumOfPage--)
      {
        /* 将数据大小保存 */
        sEEDataNum = sEE_PAGESIZE;        
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
		 
        /* 等待DMA传输完成 */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }      
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;
      }

      if(NumOfSingle!=0)
      {
        /* 保存要写的数据大小 */
        sEEDataNum = NumOfSingle;          
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));

        /* 等待DMA传输完成 */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }    
        sEE_WaitEepromStandbyState();
      }
    }
  }
  /*如果地址不是页对齐 */
  else 
  {
    /*如果写入的数据小于一页的大小 */
    if(NumOfPage== 0) 
    {
      /*如果数据大于本页剩余空间的大小*/
      if (NumByteToWrite > count)
      {
        /* 保存要写数据的大小 */
        sEEDataNum = count;  
		      
        /*将本页剩余空间写满 */
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));

        /*等待DMA传输完成*/
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }          
        sEE_WaitEepromStandbyState();      
        
        /* 保存写入数据的大小 */
        sEEDataNum = (NumByteToWrite - count);
		         
        /*将剩余数据写入后面的页空间 */
        sEE_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint8_t*)(&sEEDataNum));

        /* 等待DMA传输完成 */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }     
        sEE_WaitEepromStandbyState();        
      }      
      else      
      {
        /* 保存写入数据的大小*/
        sEEDataNum = NumOfSingle;         
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));

        /* 等待DMA传输完成*/
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }          
        sEE_WaitEepromStandbyState();        
      }     
    }
    /*如果写入数据大于页空间 */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
      NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
      
      if(count != 0)
      {  
        /* 保存写入数据的大小*/
        sEEDataNum = count;         
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));

        /*等待DMA传输完成 */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }     
        sEE_WaitEepromStandbyState();
        WriteAddr += count;
        pBuffer += count;
      } 
      
      while(NumOfPage--)
      {
        /*保存写入数据的大小 */
        sEEDataNum = sEE_PAGESIZE;          
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));

        /* 等待DMA传输完成 */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }        
        sEE_WaitEepromStandbyState();
        WriteAddr +=  sEE_PAGESIZE;
        pBuffer += sEE_PAGESIZE;  
      }
      if(NumOfSingle != 0)
      {
        /*保存要写入的数据大小*/
        sEEDataNum = NumOfSingle;           
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum)); 

        /* 等待DMA传输完成*/
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }         
        sEE_WaitEepromStandbyState();
      }
    }
  }  
}

/***************************************************
*函数名:sEE_WaitEepromStandbyState
*输入参数:
*		无
*返回值:
*	  uint32_t:操作成功返回sEE_OK	
*函数说明:等待eeprom完成上次操作，
*		  
****************************************************/
uint32_t sEE_WaitEepromStandbyState(void)      
{
  __IO uint16_t tmpSR1 = 0;
  __IO uint32_t sEETrials = 0;

  /*总线忙 */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }

  /*循环直到从机应答或者最大尝试次数到达*/
  while (1)
  {
    /*发送起始信号 */
    I2C_GenerateSTART(sEE_I2C, ENABLE);

    /*检测 EV5 位并清除 */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }    

    /*发送eeprom的基地址 */
    I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);
    
    /* 等待ADDR置位 (从机应答) */
    sEETimeout = sEE_LONG_TIMEOUT;
    do
    {     
      /* 获取当前寄存器 SR1的值 */
      tmpSR1 = sEE_I2C->SR1;
      
      /*超时退出*/
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }
	   /*等待地址确认或者AF位置位*/
    while((tmpSR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);
     
    /* 检测ADDR是否置位 */
    if (tmpSR1 & I2C_SR1_ADDR)
    {
	  /*通过读取SR1和SR2寄存器来清除ADDR标志*/
      (void)sEE_I2C->SR2;
      
      /*发送结束信号*/    
      I2C_GenerateSTOP(sEE_I2C, ENABLE);
        
      /* 退出*/
      return sEE_OK;
    }
    else
    {
      /*清除AF 标志*/
      I2C_ClearFlag(sEE_I2C, I2C_FLAG_AF);                  
    }
    
    /* 检测是否到达最大尝试次数*/
    if (sEETrials++ == sEE_MAX_TRIALS_NUMBER)
    {
      /* 如果最大尝试次数到达，退出 */
      return sEE_TIMEOUT_UserCallback();
    }
  }
}

/***************************************************
*函数名:sEE_I2C_DMA_TX_IRQHandler
*输入参数:
*		无
*返回值:
*	    无	
*函数说明:等DMA发送中断服务子程序
*		  
****************************************************/
void sEE_I2C_DMA_TX_IRQHandler(void)
{
  /* 检测DMA是否传输完成 */
  if(DMA_GetFlagStatus(sEE_I2C_DMA_STREAM_TX, sEE_TX_DMA_FLAG_TCIF) != RESET)
  {  
    
	/*关闭DMA 发送流，并清除TC标志*/ 
    DMA_Cmd(sEE_I2C_DMA_STREAM_TX, DISABLE);
    DMA_ClearFlag(sEE_I2C_DMA_STREAM_TX, sEE_TX_DMA_FLAG_TCIF);

    /*等待所有数据发送到数据总线上 */
    sEETimeout = sEE_LONG_TIMEOUT;
    while(!I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF))
    {
      if((sEETimeout--) == 0) sEE_TIMEOUT_UserCallback();
    }
    
    /*发送停止信号 */
    I2C_GenerateSTOP(sEE_I2C, ENABLE);
    
    *sEEDataWritePointer = 0;  
  }
}
/************************************************************************************
*函数名:sEE_I2C_DMA_RX_IRQHandler
*函数说明: DMA接收中断服务子程序
*输入参数:
*		  无
*返回值:
*		无
*
**************************************************************************************/
void sEE_I2C_DMA_RX_IRQHandler(void)
{
  /* 检测DMA是否传输完成 */
  if(DMA_GetFlagStatus(sEE_I2C_DMA_STREAM_RX, sEE_RX_DMA_FLAG_TCIF) != RESET)
  {      
    /*发送结束信号*/
    I2C_GenerateSTOP(sEE_I2C, ENABLE);    
    
	/*关闭DMA接收数据流，并清除TC标志*/ 
    DMA_Cmd(sEE_I2C_DMA_STREAM_RX, DISABLE);
    DMA_ClearFlag(sEE_I2C_DMA_STREAM_RX, sEE_RX_DMA_FLAG_TCIF);
    
    *sEEDataReadPointer = 0;
  }
}

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/************************************************************************************
*函数名:sEE_TIMEOUT_UserCallback
*函数说明:超时调用函数
*输入参数:无
*
*返回值:无
*
*
**************************************************************************************/
uint32_t sEE_TIMEOUT_UserCallback(void)
{
 
  while (1)
  {   
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */  

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
