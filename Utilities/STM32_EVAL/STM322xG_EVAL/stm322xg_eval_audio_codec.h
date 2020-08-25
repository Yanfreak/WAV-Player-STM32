 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： stm322xg_eval_audio_codec.h
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
#ifndef __STM322xG_EVAL_AUDIOCODEC_H
#define __STM322xG_EVAL_AUDIOCODEC_H

/* 头文件 ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "stm322xg_eval_ioe.h" 

#define AUDIO_MAL_MODE_NORMAL  /*DMA工作模式定义*/

// #define AUDIO_MAL_MODE_CIRCULAR  
                                      
#define AUDIO_MAL_DMA_IT_TC_EN   /* DMA中断选择*/
/* #define AUDIO_MAL_DMA_IT_HT_EN */ 
/* #define AUDIO_MAL_DMA_IT_TE_EN */  

/* 选择DMA的中断优先级 */
#define EVAL_AUDIO_IRQ_PREPRIO           0   
#define EVAL_AUDIO_IRQ_SUBRIO            2  
/**********I2C的速度*********************************/
#ifndef I2C_SPEED
 #define I2C_SPEED                        100000
#endif /* I2C_SPEED */

#define I2S_STANDARD_PHILLIPS /*I2S模式选择*/
/* #define I2S_STANDARD_MSB */
/* #define I2S_STANDARD_LSB */

//#define CODEC_MCLK_ENABLED	  /*是否使能MCLK*/
#define CODEC_MCLK_DISABLED 
#define VERIFY_WRITTENDATA 
/*----------------------------------------------------------------------------*/

/*-----------------------------------
                   硬件配置定义参数
                                     -----------------------------------------*/
/* I2S外设定义 */
#define CODEC_I2S                      SPI2
#define CODEC_I2S_CLK                  RCC_APB1Periph_SPI2
#define CODEC_I2S_ADDRESS              0x4000380C
#define CODEC_I2S_GPIO_AF              GPIO_AF_SPI2
#define CODEC_I2S_IRQ                  SPI2_IRQn
#define CODEC_I2S_GPIO_CLOCK           (RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOB)
#define CODEC_I2S_WS_PIN               GPIO_Pin_12
#define CODEC_I2S_SCK_PIN              GPIO_Pin_13
#define CODEC_I2S_SD_PIN               GPIO_Pin_15
#define CODEC_I2S_MCK_PIN              GPIO_Pin_6
#define CODEC_I2S_WS_PINSRC            GPIO_PinSource12
#define CODEC_I2S_SCK_PINSRC           GPIO_PinSource13
#define CODEC_I2S_SD_PINSRC            GPIO_PinSource15
#define CODEC_I2S_MCK_PINSRC           GPIO_PinSource6
#define CODEC_I2S_GPIO                 GPIOB
#define CODEC_I2S_MCK_GPIO             GPIOC


 /* I2S DMA定义 */
 #define AUDIO_MAL_DMA_CLOCK            RCC_AHB1Periph_DMA1
 #define AUDIO_MAL_DMA_STREAM           DMA1_Stream4
 #define AUDIO_MAL_DMA_CHANNEL          DMA_Channel_0
 #define AUDIO_MAL_DMA_IRQ              DMA1_Stream4_IRQn
 #define AUDIO_MAL_DMA_FLAG_TC          DMA_FLAG_TCIF4
 #define AUDIO_MAL_DMA_FLAG_HT          DMA_FLAG_HTIF4
 #define AUDIO_MAL_DMA_FLAG_FE          DMA_FLAG_FEIF4
 #define AUDIO_MAL_DMA_FLAG_TE          DMA_FLAG_TEIF4
 #define AUDIO_MAL_DMA_FLAG_DME         DMA_FLAG_DMEIF4
 #define AUDIO_MAL_DMA_PERIPH_DATA_SIZE DMA_PeripheralDataSize_HalfWord
 #define AUDIO_MAL_DMA_MEM_DATA_SIZE    DMA_MemoryDataSize_HalfWord
 #define DMA_MAX_SZE                    0xFFFF

 #define Audio_MAL_IRQHandler           DMA1_Stream4_IRQHandler

/* i2c codec定义*/
#define CODEC_I2C                      I2C2
#define CODEC_I2C_CLK                  RCC_APB1Periph_I2C2
#define CODEC_I2C_GPIO_CLOCK           RCC_AHB1Periph_GPIOB
#define CODEC_I2C_GPIO_AF              GPIO_AF_I2C2
#define CODEC_I2C_GPIO                 GPIOB
#define CODEC_I2C_SCL_PIN              GPIO_Pin_10
#define CODEC_I2C_SDA_PIN              GPIO_Pin_11
#define CODEC_I2S_SCL_PINSRC           GPIO_PinSource10
#define CODEC_I2S_SDA_PINSRC           GPIO_PinSource11

/*延时定义*/  
#define CODEC_FLAG_TIMEOUT             ((uint32_t)0x10000)
#define CODEC_LONG_TIMEOUT             ((uint32_t)(600 * CODEC_FLAG_TIMEOUT))
/*----------------------------------------------------------------------------*/

/*-----------------------------------
                       音频定义
                                     -----------------------------------------*/
/*输出选择定义 */
#define OUTPUT_DEVICE_SPEAKER         1
#define OUTPUT_DEVICE_HEADPHONE       2
#define OUTPUT_DEVICE_BOTH            3
#define OUTPUT_DEVICE_AUTO            4

/* 音量定义 */
#define DEFAULT_VOLMIN                0x00
#define DEFAULT_VOLMAX                0xFF
#define DEFAULT_VOLSTEP               0x04

#define AUDIO_PAUSE                   0
#define AUDIO_RESUME                  1
#define AUDIO_STOP                    2


#define CODEC_PDWN_HW                 1
#define CODEC_PDWN_SW                 2

#define AUDIO_MUTE_ON                 1
#define AUDIO_MUTE_OFF                0
 
#define VOLUME_CONVERT(x)    ((Volume > 100)? 100:((uint8_t)((Volume * 255) / 100)))
#define DMA_MAX(x)           (((x) <= DMA_MAX_SZE)? (x):DMA_MAX_SZE)

/*****函数声明**************************************/
uint32_t EVAL_AUDIO_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);
uint32_t EVAL_AUDIO_DeInit(void);
uint32_t EVAL_AUDIO_Play(uint16_t* pBuffer, uint32_t Size);
uint32_t EVAL_AUDIO_PauseResume(uint32_t Cmd);
uint32_t EVAL_AUDIO_Stop(uint32_t CodecPowerDown_Mode);
uint32_t EVAL_AUDIO_VolumeCtl(uint8_t Volume);
uint32_t EVAL_AUDIO_Mute(uint32_t Command);

uint32_t Codec_SwitchOutput(uint8_t Output);

void EVAL_AUDIO_TransferComplete_CallBack(uint32_t pBuffer, uint32_t Size);

void EVAL_AUDIO_HalfTransfer_CallBack(uint32_t pBuffer, uint32_t Size);

void EVAL_AUDIO_Error_CallBack(void* pData);

uint32_t Codec_TIMEOUT_UserCallback(void);
 
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
