/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ： sessor.h
*   摘 要    ： 温度、湿度传感器实验主程序
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
******************************************************************************/

#ifndef SENSOR_HEADER_FILE
#define SENSOR_HEADER_FILE

/*************头文件**********************************************************/
#include "stm322xg_eval_lcd.h"
#include "stm32_eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void SensorDataInit(void);
float NTC3950(float volt,float resi_div,float vcc);
float HumiditySensor(float volt);
int Sessor(void);

#endif
