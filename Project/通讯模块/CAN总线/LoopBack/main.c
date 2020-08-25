 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： CAN/LoopBack/main.c
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

/* 头文件声明 ------------------------------------------------------------------*/
#include "main.h"

/* 类型定义  -------------------------------------------------------------------*/
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/* 宏定义  ---------------------------------------------------------------------*/
/* 变量定义 --------------------------------------------------------------------*/
__IO uint32_t ret = 0; /* 中断服务程序返回状态的变量 */
volatile TestStatus TestRx;


/* 函数定义---------------------------------------------------------------------*/
void NVIC_Config(void);
TestStatus CAN_Polling(void);
TestStatus CAN_Interrupt(void);

/******************************************************************************
* 函数名：main.c 
* 功  能：CAN控制器 LOOPBACK功能测试 
* 输  入： 
* 输  出：无  
* 备  注：无
********************************************************************************/

int main(void)
{
  /*在此阶段处理器的时钟已经被初始化完成，时钟初始化工作由SystemInit函数实现，SystemInit函数
   *存放在startup_stm32f2xx.s的汇编启动文件中。
   *在系统上电初始化到跳转到main函数之前，启动代码会先调用SystemInit函数。
   *	
   *如果用户希望重新配置时钟可以在此处再次调用 SystemInit()，它的函数实现存放在system_stm32f2xx.c文件中。
   */

#ifdef USE_CAN1
  /* 打开CANx 外设时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
#else /* USE_CAN2 */
  /* 打开CAN1 & 2  外设时钟 */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 | RCC_APB1Periph_CAN2, ENABLE);
#endif  /* USE_CAN1 */

  /* NVIC 中断配置 */
  NVIC_Config();
  
  /* 配置 LED 1..4 */
  STM_EVAL_LEDInit(LED1);
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);

  /* 在loopback模式下，采用polling轮询方式 进行CAN总线的发送和接收测试，速率配置为125Kb/s */
  TestRx = CAN_Polling();

  if (TestRx !=  FAILED)
  { /* 接收测试成功 */

    /* 点亮LED1 */
    STM_EVAL_LEDOn(LED1);
  }
  else
  { /* 接收测试失败 */

    /* 点亮LED3 */
    STM_EVAL_LEDOn(LED3);
  }

  /* 在loopback模式下，采用中断方式 进行CAN总线的发送和接收测试，速率配置为500Kb/s */
  TestRx = CAN_Interrupt();

  if (TestRx !=  FAILED)
  { /* 接收测试成功 */

    /* 点亮LED4 */
    STM_EVAL_LEDOn(LED4);
  }
  else
  { /* 接收测试失败 */

    /* 点亮LED2 */
    STM_EVAL_LEDOn(LED2);
  }

  /* 无线循环 */
  while (1)
  {
  }
}

/******************************************************************************
* 函数名：CAN_Polling 
* 功  能：配置CAN控制器，通过轮询方式实现发送和接收功能 
* 输  入： 
* 输  出： PASSED : 发送和接收成功
*          FAILED : 发送和接收出错 
* 备  注：无
********************************************************************************/

TestStatus CAN_Polling(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  CanTxMsg TxMessage;
  CanRxMsg RxMessage;
  uint32_t i = 0;
  uint8_t TransmitMailbox = 0;

  /* CAN 寄存器初始化 */
  CAN_DeInit(CANx);

  CAN_StructInit(&CAN_InitStructure);

  /* CAN 初始化结构体配置参数 */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;

  /* CAN 波特率配置为 125kbps (CAN内部时钟工作在 30 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 16;
  CAN_Init(CANx, &CAN_InitStructure);

  /* CAN 滤波初始化 */
#ifdef  USE_CAN1
  CAN_FilterInitStructure.CAN_FilterNumber = 0;
#else /* USE_CAN2 */
  CAN_FilterInitStructure.CAN_FilterNumber = 14;
#endif  /* USE_CAN1 */
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;  
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;

  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* 发送 */
  TxMessage.StdId = 0x11;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 2;
  TxMessage.Data[0] = 0xCA;
  TxMessage.Data[1] = 0xFE;

  TransmitMailbox = CAN_Transmit(CANx, &TxMessage);
  i = 0;
  while((CAN_TransmitStatus(CANx, TransmitMailbox)  !=  CANTXOK) && (i  !=  0xFFFF))
  {
    i++;
  }

  i = 0;
  while((CAN_MessagePending(CANx, CAN_FIFO0) < 1) && (i  !=  0xFFFF))
  {
    i++;
  }

  /* 接收 */
  RxMessage.StdId = 0x00;
  RxMessage.IDE = CAN_ID_STD;
  RxMessage.DLC = 0;
  RxMessage.Data[0] = 0x00;
  RxMessage.Data[1] = 0x00;
  CAN_Receive(CANx, CAN_FIFO0, &RxMessage);

  if (RxMessage.StdId != 0x11)
  {
    return FAILED;  
  }

  if (RxMessage.IDE != CAN_ID_STD)
  {
    return FAILED;
  }

  if (RxMessage.DLC != 2)
  {
    return FAILED;  
  }

  if ((RxMessage.Data[0]<<8|RxMessage.Data[1]) != 0xCAFE)
  {
    return FAILED;
  }
  
  return PASSED; /* 测试通过 */
}

/******************************************************************************
* 函数名：CAN_Polling 
* 功  能：配置CAN控制器，通过中断方式实现发送和接收功能 
* 输  入： 
* 输  出： PASSED : 发送和接收成功
*          FAILED : 发送和接收出错 
* 备  注：无
********************************************************************************/
TestStatus CAN_Interrupt(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;
  CanTxMsg TxMessage;
  uint32_t i = 0;

  /* CAN 寄存器初始化 */
  CAN_DeInit(CANx);

  CAN_StructInit(&CAN_InitStructure);

  /* CAN 寄存器初始化 */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_LoopBack;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
  
  /* CAN 波特率配置为 500kbps (CAN内部时钟工作在 30 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 4;
  CAN_Init(CANx, &CAN_InitStructure);

  /* CAN 滤波 初始化 */
#ifdef  USE_CAN1
  CAN_FilterInitStructure.CAN_FilterNumber = 1;
#else /* USE_CAN2 */
  CAN_FilterInitStructure.CAN_FilterNumber = 15;
#endif  /* USE_CAN1 */
  CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* 打开CAN FIFO0 中断 */ 
  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);

  /*发送1次数据 */
  TxMessage.StdId = 0;
  TxMessage.ExtId = 0x1234;
  TxMessage.IDE = CAN_ID_EXT;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.DLC = 2;
  TxMessage.Data[0] = 0xDE;
  TxMessage.Data[1] = 0xCA;
  CAN_Transmit(CANx, &TxMessage);

  /* 初始化中断状态返回值 */
  ret = 0xFF;
       
  /*等待中断服务程序接收数据完成 */
  i = 0;
  while((ret ==  0xFF) && (i < 0xFFF))
  {
    i++;
  }
  
  if (i ==  0xFFF)
  {
    ret = 0;  
  }

  /* 关闭CAN中断 */
  CAN_ITConfig(CANx, CAN_IT_FMP0, DISABLE);

  return (TestStatus)ret;
}

/******************************************************************************
* 函数名：NVIC_Config 
* 功  能：配置CAN RX0中断 
* 输  入： 
* 输  出：无  
* 备  注：无
********************************************************************************/
void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* 打开 CANx RX0 中断 */
#ifdef  USE_CAN1 
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
#else  /* USE_CAN2 */
  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
#endif /* USE_CAN1 */

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
