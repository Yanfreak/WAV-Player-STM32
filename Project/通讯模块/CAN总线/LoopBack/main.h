/******************************************************************************
* 函数名：CAN/LoopBack/main.h 
* 功  能： 
* 输  入： 
* 输  出：无  
* 备  注：无
********************************************************************************/


/* 定义 防止递归调用 -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* 包含头文件 ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "stm32_eval.h"

/* 输出定义类型 ------------------------------------------------------------*/
/* 输出常量 --------------------------------------------------------*/
#define USE_CAN1		/* 定义使用CAN1*/
/* #define USE_CAN2 */

#ifdef  USE_CAN1 /* 使用CAN1 */
  #define CANx CAN1
#else 			 /* 使用CAN2 */
  #define CANx CAN2
#endif  

/* 输出宏 ------------------------------------------------------------*/
/* 输出函数 ------------------------------------------------------- */

#endif /* __MAIN_H */

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/


