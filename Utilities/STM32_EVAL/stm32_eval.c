 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： stm32_eval.h
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
  
/* 头文件 ------------------------------------------------------------------*/
#include "stm32_eval.h"

/*************头文件选择******************************/
  
#ifdef USE_STM32100B_EVAL
 #include "stm32100b_eval/stm32100b_eval.c"
#elif defined USE_STM3210B_EVAL
 #include "stm3210b_eval/stm3210b_eval.c" 
#elif defined USE_STM3210E_EVAL
 #include "stm3210e_eval/stm3210e_eval.c"
#elif defined USE_STM3210C_EVAL
 #include "stm3210c_eval/stm3210c_eval.c"
#elif defined USE_STM32L152_EVAL
 #include "stm32l152_eval/stm32l152_eval.c"
#elif defined USE_STM322xG_EVAL
 #include "stm322xg_eval/stm322xg_eval.c"
#elif defined USE_STM32100E_EVAL
 #include "stm32100e_eval/stm32100e_eval.c"
#else 
 #error "Please select first the STM32 EVAL board to be used (in stm32_eval.h)"
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
    
/******* Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.***END OF FILE****/
