 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： stm322xg_eval.h
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
#ifndef __STM322xG_EVAL_H
#define __STM322xG_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件------------------------------------------------------------------*/
#include "stm32_eval.h"
#include "stm32f2xx_dma.h"
#include "stm32f2xx_i2c.h"
#include "stm32f2xx_usart.h"
/*LED灯宏定义---------------------------------------------------------------------*/
#define LEDn                             4

#define LED1_PIN                         GPIO_Pin_6
#define LED1_GPIO_PORT                   GPIOF
#define LED1_GPIO_CLK                    RCC_AHB1Periph_GPIOF  
  
#define LED4_PIN                         GPIO_Pin_10
#define LED4_GPIO_PORT                   GPIOF
#define LED4_GPIO_CLK                    RCC_AHB1Periph_GPIOF  
  
#define LED2_PIN                         GPIO_Pin_7
#define LED2_GPIO_PORT                   GPIOF
#define LED2_GPIO_CLK                    RCC_AHB1Periph_GPIOF  
  
#define LED3_PIN                         GPIO_Pin_9
#define LED3_GPIO_PORT                   GPIOF
#define LED3_GPIO_CLK                    RCC_AHB1Periph_GPIOF
/*按键宏定义-------------------------------------------------------------------*/ 
#define BUTTONn                          2 

#define USER2_BUTTON_PIN                	GPIO_Pin_0
#define USER2_BUTTON_GPIO_PORT          	GPIOA
#define USER2_BUTTON_GPIO_CLK           	RCC_AHB1Periph_GPIOA
#define USER2_BUTTON_EXTI_LINE          	EXTI_Line0
#define USER2_BUTTON_EXTI_PORT_SOURCE   	EXTI_PortSourceGPIOA
#define USER2_BUTTON_EXTI_PIN_SOURCE    	EXTI_PinSource0
#define USER2_BUTTON_EXTI_IRQn          	EXTI0_IRQn 


#define USER1_BUTTON_PIN                   	GPIO_Pin_15
#define USER1_BUTTON_GPIO_PORT             	GPIOG
#define USER1_BUTTON_GPIO_CLK              	RCC_AHB1Periph_GPIOG
#define USER1_BUTTON_EXTI_LINE             	EXTI_Line15
#define USER1_BUTTON_EXTI_PORT_SOURCE      	EXTI_PortSourceGPIOG
#define USER1_BUTTON_EXTI_PIN_SOURCE       	EXTI_PinSource15
#define USER1_BUTTON_EXTI_IRQn             	EXTI15_10_IRQn
/*串口宏定义--------------------------------------------------------------*/
#define COMn                             1

#define EVAL_COM1                        USART1
#define EVAL_COM1_CLK                    RCC_APB2Periph_USART1
#define EVAL_COM1_TX_PIN                 GPIO_Pin_9
#define EVAL_COM1_TX_GPIO_PORT           GPIOA
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource9
#define EVAL_COM1_TX_AF                  GPIO_AF_USART1
#define EVAL_COM1_RX_PIN                 GPIO_Pin_10
#define EVAL_COM1_RX_GPIO_PORT           GPIOA
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource10
#define EVAL_COM1_RX_AF                  GPIO_AF_USART1
#define EVAL_COM1_IRQn                   USART1_IRQn

/*SD卡spi宏定义-----------------------------------------------------------*/
#define SD_SPI                           SPI3
#define SD_SPI_CLK                       RCC_APB1Periph_SPI3
#define SD_SPI_SCK_PIN                   GPIO_Pin_3                 /* PB.3 */
#define SD_SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOB */
#define SD_SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define SD_SPI_SCK_PIN_SOURCE            GPIO_PinSource3

#define SD_SPI_MISO_PIN                  GPIO_Pin_4                 /* PB.4 */
#define SD_SPI_MISO_GPIO_PORT            GPIOB                       /* GPIOB */
#define SD_SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define SD_SPI_MISO_PIN_SOURCE           GPIO_PinSource4

#define SD_SPI_MOSI_PIN                  GPIO_Pin_5                 /* PB.5 */
#define SD_SPI_MOSI_GPIO_PORT            GPIOB                      /* GPIOB */
#define SD_SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define SD_SPI_MOSI_PIN_SOURCE           GPIO_PinSource5

#define SD_CS_PIN                        GPIO_Pin_2                  /* PD.02 */
#define SD_CS_GPIO_PORT                  GPIOD                       /* GPIOD */
#define SD_CS_GPIO_CLK                   RCC_AHB1Periph_GPIOD

#define SD_DETECT_PIN                                       /* P  */
#define SD_DETECT_GPIO_PORT                                      /* GPIO  */
#define SD_DETECT_GPIO_CLK               

/*I2C宏定义---------------------------------------------------------------*/ 
#define sEE_I2C                          I2C2
#define sEE_I2C_CLK                      RCC_APB1Periph_I2C2
#define sEE_I2C_SCL_PIN                  GPIO_Pin_10                  /* PB.06 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SCL_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define sEE_I2C_SCL_SOURCE               GPIO_PinSource10
#define sEE_I2C_SCL_AF                   GPIO_AF_I2C2
#define sEE_I2C_SDA_PIN                  GPIO_Pin_11                  /* PB.09 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SDA_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define sEE_I2C_SDA_SOURCE               GPIO_PinSource11
#define sEE_I2C_SDA_AF                   GPIO_AF_I2C2
//#define sEE_M24C64_32
#define sEE_AT24C02

#define sEE_I2C_DMA                      DMA1   
#define sEE_I2C_DMA_CHANNEL              DMA_Channel_7
#define sEE_I2C_DMA_STREAM_TX            DMA1_Stream7
#define sEE_I2C_DMA_STREAM_RX            DMA1_Stream2  
#define sEE_I2C_DMA_CLK                  RCC_AHB1Periph_DMA1
#define sEE_I2C_DR_Address               ((uint32_t)0x40005810)
#define sEE_USE_DMA
   
#define sEE_I2C_DMA_TX_IRQn              DMA1_Stream7_IRQn
#define sEE_I2C_DMA_RX_IRQn              DMA1_Stream2_IRQn
#define sEE_I2C_DMA_TX_IRQHandler        DMA1_Stream7_IRQHandler
#define sEE_I2C_DMA_RX_IRQHandler        DMA1_Stream2_IRQHandler   
#define sEE_I2C_DMA_PREPRIO              0
#define sEE_I2C_DMA_SUBPRIO              0   
   
#define sEE_TX_DMA_FLAG_FEIF             DMA_FLAG_FEIF7
#define sEE_TX_DMA_FLAG_DMEIF            DMA_FLAG_DMEIF7
#define sEE_TX_DMA_FLAG_TEIF             DMA_FLAG_TEIF7
#define sEE_TX_DMA_FLAG_HTIF             DMA_FLAG_HTIF7
#define sEE_TX_DMA_FLAG_TCIF             DMA_FLAG_TCIF7
#define sEE_RX_DMA_FLAG_FEIF             DMA_FLAG_FEIF2
#define sEE_RX_DMA_FLAG_DMEIF            DMA_FLAG_DMEIF2
#define sEE_RX_DMA_FLAG_TEIF             DMA_FLAG_TEIF2
#define sEE_RX_DMA_FLAG_HTIF             DMA_FLAG_HTIF2
#define sEE_RX_DMA_FLAG_TCIF             DMA_FLAG_TCIF2
   
#define sEE_DIRECTION_TX                 0
#define sEE_DIRECTION_RX                 1  

#define sEE_TIME_CONST                   120 
/*函数声明-----------------------------------------------------*/
void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);
void STM_EVAL_LEDToggle(Led_TypeDef Led);
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button);
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct); 
void SD_LowLevel_DeInit(void);
void SD_LowLevel_Init(void); 
void SD_LowLevel_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize);
void SD_LowLevel_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize);
uint32_t SD_DMAEndOfTransferStatus(void);
void sEE_LowLevel_DeInit(void);
void sEE_LowLevel_Init(void); 
void sEE_LowLevel_DMAConfig(uint32_t pBuffer, uint32_t BufferSize, uint32_t Direction);
/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM322xG_EVAL_H */
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

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.****END OF FILE****/
