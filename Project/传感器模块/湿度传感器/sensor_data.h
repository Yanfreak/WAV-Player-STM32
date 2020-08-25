/****************************************************************************
*
*   Copyright (c) 2012, BeiJing EFLAG Technology Co., LTD.
*   All rights reserved.
*
*   文件名称 ：sensor_data.h
*   摘 要    ： 温度传感器分度表
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

#ifndef SENSOR_DATA_HEADER_FILE
#define SENSOR_DATA_HEADER_FILE

typedef struct {			  /* 数据分度表查询结构体 */
	float nIndexLow;		  /* 根据分度表查询出的数据对应的最小值 */
	float nIndexHigh;	      /* 根据分度表查询出的数据对应的最大值 */
	float nIndexFactor;		  /* 根据分度表查询出的数据与实际分度的比例系数 */
	int nTableSize;			  /* 分度表大小 */
	const float *pTableData;  /* 保存分度表数据的数组指针 */
}TIndexTable;

#ifndef SENSOR_DATA_FILE
extern const float TableData_NTC3950[];	/* NTC3950 电阻/温度分度表*/
extern TIndexTable TIndexTab_NTC3950; /* NTC3950 电阻/温度分度表查询结构体 */
#endif

#endif

/****** (C) 版权2012北京亿旗创新科技发展有限公司 ******文档结束**************/
