 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� stm322xg_eval.i2c_ee.c
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
/* ͷ�ļ� ------------------------------------------------------------------*/
#include "stm322xg_eval_i2c_ee.h"

/******ȫ�ֱ�������****************************************************/
__IO uint16_t  sEEAddress = 0;   
__IO uint32_t  sEETimeout = sEE_LONG_TIMEOUT;   
__IO uint16_t* sEEDataReadPointer;   
__IO uint8_t*  sEEDataWritePointer;  
__IO uint8_t   sEEDataNum;

/***************************************************
*������:sEE_DeInit
*�������:��
*����ֵ:��
*����˵��:��
*
****************************************************/
void sEE_DeInit(void)
{
  sEE_LowLevel_DeInit(); 
}

/***************************************************
*������:sEE_Init
*�������:��
*����ֵ:��
*����˵��:eeprom��ʼ��
*
****************************************************/
void sEE_Init(void)
{ 
  I2C_InitTypeDef  I2C_InitStructure;
  
  sEE_LowLevel_Init();
  
  /* I2C��ʼ������ */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;  
  /* ʹ��I2C */
  I2C_Cmd(sEE_I2C, ENABLE);
  /* ������д��I2C�Ĵ��� */
  I2C_Init(sEE_I2C, &I2C_InitStructure);
  /* ʹ��I2C��DMA */
  I2C_DMACmd(sEE_I2C, ENABLE);
  sEEAddress = 0xA0;
}

/***************************************************
*������:sEE_ReadBuffer
*�������:
*		  pBuffer:���ݻ�����
*		  ReadAddr:eeprom��ַ
*		  NumByteToRead:��ȡ�ֽ���
*����ֵ:�����ɹ�����sEE_OK
*����˵��:��eeprom��ReadAddr��ʼ��ȡNumByteToRead���ֽ�
*		  ��������pBuffer�У�����֧�ֶ����ͺ�eeprom 
****************************************************/
uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead)
{  
  /*����ָ�룬��ָ������DMA*/
  sEEDataReadPointer = NumByteToRead;
  
  /*����æ */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /*������ʼ�ź�*/
  I2C_GenerateSTART(sEE_I2C, ENABLE);
  
  /*��� EV5 λ����� */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /*����eeprom�Ļ���ַ */
  I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);

  /*��� EV6 λ����� */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  } 

#if  defined sEE_M24C08 || defined sEE_AT24C02 
  
  /*����eeprom���ڲ��ռ��ַ���ж�д */
  I2C_SendData(sEE_I2C, ReadAddr);  

#elif defined (sEE_M24C64_32)

  /*����eeprom���ڲ��ռ��ַ���ж�д */
  I2C_SendData(sEE_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));    

  /*��� EV8 λ ����� */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }

  /*����eeprom���ڲ��ռ��ַ���ж�д */
  I2C_SendData(sEE_I2C, (uint8_t)(ReadAddr & 0x00FF));    
  
#endif /*!< sEE_M24C08 */

  /*!< ��� EV8 λ ����� */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF) == RESET)
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /*�ڶ��η�����ʼ�ź� */  
  I2C_GenerateSTART(sEE_I2C, ENABLE);
  
  /*��� EV5 λ����� */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  } 
  
  /*����eeprom�Ļ���ַ����ģʽ */
  I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Receiver);  
  
  /* �����ȡ�����ֽ���Ϊ1��������DMA */
  /* ��ѯ����һ���ֽ�����-*/
  if ((uint16_t)(*NumByteToRead) < 2)
  {
    /*�ȴ�ADDR��λ */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_ADDR) == RESET)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }     
    
    /*ȡ��Ӧ���� */
    I2C_AcknowledgeConfig(sEE_I2C, DISABLE);   
    
    /* ͨ����SR1��SR2�����ADDR�Ĵ��� */
    (void)sEE_I2C->SR2;
    
    /*���ͽ����ź�*/
    I2C_GenerateSTOP(sEE_I2C, ENABLE);
    
    /* �ȴ���������*/
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_RXNE) == RESET)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }
    
    /*��eeprom����һ���ֽ����ݵ��������� */
    *pBuffer = I2C_ReceiveData(sEE_I2C);
    
    /*Ҫ��ȡ���ֽ�����һ */
    (uint16_t)(*NumByteToRead)--;        
    
    /*�ȴ�ȷ��ֹͣλ����� */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(sEE_I2C->CR1 & I2C_CR1_STOP)
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }  
    
    /*ʹ��Ӧ����ƣ������µĽ��� */
    I2C_AcknowledgeConfig(sEE_I2C, ENABLE);    
  }
  else/* �������ݴ���һ���Լ�ʹ��DMA��ʽ-----------------*/
  {
    /*���EV6 λ����� */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }  
    
    /* ����DMA����ͨ���Ļ�������ַ����С */
    sEE_LowLevel_DMAConfig((uint32_t)pBuffer, (uint16_t)(*NumByteToRead), sEE_DIRECTION_RX);
    
    /* ֪ͨDMA��һ�������ź������ݴ��������ź� */
    I2C_DMALastTransferCmd(sEE_I2C, ENABLE); 
    
    /* ʹ��DMA�Ľ��������� */
    DMA_Cmd(sEE_I2C_DMA_STREAM_RX, ENABLE);    
  }
  
  /* ��������ɹ�����sEE_OK */
  return sEE_OK;
}

/***************************************************
*������:sEE_WritePage
*�������:
*		  pBuffer:���ݻ�����
*		  ReadAddr:eeprom��ַ
*		  NumByteToRead:д���ֽ���
*����ֵ:
*		 uint32_t�������ɹ�����sEE_OK
*����˵��:��pBuffer�е�NumByteToWrite���ֽ�д��
*		  ��eeprom��WriteAddr��ַ��
****************************************************/
uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite)
{ 
   /*���ô�д�����ݻ������ĵ�ַ��DMA���õ�*/
  sEEDataWritePointer = NumByteToWrite;  
  
  /*�������æ */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /*������ʼ�ź� */
  I2C_GenerateSTART(sEE_I2C, ENABLE);
  
  /*��� EV5λ����� */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }
  
  /*����eeprom�Ļ���ַ */
  sEETimeout = sEE_FLAG_TIMEOUT;
  I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);

  /*��� EV6 λ����� */
  sEETimeout = sEE_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }

#if defined sEE_M24C08 || defined sEE_AT24C02
  
  /*����eeprom���ڲ���ַ��ֻ��һ���ֽڵĵ�ַ */
  I2C_SendData(sEE_I2C, WriteAddr);
  
#elif defined(sEE_M24C64_32)
  
  /*����eeprom���ڲ���ַ ��λ�ֽڵ�ַ */
  I2C_SendData(sEE_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /*��� EV8 λ����� */
  sEETimeout = sEE_FLAG_TIMEOUT;  
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }  
  
  /*����eeprom���ڲ���ַ ��λ�ֽڵ�ַ */
  I2C_SendData(sEE_I2C, (uint8_t)(WriteAddr & 0x00FF));
  
#endif /*!< sEE_M24C08 */  
  
  /*!< ���EV8 λ����� */
  sEETimeout = sEE_FLAG_TIMEOUT; 
  while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }  
  
  /*���÷���DMA�����ݵ�ַָ�룬���ݴ�С�� */
  sEE_LowLevel_DMAConfig((uint32_t)pBuffer, (uint8_t)(*NumByteToWrite), sEE_DIRECTION_TX);
  
  /* ʹ�ܷ���DMA������ */
  DMA_Cmd(sEE_I2C_DMA_STREAM_TX, ENABLE);
  
  /* ��������ɹ�������sEE_OK */
  return sEE_OK;
}

/***************************************************
*������:sEE_WriteBuffer
*�������:
*		  pBuffer:���ݻ�����
*		  ReadAddr:eepromд���ڲ���ַ
*		  NumByteToWrite:д�������ֽ���
*����ֵ:��
*����˵��:��pBuffer�е�NumByteToWrite���ֽ�д��
*		  ��eeprom��WriteAddr��ַ��
****************************************************/
void sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, count = 0;
  uint16_t Addr = 0;

  Addr = WriteAddr % sEE_PAGESIZE;
  count = sEE_PAGESIZE - Addr;
  NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
  NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
 
  /*�����ַ��ҳ����  */
  if(Addr == 0) 
  {
    /*���д�������С��eepromһҳ�Ĵ�С */
    if(NumOfPage == 0) 
    {
      /* �����ݴ�С���� */
      sEEDataNum = NumOfSingle;

      /* ��ʼд���� */
      sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));

      /* �ȴ�DMA������� */
      sEETimeout = sEE_LONG_TIMEOUT;
      while (sEEDataNum > 0)
      {
        if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
      }
      sEE_WaitEepromStandbyState();
    }
    /*���д������ݴ���eepromһҳ�Ĵ�С */
    else  
    {
      while(NumOfPage--)
      {
        /* �����ݴ�С���� */
        sEEDataNum = sEE_PAGESIZE;        
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));
		 
        /* �ȴ�DMA������� */
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
        /* ����Ҫд�����ݴ�С */
        sEEDataNum = NumOfSingle;          
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));

        /* �ȴ�DMA������� */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }    
        sEE_WaitEepromStandbyState();
      }
    }
  }
  /*�����ַ����ҳ���� */
  else 
  {
    /*���д�������С��һҳ�Ĵ�С */
    if(NumOfPage== 0) 
    {
      /*������ݴ��ڱ�ҳʣ��ռ�Ĵ�С*/
      if (NumByteToWrite > count)
      {
        /* ����Ҫд���ݵĴ�С */
        sEEDataNum = count;  
		      
        /*����ҳʣ��ռ�д�� */
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));

        /*�ȴ�DMA�������*/
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }          
        sEE_WaitEepromStandbyState();      
        
        /* ����д�����ݵĴ�С */
        sEEDataNum = (NumByteToWrite - count);
		         
        /*��ʣ������д������ҳ�ռ� */
        sEE_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint8_t*)(&sEEDataNum));

        /* �ȴ�DMA������� */
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }     
        sEE_WaitEepromStandbyState();        
      }      
      else      
      {
        /* ����д�����ݵĴ�С*/
        sEEDataNum = NumOfSingle;         
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));

        /* �ȴ�DMA�������*/
        sEETimeout = sEE_LONG_TIMEOUT;
        while (sEEDataNum > 0)
        {
          if((sEETimeout--) == 0) {sEE_TIMEOUT_UserCallback(); return;};
        }          
        sEE_WaitEepromStandbyState();        
      }     
    }
    /*���д�����ݴ���ҳ�ռ� */
    else
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
      NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
      
      if(count != 0)
      {  
        /* ����д�����ݵĴ�С*/
        sEEDataNum = count;         
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));

        /*�ȴ�DMA������� */
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
        /*����д�����ݵĴ�С */
        sEEDataNum = sEE_PAGESIZE;          
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum));

        /* �ȴ�DMA������� */
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
        /*����Ҫд������ݴ�С*/
        sEEDataNum = NumOfSingle;           
        sEE_WritePage(pBuffer, WriteAddr, (uint8_t*)(&sEEDataNum)); 

        /* �ȴ�DMA�������*/
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
*������:sEE_WaitEepromStandbyState
*�������:
*		��
*����ֵ:
*	  uint32_t:�����ɹ�����sEE_OK	
*����˵��:�ȴ�eeprom����ϴβ�����
*		  
****************************************************/
uint32_t sEE_WaitEepromStandbyState(void)      
{
  __IO uint16_t tmpSR1 = 0;
  __IO uint32_t sEETrials = 0;

  /*����æ */
  sEETimeout = sEE_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BUSY))
  {
    if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
  }

  /*ѭ��ֱ���ӻ�Ӧ���������Դ�������*/
  while (1)
  {
    /*������ʼ�ź� */
    I2C_GenerateSTART(sEE_I2C, ENABLE);

    /*��� EV5 λ����� */
    sEETimeout = sEE_FLAG_TIMEOUT;
    while(!I2C_CheckEvent(sEE_I2C, I2C_EVENT_MASTER_MODE_SELECT))
    {
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }    

    /*����eeprom�Ļ���ַ */
    I2C_Send7bitAddress(sEE_I2C, sEEAddress, I2C_Direction_Transmitter);
    
    /* �ȴ�ADDR��λ (�ӻ�Ӧ��) */
    sEETimeout = sEE_LONG_TIMEOUT;
    do
    {     
      /* ��ȡ��ǰ�Ĵ��� SR1��ֵ */
      tmpSR1 = sEE_I2C->SR1;
      
      /*��ʱ�˳�*/
      if((sEETimeout--) == 0) return sEE_TIMEOUT_UserCallback();
    }
	   /*�ȴ���ַȷ�ϻ���AFλ��λ*/
    while((tmpSR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);
     
    /* ���ADDR�Ƿ���λ */
    if (tmpSR1 & I2C_SR1_ADDR)
    {
	  /*ͨ����ȡSR1��SR2�Ĵ��������ADDR��־*/
      (void)sEE_I2C->SR2;
      
      /*���ͽ����ź�*/    
      I2C_GenerateSTOP(sEE_I2C, ENABLE);
        
      /* �˳�*/
      return sEE_OK;
    }
    else
    {
      /*���AF ��־*/
      I2C_ClearFlag(sEE_I2C, I2C_FLAG_AF);                  
    }
    
    /* ����Ƿ񵽴�����Դ���*/
    if (sEETrials++ == sEE_MAX_TRIALS_NUMBER)
    {
      /* �������Դ�������˳� */
      return sEE_TIMEOUT_UserCallback();
    }
  }
}

/***************************************************
*������:sEE_I2C_DMA_TX_IRQHandler
*�������:
*		��
*����ֵ:
*	    ��	
*����˵��:��DMA�����жϷ����ӳ���
*		  
****************************************************/
void sEE_I2C_DMA_TX_IRQHandler(void)
{
  /* ���DMA�Ƿ������ */
  if(DMA_GetFlagStatus(sEE_I2C_DMA_STREAM_TX, sEE_TX_DMA_FLAG_TCIF) != RESET)
  {  
    
	/*�ر�DMA �������������TC��־*/ 
    DMA_Cmd(sEE_I2C_DMA_STREAM_TX, DISABLE);
    DMA_ClearFlag(sEE_I2C_DMA_STREAM_TX, sEE_TX_DMA_FLAG_TCIF);

    /*�ȴ��������ݷ��͵����������� */
    sEETimeout = sEE_LONG_TIMEOUT;
    while(!I2C_GetFlagStatus(sEE_I2C, I2C_FLAG_BTF))
    {
      if((sEETimeout--) == 0) sEE_TIMEOUT_UserCallback();
    }
    
    /*����ֹͣ�ź� */
    I2C_GenerateSTOP(sEE_I2C, ENABLE);
    
    *sEEDataWritePointer = 0;  
  }
}
/************************************************************************************
*������:sEE_I2C_DMA_RX_IRQHandler
*����˵��: DMA�����жϷ����ӳ���
*�������:
*		  ��
*����ֵ:
*		��
*
**************************************************************************************/
void sEE_I2C_DMA_RX_IRQHandler(void)
{
  /* ���DMA�Ƿ������ */
  if(DMA_GetFlagStatus(sEE_I2C_DMA_STREAM_RX, sEE_RX_DMA_FLAG_TCIF) != RESET)
  {      
    /*���ͽ����ź�*/
    I2C_GenerateSTOP(sEE_I2C, ENABLE);    
    
	/*�ر�DMA�����������������TC��־*/ 
    DMA_Cmd(sEE_I2C_DMA_STREAM_RX, DISABLE);
    DMA_ClearFlag(sEE_I2C_DMA_STREAM_RX, sEE_RX_DMA_FLAG_TCIF);
    
    *sEEDataReadPointer = 0;
  }
}

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/************************************************************************************
*������:sEE_TIMEOUT_UserCallback
*����˵��:��ʱ���ú���
*�������:��
*
*����ֵ:��
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
