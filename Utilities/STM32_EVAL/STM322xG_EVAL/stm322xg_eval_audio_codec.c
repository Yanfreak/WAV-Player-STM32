 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� stm322xg_eval_audio_codec.c
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
#include "stm322xg_eval_audio_codec.h"
#include "stm322xg_eval_keypad.h"
/**********�궨��*********************************/
/* I2S���üĴ������� */
#define I2S_ENABLE_MASK                 0x0400

/* ��λ��ʱ */
#define CODEC_RESET_DELAY               0x4FFF

/* ��Ƶ��׼*/
#ifdef I2S_STANDARD_PHILLIPS
 #define  CODEC_STANDARD                0x04
 #define I2S_STANDARD                   I2S_Standard_Phillips         
#elif defined(I2S_STANDARD_MSB)
 #define  CODEC_STANDARD                0x00
 #define I2S_STANDARD                   I2S_Standard_MSB    
#elif defined(I2S_STANDARD_LSB)
 #define  CODEC_STANDARD                0x08
 #define I2S_STANDARD                   I2S_Standard_LSB    
#else 
 #error "Error: No audio communication standard selected !"
#endif 
/*��ƵоƬ����ַ*/
#define CODEC_ADDRESS                   0x94  /* b00100111 */
#define SalveAddress  0x1A<<1
/**************ȫ�ֱ�������*****************************/
static DMA_InitTypeDef DMA_InitStructure;   /*DMA��ʼ���ṹ��*/
DMA_InitTypeDef AUDIO_MAL_DMA_InitStructure;

uint32_t AudioTotalSize = 0xFFFF; /* �ļ���С */
uint32_t AudioRemSize   = 0xFFFF; /* ʣ����Ƶ�ļ���С */
uint16_t *CurrentPos;             /* ��Ƶ�ļ���ǰλ�� */

__IO uint32_t  CODECTimeout = CODEC_LONG_TIMEOUT;   
__IO uint8_t OutputDev = 0;
extern uint32_t volume;
/*****************��������**************************/
static uint32_t Codec_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);
static uint32_t Codec_DeInit(void);
static uint32_t Codec_Play(void);
static uint32_t Codec_PauseResume(uint32_t Cmd);
static uint32_t Codec_Stop(uint32_t Cmd);
static uint32_t Codec_VolumeCtrl(uint8_t Volume);
static uint32_t Codec_Mute(uint32_t Cmd);
static void     Codec_CtrlInterface_Init(void);
static void     Codec_CtrlInterface_DeInit(void);
static void     Codec_AudioInterface_Init(uint32_t AudioFreq);
static void     Codec_AudioInterface_DeInit(void);
static void     Codec_Reset(void);
static uint32_t Codec_WriteRegister(uint8_t RegisterAddr, uint8_t RegisterValue);
static uint32_t Codec_ReadRegister(uint8_t RegisterAddr);
static void     Codec_GPIO_Init(void);
static void     Codec_GPIO_DeInit(void);
static void     Delay(__IO uint32_t nCount);
static void     Audio_MAL_Init(void);
static void     Audio_MAL_DeInit(void);
static void     Audio_MAL_Play(uint32_t Addr, uint32_t Size);
static void     Audio_MAL_PauseResume(uint32_t Cmd, uint32_t Addr);
static void     Audio_MAL_Stop(void);
extern u8 BufFlag;
/************************************************************************************
*������: EVAL_AUDIO_Init
*����˵��:��Ƶ�����ʼ��
*�������:
*		 OutputDevice������豸��OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO .
*		 Volume��������С
*		 AudioFreq����Ƶ�ļ�������
*����ֵ:
*		0  ��ʼ���ɹ�
*		1����ʼ������
**************************************************************************************/
uint32_t EVAL_AUDIO_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{    
  /* ��Ƶ�����ʼ��*/
  if (Codec_Init(OutputDevice, VOLUME_CONVERT(Volume), AudioFreq) != 0)
  {
    return 1;                
  }
  else
  {    
   /*****DMA��ʼ��**************************************/
    Audio_MAL_Init();
    return 0;
  }
}
/************************************************************************************
*������:EVAL_AUDIO_DeInit
*����˵��:��
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
uint32_t EVAL_AUDIO_DeInit(void)
{ 
  Audio_MAL_DeInit();
   
  Codec_DeInit();  
  
  return 0;
}
/************************************************************************************
*������:EVAL_AUDIO_Play
*����˵��:��ʼ���ڴ��д�����Ƶ����
*�������:
*		 pBuffer��Ƶ���ݵ�ַ��
*		 Size����Ƶ���ݴ�С
*����ֵ:
*		0�������ɹ�
*
**************************************************************************************/
uint32_t EVAL_AUDIO_Play(uint16_t* pBuffer, uint32_t Size)
{
  AudioTotalSize = Size/2; /*������Ƶ���ݴ�С*/

  Codec_Play();	/*��Ƶ���ų���*/
   
  Audio_MAL_Play((uint32_t)pBuffer,Size); /*����DMA*/
  
  AudioRemSize = (Size/2) - DMA_MAX(AudioTotalSize);/*����ʣ���ļ���С*/
  
  CurrentPos = pBuffer + DMA_MAX(AudioTotalSize);/*���µ�ǰ����λ��*/
  
  return 0;
}

/************************************************************************************
*������:EVAL_AUDIO_PauseResume
*����˵��:��ͣ/����ѡ��
*�������:
*		Cmd	��AUDIO_PAUSE����AUDIO_RESUME
*����ֵ:
*		1:����ʧ��
*		0:�����ɹ�
**************************************************************************************/
uint32_t EVAL_AUDIO_PauseResume(uint32_t Cmd)
{    
  if (Codec_PauseResume(Cmd) != 0)
  {
    return 1;
  }
  else
  {
    /* �����м����ͣ����ѡ����*/
    Audio_MAL_PauseResume(Cmd, 0);
    return 0;
  }
}

/************************************************************************************
*������:EVAL_AUDIO_Stop
*����˵��: ֹͣ����
*�������:��
*
*����ֵ:
*	   1������ʧ��
*	   0�������ɹ�
**************************************************************************************/
uint32_t EVAL_AUDIO_Stop(uint32_t Option)
{
  if (Codec_Stop(Option) != 0)
  {
    return 1;
  }
  else
  {
    /* �����м��ֹͣ���� */
    Audio_MAL_Stop();
    
    /* ����ʣ����Ƶ���ݴ�С */
    AudioRemSize = AudioTotalSize;    
    return 0;
  }
}

/************************************************************************************
*������:EVAL_AUDIO_VolumeCtl
*����˵��:��������
*�������:
*		Volume:����ֵ
*����ֵ:��
*
*
**************************************************************************************/
uint32_t EVAL_AUDIO_VolumeCtl(uint8_t Volume)
{
  /* ���õײ��������ƺ���*/
  return (Codec_VolumeCtrl(Volume));
}

/************************************************************************************
*������:EVAL_AUDIO_Mute
*����˵��:��������
*�������:
*		 Cmd:AUDIO_MUTE_ON,AUDIO_MUTE_OFF
*����ֵ:��
*
*
**************************************************************************************/
uint32_t EVAL_AUDIO_Mute(uint32_t Cmd)
{ 
  /* ���õײ㾲������ */
  return (Codec_Mute(Cmd));
}

/************************************************************************************
*������:Audio_MAL_IRQHandler
*����˵��:�жϺ���
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
void Audio_MAL_IRQHandler(void)
{    
#ifndef AUDIO_MAL_MODE_NORMAL
  uint16_t *pAddr = (uint16_t *)CurrentPos;
  uint32_t Size = AudioRemSize;
#endif /* AUDIO_MAL_MODE_NORMAL */
  
#ifdef AUDIO_MAL_DMA_IT_TC_EN
  /* ��������ж�*/
  if (DMA_GetFlagStatus(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_TC) != RESET)
  {     
 #ifdef AUDIO_MAL_MODE_NORMAL
    /* �����Ƶ�ļ��Ƿ񲥷����*/
    if (AudioRemSize > 0)
    {      
      /* �ȴ�DMA�ر� */
      while (DMA_GetCmdStatus(AUDIO_MAL_DMA_STREAM) != DISABLE)
      {}
      
      /* ����жϱ�־ */
      DMA_ClearFlag(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_TC);  
      
      /*�������û�������ַ�����ݴ�С */
      DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t) CurrentPos;
      DMA_InitStructure.DMA_BufferSize = (uint32_t) (DMA_MAX(AudioRemSize));
      
      /* ʹ���²�������DMA*/
      DMA_Init(AUDIO_MAL_DMA_STREAM, &DMA_InitStructure);
      
      /*ʹ��DMA��*/
      DMA_Cmd(AUDIO_MAL_DMA_STREAM, ENABLE);    
      
      /*���µ�ǰ��Ƶ�ļ�ָ�� */
      CurrentPos += DMA_MAX(AudioRemSize);        
      
      /* ����ʣ����Ƶ�ļ���С */
      AudioRemSize -= DMA_MAX(AudioRemSize);  
      BufFlag=1;  
    }
    else
    {
      /* �ر�DMA��*/
      DMA_Cmd(AUDIO_MAL_DMA_STREAM, DISABLE);   
      
      /*����жϱ�־*/
      DMA_ClearFlag(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_TC);       
       
      EVAL_AUDIO_TransferComplete_CallBack((uint32_t)CurrentPos, 0);  
      BufFlag=1;     
    }
    
 #elif defined(AUDIO_MAL_MODE_CIRCULAR) 
//    EVAL_AUDIO_TransferComplete_CallBack(pAddr, Size);    
    
    DMA_ClearFlag(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_TC);
 #endif /* AUDIO_MAL_MODE_NORMAL */  
  }
#endif /* AUDIO_MAL_DMA_IT_TC_EN */

#ifdef AUDIO_MAL_DMA_IT_HT_EN  
  /* ����һ���ж� */
  if (DMA_GetFlagStatus(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_HT) != RESET)
  { 
    EVAL_AUDIO_HalfTransfer_CallBack((uint32_t)pAddr, Size);    
   
    DMA_ClearFlag(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_HT);    
  }
#endif /* AUDIO_MAL_DMA_IT_HT_EN */
  
#ifdef AUDIO_MAL_DMA_IT_TE_EN  
  /* FIFO �����ж� */
  if ((DMA_GetFlagStatus(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_TE) != RESET) || \
     (DMA_GetFlagStatus(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_FE) != RESET) || \
     (DMA_GetFlagStatus(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_DME) != RESET))
    
  {  
    EVAL_AUDIO_Error_CallBack((uint32_t*)&pAddr);    

    DMA_ClearFlag(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_TE | AUDIO_MAL_DMA_FLAG_FE | \
                                        AUDIO_MAL_DMA_FLAG_DME);
  }  
#endif /* AUDIO_MAL_DMA_IT_TE_EN */
}

/************************************************************************************
*������:Codec_Init
*����˵��:��Ƶ�ļ���ʼ��
*�������:
*		OutputDevice������豸ѡ��
*		Volume��ʼ������
*		AudioFreq��������
*����ֵ:��
*
*
**************************************************************************************/
static uint32_t Codec_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{
  uint32_t counter = 0; 

  /* �������GPIO�� */
  Codec_GPIO_Init();      
  I2C_ByteWrite(0x00FF,0x06);  /*�رյ�Դ*/
  I2C_ByteWrite(volume,0x00); /*����������������*/
  I2C_ByteWrite(volume,0x01); /*����������������*/
  I2C_ByteWrite(volume,0x02); /*�������������*/
  I2C_ByteWrite(volume,0x03); /*�Ҷ�����������*/
 // I2C_ByteWrite(0x0124,0x04);  /*ģ��·������*/
  I2C_ByteWrite(0x0004,0x05);  /*����·������*/
  I2C_ByteWrite(0x0012,0x07);	/*����·�����Ʒ�ʽ*/
  I2C_ByteWrite(0x0000,0x08);  /*�����ʿ���*/
  I2C_ByteWrite(0x00c1,0x09);  /*�������*/
  I2C_ByteWrite(0x0007,0x06);  /*�������*/
  /* ����I2S���� */
  Codec_AudioInterface_Init(AudioFreq);  
  return counter;  
}

/************************************************************************************
*������:Codec_DeInit
*����˵��:
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static uint32_t Codec_DeInit(void)
{
  uint32_t counter = 0; 

  Codec_Reset();  
  
  counter += Codec_WriteRegister(0x02, 0x01);    

  Codec_GPIO_DeInit();


  Codec_CtrlInterface_DeInit();
  

  Codec_AudioInterface_DeInit(); 
  

  return counter;  
}

/************************************************************************************
*������:Codec_Play
*����˵��:
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/

static uint32_t Codec_Play(void)
{

  return 0;  
}

/**
  * @brief  Pauses and resumes playing on the audio codec.
  * @param  Cmd: AUDIO_PAUSE (or 0) to pause, AUDIO_RESUME (or any value different
  *         from 0) to resume. 
  * @retval 0 if correct communication, else wrong communication
  */
/************************************************************************************
*������:Codec_PauseResume
*����˵��:��ͣ���߲���
*�������:
*		 Cmd:AUDIO_PAUSE���� AUDIO_RESUME
*����ֵ:��
*
*
**************************************************************************************/

static uint32_t Codec_PauseResume(uint32_t Cmd)
{
//  uint32_t counter = 0;   
  
  if (Cmd == AUDIO_PAUSE)
  { 
  
  }
  else /* AUDIO_RESUME */
  {

  }

  return 0;
}

/************************************************************************************
*������:Codec_Stop
*����˵��:
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static uint32_t Codec_Stop(uint32_t CodecPdwnMode)
{
  uint32_t counter = 0;   

  Codec_Mute(AUDIO_MUTE_ON);
  
  if (CodecPdwnMode == CODEC_PDWN_SW)
  {    

    counter += Codec_WriteRegister(0x02, 0x9F);
  }
  else /* CODEC_PDWN_HW */
  { 
    counter += Codec_WriteRegister(0x02, 0x9F);
    
    Delay(0xFFF);
    
  }
  
  return counter;    
}

/************************************************************************************
*������:Codec_VolumeCtrl
*����˵��:�ײ���������
*�������:
*		Volume����������ֵ 0----0x7f
*����ֵ:��
*
*
**************************************************************************************/
static uint32_t Codec_VolumeCtrl(uint8_t Volume)
{
  uint32_t counter = 0;
  
    counter += I2C_ByteWrite(Volume,0x02); 

    counter += I2C_ByteWrite(Volume,0x02); 
  return counter;  
}

/************************************************************************************
*������:Codec_Mute
*����˵��: ��������
*�������:
*		  Cmd:�Ƿ���
*����ֵ:��
*
*
**************************************************************************************/
static uint32_t Codec_Mute(uint32_t Cmd)
{
  uint32_t counter = 0;  
  
  if (Cmd == AUDIO_MUTE_ON)
  {
    counter += I2C_ByteWrite(0x0100, 0x02);
  }
  else
  {
    counter += I2C_ByteWrite(OutputDev,0x02);
  }
  
  return counter; 
}

/************************************************************************************
*������: Codec_Reset
*����˵��:
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void Codec_Reset(void)
{

}

/************************************************************************************
*������:Codec_SwitchOutput
*����˵��:
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
uint32_t Codec_SwitchOutput(uint8_t Output)
{
  uint8_t counter = 0;
  
  switch (Output) 
  {
    case OUTPUT_DEVICE_SPEAKER:
      counter += Codec_WriteRegister(0x04, 0xFA); 
      OutputDev = 0xFA;
      break;
      
    case OUTPUT_DEVICE_HEADPHONE:
      counter += Codec_WriteRegister(0x04, 0xAF); 
      OutputDev = 0xAF;
      break;
      
    case OUTPUT_DEVICE_BOTH:
      counter += Codec_WriteRegister(0x04, 0xAA); 
      OutputDev = 0xAA;
      break;
      
    case OUTPUT_DEVICE_AUTO:
      counter += Codec_WriteRegister(0x04, 0x05); 
      OutputDev = 0x05;
      break;    
      
    default:
      counter += Codec_WriteRegister(0x04, 0x05); 
      OutputDev = 0x05;
      break;
  }
  
  return counter;
}

/************************************************************************************
*������:Codec_WriteRegister
*����˵��: ��ʵ��û���õ�
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static uint32_t Codec_WriteRegister(uint8_t RegisterAddr, uint8_t RegisterValue)
{
  uint32_t result = 0;

  CODECTimeout = CODEC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BUSY))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }

  I2C_GenerateSTART(CODEC_I2C, ENABLE);

  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Transmitter);

  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }

 
  I2C_SendData(CODEC_I2C, RegisterAddr);


  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
 
  I2C_SendData(CODEC_I2C, RegisterValue);
  
  CODECTimeout = CODEC_LONG_TIMEOUT;
  while(!I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BTF))
  {
    if((CODECTimeout--) == 0) Codec_TIMEOUT_UserCallback();
  }
  

  I2C_GenerateSTOP(CODEC_I2C, ENABLE);  
  
#ifdef VERIFY_WRITTENDATA

  result = (Codec_ReadRegister(RegisterAddr) == RegisterValue)? 0:1;
#endif

  return result;  
}

/************************************************************************************
*������:Codec_ReadRegister
*����˵��: ��ʵ��û���õ�
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static uint32_t Codec_ReadRegister(uint8_t RegisterAddr)
{
  uint32_t result = 0;

  CODECTimeout = CODEC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BUSY))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  I2C_GenerateSTART(CODEC_I2C, ENABLE);

  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Transmitter);


  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }

  I2C_SendData(CODEC_I2C, RegisterAddr);

  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while (I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_BTF) == RESET)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  } 
  I2C_GenerateSTART(CODEC_I2C, ENABLE);
  
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while(!I2C_CheckEvent(CODEC_I2C, I2C_EVENT_MASTER_MODE_SELECT))
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  } 
  
  I2C_Send7bitAddress(CODEC_I2C, CODEC_ADDRESS, I2C_Direction_Receiver);  
  
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_ADDR) == RESET)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }     
  
  I2C_AcknowledgeConfig(CODEC_I2C, DISABLE);   
  
  (void)CODEC_I2C->SR2;
  
  I2C_GenerateSTOP(CODEC_I2C, ENABLE);
  
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_FLAG_RXNE) == RESET)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  result = I2C_ReceiveData(CODEC_I2C);
  
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while(CODEC_I2C->CR1 & I2C_CR1_STOP)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }  
  
  I2C_AcknowledgeConfig(CODEC_I2C, ENABLE);  
  
  I2C_ClearFlag(CODEC_I2C, I2C_FLAG_AF); 
  
  return result;
}

/************************************************************************************
*������:Codec_CtrlInterface_Init
*����˵��:��Ƶ�ӿڳ�ʼ������
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void Codec_CtrlInterface_Init(void)
{
  I2C_InitTypeDef I2C_InitStructure;
  
  /* ʹ��ʱ��*/
  RCC_APB1PeriphClockCmd(CODEC_I2C_CLK, ENABLE);
  
  I2C_DeInit(CODEC_I2C);
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = SalveAddress;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = I2C_SPEED;
  I2C_Cmd(CODEC_I2C, ENABLE);  
  I2C_Init(CODEC_I2C, &I2C_InitStructure);
}

/************************************************************************************
*������:Codec_CtrlInterface_DeInit
*����˵��:
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void Codec_CtrlInterface_DeInit(void)
{

}

/************************************************************************************
*������:Codec_AudioInterface_Init
*����˵��: �ӿڳ�ʼ������
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void Codec_AudioInterface_Init(uint32_t AudioFreq)
{
  I2S_InitTypeDef I2S_InitStructure;

  /* ʹ��ʱ��*/
  RCC_APB1PeriphClockCmd(CODEC_I2S_CLK, ENABLE);

  /*��ʼ��I2S */
  SPI_I2S_DeInit(CODEC_I2S);
  I2S_InitStructure.I2S_AudioFreq = AudioFreq;
  I2S_InitStructure.I2S_Standard = I2S_STANDARD;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
#ifdef CODEC_MCLK_ENABLED
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
#elif defined(CODEC_MCLK_DISABLED)
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
#else
 #error "No selection for the MCLK output has been defined !"
#endif /* CODEC_MCLK_ENABLED */

  I2S_Init(CODEC_I2S, &I2S_InitStructure);

}

/************************************************************************************
*������:Codec_AudioInterface_DeInit
*����˵��:
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void Codec_AudioInterface_DeInit(void)
{
  I2S_Cmd(CODEC_I2S, DISABLE); /*�ر�I2S*/
  
  SPI_I2S_DeInit(CODEC_I2S);
  
  RCC_APB1PeriphClockCmd(CODEC_I2S_CLK, DISABLE); /*�ر�I2Sʱ��*/
}

/************************************************************************************
*������:Codec_GPIO_Init
*����˵��:��ƵI2S�ӿڳ�ʼ��
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void Codec_GPIO_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
  
  /* ʹ��I2Sʱ�� */
  RCC_AHB1PeriphClockCmd(CODEC_I2C_GPIO_CLOCK | CODEC_I2S_GPIO_CLOCK, ENABLE);
  /* I2S�ܽų�ʼ��--*/
  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_WS_PIN | CODEC_I2S_SCK_PIN | CODEC_I2S_SD_PIN; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(CODEC_I2S_GPIO, &GPIO_InitStructure);     
  /* �ܽŸ��� */
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_WS_PINSRC, CODEC_I2S_GPIO_AF);  
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_SCK_PINSRC, CODEC_I2S_GPIO_AF);
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_SD_PINSRC, CODEC_I2S_GPIO_AF);

 #ifdef CODEC_MCLK_ENABLED
  /*  MCK�ܽų�ʼ�� */
  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_MCK_PIN; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(CODEC_I2S_MCK_GPIO, &GPIO_InitStructure);   
  /* ����  */
  GPIO_PinAFConfig(CODEC_I2S_MCK_GPIO, CODEC_I2S_MCK_PINSRC, CODEC_I2S_GPIO_AF); 
 #endif 
}

/************************************************************************************
*������:Codec_GPIO_DeInit
*����˵��: ��
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void Codec_GPIO_DeInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_WS_PIN | CODEC_I2S_SCK_PIN | CODEC_I2S_SD_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(CODEC_I2S_GPIO, &GPIO_InitStructure);     
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_WS_PIN, 0x00);  
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_SCK_PIN, 0x00);
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_SD_PIN, 0x00);  
  
#ifdef CODEC_MCLK_ENABLED

  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_MCK_PIN; 
  GPIO_Init(CODEC_I2S_MCK_GPIO, &GPIO_InitStructure);   

  GPIO_PinAFConfig(CODEC_I2S_MCK_GPIO, CODEC_I2S_MCK_PINSRC, CODEC_I2S_GPIO_AF); 
#endif   
}

/************************************************************************************
*������:Delay
*����˵��:��ʱ����
*�������:��
*		 nCount����ʱ����
*����ֵ:��
*
*
**************************************************************************************/
static void Delay( __IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
}

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/************************************************************************************
*������:Codec_TIMEOUT_UserCallback
*����˵��:
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
uint32_t Codec_TIMEOUT_UserCallback(void)
{

  while (1)
  {   
  }
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */
/************************************************************************************
*������:Audio_MAL_Init
*����˵��:��ƵDMA���ú���
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void Audio_MAL_Init(void)  
{   
#if defined(AUDIO_MAL_DMA_IT_TC_EN) || defined(AUDIO_MAL_DMA_IT_HT_EN) || defined(AUDIO_MAL_DMA_IT_TE_EN)
  NVIC_InitTypeDef NVIC_InitStructure;
#endif
  
  /* ʹ��DMAʱ��*/
  RCC_AHB1PeriphClockCmd(AUDIO_MAL_DMA_CLOCK, ENABLE); 
  
  DMA_Cmd(AUDIO_MAL_DMA_STREAM, DISABLE);
  DMA_DeInit(AUDIO_MAL_DMA_STREAM);
  /*����DMA����*/
  DMA_InitStructure.DMA_Channel = AUDIO_MAL_DMA_CHANNEL;  
  DMA_InitStructure.DMA_PeripheralBaseAddr = CODEC_I2S_ADDRESS;
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)0;      /* ��ַ�ɲ��ź���ָ�� */
  DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  DMA_InitStructure.DMA_BufferSize = (uint32_t)0xFFFE;      /* �ļ���СҲ�ɲ��ź���ָ�� */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = AUDIO_MAL_DMA_PERIPH_DATA_SIZE;
  DMA_InitStructure.DMA_MemoryDataSize = AUDIO_MAL_DMA_MEM_DATA_SIZE; 
#ifdef AUDIO_MAL_MODE_NORMAL
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
#elif defined(AUDIO_MAL_MODE_CIRCULAR)
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
#else
  #error "AUDIO_MAL_MODE_NORMAL or AUDIO_MAL_MODE_CIRCULAR should be selected !!"
#endif /* AUDIO_MAL_MODE_NORMAL */  
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;         
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  
  DMA_Init(AUDIO_MAL_DMA_STREAM, &DMA_InitStructure);  

  /* ʹ��ѡ�õ�DMA�ж�*/
#ifdef AUDIO_MAL_DMA_IT_TC_EN
  DMA_ITConfig(AUDIO_MAL_DMA_STREAM, DMA_IT_TC, ENABLE);
#endif /* AUDIO_MAL_DMA_IT_TC_EN */
#ifdef AUDIO_MAL_DMA_IT_HT_EN
  DMA_ITConfig(AUDIO_MAL_DMA_STREAM, DMA_IT_HT, ENABLE);
#endif /* AUDIO_MAL_DMA_IT_HT_EN */
#ifdef AUDIO_MAL_DMA_IT_TE_EN
  DMA_ITConfig(AUDIO_MAL_DMA_STREAM, DMA_IT_TE | DMA_IT_FE | DMA_IT_DME, ENABLE);
#endif /* AUDIO_MAL_DMA_IT_TE_EN */
  
  /* ʹ��DMA���� */
  SPI_I2S_DMACmd(CODEC_I2S, SPI_I2S_DMAReq_Tx, ENABLE);
  
#if defined(AUDIO_MAL_DMA_IT_TC_EN) || defined(AUDIO_MAL_DMA_IT_HT_EN) || defined(AUDIO_MAL_DMA_IT_TE_EN)
  /* �ж�����*/
  NVIC_InitStructure.NVIC_IRQChannel = AUDIO_MAL_DMA_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EVAL_AUDIO_IRQ_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = EVAL_AUDIO_IRQ_SUBRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif 
}

/************************************************************************************
*������: Audio_MAL_DeInit
*����˵��:��
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void Audio_MAL_DeInit(void)  
{   
#if defined(AUDIO_MAL_DMA_IT_TC_EN) || defined(AUDIO_MAL_DMA_IT_HT_EN) || defined(AUDIO_MAL_DMA_IT_TE_EN)
  NVIC_InitTypeDef NVIC_InitStructure;  
  
  NVIC_InitStructure.NVIC_IRQChannel = AUDIO_MAL_DMA_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EVAL_AUDIO_IRQ_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = EVAL_AUDIO_IRQ_SUBRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);  
#endif 
  

  DMA_Cmd(AUDIO_MAL_DMA_STREAM, DISABLE);
  

  DMA_DeInit(AUDIO_MAL_DMA_STREAM);
  
}

/************************************************************************************
*������: Audio_MAL_Play
*����˵��:
*�������:��
*		   Addr����Ƶ�ļ���ַ
*		   Size����Ƶ�ļ���С
*����ֵ:��
*
*
**************************************************************************************/
static void Audio_MAL_Play(uint32_t Addr, uint32_t Size)
{   
  /* �������ݵ�ַ�ʹ�С */
  DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)Addr;
  DMA_InitStructure.DMA_BufferSize = (uint32_t)Size;

  /* ���²���д�� */
  DMA_Init(AUDIO_MAL_DMA_STREAM, &DMA_InitStructure);
  
  /* ʹ��DMA��*/
  DMA_Cmd(AUDIO_MAL_DMA_STREAM, ENABLE);

  /* ʹ��I2S*/
  if ((CODEC_I2S->I2SCFGR & I2S_ENABLE_MASK) == 0)
  {
    I2S_Cmd(CODEC_I2S, ENABLE);
  }   
}

/************************************************************************************
*������: Audio_MAL_PauseResume
*����˵��:����/��ͣ����
*�������:
*		 Cmd:AUDIO_PAUSE,
*		 Addr:
*����ֵ:��
*
*
**************************************************************************************/
static void Audio_MAL_PauseResume(uint32_t Cmd, uint32_t Addr)
{

  if (Cmd == AUDIO_PAUSE)
  {   
   
    SPI_I2S_DMACmd(CODEC_I2S, SPI_I2S_DMAReq_Tx, DISABLE);

    DMA_Cmd(AUDIO_MAL_DMA_STREAM, DISABLE);
  }
  else /* AUDIO_RESUME */
  {
    SPI_I2S_DMACmd(CODEC_I2S, SPI_I2S_DMAReq_Tx, ENABLE);
 
    DMA_Cmd(AUDIO_MAL_DMA_STREAM, ENABLE);

    if ((CODEC_I2S->I2SCFGR & I2S_ENABLE_MASK) == 0)
    {
      I2S_Cmd(CODEC_I2S, ENABLE);
    } 
	I2S_Cmd(CODEC_I2S, ENABLE);   
  }  
}

/************************************************************************************
*������:Audio_MAL_Stop
*����˵��:ֹͣ
*�������:��
*
*����ֵ:��
*
*
**************************************************************************************/
static void Audio_MAL_Stop(void)
{   

  DMA_Cmd(AUDIO_MAL_DMA_STREAM, DISABLE);


  DMA_ClearFlag(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_TC |AUDIO_MAL_DMA_FLAG_HT | \
                                  AUDIO_MAL_DMA_FLAG_FE | AUDIO_MAL_DMA_FLAG_TE);
  

  I2S_Cmd(CODEC_I2S, DISABLE);
}

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
