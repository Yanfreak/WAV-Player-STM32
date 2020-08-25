/*
*******************************************************************************
  Copyright (C), 2010-2012, Eflag Tech. Co., Ltd.
  FileName		: PWR/BOR/main.c
  Author		:        
  Version 		: 1.0.0     
  Date			: 2011/09/29
  Description	: Describes how to set BOR_LEVEL. during power on, the Brownout
  				  reset(BOR) keeps the device under reset until the supply voltage
				  reaches the specified V_BOR thereshold. 
  Function List : 
  History		:
  <author>  	:       
  <time>     	: 2011/10/09
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

/** @addtogroup PWR_BOR
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Uncomment the corresponding line to select the BOR level */
//#define BOR_LEVEL OB_BOR_OFF /*!< BOR Reset threshold levels for 1.62V - 2.10V VDD power supply  */
//#define BOR_LEVEL OB_BOR_LEVEL1  /*!< BOR Reset threshold levels for 2.10V - 2.40V VDD power supply */
//#define BOR_LEVEL OB_BOR_LEVEL2  /*!< BOR Reset threshold levels for 2.40V - 2.70V VDD power supply */
#define BOR_LEVEL OB_BOR_LEVEL3  /*!< BOR Reset threshold levels for 2.70V - 3.60V VDD power supply */

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
       
  /* Initialize LED1 on DevKit1207 board */
  STM_EVAL_LEDInit(LED2);

  /* Initialize USER1 Button mounted on DevKit1207 board */
  STM_EVAL_PBInit(BUTTON_USER1, BUTTON_MODE_GPIO);

  /* Test if USER1 push-button on DevKit1207 Board is pressed */
  if (STM_EVAL_PBGetState(BUTTON_USER1) == 0x00)
  {
    /* Get BOR Option Bytes */
    if((FLASH_OB_GetBOR() & 0x0C) != BOR_LEVEL) 
    {
      /* Unlocks the option bytes block access */
      FLASH_OB_Unlock();

      /* Select the desired V(BOR) Level -------------------------------------*/
      FLASH_OB_BORConfig(BOR_LEVEL); 

      /* Launch the option byte loading */
      FLASH_OB_Launch();

      /* Locks the option bytes block access */
      FLASH_OB_Lock();
    }
  } 
   
  while (1)
  {
    /* Toggle The LED1 */
    STM_EVAL_LEDToggle(LED2);

    /* Inserted Delay */
    for(i = 0; i < 0x5FFFF; i++);
  }
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
