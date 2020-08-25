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
#ifndef __STM322xG_EVAL_I2C_EE_H
#define __STM322xG_EVAL_I2C_EE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* 头文件------------------------------------------------------------------*/
#include "stm32_eval.h"
   
#if !defined (sEE_M24C08) && !defined (sEE_M24C64_32) && !defined (sEE_AT24C02)
#error "you mast define eeprom type"
#endif

#ifdef sEE_M24C64_32
 #define sEE_HW_ADDRESS         0xA0   /* E0 = E1 = E2 = 0 */ 

#elif defined (sEE_AT24C02)

 #define sEE_Block0_ADDRESS     0xA0   /* E2 = 0 */ 


#endif /* sEE_M24C64_32 */

/********I2C速度******************************************/
#ifndef I2C_SPEED
 #define I2C_SPEED            100000
#endif 

#define I2C_SLAVE_ADDRESS7      0xA0

#if defined (sEE_M24C08)
 #define sEE_PAGESIZE           16
#elif defined (sEE_M24C64_32)
 #define sEE_PAGESIZE           32
#elif defined sEE_AT24C02
   #define sEE_PAGESIZE          4
#endif
   
/**********超时定义****************************************/  
#define sEE_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define sEE_LONG_TIMEOUT         ((uint32_t)(10 * sEE_FLAG_TIMEOUT))

/* 最大尝试次数 sEE_WaitEepromStandbyState() function */
#define sEE_MAX_TRIALS_NUMBER     300
#define USE_DEFAULT_TIMEOUT_CALLBACK   
/* DMA传输状态定义 */   
#define sEE_STATE_READY           0
#define sEE_STATE_BUSY            1
#define sEE_STATE_ERROR           2
   
#define sEE_OK                    0
#define sEE_FAIL                  1   
/*************函数声明*************************************/
void     sEE_DeInit(void);
void     sEE_Init(void);
uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead);
uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite);
void     sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
uint32_t sEE_WaitEepromStandbyState(void);
uint32_t sEE_TIMEOUT_UserCallback(void);



#ifdef __cplusplus
}
#endif

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


