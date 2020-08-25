/*
*******************************************************************************
  Copyright (C), 2010-2012, Eflag Tech. Co., Ltd.
  FileName		: WWDG/WWDG_Example/main.c
  Author		: haiqing       
  Version 		: 1.0.0     
  Date			: 2012/7/5
  Description	: This example shows how to update at regular period the WWDG 
  				  counter and how to simulate a software fault generating an 
				  MCU WWDG reset on expiry of a programmed time period.
  Function List : 
  History		:
  <author>  	: haiqing      
  <time>     	: 2012/7/5
  <version >    : 1.0.0 	
  <desc>		: build this moudle				 
*******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f2xx.h"
#include "stm32_eval.h"

/** @addtogroup STM32F2xx_StdPeriph_Examples
  * @{
  */

/** @addtogroup WWDG_Example
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t TimingDelay = 0;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nTime);

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
       
   /* Initialize LEDs  mounted on EITP board */       
  STM_EVAL_LEDInit(LED2);
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_PBInit(BUTTON_USER1, BUTTON_MODE_EXTI);

  /* Check if the system has resumed from WWDG reset */
  if (RCC_GetFlagStatus(RCC_FLAG_WWDGRST) != RESET)
  { 
    /* WWDGRST flag set */
    /* Turn on LED2 */
    STM_EVAL_LEDOn(LED2);

    /* Clear reset flags */
    RCC_ClearFlag();
  }
  else
  {
    /* WWDGRST flag is not set */
    /* Turn off LED1 */
    STM_EVAL_LEDOff(LED2);
  }

  /* Setup SysTick Timer for 1 msec interrupts  */
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }

  /* WWDG configuration */
  /* Enable WWDG clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);

  /* WWDG clock counter = (PCLK1 (30MHz)/4096)/8 = 915 Hz (~1092 us)  */
  WWDG_SetPrescaler(WWDG_Prescaler_8);

  /* Set Window value to 80; WWDG counter should be refreshed
   only when the counteris below 80 (and greater than 64)
    otherwise a reset will be generated */
  WWDG_SetWindowValue(80);

  /* Enable WWDG and set counter value to 127, 
  WWDG timeout = ~1092 us * 64 = 69.9 ms 
  In this case the refresh window is:
  ~1092 * (127-80) = 51.3 ms < refresh window < ~1092 * 64 = 69.9ms
  */
  WWDG_Enable(127);
   
  while (1)
  {
    /* Toggle LED3 */
    STM_EVAL_LEDToggle(LED3);
    /* Insert 53 ms delay */
    Delay(63);
    /* Update WWDG counter */
    WWDG_SetCounter(127);
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
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
