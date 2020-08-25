/*
*******************************************************************************
  Copyright (C), 2010-2012, timll Tech. Co., Ltd.
  FileName		: PWR/CurrentConsumption/main.c
  Author		: lichy       
  Version 		: 1.0.0     
  Date			: 2011/09/29
  Description	: This example shows how to configure the STM32F2xx system to 
  				  measure different low power modes current consumption. 		
  Function List : 
  History		:
  <author>  	: lichy       
  <time>     	: 2011/10/09
  <version >    : 1.0.0 	
  <desc>		: build this moudle			 
*******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "stm32_eval.h"
#include "stm32f2xx_lp_modes.h"

/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup PWR_CurrentConsumption
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t i = 0;

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

  /* Enable PWR APB1 Clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Allow access to Backup */
  PWR_BackupAccessCmd(ENABLE);

  /* Reset RTC Domain */
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);

  /*  Configure USER1 Button */
  STM_EVAL_PBInit(BUTTON_USER1,BUTTON_MODE_GPIO);

  /* Wait until USER1 button is pressed to enter the Low Power mode */
  while(STM_EVAL_PBGetState(BUTTON_USER1) != RESET)
  {
  }
  /* Loop while USER1 button is maintained pressed */
  while(STM_EVAL_PBGetState(BUTTON_USER1) != RESET)
  {
  }

#if defined (SLEEP_MODE)
  /* Sleep Mode Entry 
      - System Running at PLL (120MHz)
      - Flash 3 wait state
      - Prefetch and Cache enabled
      - Code running from Internal FLASH
      - All peripherals disabled.
      - Wakeup using EXTI Line (USER1 Button PG.15)
   */
  SleepMode_Measure();
#elif defined (STOP_MODE)
  /* STOP Mode Entry 
      - RTC Clocked by LSE/LSI
      - Regulator in LP mode
      - HSI, HSE OFF and LSI OFF if not used as RTC Clock source  
      - No IWDG
      - FLASH in deep power down mode
      - Automatic Wakeup using RTC clocked by LSE/LSI (after ~20s)
   */
  StopMode_Measure();
#elif defined (STANDBY_MODE)
  /* STANDBY Mode Entry 
      - Backup SRAM and RTC OFF
      - IWDG and LSI OFF
      - Wakeup using WakeUp Pin (PA.00)
   */
  StandbyMode_Measure();
#elif defined (STANDBY_RTC_MODE)
  /* STANDBY Mode with RTC on LSE/LSI Entry 
      - RTC Clocked by LSE or LSI
      - IWDG OFF and LSI OFF if not used as RTC Clock source
      - Backup SRAM OFF
      - Automatic Wakeup using RTC clocked by LSE/LSI (after ~20s)
   */
  StandbyRTCMode_Measure();
#elif defined (STANDBY_RTC_BKPSRAM_MODE)
  /* STANDBY Mode with RTC on LSE/LSI Entry 
      - RTC Clocked by LSE/LSI
      - Backup SRAM ON
      - IWDG OFF
      - Automatic Wakeup using RTC clocked by LSE/LSI (after ~20s)
  */
  StandbyRTCBKPSRAMMode_Measure();
#else

  /* Initialize LED1 on DevKit1207 board */
  STM_EVAL_LEDInit(LED2);
  
  /* Infinite loop */
  while (1)
  {
    /* Toggle The LED1 */
    STM_EVAL_LEDToggle(LED2);

    /* Inserted Delay */
    for(i = 0; i < 0x5FF; i++);
  }
#endif
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
