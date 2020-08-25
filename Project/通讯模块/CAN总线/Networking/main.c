 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： CAN/Networking/main.c 
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
/* 宏定义  ---------------------------------------------------------------------*/
/* 变量定义 --------------------------------------------------------------------*/
CAN_InitTypeDef        CAN_InitStructure;
CAN_FilterInitTypeDef  CAN_FilterInitStructure;
CanTxMsg TxMessage;
uint8_t KeyNumber = 0x0;

/* 函数定义---------------------------------------------------------------------*/
void NVIC_Config(void);
void CAN_Config(void);
void Init_RxMes(CanRxMsg *RxMessage);
void Delay(void);
void Display_Init(void);


/******************************************************************************
* 函数名：main 
* 功  能：通过按键方式控制CAN控制器的数据发送和接收功能 
* 输  入： 
* 输  出：无  
* 备  注：无
********************************************************************************/

int main(void)
{
 u8  SendNum=0;
 char buf[50] = {0};
  /*在此阶段处理器的时钟已经被初始化完成，时钟初始化工作由SystemInit函数实现，SystemInit函数
   *存放在startup_stm32f2xx.s的汇编启动文件中。
   *在系统上电初始化到跳转到main函数之前，启动代码会先调用SystemInit函数。
   *	
   *如果用户希望重新配置时钟可以在此处再次调用 SystemInit()，它的函数实现存放在system_stm32f2xx.c文件中。
   */
           
  /* NVIC配置，配置CAN中断 */
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

  /* 操作菜单显示 */
  Display_Init();
  
  /* 配置CAN  */
  CAN_Config();
  while(1)
  {
        TxMessage.Data[0] = SendNum;
        CAN_Transmit(CANx, &TxMessage);
		SendNum++;
		sprintf(buf, " Send Data = %d", SendNum);				  /* 将字符串信息存放在buf中*/
        LCD_DisplayStringLine(LINE(3),"                          ");
		LCD_DisplayStringLine(LINE(3), (uint8_t *)buf);	  /* 将buf中的字符串信息显示在液晶的第4行*/
        delay_ms(1000);
  }
  
}

/******************************************************************************
* 函数名：CAN_Config 
* 功  能：配置CAN 控制器功能 
* 输  入： 
* 输  出：无  
* 备  注：无
********************************************************************************/
void CAN_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* CAN 的GPIOs 管脚配置 **************************************************/

  /* 打开 GPIO 时钟 */
  RCC_AHB1PeriphClockCmd(CAN_GPIO_CLK, ENABLE);

  /* 连接 CAN pins 到 AF9 */
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_RX_SOURCE, CAN_AF_PORT);
  GPIO_PinAFConfig(CAN_GPIO_PORT, CAN_TX_SOURCE, CAN_AF_PORT); 
  
  /* 配置 CAN RX 和 TX 管脚 */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN | CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

  /* 打开 CAN 时钟 */
  RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
  
  /* 初始化 CAN 寄存器 */
  CAN_DeInit(CANx);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN 初始化结构体参数配置 */
  CAN_InitStructure.CAN_TTCM = DISABLE;
  CAN_InitStructure.CAN_ABOM = DISABLE;
  CAN_InitStructure.CAN_AWUM = DISABLE;
  CAN_InitStructure.CAN_NART = DISABLE;
  CAN_InitStructure.CAN_RFLM = DISABLE;
  CAN_InitStructure.CAN_TXFP = DISABLE;
  CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    
  /* CAN 波特率配置为 1MBps (CAN 内部时钟工作在 30 MHz) */
  CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2 = CAN_BS2_8tq;
  CAN_InitStructure.CAN_Prescaler = 2;
  CAN_Init(CANx, &CAN_InitStructure);

  /* 初始化CAN 过滤号 */
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
  
  /* 填写 发送发送数据 */
  TxMessage.StdId = 0x321;
  TxMessage.ExtId = 0x01;
  TxMessage.RTR = CAN_RTR_DATA;
  TxMessage.IDE = CAN_ID_STD;
  TxMessage.DLC = 1;
  
  /* 打开 FIFO 0 接收中断 */
  CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
}

/******************************************************************************
* 函数名：NVIC_Config 
* 功  能：配置CAN的RX0中断 
* 输  入： 
* 输  出：无  
* 备  注：无
********************************************************************************/
void NVIC_Config(void)
{
  NVIC_InitTypeDef  NVIC_InitStructure;

#ifdef  USE_CAN1 
  NVIC_InitStructure.NVIC_IRQChannel = CAN1_RX0_IRQn;
#else  /* USE_CAN2 */
  NVIC_InitStructure.NVIC_IRQChannel = CAN2_RX0_IRQn;
#endif /* USE_CAN1 */

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Initializes the Rx Message.
  * @param  RxMessage: pointer to the message to initialize
  * @retval None
  */
/******************************************************************************
* 函数名：Init_RxMes 
* 功  能：初始化接收数据的信息结构体 
* 输  入：RxMessage ： 指向接收数据的信息结构体
* 输  出：无  
* 备  注：无
********************************************************************************/

void Init_RxMes(CanRxMsg *RxMessage)
{
  uint8_t i = 0;

  RxMessage->StdId = 0x00;
  RxMessage->ExtId = 0x00;
  RxMessage->IDE = CAN_ID_STD;
  RxMessage->DLC = 0;
  RxMessage->FMI = 0;
  for (i = 0;i < 8;i++)
  {
    RxMessage->Data[i] = 0x00;
  }
}

/**
  * @brief  Turn ON/OFF the dedicated led
  * @param  Ledstatus: Led number from 0 to 3.
  * @retval None
  */
/******************************************************************************
* 函数名：LED_Display 
* 功  能：LED 指示灯显示，通过输入参数显示对应的LED 
* 输  入：Ledstatus : 0~4 ，指定对应LED点亮 
* 输  出：无  
* 备  注：无
********************************************************************************/

void LED_Display(uint8_t Ledstatus)
{
  /* Turn off all leds */
  STM_EVAL_LEDOff(LED1);
  STM_EVAL_LEDOff(LED2);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOff(LED4);
  
  switch(Ledstatus)
  {
    case(1): 
      STM_EVAL_LEDOn(LED1);
      break;
   
    case(2): 
      STM_EVAL_LEDOn(LED2);
      break;
 
    case(3): 
      STM_EVAL_LEDOn(LED3);
      break;

    case(4): 
      STM_EVAL_LEDOn(LED4);
      break;
    default:
      break;
  }
}

/******************************************************************************
* 函数名：Delay 
* 功  能：延时函数 
* 输  入： 
* 输  出：无  
* 备  注：无
********************************************************************************/

void Delay(void)
{
  uint16_t nTime = 0x0000;

  for(nTime = 0; nTime <0xFFF; nTime++)
  {
  }
}

/******************************************************************************
* 函数名：Display_Init 
* 功  能：显示RS485操作界面 
* 输  入：无
* 输  出：无  
* 备  注：无
********************************************************************************/

void Display_Init(void)
{
  /* 初始化液晶屏幕 */
  STM322xG_LCD_Init();

  /* 清除屏幕为白色 */ 
  LCD_Clear(White);

  /* 选择字体 */
  LCD_SetFont(&Font8x12);

  /* 设置液晶背景色为蓝色，文本色为白色*/
  LCD_SetBackColor(Blue);
  LCD_SetTextColor(White);

  /* 显示字符串 */
  LCD_DisplayStringLine(LINE(0x13), "          CAN bus example               ");

  /* 设置字体 */
  LCD_SetFont(&Font16x24);

  /* 显示字符串 */
  LCD_DisplayStringLine(LINE(0), "   CAN bus example  ");

  /* 设置液晶背景色为白，文本色为蓝色*/
  LCD_SetBackColor(White);
  LCD_SetTextColor(Blue);
}

#ifdef  USE_FULL_ASSERT

/***************************************************************
*函数名:assert_failed
*函数说明:报告参数错误的位置
*	
*输入参数:
*		file:错误所在的文件名
*		line:错误所在文件中的行号
*	
*返回值:无	
*****************************************************************/
void assert_failed(uint8_t* file, uint32_t line)
{ 
 /****此处可以添加用户错误报告方式**********************/

  while (1)
  {
  }
}
#endif
/**
  * @}
  */


/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
