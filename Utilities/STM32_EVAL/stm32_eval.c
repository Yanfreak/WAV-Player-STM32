 /**********************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   �ļ����� �� stm32_eval.h
*   ժ Ҫ    �� 
*
*   ��ǰ�汾 �� 
*   �� ��    �� EFLAG
*   ������� �� 
*
*   ȡ���汾 ��
*   ԭ����   �� 
*   �޸����� ��
*   ������� ��2012.08.01 
*
**********************************************************/
  
/* ͷ�ļ� ------------------------------------------------------------------*/
#include "stm32_eval.h"

/*************ͷ�ļ�ѡ��******************************/
  
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
