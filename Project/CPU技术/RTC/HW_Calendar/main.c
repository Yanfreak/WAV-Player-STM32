/*
*******************************************************************************
  Copyright (C), 2010-2012, Eflag Tech. Co., Ltd.
  FileName		: RTC/HW_Calendar/main.c
  Author		: haiqing       
  Version 		: 1.0.0     
  Date			: 2011/10/08
  Description	: This example demonstrates and explains how to use the RTC 
  				  peripheral.As an application example, it demonstrates how 
				  to setup the RTC peripheral, in terms of prescaler and 
				  interrupts, to be used to keep time.
  Function List : 
  History		:
  <author>  	: haiqing      
  <time>     	: 2011/10/09
  <version >    : 1.0.0 	
  <desc>		: build this moudle					 
*******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "main.h"
#include <stdio.h>

#pragma import(__use_no_semihosting_swi)
/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup HW_Calendar
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Uncomment the corresponding line to select the RTC Clock source */
#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */
/* #define RTC_CLOCK_SOURCE_LSI */ /* LSI used as RTC source clock. The RTC Clock
                                      may varies due to LSI frequency dispersion. */ 

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
USART_InitTypeDef USART_InitStructure;
RTC_TimeTypeDef RTC_TimeStructure;
RTC_InitTypeDef RTC_InitStructure;
RTC_AlarmTypeDef  RTC_AlarmStructure;

__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;

__IO uint32_t time_flag_value = 0;
void RTC_TimeSchow(void);
void RTC_Configuration(void);
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
   /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f2xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f2xx.c file
     */     
       
  /* USARTx configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  int i;
  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

  STM_EVAL_COMInit(COM1, &USART_InitStructure);

  /* RTC configuration  */
  RTC_Configuration();
  
  /* Output a message on Hyperterminal using printf function */
  printf("\n\r  *********************** RTC Calendar Example ***********************\n\r");
  
  if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x3220)
  {  
    /* configure the time register */
    RTC_TimeRegulate();
  
  }
  else
  {
    /* Check if the Power On Reset flag is set */
    if (RCC_GetFlagStatus(RCC_FLAG_PORRST) != RESET)
    {
      printf("\r\n\n Power On Reset occurred....");
    }
    /* Check if the Pin Reset flag is set */
    else if (RCC_GetFlagStatus(RCC_FLAG_PINRST) != RESET)
    {
      printf("\r\n\n External Reset occurred....");
    }

    printf("\r\n No need to configure RTC....");
  }
  
  /* Configure the RTC data register and RTC prescaler */
  RTC_InitStructure.RTC_AsynchPrediv    = AsynchPrediv;
  RTC_InitStructure.RTC_SynchPrediv	=  SynchPrediv;
  RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;
   
  /* Check on RTC init */
  if (RTC_Init(&RTC_InitStructure) == ERROR)
  {
    printf("\n\r        /!\\***** RTC Prescaler Config failed ********/!\\ \n");
  }
  
  /* Configure the external interrupt "Key"and "Tamper" button */
//  STM_EVAL_PBInit(Button_KEY, Mode_EXTI); 
 // STM_EVAL_PBInit(Button_TAMPER , Mode_EXTI);
  while (1)
  {
	  RTC_TimeSchow();
	  for(i=0;i<15000000;i++);
  }
}

void RTC_Configuration(void)
{
  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to RTC */
  PWR_BackupAccessCmd(ENABLE);
    
#if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
  /* Enable the LSI OSC */ 
  RCC_LSICmd(ENABLE);
  /* Wait till LSI is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
  {
  }
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
  
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;
#elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock*/
  /* Enable the LSE OSC */
  RCC_LSEConfig(RCC_LSE_ON);

  /* Wait till LSE is ready */  
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {
  }
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  SynchPrediv = 0xFF;
  AsynchPrediv = 0x7F;
#elif defined (RTC_CLOCK_SOURCE_HSE)  /* HSE used as RTC source clock*/
  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);
  /* Wait till HSE is ready */
  while(RCC_GetFlagStatus(RCC_FLAG_HSERDY) == RESET)
  {
  }
  /* Select the RTC Clock Source */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_HSE_Div31);
  
  SynchPrediv =  0x189D;
  AsynchPrediv = 0x7F;
#endif /* RTC_CLOCK_SOURCE_LSI */
  
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);

  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
}
/**
  * @brief  Returns the time entered by user, using Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeRegulate(void)
{
  uint32_t Tmp_HH = 0xFF, Tmp_MM = 0xFF, Tmp_SS = 0xFF;

  printf("\r\n==============Time Settings=====================================");
  RTC_TimeStructure.RTC_H12     = RTC_H12_AM;
  printf("\r\n  Please Set Hours");
  while (Tmp_HH == 0xFF)
  {
    Tmp_HH = USART_Scanf(23);
    RTC_TimeStructure.RTC_Hours =Tmp_HH;
  }
  printf(":  %d", Tmp_HH);
  
  printf("\r\n  Please Set Minutes");
  while (Tmp_MM == 0xFF)
  {
    Tmp_MM = USART_Scanf(59);
    RTC_TimeStructure.RTC_Minutes = Tmp_MM;
  }
  printf(":  %d", Tmp_MM);
  
  printf("\r\n  Please Set Seconds");
  while (Tmp_SS == 0xFF)
  {
    Tmp_SS = USART_Scanf(59);
    RTC_TimeStructure.RTC_Seconds =Tmp_SS;
  }
  printf(":  %d", Tmp_SS);
   
   /* Configure the RTC time register */
  if(RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructure) == ERROR)
    {
      printf("\n>> !! RTC Set Time failed. !! <<\n");
    } 
    else
    {
      printf("\n>> !! RTC Set Time success. !! <<\n");
      RTC_TimeSchow();
      /* Indicator for the RTC configuration */
      RTC_WriteBackupRegister(RTC_BKP_DR0,0x3220);
    }
}

/**
  * @brief  Display the curent time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_TimeSchow(void)
{
  __IO uint8_t tmp1, tmp3,tmp5 =0;
  /* Get the current Time*/
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
  
  tmp1 = (((RTC_TimeStructure.RTC_Hours)));
  tmp3 = (RTC_TimeStructure.RTC_Minutes );
  tmp5 = (((RTC_TimeStructure.RTC_Seconds )));
 
  printf(" \r The current time is :  %d %d %d \r " ,tmp1, tmp3 , tmp5 );  
}


/**
  * @brief  Display the current time on the Hyperterminal.
  * @param  None
  * @retval None
  */
void RTC_AlarmShow(void)
{
  /* Get the current Alarm */
  RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
  printf("\n\r  The current alarm is :  %0.2d:%0.2d:%0.2d \n", RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours, RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes, RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds);
}


/**
  * @brief  Gets numeric values from the hyperterminal.
  * @param  None
  * @retval None
  */
uint8_t USART_Scanf(uint32_t value)
{
  uint32_t index = 0;
  uint32_t tmp[2] = {0, 0};

  while (index < 2)
  {
    /* Loop until RXNE = 1 */
    while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_RXNE) == RESET)
    {}
    tmp[index++] = (USART_ReceiveData(EVAL_COM1));
    if ((tmp[index - 1] < 0x30) || (tmp[index - 1] > 0x39))
    {
      printf("\n\rPlease enter valid number between 0 and 9");
      index--;
    }
  }
  /* Calculate the Corresponding value */
  index = (tmp[1] - 0x30) + ((tmp[0] - 0x30) * 10);
  /* Checks */
  if (index > value)
  {
    printf("\n\rPlease enter valid number between 0 and %d", value);
    return 0xFF;
  }
  return index;
}


/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */

PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(EVAL_COM1, (uint8_t) ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(EVAL_COM1, USART_FLAG_TC) == RESET)
  {}

  return ch;
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
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

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
