/**
  ******************************************************************************
  * @file    system_stm32f2xx.c
  * @author  MCD Application Team
  * @version V0.0.3
  * @date    3/19/2011
   * @brief CMSIS Cortex-M3 Device Peripheral Access Layer System Source File.
   *                   This file is generated by the automatic clock configuration system 
   *                   "STM32F2xx-RevA-Z_Clock_Configuration_V0.0.3.xls". 
  ******************************************************************************  
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2010 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/** @addtogroup CMSIS
  * @{
  */

/** @addtogroup stm32f2xx_system
  * @{
  */  
  
/** @addtogroup STM32F2xx_System_Private_Includes
  * @{
  */

#include "stm32f2xx.h"

/**
  * @}
  */

/** @addtogroup STM32F2xx_System_Private_TypesDefinitions
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F2xx_System_Private_Defines
  * @{
  */

/*!< Uncomment the line corresponding to the desired System clock (SYSCLK)
   frequency (after reset the HSI is used as SYSCLK source)
   
   IMPORTANT NOTE:
   ============== 
   1. After each device reset the HSI is used as System clock source.

   2. Please make sure that the selected System clock doesn't exceed your device's
      maximum frequency.
      
    */
    

/* PLLVCO = (HSE_VALUE / PLL_M) * PLL_N */   
#define PLL_M    25
#define PLL_N    240

/* SYSCLK = PLLVCO / PLL_P !!!! DO NOT EXCEED 120MHz */
#define PLL_P    2

/* OTGFS, SDIO and RNG Clock =  PLLVCO / PLLQ */
#define PLL_Q    5

/* I2SCLK =  PLLVCO / PLLR */                          
#define PLL_R    2
                          

/*!< Uncomment the following line if you need to use external SRAM mounted
     on STM3220F-EVAL board as data memory  */ 
/* #define DATA_IN_ExtSRAM */

/*!< Uncomment the following line if you need to relocate your vector Table in
     Internal SRAM. */ 
/* #define VECT_TAB_SRAM */
#define VECT_TAB_OFFSET  0x0 /*!< Vector Table base offset field. 
                                  This value must be a multiple of 0x100. */

/**
  * @}
  */

/** @addtogroup STM32F2xx_System_Private_Macros
  * @{
  */

/**
  * @}
  */

/** @addtogroup STM32F2xx_System_Private_Variables
  * @{
  */

/*******************************************************************************
*  Clock Definitions
*******************************************************************************/
uint32_t SystemCoreClock = ((HSE_VALUE / PLL_M) * PLL_N) / PLL_P;

/**
  * @}
  */

/** @addtogroup STM32F2xx_System_Private_FunctionPrototypes
  * @{
  */

static void SetSysClock(void);

/**
  * @}
  */

/** @addtogroup STM32F2xx_System_Private_Functions
  * @{
  */

/**
  * @brief  Setup the microcontroller system
  *         Initialize the Embedded Flash Interface, the PLL and update the SystemFrequency variable.
  * @note   This function should be used only after reset.
  * @param  None
  * @retval None
  */
void SystemInit (void)
{
  /* Reset the RCC clock configuration to the default reset state(for debug purpose) */
  /* Set HSION bit */
  RCC->CR |= (uint32_t)0x00000001;
  /* Reset CFGR register */
  RCC->CFGR = 0x00000000;
  /* Reset HSEON, CSSON and PLLON bits */
  RCC->CR &= (uint32_t)0xFEF6FFFF;
  /* Reset PLLCFGR register */
  RCC->PLLCFGR = 0x00000000;
  /* Reset HSEBYP bit */
  RCC->CR &= (uint32_t)0xFFFBFFFF;
  /* Disable all interrupts */
  RCC->CIR = 0x00000000;
       
  /* Configure the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers */
  /* Configure the Flash Latency cycles and enable prefetch buffer */
  SetSysClock();

#ifdef VECT_TAB_SRAM
  SCB->VTOR = SRAM_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal SRAM. */
#else
  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH. */
#endif   

}

/**
  * @brief  Update SystemCoreClock according to Clock Register Values
  * @note   None
  * @param  None
  * @retval None
  */
void SystemCoreClockUpdate(void)
{
  /* To be implemented in next version */
}

/**
  * @brief  Setup the external memory controller. Called in startup_stm32f2xx.s 
  *          before jump to __main
  * @param  None
  * @retval None
  */ 
#ifdef DATA_IN_ExtSRAM
/**
  * @brief  Setup the external memory controller. 
  *         Called in startup_stm32f2xx.s/.c before jump to main.
  *         This function configures the external PSRAM mounted on STM3220F_EVAL board
  *         This PSRAM will be used as program data memory (including heap and stack).
  * @param  None
  * @retval None
  */ 
void SystemInit_ExtMemCtl(void) 
{
  /* To be implemented in next version */
}
#endif /* DATA_IN_ExtSRAM */

/**
  * @brief  Configures the System clock frequency, HCLK, PCLK2 and PCLK1 prescalers.
  * @param  None
  * @retval None
  */
static void SetSysClock(void)
{
  __IO uint32_t StartUpCounter = 0, HSEStatus = 0;
  /* Enable HSE */
  RCC->CR |= ((uint32_t)RCC_CR_HSEON);
 
  /* Wait till HSE is ready and if Time out is reached exit */
  do
  {
    HSEStatus = RCC->CR & RCC_CR_HSERDY;
    StartUpCounter++;
  } while((HSEStatus == 0) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  if ((RCC->CR & RCC_CR_HSERDY) != RESET)
  {
    HSEStatus = (uint32_t)0x01;
  }
  else
  {
    HSEStatus = (uint32_t)0x00;
  }

  if (HSEStatus == (uint32_t)0x01)
  {
    /* HCLK = SYSCLK / 1 */
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
      
    /* PCLK2 = HCLK / 2 */
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
    
    /* PCLK1 = HCLK / 4 */
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;

    /* */
    RCC->PLLCFGR = PLL_M | (PLL_N << 6) | (((PLL_P >> 1) -1) << 16) | (RCC_PLLCFGR_PLLSRC_HSE) |
                 (PLL_Q << 24) | (PLL_R << 28);

    /* Enable PLL */
    RCC->CR |= RCC_CR_PLLON;

    /* Wait till PLL is ready */
    while((RCC->CR & RCC_CR_PLLRDY) == 0)
    {
    }
   
    /* Configure Flash prefetch, Instruction cache, Data cache and wait state */
    FLASH->ACR = FLASH_ACR_PRFTEN | FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_LATENCY_3WS; 

    /* Select PLL as system clock source */
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= RCC_CFGR_SW_PLL;

    /* Wait till PLL is used as system clock source */
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS ) != RCC_CFGR_SWS_PLL); 
    {
    }
  }
  else
  { /* If HSE fails to start-up, the application will have wrong clock
         configuration. User can add here some code to deal with this error */
  }
}
/**
  * @}
  */
/**
  * @}
  */
  
/**
  * @}
  */
/******************* (C) COPYRIGHT 2010 STMicroelectronics *****END OF FILE****/
